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

long long s2n(std::string str)
{
    long long n{};
    if(str.find('x') != std::string::npos)
    {
        n = std::stoll(str, 0,16);
        str.erase(str.find('x'), 1);
    }
    else if(str.find('o') != std::string::npos)
    {
        n = std::stoll(str, 0, 8);
        str.erase(str.find('o'), 1);
    }
    else if(str.find('b') != std::string::npos)
    {
        n = std::stoll(str, 0, 2);
        str.erase(str.find('b'), 1);
    }
    else
        n = std::stoll(str, 0);
    return n;
}

std::string delete_all_spaces(std::string str)
{
    for(long long p = str.find(' '); p != std::string::npos; p = str.find(' '))
        str.erase(p, 1);
    for(long long p = str.find('\t'); p != std::string::npos; p = str.find('\t'))
        str.erase(p, 1);
    for(long long p = str.find('\n'); p != std::string::npos; p = str.find('\n'))
        str.erase(p, 1);
    return str;
}

void get_all_sig_names(std::string sigf, std::vector<std::string> &v)
{
    std::ifstream sig_file(sigf, std::ios::binary);
    if(!sig_file.is_open())
    {
        std::cout << "Can't open file: " << sigf << '\n';
        return;
    }
    long long sf_size = sig_file.seekg(0, std::ios::end).tellg();
    sig_file.seekg(0);
    char *sf_buff = new char[sf_size+1];
    sig_file.read(sf_buff, sf_size);
    sig_file.close();

    std::string srch = delete_all_spaces(std::string(sf_buff));

    for(unsigned long long i = 0; i < srch.size(); ++i)
    {
        std::string sn;
        if(srch[i] == '<')
        {
            for(int j = i + 1; srch[j] != '>'; ++j)
            {
                sn += srch[j];
            }
            v.push_back(sn);
            sn.clear();
        }
    }
    delete [] sf_buff;
}
