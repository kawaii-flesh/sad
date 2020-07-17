#include "sad.h"

SAD::SAD(std::string sig_fn, std::string tar_n, std::string sig) : sad_error(false), sf_buff(nullptr), tf_buff(nullptr), tf_pos(0), sig_nm(sig)
{
    sig_file.open(sig_fn, std::ios::binary);
    if(!sig_file.is_open())
    {
        std::cout << "Can't open file: " << sig_fn << '\n';
        sad_error = true;
        return;
    }
    unsigned long long sf_size = sig_file.seekg(0, std::ios::end).tellg();
    sig_file.seekg(0);
    sf_buff = new char[sf_size+1];
    sig_file.read(sf_buff, sf_size);

    tar_file.open(tar_n);
    if(!tar_file.is_open())
    {
        std::cout << "Can't open file: " << tar_n << '\n';
        sad_error = true;
        return;
    }
    tf_size = tar_file.seekg(0, std::ios::end).tellg();
    tar_file.seekg(0);
    tf_buff = new char[tf_size+1];
    tar_file.read(tf_buff, tf_size);
}

SAD::~SAD()
{
    delete sf_buff;
    delete tf_buff;
}

int SAD::get_signatures()
{
    std::string sf_b(sf_buff);
    unsigned long long cur_pos{0};
    for(unsigned long long p = sf_b.find(' '); p != std::string::npos; p = sf_b.find(' '))
        sf_b.erase(p, 1);
    for(unsigned long long p = sf_b.find('\t'); p != std::string::npos; p = sf_b.find('\t'))
        sf_b.erase(p, 1);
    for(unsigned long long p = sf_b.find('\n'); p != std::string::npos; p = sf_b.find('\n'))
        sf_b.erase(p, 1);
    cur_pos = sf_b.find('[' + sig_nm + ']');
    if(cur_pos == std::string::npos)
    {
        std::cout << "Can't find sig container: " << sig_nm << "!\n";
        return -1;
    }
    cur_pos += 3 + sig_nm.size(); // skip sig name and {
    for(; sf_b[cur_pos] != '}'; ++cur_pos)
    {
        std::string sb{};
        for(; sf_b[cur_pos] != ';'; ++cur_pos)
            sb += sf_b[cur_pos];
        std::vector<std::string> svb;
        strip(sb, ":", svb);
        Signature sig(svb, tf_buff);
        signatures.push_back(sig);
    }
    return 0;
}
