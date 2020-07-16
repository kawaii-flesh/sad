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
