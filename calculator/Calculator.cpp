#include <iostream>
#include <vector> 
#include <string>

#include "./Calculator.h"
#include "../error_handling/ParseError.h"

#define N 1000

using namespace std; 


double Calculator::get_val(string& s){
    int n = s.length();
    if(n < 2){
        return stod(s);
    }
    if(s[1] == 'p'){
        int p_i = stoi(s.substr(2));
        return (s[0] == '-' ? -pi_to_result[p_i] : pi_to_result[p_i]);
    }
    return stod(s);
}

void Calculator::put_val(string& s, double val){
    int n = s.length();
    if(n < 2){
        s = to_string(val);
        return;
    }
    if(s[1] == 'p'){
        int p_i = stod(s.substr(2));
        pi_to_result[p_i] = val;
        s[0] = '+'; 
        return;
    }
    s = to_string(val);
}

void Calculator::divide(){
    for(auto& p_i: order){
        vector<string> &inside = pi_parsed[p_i];
        for(int i = 1; i<inside.size()-1; i++){
            if(inside[i] == "/"){
                double a = get_val(inside[i-1]);
                double b = get_val(inside[i+1]);
                put_val(inside[i-1], 1);
                inside[i] = "*";
                put_val(inside[i+1], a/b);
            }
        }
        multiply(p_i);
    }
}

void Calculator::multiply(int p_i){

    vector<string> &inside = pi_parsed[p_i];
    for(int i = 1; i<inside.size()-1; i++){
        if(inside[i] == "*"){
            double a = get_val(inside[i-1]);
            double b = get_val(inside[i+1]);
            
            put_val(inside[i-1], 0);
            inside[i] = "+";
            put_val(inside[i+1], a*b);
        }
    }
    add(p_i);

}

void Calculator::add(int p_i){
    vector<string> &inside = pi_parsed[p_i];

    double sum = 0;
    if(inside.size() == 1) sum = get_val(inside[0]);
    if(inside.size() == 2) sum = get_val(inside[1]);

    for(int i = 1; i<inside.size()-1; i++){
        if(inside[i] == "+"){
            double a = get_val(inside[i-1]);
            double b = get_val(inside[i+1]);
            
            put_val(inside[i+1], a+b);
            sum = get_val(inside[i+1]);
        }
    }
    pi_to_result[p_i] = sum;
}

bool Calculator::check_if_number(string& s1){
    if(s1.size() > 2){
        if(s1[1] == 'p') return true; 
    }
    try{
        stod(s1);
    }catch(...){
        return false;
    } 
    return true;
}

void Calculator::check_parse_error(vector<string>& now_parsed){
    if(now_parsed.size() == 0){
        throw Parse_error{"Empty Paranthesis Is Not Allowed!"};
    }

    if(now_parsed.size() == 1){
        if(!check_if_number(now_parsed[0])){
            throw Parse_error{"Unexpected Operator In Paranthesis!"};
        }
    }

    for(int i = 1; i<now_parsed.size(); i++){
        if(now_parsed[i] == "*" || now_parsed[i] == "+" || now_parsed[i] == "/"){
            
            if( !check_if_number(now_parsed[i-1]) ){
                throw Parse_error{"Operator Usage Without Appropriate Operand!"};
            }else if( i + 1 == now_parsed.size() || !check_if_number(now_parsed[i+1])){
                throw Parse_error{"Operator Usage Without Appropriate Operand!"};
            }

        }else{
            
            if(now_parsed[i-1] != "*" && now_parsed[i-1] != "+" && now_parsed[i-1] != "/"){
                throw Parse_error{"Number Usage Without Appropriate Operand!"};

            }else if(now_parsed[i-1] == "*" || now_parsed[i-1] == "/"){
                if(i-2 < 0 || !check_if_number(now_parsed[i-2] )){
                    throw Parse_error{"Operator Usage Without Appropriate Number!"};
                }
            }

        }
    }
}

void Calculator::parse(int p_index, int p_st_index, int p_end_index, string& s){

    int nums = 0;
    int ops = 0;

    vector<string> now_parsed;
    bool neg = false;
    for(int i = p_st_index+1; i<p_end_index; i++){
        if(s[i] == '-'){
            now_parsed.push_back("+");
            neg = true;
        }
        if(s[i] == '*' || s[i] == '+' || s[i] == '/'){
            string strq = "";
            strq += s[i];
            now_parsed.push_back(strq);
            continue;
        }

        if(s[i] == '('){
            if(neg){
                now_parsed.push_back("-p"+to_string(sti_to_p[i]));
                neg = false;
            }else{
                now_parsed.push_back("+p"+to_string(sti_to_p[i]));
            }
            i = paranthesis[sti_to_p[i]].second;
            continue;
        }

        if(s[i] >= '0' && s[i] <= '9'){
            int ind = i;
            string nstr = "";
            if(neg){
                nstr = "-";
                neg = false;
            }

            while(ind < p_end_index && s[ind] >= '0' && s[ind] <= '9'){
                nstr += s[ind];
                ind++;
            }
    
            i = ind - 1,
            now_parsed.push_back(nstr);
        }
    }
    check_parse_error(now_parsed);

    pi_parsed[p_index] = now_parsed;

}

void Calculator::apply_paranthesis_precedence(string &s){
    int p_i = -1;
    vector<pair<int, int>> last_open;
    for(int i = 0; i<s.length(); i++){
        if(s[i] == '(') {
            p_i++;
            last_open.push_back(make_pair(p_i, i));
            sti_to_p[i] = p_i;
        }else if(s[i] == ')'){
            int p_index = last_open.back().first;
            int p_st_index = last_open.back().second;
            paranthesis[p_index] = make_pair(p_st_index, i);
            order.push_back(p_index);
            last_open.pop_back();
            parse(p_index, p_st_index, i, s);
        }
    }
}



void Calculator::check_paranthesis(string &s){
    int open = 0;

    for(int i = 0; i<s.length(); i++){
        if(s[i] == '(') open++;
        else if(s[i] == ')') open--;
        if(open < 0) throw Parse_error{"Closing Unopened Paranthesis Error!"};
    }
    if(open > 0) throw Parse_error{"Unclosed Paranthesis Error!"};
}

void Calculator::check_characters(string& s){
    for(int i = 0; i<s.length(); i++){
        if(s[i] >= '0' && s[i] <= '9') continue;
        if(s[i] == '+' || s[i] == '/' || s[i] == '*' || s[i] == '-' || s[i] == '(' || s[i] == ')') continue;

        throw Parse_error{"Input Contains UNKNOWN Character Error!"};
        return;
    }
}


void Calculator::trim(string &s){
    auto ls = s.find_first_not_of(" ");
    auto rs = s.find_last_not_of(" ");
    s = s.substr(ls, rs - ls + 1);
    if(s.length() == 0) throw Parse_error{"Expression Is Empty!"};

    for(int i = 1; i<s.length(); i++){
        if(s[i] == ' ' && s[i-1] >= '0' && s[i-1] <= '9'){
            int ix = i+1;
            bool op = false;
            while(ix < s.length() && (s[ix] < '0' || s[ix] > '9')){
                if(s[ix] == '*' || s[ix] == '/' || s[ix] == '+' || s[ix] == '-' || s[ix] == '(' || s[ix] == ')'){
                    op = true;
                }
                ix++;
            }
            i = ix;
            if(ix < s.length()){
                if(!op){
                    throw Parse_error{"Subsequent Numbers Have No Operator Between!"};
                }
            }
        }
    }

    string nstr = "";
    for(int i = 0; i<s.length(); i++){
        if(s[i] != ' ')
            nstr.push_back(s[i]);
    }
    s = nstr;

    check_characters(s);
}

void Calculator::clear_all(){
    order.clear();
}

Calculator::Calculator(){
    for(int i = 0; i<N; i++) pi_to_result[i] = 0, sti_to_p[i] = 0;
    paranthesis.resize(N);
    pi_parsed.resize(N);
}


void Calculator::run(string& str)
{
    try{
        trim(str);
        check_paranthesis(str);

        str = '(' + str + ')';

        apply_paranthesis_precedence(str);
        divide();


        cout << pi_to_result[0] << endl;
    }catch(Parse_error& error){
        cerr << "ERROR: " << error.msg << endl;
    }

}