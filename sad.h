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
    bool sad_error;
private:
    std::ifstream sig_file;
    std::ifstream tar_file;
    char *sf_buff;
    char *tf_buff;
    unsigned long long tf_pos;
    unsigned long long tf_size;
    std::vector<Signature> signatures;
    std::string sig_nm;
};
#endif // SAD_H
