#include "channel.h"

channel::channel()
{

}
void channel::rand_num_gen(int length)
{
    set_s();
    int calc = 0;
    error.resize(length,0);
    z = 18446744073709551615; //2^64 - 1
    //cout << z << endl;
    //cout << s[0] << " " << s[1] << " " << s[2] << " " << s[3] << endl;
    double temp;
    for(int i=0;i<length;i++)
    {
        temp = double(next256());
        if(temp/z < BSC_BER){
            error[i] = 1;
            calc = calc + 1;
        }
        else
            error[i] = 0;
    }
    /*
    for(int i=0;i<length;i++)
        cout << error[i] << endl;
    cout << calc;
    */

}
uint64_t channel::rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}
uint64_t channel::init64()
{
    uint64_t z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}
uint64_t channel::next256(void) {
    const uint64_t result = rotl(s[1] * 5, 7) * 9;

    const uint64_t t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = rotl(s[3], 45);

    return result;
}
