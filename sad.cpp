#include "sad.h"

SAD::SAD(std::string sig_fn, std::string tar_n, std::string sig) : sad_error(false), sf_buff(nullptr), tf_buff(nullptr), tf_pos(0), tf_pos_old(0), sig_nm(sig)
{
    sig_file.open(sig_fn, std::ios::binary);
    if(!sig_file.is_open())
    {
        std::cout << "Can't open file: " << sig_fn << '\n';
        sad_error = true;
        return;
    }
    long long sf_size = sig_file.seekg(0, std::ios::end).tellg();
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
    long long cur_pos{0};
    for(long long p = sf_b.find(' '); p != std::string::npos; p = sf_b.find(' '))
        sf_b.erase(p, 1);
    for(long long p = sf_b.find('\t'); p != std::string::npos; p = sf_b.find('\t'))
        sf_b.erase(p, 1);
    for(long long p = sf_b.find('\n'); p != std::string::npos; p = sf_b.find('\n'))
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

bool SAD::check_one_sig(Signature &s)
{
    std::vector<std::string> search_bytes;
    strip(s.srch_expr, ",", search_bytes);
    int result = 0;
    if(s.direction == Direction::Absolute)
    {
        tf_pos = s2n(s.pars_offset(s.offset, s.target_bf));
    }
    else if(s.direction == Direction::Presence)
    {
        tf_pos = 0;
    }
    tf_pos_old = tf_pos;

    if(s.direction == Direction::Absolute)
    {
        if(tf_pos + search_bytes.size() < tf_size)
        {
            for(std::string i : search_bytes)
            {
                if(tf_buff[tf_pos] == s2n(i))
                    ++result;
                ++tf_pos;
            }
        }
    }
    else if(s.direction == Direction::Presence)
    {
        for(long long i = 0; i + search_bytes.size() < tf_size; ++i)
        {
            long long k = 0;
            result = 0;
            for(std::string j : search_bytes)
            {
                if(tf_buff[tf_pos + k] == s2n(j))
                    ++result;
                ++k;
            }
            if(result == search_bytes.size())
                break;
            ++tf_pos;
        }
    }
    else if(s.direction == Direction::Forward)
    {
        for(long long i = 0; i + search_bytes.size() < tf_size; ++i)
        {
            long long k = 0;
            result = 0;
            for(std::string j : search_bytes)
            {
                if(tf_buff[tf_pos + k] == s2n(j))
                    ++result;
                ++k;
            }
            if(result == search_bytes.size())
                break;
            ++tf_pos;
        }
    }
    else if(s.direction == Direction::Backward)
    {
        for(long long i = tf_pos; i - search_bytes.size() >= 0; --i)
        {
            long long k = 0;
            result = 0;
            for(std::string j : search_bytes)
            {
                if(tf_buff[tf_pos + k] == s2n(j))
                    ++result;
                ++k;
            }
            if(result == search_bytes.size())
                break;
            --tf_pos;
        }
    }
    bool r = false;
    if(result == search_bytes.size())
        r = true;
    if(s.invert)
        r = !r;
    if(r == false || s.invert || s.direction == Direction::Presence)
        tf_pos = tf_pos_old;
    return r;
}

double SAD::check_sig()
{
    double total{};
    double prcnt{};
    for(Signature s : signatures)
    {
        total += s.est_wght;
        if(check_one_sig(s))
        {
            prcnt += s.est_wght;
        }
    }
    return prcnt * 100 / total;
}
