#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <iostream>
#include <vector>

class Signature
{
public:
    Signature(std::vector<std::string> &s) : sig_error(false)
    {
        std::string offs, se, est_wt;
        offs = s[0];
        est_wt = s[2];
        try
        {
            offset = std::stoull(offs);
        }
        catch(std::invalid_argument)
        {
            std::cout << "Offset in signature wrong!\n";
            sig_error = true;
        }
        catch(std::out_of_range)
        {
            std::cout << "Offset in signature wrong!\n";
            sig_error = true;
        }
        try
        {
            est_wght = std::stoi(est_wt);
        }
        catch(std::invalid_argument)
        {
            std::cout << "Estimated weight in signature wrong!\n";
            sig_error = true;
        }
        catch(std::out_of_range)
        {
            std::cout << "Estimated weight in signature wrong!\n";
            sig_error = true;
        }
        srch_expr = se;
    }
    long long offset;
    std::string srch_expr;
    int est_wght;
    bool sig_error;
};
#endif // SIGNATURE_H
