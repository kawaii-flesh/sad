#ifndef SOME_FUNC_H
#define SOME_FUNC_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

void strip(std::string str, std::string sym, std::vector<std::string> &sl);
long long s2n(std::string str);
std::string delete_all_spaces(std::string str);
void get_all_sig_names(std::string sigf, std::vector<std::string> &v);
#endif // SOME_FUNC_H
