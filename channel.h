#ifndef CHANNEL_H
#define CHANNEL_H
#include <vector>
#include <math.h>
#include <stdint.h>
#include <iostream>

using namespace std;

class channel
{
    private:
        double BSC_BER;
        vector<int> error;
        //For RNG Use
        uint64_t x;
        uint64_t s[4];
        uint64_t z;

    public:
        channel();
        void set_BSC_BER(double BER){BSC_BER = BER;}
        void rand_num_gen(int);
        double get_BER(){return BSC_BER;}
        vector<int> get_error(){return error;}
        void set_seed(uint64_t seed_num){x = seed_num;}
        void set_s(){s[0] = init64();s[1] = init64(); s[2] = init64(); s[3] = init64();}
        uint64_t init64();
        uint64_t rotl(const uint64_t,int);
        uint64_t next256(void);
};

#endif // CHANNEL_H
