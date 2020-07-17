#include "some_func.h"

void strip(std::string str, std::string sym, std::vector<std::string> &sl)
{
   size_t pos = str.find(sym);
    bool done = false;
    for(; !done; pos = str.find(sym))
    {
        std::string subs;
        subs = str.substr(0, pos);
        sl.push_back(subs);
        str = str.substr(pos + 1);
        if(pos == std::string::npos)
            done = true;
    }
}

unsigned long long s2n(std::string str)
{
    unsigned long long n{};
    if(str.find('x') != std::string::npos)
    {
        n = std::stoull(str, 0,16);
        str.erase(str.find('x'), 1);
    }
    else if(str.find('o') != std::string::npos)
    {
        n = std::stoull(str, 0, 8);
        str.erase(str.find('o'), 1);
    }
    else if(str.find('b') != std::string::npos)
    {
        n = std::stoull(str, 0, 2);
        str.erase(str.find('b'), 1);
    }
    else
        n = std::stoull(str, 0);
    return n;
}
