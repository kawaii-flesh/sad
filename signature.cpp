#include "signature.h"

Signature::Signature(std::vector<std::string> &s, char *tb) : direction(Direction::Absolute), invert(false), optional(false), target_bf(tb), sig_err(Sig_errors::Good)
{
    std::string est_wt;
    set_offs_type(s[0]);
    offset = s[0];
    est_wt = s[2];
    try
    {
        est_wght = std::stoi(est_wt);
    }
    catch(std::invalid_argument)
    {
        sig_err = Sig_errors::Estw;
    }
    catch(std::out_of_range)
    {        
        sig_err = Sig_errors::Estw;
    }
    srch_expr = s[1];
}

bool Signature::valid()
{
    if(sig_err == Sig_errors::Estw)
    {
        std::cout << "Estimated weight in signature wrong!\n";
        return false;
    }
    if(sig_err == Sig_errors::Offset)
    {
        std::cout << "Offset in signature wrong!\n";
        return false;
    }
    return true;
}

void Signature::set_offs_type(std::string &str)
{
    long long p = str.find('!');
    if(p != std::string::npos)
    {
        str.erase(p, 1);
        invert = true;
    }
    p = str.find('~');
    if(p != std::string::npos)
    {
        str.erase(p, 1);
        optional = true;
    }
    p = str.find('@');
    if(p != std::string::npos)
    {
        str.erase(p, 1);
        direction = Direction::Absolute;
    }
    p = str.find('-');
    if(p != std::string::npos)
    {
        str.erase(p, 1);
        direction = Direction::Backward;
    }
    p = str.find('+');
    if(p != std::string::npos)
    {
        str.erase(p, 1);
        direction = Direction::Forward;
    }
    p = str.find('?');
    if(p != std::string::npos)
    {
        str.erase(p, 1);
        direction = Direction::Presence;
    }
}

std::string Signature::pars_gen(std::string str, char *tb)
{
    size_t p = str.find('[');
    if(p != std::string::npos)
    {
        long long e = str.find(']');
        if(e == std::string::npos)
        {
            sig_err = Sig_errors::Offset;
            return str;
        }
        str.replace(p, e + 1, str.substr(p + 1, e - 1));
        int size_r{1};
        long long off{};
        bool endian{false}; // false - little, true - big
        try
        {
            size_t pr;
            pr = str.find('L');
            if(pr != std::string::npos)
            {
                str.erase(pr, 1);
                endian = false;
            }
            pr = str.find('B');
            if(pr != std::string::npos)
            {
                str.erase(pr, 1);
                endian = true;
            }
            pr = str.find('c');
            if(pr != std::string::npos)
            {
                str.erase(pr, 1);
                size_r = 1;
            }
            pr = str.find('w');
            if(pr != std::string::npos)
            {
                str.erase(pr, 1);
                size_r = 2;
            }
            pr = str.find('d');
            if(pr != std::string::npos)
            {
                str.erase(pr, 1);
                size_r = 4;
            }
            pr = str.find('q');
            if(pr != std::string::npos)
            {
                str.erase(pr, 1);
                size_r = 8;
            }
            off = s2n(str);
        }
        catch(std::invalid_argument)
        {
            std::cout << "Value wrong!\n";
            sig_err = Sig_errors::Offset;
            return str;
        }
        catch(std::out_of_range)
        {
            std::cout << "Value wrong!\n";
            sig_err = Sig_errors::Offset;
            return str;
        }
        if(endian == false)
        {
            if(size_r == 1)
                str = std::to_string(uint8_t(tb[off]));
            else if(size_r == 2)
                str = std::to_string(uint16_t(tb[off]) + (uint16_t(tb[off+1]) << 8));
            else if(size_r == 4)
                str = std::to_string(uint32_t(tb[off]) + (uint32_t(tb[off+1]) << 8) + (uint32_t(tb[off+2]) << 16)
                        + (uint32_t(tb[off+3]) << 24));
            else if(size_r == 8)
                str = std::to_string(uint64_t(tb[off]) + (uint64_t(tb[off+1]) << 8) + (uint64_t(tb[off+2]) << 16)
                        + (uint64_t(tb[off+3]) << 24) + (uint64_t(tb[off+4]) << 32) + (uint64_t(tb[off+5]) << 40)
                        + (uint64_t(tb[off+6]) << 48) + (uint64_t(tb[off+7]) << 56));
        }
        else
        {
            if(size_r == 1)
                str = std::to_string(uint8_t(tb[off]));
            else if(size_r == 2)
                str = std::to_string(uint16_t(tb[off+1]) + (uint16_t(tb[off]) << 8));
            else if(size_r == 4)
                str = std::to_string(uint32_t(tb[off+3]) + (uint32_t(tb[off+2]) << 8) + (uint32_t(tb[off+1]) << 16)
                        + (uint32_t(tb[off]) << 24));
            else if(size_r == 8)
                str = std::to_string(uint64_t(tb[off+7]) + (uint64_t(tb[off+6]) << 8) + (uint64_t(tb[off+5]) << 16)
                        + (uint64_t(tb[off+4]) << 24) + (uint64_t(tb[off+3]) << 32) + (uint64_t(tb[off+2]) << 40)
                        + (uint64_t(tb[off+1]) << 48) + (uint64_t(tb[off]) << 56));
        }
    }
    return str;
}

std::string Signature::pars_offset(const std::string &str, char *tb)
{    
    if(direction != Direction::Absolute)
    {
        return "0";
    }
    return pars_gen(str, tb);
}
