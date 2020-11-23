#ifndef DECODER_H
#define DECODER_H
#include <math.h>
#include <iostream>
#include <vector>
#include "ldpc.h"
#include "encoder.h"
#include "channel.h"
#include <time.h>


using namespace std;

class decoder
{
    private:
         vector<vector<vector<double>>> bit_node,check_node; //2d vector, save the result of every iteration
                                                        //save only the index
                                                        //(ex.0.98,0.34,0.23....) first Iteration
                                                        //(ex.0.99,0,35,0,24...) second iteration
                                                        //the corresponding index reference to the parity check matrix
                                                        //bit node and check node are transpose matrix
         vector<vector<double>> bit_node_temp,check_node_temp;
         vector<vector<vector<int>>> check_node_index_save[2],bit_node_index_save[2],app_index_save[2];
         vector<vector<int>> check_node_index_save_temp, bit_node_index_save_temp,app_index_save_temp;
         vector<vector<double>> APP_ratio; //
         vector<double> APP_ratio_real; //
         vector<int> hard_decision;//hard_decision_one, hard_decision_two;
         int iterative_round;
         double check_to_bit_time,bit_to_check_time,app_time,hard_time;
         int column_num,row_num,column_ones_num,row_ones_num;
         vector<vector<int>> bit_node_index,check_node_index;
         double BER_rate;
         vector<int> input_signal,output_signal;


    public:
        decoder();
        void set_decoder(LDPC,channel,encoder);
        void set_decoder_enc_out(encoder);
        int belief_propagation(int,int);
        void Initialization();
        void check_to_bit(int);
        void check_to_bit_simple(int,LDPC,encoder);
        void check_to_bit_simple_simple(int);
        void bit_to_check(int);
        void bit_to_check_simple(int,LDPC,encoder);
        void bit_to_check_simple_simple(int);
        void APP_ratio_calc(int);
        void APP_ratio_calc_simple(int,LDPC,encoder);
        void APP_ratio_calc_simple_simple(int);
        void APP_ratio_calc_simple_simple_simple(int);
        void Hard_Decision();
        void Hard_Decision_simple();
        void output_result();
        void output_result_soft(int round);
        bool decode_success();

};

#endif // DECODER_H
