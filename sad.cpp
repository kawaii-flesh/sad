#include "sad.h"

SAD::SAD(const std::string &sig, long long tfs) : sad_error(false), tf_pos(0), tf_pos_old(0), tf_size(tfs), sig_nm(sig)
{
}

int SAD::get_signatures_param(char *sf_buff, char *tf_buff)
{
    std::string sf_b(sf_buff);

    size_t dpos = sf_b.find('<' + sig_nm + '>');
    dpos += 2 + sig_nm.size();
    if(dpos == std::string::npos)
    {
        std::cout << "Can't find sig container: " << sig_nm << "!\n";
        return -1;
    }
    std::string s = sf_b.substr(dpos, sf_b.find('{', dpos) - dpos - 1);
    dpos = s.find('(');
    size_t edpos = s.rfind(')');
    std::string descr = "";    
    if(dpos != std::string::npos && edpos != std::string::npos)
    {
        descr = s.substr(dpos + 1, edpos - dpos - 1) + '\n';
    }

    sf_b = delete_all_spaces(sf_b);
    long long cur_pos = sf_b.find('<' + sig_nm + '>');
    cur_pos = sf_b.find('{', cur_pos) + 1; // skip sig name and {
    for(; sf_b[cur_pos] != '}'; ++cur_pos)
    {
        std::string sb{};
        for(; sf_b[cur_pos] != ';'; ++cur_pos)
        {
            if(cur_pos >= sf_b.size())
            {
                std::cout << "Can't find ';'!\n";
                return -1;
            }
            sb += sf_b[cur_pos];
        }
        std::vector<std::string> svb;
        strip(sb, ":", svb);
        Signature sig(svb, tf_buff);
        sig.description = descr;
        if(sig.valid() == true)
            signatures.push_back(sig);
    }
    return 0;
}

bool SAD::check_one_sig(Signature &s, char *tf_buff)
{
    std::vector<std::string> search_bytes;
    strip(s.srch_expr, ",", search_bytes);
    int result = 0;

    tf_pos_old = tf_pos;
    if(s.direction == Direction::Absolute)
        tf_pos = s2n(s.pars_offset(s.offset, s.target_bf));
    if(s.direction == Direction::Forward || s.direction == Direction::Backward)
        tf_pos = tf_pos_old;
    if(s.direction == Direction::Presence)
        tf_pos = 0;

    if(s.direction == Direction::Absolute)
    {
        if(tf_pos + search_bytes.size() < tf_size)
        {
            for(std::string i : search_bytes)
            {
                if((int)static_cast<uint8_t>(tf_buff[tf_pos]) == s2n(i))
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
                if((int)static_cast<uint8_t>(tf_buff[tf_pos + k]) == s2n(j))
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
        for(long long i = 0; tf_pos + search_bytes.size() < tf_size; ++i)
        {
            long long k = 0;
            result = 0;
            for(std::string j : search_bytes)
            {
                if((int)static_cast<uint8_t>(tf_buff[tf_pos + k]) == s2n(j))
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
        for(long long i = tf_pos; i >= 0; --i)
        {
            long long k = 0;
            result = 0;
            for(std::string j : search_bytes)
            {
                if((int)static_cast<uint8_t>(tf_buff[tf_pos + k]) == s2n(j))
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
    {
        tf_pos += search_bytes.size();
        r = true;
    }
    if(s.invert)
        r = !r;
    if(r == false || s.invert || s.direction == Direction::Presence)
        tf_pos = tf_pos_old;    
    return r;
}

std::pair<double, int> SAD::check_sig(char *tf_buff)
{
    double total{};
    double result{};
    for(Signature s : signatures)
    {
        bool res = check_one_sig(s, tf_buff);
        if(res)
        {            
            result += s.est_wght;
        }
        total += s.est_wght;
        if(res == false and s.optional)
            total -= s.est_wght;
    }
    if(total == 0)
        return std::make_pair(0, 0);
    return std::make_pair(result * 100 / total, result);
}
