#ifndef ENCODER_H
#define ENCODER_H
#include <vector>
#include <iostream>
#include "ldpc.h"


using namespace std;

class encoder
{
    private:
        vector<int> input_signal,input_modulated_signal;//signal before channel(output to decoder)
        vector<int> output_signal;//signal after channel(input to decoder)
        double AWGN_noise_variance;

    public:
        encoder();
        void set_parmeter(LDPC);
        void set_parmeter_BSC(LDPC);
        void set_parmeter_BSC_curve();
        void set_in_signal(vector<int>);
        void set_out_signal(vector<int>);
        void Modulation();
        void parm_output();
        vector<int> get_input_modulated(){return input_modulated_signal;}
        vector<int> get_input_signal(){return input_signal;}
        vector<int> get_output_signal(){return output_signal;}
        double get_AWGN_noise_variance(){return AWGN_noise_variance;}
};

#endif // ENCODER_H

