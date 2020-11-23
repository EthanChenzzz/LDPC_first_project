#ifndef STAT_LDPC_H
#define STAT_LDPC_H
#include "encoder.h"
#include "ldpc.h"
#include "channel.h"
#include "decoder.h"
#include "algorithm"
#include "functional"
#include <time.h>

using namespace std;


class stat_ldpc
{
    private:
        int n_error,max_iteration;
        double n_cw;
        double avg_iteration, BLER;
        long int iteration_sum;
        vector<int> input_signal,error;

    public:
        stat_ldpc();
        void stat_count(channel,encoder,decoder,LDPC);
        void print_stat();

};

#endif // STAT_LDPC_H
