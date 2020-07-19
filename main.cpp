#include <iostream>
#include "sad.h"
#include "some_func.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        std::cout << "sad [sig_file] [target_file] [sig_name/id/all]\n";
        return 1;
    }
    if(std::string(argv[3]) == "all")
    {
        std::vector<std::string> snm;
        get_all_sig_names(argv[1], snm);
        for(auto i : snm)
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
        for(auto i : snm)
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
                    return 0;
                }
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
            cout << argv[3] << ": " << sad.check_sig() << "% similarity\n";
        return 0;
    }
}
