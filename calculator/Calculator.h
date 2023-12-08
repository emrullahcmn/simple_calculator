#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>

class Calculator
{
    private:
    std::string expression;
    double pi_to_result[1000];
    std::vector<std::pair<int, int>> paranthesis;
    std::vector<std::vector<std::string>> pi_parsed;
    std::vector<int> order;
    int sti_to_p[1000];

    double get_val(std::string& s);
    void put_val(std::string& s, double val);
    void divide();
    void multiply(int p_i);
    void add(int p_i);
    void parse(int p_index, int p_st_index, int p_end_index, std::string& s);
    void check_parse_error(std::vector<std::string>& now_parsed);
    bool check_if_number(std::string& s1);
    void apply_paranthesis_precedence(std::string &s);
    void check_paranthesis(std::string& s);
    void check_characters(std::string& s);
    void trim(std::string& s);
    
    public:
    Calculator();
    void run(std::string& str);
    void clear_all();
};

#endif