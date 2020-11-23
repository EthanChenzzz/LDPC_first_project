#include <iostream>
#include "ldpc.h"
#include "encoder.h"
#include "decoder.h"
#include "channel.h"
#include "stat_ldpc.h"
using namespace std;

int main()
{
    //STAT COUNT RELATED//

    LDPC test;
    test.set_parity_check_matrix_from_file();
    //test.print_parity_check_matrix();

    encoder test1;
    test1.set_parmeter_BSC(test);
    test1.Modulation();
    test1.parm_output();

    channel ch;
    ch.set_BSC_BER(0.119);
    ch.set_seed(18909);

    decoder test2;

    //Stat Count
    stat_ldpc final;
    final.stat_count(ch,test1,test2,test);
    final.print_stat();

/*
    //PRINT SOFT DECISION RESULT//

    LDPC test;
    test.set_parity_check_matrix_from_file();

    encoder test1;
    test1.set_parmeter_BSC_curve();

    channel ch;
    ch.set_BSC_BER(0.26);

    decoder test2;
    test2.set_decoder(test,ch,test1);
    test2.set_decoder_enc_out(test1);
    int round = test2.belief_propagation(100,1);
    cout << round;
*/
    return 0;
}
