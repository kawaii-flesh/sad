#include <iostream>
#include "sad.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        std::cout << "sad [sig_file] [target_file] [sig_name]\n";
        return 1;
    }
    else
    {
        SAD sad(argv[1], argv[2], argv[3]);
        if(sad.sad_error)
        {
            std::cout << "Error!\n";
            return 1;
        }
        sad.get_signatures();
        cout << sad.check_sig() << "% similarity\n";
        return 0;
    }
}
