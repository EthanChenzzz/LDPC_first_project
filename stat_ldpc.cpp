#include "stat_ldpc.h"

stat_ldpc::stat_ldpc()
{

}
void stat_ldpc::stat_count(channel ch,encoder enc,decoder dec, LDPC ldp)
{
    vector<int> in_sig;
    in_sig.resize(ldp.get_column_num(),0);
    enc.set_in_signal(in_sig);//[0,0,0,0,....]
    input_signal = enc.get_input_signal();
    iteration_sum = 0;
    n_cw = 0; n_error = 0;
    dec.set_decoder(ldp,ch,enc);

    while(1)
    {
        //cout << (double)clock() << endl;
        ch.rand_num_gen(ldp.get_column_num()); //set random noise
        error = ch.get_error();
        vector<int> out_sig_temp;
        out_sig_temp.resize(ldp.get_column_num(),0);
        //transform (enc.get_input_signal().begin(), enc.get_input_signal().end(), ch.get_error().begin(), out_sig_temp.begin(), [](int e1, int e2) {return e1 ^ e2;});//add signal & noise(XOR)
        //for(int i=0;i<ldp.get_column_num();i++)
        //    out_sig_temp[i] = enc.get_input_signal()[i] ^ ch.get_error()[i];
        bool flag = false;
        for(int i=0;i<ldp.get_column_num();i++){
            out_sig_temp[i] = input_signal[i] ^ error[i];
            if(error[i] != 0)
                flag = true;
        }
        enc.set_out_signal(out_sig_temp);
        dec.set_decoder_enc_out(enc);
        n_cw = n_cw + 1;
        cout << n_cw << " ";
        int iteration_temp = 0;
        if((flag == true) | (flag == false && n_cw == 1))
                iteration_temp = dec.belief_propagation(10,n_cw);

        cout << iteration_temp << endl;
        iteration_sum = iteration_sum + iteration_temp;

        if(iteration_temp == 10){
            n_error = n_error + 1;
            cout << "ERROR" << n_error << endl;
        }
        if(n_error == 100)
            break;
    }
    BLER = n_error/n_cw;
    avg_iteration = iteration_sum / n_cw;
}
void ::stat_ldpc::print_stat()
{
    cout << "Iteration Sum:" <<iteration_sum << endl;
    cout << "AVG:" << avg_iteration << endl;
    cout << "n_error" << n_error << endl;
    cout << "n_cw" << n_cw << endl;
}

