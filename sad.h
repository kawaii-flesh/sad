#ifndef SAD_H
#define SAD_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "signature.h"
#include "some_func.h"

class SAD
{
public:
    SAD(std::string sig, long long tfs);
    int get_signatures_param(char *sf_buff, char *tf_buff);
    bool check_one_sig(Signature &, char *tf_buff);
    std::pair<double, int> check_sig(char *tf_buff); // get %
    bool sad_error;
    std::vector<Signature> &get_signatures_vector(){return signatures;}    
private:    
    long long tf_pos;
    long long tf_pos_old;
    long long tf_size;
    std::vector<Signature> signatures;
    std::string sig_nm;
};
#endif // SAD_H
