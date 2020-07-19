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
    std::string offset;
    std::string srch_expr;
    int est_wght;
    Direction direction;
    Sig_errors sig_err;
    bool invert;
    bool optional;
    char *target_bf;

    void get_offs_type(std::string &);
    std::string pars_gen(std::string, char *);
    std::string pars_offset(std::string, char *);
};
#endif // SIGNATURE_H
