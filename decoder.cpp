#include "decoder.h"

decoder::decoder()
{

}
void decoder::set_decoder(LDPC ldp,channel ch,encoder enc)
{
    column_num = ldp.get_column_num();
    row_num = ldp.get_row_num();
    column_ones_num = ldp.get_column_ones_num();
    row_ones_num = ldp.get_row_ones_num();
    bit_node_index = ldp.get_bit_node_index();
    check_node_index = ldp.get_check_node_index();
    input_signal = enc.get_input_signal();
    BER_rate = ch.get_BER();
}
void decoder::set_decoder_enc_out(encoder enc)
{
    output_signal = enc.get_output_signal();
}
int decoder::belief_propagation(int max_round,int num_cw)
{
    //Instantiation of 3d vector
    //cout <<"Please input iteration Round" <<endl;


    iterative_round = max_round;
    bit_node_temp.resize(column_num,vector<double>(column_ones_num,0));
    bit_node.resize(iterative_round,bit_node_temp);
    /*
    bit_node_index_save_temp.resize(ldp.get_column_ones_num(),vector<int>(1,0));
    bit_node_index_save[0].resize(ldp.get_column_num(),bit_node_index_save_temp);
    bit_node_index_save_temp.resize(ldp.get_column_ones_num(),vector<int>(1,0));
    bit_node_index_save[1].resize(ldp.get_column_num(),bit_node_index_save_temp);
    */
    check_node_temp.resize(row_num,vector<double>(row_ones_num,0));
    check_node.resize(iterative_round,check_node_temp);
    /*
    check_node_index_save_temp.resize(ldp.get_row_ones_num(),vector<int>(1,0));
    check_node_index_save[0].resize(ldp.get_row_num(),check_node_index_save_temp);
    check_node_index_save_temp.resize(ldp.get_row_ones_num(),vector<int>(1,0));
    check_node_index_save[1].resize(ldp.get_row_num(),check_node_index_save_temp);
    */
    APP_ratio.resize(column_num,vector<double>(column_ones_num,0));
    APP_ratio_real.resize(column_num,0);
    //if(input)

    //Main Decoding ALGO
    if(num_cw ==1){
        bit_node_index_save_temp.resize(column_ones_num,vector<int>(1,0));
        bit_node_index_save[0].resize(column_num,bit_node_index_save_temp);
        //bit_node_index_save_temp.resize(ldp.get_column_ones_num(),vector<int>(1,0));
        bit_node_index_save[1].resize(column_num,bit_node_index_save_temp);
        check_node_index_save_temp.resize(row_ones_num,vector<int>(1,0));
        check_node_index_save[0].resize(row_num,check_node_index_save_temp);
        //check_node_index_save_temp.resize(ldp.get_row_ones_num(),vector<int>(1,0));
        check_node_index_save[1].resize(row_num,check_node_index_save_temp);
        app_index_save_temp.resize(column_ones_num,vector<int>(1,0));
        app_index_save[0].resize(column_num,bit_node_index_save_temp);
        app_index_save[1].resize(column_num,bit_node_index_save_temp);
        Initialization();
        check_to_bit(0);
        bit_to_check(1);
        APP_ratio_calc(1);
        Hard_Decision();
        //output_result_soft(1);
        //for(int i=0;i<ldp.get_column_num();i++)
        //    for(int j=0;j<ldp.get_column_ones_num();j++)
        //        for(int k=0;k<bit_node_index_save[0][i][j].size();k++)
        //            cout << "(" << bit_node_index_save[0][i][j][k] << "," << bit_node_index_save[1][i][j][k] << ")";

        if(decode_success())
            return 1;
        for(int i=1;i<iterative_round-1;i++){
            //cout << (double)clock() <<endl;
            check_to_bit_simple_simple(i);
            //cout << (double)clock() <<endl;
            bit_to_check_simple_simple(i+1);
            //cout << (double)clock() <<endl;
            APP_ratio_calc_simple_simple_simple(i+1);
            //cout << (double)clock() << endl;
            Hard_Decision_simple();
            //cout << (double)clock() <<endl;
            //output_result();
            //output_result_soft(i+1);
            if(decode_success())
                return i+1;
         }
    }

    //check_to_bit(0,ldp,enc);
    //bit_to_check(1,ldp,enc);
    //APP_ratio_calc(1,ldp,enc);
    //Hard_Decision(ldp);
    /*
    check_to_bit(1,ldp,enc);
    bit_to_check(2,ldp,enc);
    //APP_ratio_calc(2,ldp,enc);
    check_to_bit(2,ldp,enc);
    bit_to_check(3,ldp,enc);
    APP_ratio_calc(3,ldp,enc);
    */
    //Hard_Decision(ldp);



    else{
        Initialization();
        for(int i=0;i<iterative_round-1;i++){
            check_to_bit_simple_simple(i);
            bit_to_check_simple_simple(i+1);
            //APP_ratio_calc_simple_simple(i+1);
            APP_ratio_calc_simple_simple_simple(i+1);//APP_ratio_calc_simple(i+1,ldp,enc);
            //Hard_Decision();
            Hard_Decision_simple();
            //if(hard_decision_one != hard_decision_two)
            //    cout << "NO SAME!!";
            if(decode_success())
                return i+1;
         }
    }
/*
    Initialization(ldp,enc,ch);
    for(int i=0;i<iterative_round-1;i++){
        check_to_bit(i,ldp,enc);
        bit_to_check(i+1,ldp,enc);
        APP_ratio_calc(i+1,ldp,enc);
        Hard_Decision(ldp);
        if(decode_success(ldp,enc))
            return i+1;
     }
*/
    return iterative_round;
}


void decoder::Initialization()
{
    //Initialization : Pi = P(Ci=1|Yi=yi)
    for(int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            if(output_signal[i] == 1 && bit_node_index[i][j]!=0)//ex. //P(Ci=1|Yi=1), aka correct probabitlity
                bit_node[0][i][j] = 1-BER_rate;//Layer 1
            if(output_signal[i] == 0 && bit_node_index[i][j]!=0)//P(Ci=1|Yi=0), aka wrong probabitlity
                bit_node[0][i][j] = BER_rate;//Layer 1
        }
    }
}

void decoder::check_to_bit(int current_round)
{
    for(int i=0;i<row_num;i++){
        for (int j=0;j<row_ones_num;j++){
            if(check_node_index[i][j]!=0){ //Make sure the bit-check node connection exist
                double temp_mult = 1;
                for(int k=0;k<row_ones_num;k++){//get index of q to calculate r
                        if(check_node_index[i][j] != check_node_index[i][k]){//Rj\i
                            for(int l=0;l<column_ones_num;l++){//find true index of q in container
                                if(check_node_index[i][k] != 0){
                                    if(bit_node_index[check_node_index[i][k]-1][l] == i+1){
                                        temp_mult = temp_mult * (1-2*bit_node[current_round][check_node_index[i][k]-1][l]);
                                        (check_node_index_save[0])[i][j].push_back(check_node_index[i][k]-1);
                                        (check_node_index_save[1])[i][j].push_back(l);
                                    }
                                }
                            }
                         }
                }
                check_node[current_round][i][j] =  1.0 -((0.5)+(0.5)*temp_mult); //r(-1)
                //cout << check_node[current_round][i][j] <<endl;
            }
        }
    }

}
void decoder::bit_to_check(int current_round)
{
    for(int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            if(bit_node_index[i][j]!=0){ //Make sure the bit-check node connection exist
                double temp_mult_minus_one = 1,temp_mult_plus_one = 1;
                for(int k=0;k<column_ones_num;k++){//get index of r to calculate q
                    if(bit_node_index[i][j] != bit_node_index[i][k]){//Ci\j
                        for(int l=0;l<row_ones_num;l++){//find true index of r in container
                            if(bit_node_index[i][k] != 0){
                                if(check_node_index[bit_node_index[i][k]-1][l] == i+1){//Searching
                                    temp_mult_minus_one *= (check_node[current_round-1][bit_node_index[i][k]-1][l]);
                                    temp_mult_plus_one *= (1.0-check_node[current_round-1][bit_node_index[i][k]-1][l]);
                                    (bit_node_index_save[0])[i][j].push_back(bit_node_index[i][k]-1);
                                    (bit_node_index_save[1])[i][j].push_back(l);
                                }
                            }
                        }
                    }
                }
                double temp_q_minus_1 = ((bit_node[0][i][j])*temp_mult_minus_one);
                double temp_q_plus_1 = ((1.0-bit_node[0][i][j])*temp_mult_plus_one);
                bit_node[current_round][i][j] =  (temp_q_minus_1)/(temp_q_minus_1 + temp_q_plus_1);
                //cout << check_node[current_round][i][j] <<endl;
            }
        }
    }
}
void decoder::APP_ratio_calc(int current_round)
{
    //APP_ratio.resize(ldp.get_column_num(), vector<double>(ldp.get_row_num(), 0));
    for(int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            if(bit_node_index[i][j]!=0){//Make sure the bit-check node connection exist
                double temp_app_minus_1,temp_app_plus_1;
                for(int l=0;l<row_ones_num;l++){//find true index of r in container
                    if(check_node_index[bit_node_index[i][j]-1][l] == i+1){
                        temp_app_minus_1 = bit_node[current_round][i][j] * check_node[current_round-1][bit_node_index[i][j]-1][l];//qij(-1)*rji(-1)
                        temp_app_plus_1 = (1-bit_node[current_round][i][j]) * (1-check_node[current_round-1][bit_node_index[i][j]-1][l]);//qij(+1)*rji(+1)
                        APP_ratio[i][j] = (temp_app_minus_1)/(temp_app_minus_1 + temp_app_plus_1);
                        (app_index_save[0])[i][j].push_back(bit_node_index[i][j]-1);
                        (app_index_save[1])[i][j].push_back(l);
                    }
                }
            }
        }
    }
}
void decoder::Hard_Decision()
{
    //WRONG!!!Cannot Do this. Because the hard decision result only depends on app_ratio's last value of in the column.
    //However, the the last value of the column often equals to zero.
    /*
    hard_decision_one.resize(column_num,0);
    for(int i=0;i<column_num;i++){
        for(int j=0;j<column_ones_num;j++){
            if(APP_ratio[i][j] >= 0.5)
                hard_decision_one[i] = 1;
            else
                hard_decision_one[i] = 0;
        }
    }
    */
    hard_decision.resize(column_num,0);
    for(int i=0;i<column_num;i++){
        for(int j=0;j<column_ones_num;j++){
            if(APP_ratio[i][j] >= 0.5){
                hard_decision[i] = 1;
                break;
             }
            else if(APP_ratio[i][j]!=0){
                hard_decision[i] = 0;
                break;
            }
        }
    }
}
void decoder::Hard_Decision_simple()
{
    //Correct
    hard_decision.resize(column_num,0);
    for(int i=0;i<column_num;i++){
            if(APP_ratio_real[i] >= 0.5)
                hard_decision[i] = 1;
            else
                hard_decision[i] = 0;
    }
}
void decoder::output_result()
{

    //Output bit node
    cout << "-------BIT_NODE-----------" <<endl;
    for(int i=0;i<iterative_round;i++){
        cout << "Iteration: " << i << endl;
        for(int j=0;j<column_num;j++){
            for(int k=0;k<column_ones_num;k++){
                cout << bit_node[i][j][k] << "  " ;
            }
            cout << endl;
        }

        cout << "-------------------" << endl;
    }
/*
    //Output Check node
    cout << "-------CHECK_NODE-----------" <<endl;
    for(int i=0;i<iterative_round;i++){
        cout << "Iteration: " << i << endl;
        for(int j=0;j<row_num;j++){
            for(int k=0;k<row_ones_num;k++){
                cout << check_node[i][j][k] << "  " ;
            }
            cout << endl;
        }
        cout << "-------------------" << endl;
    }
*/
    //Output APP ratio

    cout << "-------APP_RATIO-----------" <<endl;
    for(int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            cout << APP_ratio[i][j] << " ";
        }
        cout << endl;
    }
    cout << "-------------------" << endl;
/*
    //Output Hard Decision
    cout << "-------Hard_Decision-----------" <<endl;
    for(int i=0;i<column_num;i++){
            cout << hard_decision[i] << " ";
    }
    cout << endl;
    cout << "-------------------" << endl;
    */
}

bool decoder::decode_success()
{
    for(int i=0;i<column_num;i++){
        if(hard_decision[i] != input_signal[i])
            return false;
    }
    return true;
}

void decoder::check_to_bit_simple(int current_round, LDPC ldp,encoder enc)
{
    for(int i=0;i<ldp.get_row_num();i++){
        for (int j=0;j<ldp.get_row_ones_num();j++){
            if(ldp.get_check_node_index()[i][j]!=0){ //Make sure the bit-check node connection exist
                double temp_mult = 1;
                for(int k=1;k<(check_node_index_save[0])[i][j].size();k++){
                    temp_mult = temp_mult * (1-2*bit_node[current_round][(check_node_index_save[0])[i][j][k]][(check_node_index_save[1])[i][j][k]]);
                }
                check_node[current_round][i][j] =  1.0 -((0.5)+(0.5)*temp_mult); //r(-1)
                //cout << check_node[current_round][i][j] <<endl;
            }
        }
    }
}
void decoder::check_to_bit_simple_simple(int current_round)
{
    for(int i=0;i<row_num;i++){
        for (int j=0;j<row_ones_num;j++){
            if(check_node_index[i][j]!=0){ //Make sure the bit-check node connection exist
                double temp_mult = 1;
                for(int k=1;k<(check_node_index_save[0])[i][j].size();k++){
                    temp_mult = temp_mult * (1-2*bit_node[current_round][(check_node_index_save[0])[i][j][k]][(check_node_index_save[1])[i][j][k]]);
                }
                check_node[current_round][i][j] =  1.0 -((0.5)+(0.5)*temp_mult); //r(-1)
                //cout << check_node[current_round][i][j] <<endl;
            }
        }
    }
}
void decoder::bit_to_check_simple(int current_round, LDPC ldp,encoder enc)
{
    for(int i=0;i<ldp.get_column_num();i++){
        for (int j=0;j<ldp.get_column_ones_num();j++){
            if(ldp.get_bit_node_index()[i][j]!=0){ //Make sure the bit-check node connection exist
                double temp_mult_minus_one = 1,temp_mult_plus_one = 1;
                for(int k=1;k<(bit_node_index_save[0])[i][j].size();k++){
                    temp_mult_minus_one *= (check_node[current_round-1][(bit_node_index_save[0])[i][j][k]][(bit_node_index_save[1])[i][j][k]]);
                    temp_mult_plus_one *= (1.0-(check_node[current_round-1][(bit_node_index_save[0])[i][j][k]][(bit_node_index_save[1])[i][j][k]]));
                }
                double temp_q_minus_1 = ((bit_node[0][i][j])*temp_mult_minus_one);
                double temp_q_plus_1 = ((1.0-bit_node[0][i][j])*temp_mult_plus_one);
                bit_node[current_round][i][j] =  (temp_q_minus_1)/(temp_q_minus_1 + temp_q_plus_1);
            }
        }
    }
}
void decoder::bit_to_check_simple_simple(int current_round)
{
    for(int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            if(bit_node_index[i][j]!=0){ //Make sure the bit-check node connection exist
                double temp_mult_minus_one = 1,temp_mult_plus_one = 1;
                for(int k=1;k<(bit_node_index_save[0])[i][j].size();k++){
                    temp_mult_minus_one *= (check_node[current_round-1][(bit_node_index_save[0])[i][j][k]][(bit_node_index_save[1])[i][j][k]]);
                    temp_mult_plus_one *= (1.0-(check_node[current_round-1][(bit_node_index_save[0])[i][j][k]][(bit_node_index_save[1])[i][j][k]]));
                }
                double temp_q_minus_1 = ((bit_node[0][i][j])*temp_mult_minus_one);
                double temp_q_plus_1 = ((1.0-bit_node[0][i][j])*temp_mult_plus_one);
                bit_node[current_round][i][j] =  (temp_q_minus_1)/(temp_q_minus_1 + temp_q_plus_1);
            }
        }
    }
}
void decoder::APP_ratio_calc_simple(int current_round ,LDPC ldp,encoder enc)//Very Slow and fat due to consistently transmission of LDPC
{
    for(int i=0;i<ldp.get_column_num();i++){
        for (int j=0;j<ldp.get_column_ones_num();j++){
            if(ldp.get_bit_node_index()[i][j]!=0){//Make sure the bit-check node connection exist
                double temp_app_minus_1,temp_app_plus_1;
                temp_app_minus_1 = bit_node[current_round][i][j] * check_node[current_round-1][(app_index_save[0])[i][j][1]][(app_index_save[1])[i][j][1]];//qij(-1)*rji(-1)
                temp_app_plus_1 = (1-bit_node[current_round][i][j]) * (1-check_node[current_round-1][(app_index_save[0])[i][j][1]][(app_index_save[1])[i][j][1]]);//qij(+1)*rji(+1)
                APP_ratio[i][j] = (temp_app_minus_1)/(temp_app_minus_1 + temp_app_plus_1);
            }
        }
    }
}
void decoder::APP_ratio_calc_simple_simple(int current_round)
{
    for(int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            if(bit_node_index[i][j]!=0){//Make sure the bit-check node connection exist
                double temp_app_minus_1,temp_app_plus_1;
                temp_app_minus_1 = bit_node[current_round][i][j] * check_node[current_round-1][(app_index_save[0])[i][j][1]][(app_index_save[1])[i][j][1]];//qij(-1)*rji(-1)
                temp_app_plus_1 = (1-bit_node[current_round][i][j]) * (1-check_node[current_round-1][(app_index_save[0])[i][j][1]][(app_index_save[1])[i][j][1]]);//qij(+1)*rji(+1)
                APP_ratio[i][j] = (temp_app_minus_1)/(temp_app_minus_1 + temp_app_plus_1);
            }
        }
    }
}
void decoder::APP_ratio_calc_simple_simple_simple(int current_round)
{
    //Correct Method
    for(int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            if(bit_node_index[i][j]!=0){//Make sure the bit-check node connection exist
                double temp_app_minus_1,temp_app_plus_1;
                temp_app_minus_1 = bit_node[current_round][i][j] * check_node[current_round-1][(app_index_save[0])[i][j][1]][(app_index_save[1])[i][j][1]];//qij(-1)*rji(-1)
                temp_app_plus_1 = (1-bit_node[current_round][i][j]) * (1-check_node[current_round-1][(app_index_save[0])[i][j][1]][(app_index_save[1])[i][j][1]]);//qij(+1)*rji(+1)
                APP_ratio_real[i] = (temp_app_minus_1)/(temp_app_minus_1 + temp_app_plus_1);
                break;
            }
        }
    }
}
void decoder::output_result_soft(int cu_round)
{
    if(cu_round==1){
        //Output bit node
        cout << "-------BIT_NODE-----------" <<endl;
        for(int j=0;j<column_num;j++){
            cout << bit_node[0][j][0] << "  " ;
        }
        cout << endl;
        cout << "-------------------" << endl;
        cout << "-------APP_NODE-----------" <<endl;
        for(int i=0;i<column_num;i++){
            cout << APP_ratio[i][0] << "  " ;
        }
        cout << endl;
        cout << "-------------------" << endl;
    }
    if(cu_round>=2){
        //Output APP node
        cout << "-------APP_NODE-----------" <<endl;
        for(int i=0;i<column_num;i++){
            cout << APP_ratio_real[i] << "  " ;
        }
    cout << endl;
    }
}
