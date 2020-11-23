#include "encoder.h"

encoder::encoder()
{

}
void encoder::set_parmeter(LDPC temp)
{
    //Input Signal
    cout << "Please input input signal (Before Channel)" << endl;
    input_signal.resize(temp.get_column_num(),0);
    for (int i=0;i<temp.get_column_num();i++){
        cin >> input_signal[i];
    }

    //Output Signal
    cout << "Please input output signal (After Channel)" << endl;
    output_signal.resize(temp.get_column_num(),0);
    for (int i=0;i<temp.get_column_num();i++){
        cin >> output_signal[i];
    }

    //Noise Variation
    cout << "Please input AWGN Channel Variation" << endl;
    cin >> AWGN_noise_variance;
}
void encoder::set_in_signal(vector<int> in_sig)
{
    input_signal = in_sig;
}
void encoder::set_out_signal(vector<int> out_sig)
{
    output_signal = out_sig;
}
void encoder::Modulation()
{
    input_modulated_signal.resize(input_signal.size(),0);
    for (int i=0;i<input_signal.size();i++){
        if(input_signal[i] == 1)
            input_modulated_signal[i] = -1;
        if(input_signal[i] == 0)
            input_modulated_signal[i] = 1;
    }
}
void encoder::parm_output()
{
    cout << "input signal (Before Channel)" << endl;
    for (int i=0;i<input_signal.size();i++){
        cout << input_signal[i] << " ";
    }
    cout << endl;
    cout << "input modulated signal (Before Channel)" << endl;
    for (int i=0;i<input_modulated_signal.size();i++){
        cout << input_modulated_signal[i] << " ";
    }
    cout << endl;
    cout << "output signal (After Channel)" << endl;
    for (int i=0;i<output_signal.size();i++){
        cout << output_signal[i] << " ";
    }
    cout << endl;
    cout << "NOISE:" << AWGN_noise_variance << endl;
}
void encoder::set_parmeter_BSC(LDPC temp)
{
    input_signal.resize(temp.get_column_num(),0);
    output_signal.resize(temp.get_column_num(),0);
}
void encoder::set_parmeter_BSC_curve()
{
    input_signal.resize(14,0);
    output_signal.resize(14,0);
    output_signal[2] = 1;
}
