#include <iostream>
#include <algorithm>
#include "sad.h"
#include "some_func.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        std::cout << "sad [sig_file] [target_file] [sig_name/id/all/topn]\n";
        return 1;
    }
    if(std::string(argv[3]) == "all")
    {
        std::vector<std::string> snm;
        get_all_sig_names(argv[1], snm);
        for(std::string i : snm)
        {
            SAD sad(argv[1], argv[2], i);
            if(sad.sad_error)
            {
                std::cout << "Error!\n";
                return 1;
            }
            if(sad.get_signatures() != -1)
                cout << i << ": " <<  sad.check_sig() << "% similarity\n";
        }
        return 0;
    }
    else if(std::string(argv[3]) == "id")
    {
        std::vector<std::string> snm;
        get_all_sig_names(argv[1], snm);
        for(std::string i : snm)
        {
            SAD sad(argv[1], argv[2], i);
            if(sad.sad_error)
            {
                std::cout << "Error!\n";
                return 1;
            }
            if(sad.get_signatures() != -1)
                if(sad.check_sig() >= 100)
                {
                    cout << i << ": 100% similarity\n";
                    cout << "Description:\n";
                    cout << sad.get_signatures_vector()[0].description;
                    return 0;
                }
        }
        return 0;
    }
    else if(std::string(argv[3]).substr(0,3) == "top")
    {
        std::vector<std::string> snm;
        get_all_sig_names(argv[1], snm);
        std::vector<std::pair<std::string, double>> top;
        for(std::string i : snm)
        {
            SAD sad(argv[1], argv[2], i);
            if(sad.sad_error)
            {
                std::cout << "Error!\n";
                return 1;
            }
            if(sad.get_signatures() != -1)
            {
                double r = sad.check_sig();
                if(r > 0)
                    top.push_back(std::make_pair(i, r));
            }
        }
        std::sort(top.begin(), top.end(),
                  [](std::pair<std::string, double> &a, std::pair<std::string, double> &b)->bool{return a.second > b.second;});

        if(std::string(argv[3]).size() > 3)
        {
            size_t n = s2n(std::string(argv[3]).substr(3));
            if(top.size() > n)
                top.resize(n);
        }
        for(std::pair<std::string, double> &p : top)
        {
            cout << p.first << ": " << p.second << "% similarity\n";
        }
        return 0;
    }
    else
    {
        SAD sad(argv[1], argv[2], argv[3]);
        if(sad.sad_error)
        {
            std::cout << "Error!\n";
            return 1;
        }
        if(sad.get_signatures() != -1)
        {
            cout << argv[3] << ": " << sad.check_sig() << "% similarity\n";
            cout << "Description:\n";
            cout << sad.get_signatures_vector()[0].description;
        }
        return 0;
    }
}
