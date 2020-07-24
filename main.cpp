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
    std::ifstream sig_file;
    std::ifstream tar_file;

    char *tf_buff;
    char *sf_buff;
    sig_file.open(argv[1], std::ios::binary);
    if(!sig_file.is_open())
    {
        std::cout << "Can't open file: " << argv[1] << '\n';
        return -1;
    }
    long long sf_size = sig_file.seekg(0, std::ios::end).tellg();
    sig_file.seekg(0);
    sf_buff = new char[sf_size+1];
    sig_file.read(sf_buff, sf_size);

    tar_file.open(argv[2]);
    if(!tar_file.is_open())
    {
        std::cout << "Can't open file: " << argv[2] << '\n';
        return -1;
    }
    long long tf_size = tar_file.seekg(0, std::ios::end).tellg();
    tar_file.seekg(0);
    tf_buff = new char[tf_size+1];
    tar_file.read(tf_buff, tf_size);
    sig_file.close();
    tar_file.close();

    if(std::string(argv[3]) == "all")
    {
        std::vector<std::string> snm;
        get_all_sig_names(argv[1], snm);
        for(std::string i : snm)
        {
            SAD sad(i, tf_size);
            if(sad.sad_error)
            {
                std::cout << "Error!\n";
                return 1;
            }
            if(sad.get_signatures_param(sf_buff, tf_buff) != -1)
                cout << i << ": " <<  sad.check_sig(tf_buff).first << "% similarity\n";
        }
        return 0;
    }
    else if(std::string(argv[3]) == "id")
    {
        std::vector<std::string> snm;
        get_all_sig_names(argv[1], snm);
        for(std::string i : snm)
        {            
            SAD sad(i, tf_size);
            if(sad.sad_error)
            {
                std::cout << "Error!\n";
                return 1;
            }
            if(sad.get_signatures_param(sf_buff, tf_buff) != -1)
                if(sad.check_sig(tf_buff).first >= 100)
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
        std::vector<std::pair<std::string, std::pair<double, int>>> top;
        for(std::string i : snm)
        {
            SAD sad(i, tf_size);
            if(sad.sad_error)
            {
                std::cout << "Error!\n";
                return 1;
            }
            if(sad.get_signatures_param(sf_buff, tf_buff) != -1)
            {
                std::pair<double, int> r = sad.check_sig(tf_buff);
                if(r.second > 0)
                    top.push_back(std::make_pair(i, std::make_pair(r.first, r.second)));
            }
        }
        std::sort(top.begin(), top.end(),
                  [](std::pair<std::string, std::pair<double, int>> &a, std::pair<std::string, std::pair<double, int>> &b)->bool{return a.second.second > b.second.second;});

        if(std::string(argv[3]).size() > 3)
        {
            size_t n = s2n(std::string(argv[3]).substr(3));
            if(top.size() > n)
                top.resize(n);
        }
        for(std::pair<std::string, std::pair<double, int>> &p : top)
        {
            cout << p.first << ": " << p.second.first << "% similarity\n";
        }
        return 0;
    }
    else
    {
        SAD sad(argv[3], tf_size);
        if(sad.sad_error)
        {
            std::cout << "Error!\n";
            return 1;
        }
        if(sad.get_signatures_param(sf_buff, tf_buff) != -1)
        {
            cout << argv[3] << ": " << sad.check_sig(tf_buff).first << "% similarity\n";
            cout << "Description:\n";
            cout << sad.get_signatures_vector()[0].description;
        }
        return 0;
    }
    delete [] sf_buff;
    delete [] tf_buff;
}
