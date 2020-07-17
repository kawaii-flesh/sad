#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <iostream>
#include <vector>
#include "some_func.h"

enum Direction
{
    Absolute,
    Forward,
    Backward,
    Presence
};

enum Sig_errors
{
    Offset,
    Exp,
    Estw,
    Good
};

class Signature
{
public:
    Signature(std::vector<std::string> &s, char *tb);
    long long offset;
    std::string srch_expr;
    int est_wght;
    Direction direction;
    Sig_errors sig_err;
    bool invert;
private:
    std::string pars_gen(std::string, char *);
    std::string pars_offs(std::string, char *);
};
#endif // SIGNATURE_H
