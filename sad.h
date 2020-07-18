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
    SAD(std::string sig_fn, std::string tar_n, std::string sig);
    ~SAD();
    int get_signatures();
    bool check_one_sig(Signature &);
    double check_sig(); // get %
    bool sad_error;
private:
    std::ifstream sig_file;
    std::ifstream tar_file;
    char *sf_buff;
    char *tf_buff;
    long long tf_pos;
    long long tf_pos_old;
    long long tf_size;
    std::vector<Signature> signatures;
    std::string sig_nm;
};
#endif // SAD_H
