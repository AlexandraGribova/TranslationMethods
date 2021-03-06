#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include<fstream>
#include "Tables.h"
using namespace std;

struct probable_way
{
    string str;
    int status;//??????? ?????????
    int current_table; //??????? ?????
    string remove(const string& s1, string s2)//???????? ???? ???????? s2 ?? ?????? s1
    {
        string res = s1;
        char symb;
        for (int j = 0, i = 0, pos = 0; j < s2.length(); j++, i = 0, pos = 0)
        {
            symb = s2[j];
            while ((i = res.find(symb, pos)) != string::npos)
            {
                res.erase(i, 1);
                pos += i;
            }
        }
        return res;
    }
    probable_way(vector <string> char_array, int status, int current_table)
    {
        stringstream ss;
        for (size_t i = 0; i < char_array.size(); i++) ss << char_array[i];
        this->str = ss.str();
        this->status = status;
        this->current_table = current_table;
    }
    probable_way(string str, int status)
    {
        this->str = str;
        this->status = status;
    }
    probable_way(string alphabet, string del, int status)//??????? ?? ???????? ???????? ?????
    {
        string s;
        str = alphabet;
        str = remove(str, del);
        this->status = status;
    }
};

class LexicalAnalyzer {
    static const int num_states = 18;
    vector<vector<probable_way>> ways;
    void state_diagram(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith)
    {
        ways.resize(num_states);//num_states - ????? ?????????
        ways[0].push_back({ separators.get_array(),0, SEPARATOR });
        ways[0].push_back({ {'w'}, 1 });
        ways[0].push_back({ { 'i' }, 2 });
        ways[0].push_back({ alphabet,{"wi"}, 3 });//??????? ?? ???????? w ? i
        ways[0].push_back({ numbers, 4 });
        ways[0].push_back({ { '/' }, 5 });
        ways[0].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[0].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        ways[0].push_back({ { '\n' }, 0 });
        //????? ?????? 

        ways[1].push_back({ separators.get_array(),0, SEPARATOR });//?????????? ?????? ??? ???? ???? ?? while ? int
        ways[1].push_back({ {'h'}, 12 });
        ways[1].push_back({ alphabet,"h", 3 });//??????? ?? ???????? h 
        ways[1].push_back({ numbers, 3 });
        ways[1].push_back({ { '/' }, 5 });
        ways[1].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[1].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //????? ??????

        ways[2].push_back({ separators.get_array(),0, SEPARATOR });//?????????? ?????? ??? ???? ???? ?? while ? int
        ways[2].push_back({ {'n'}, 16 });
        ways[2].push_back({ alphabet,"n", 3 });//??????? ?? ???????? n 
        ways[2].push_back({ numbers, 3 });
        ways[2].push_back({ { '/' }, 5 });
        ways[2].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[2].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //????? ??????

        ways[3].push_back({ separators.get_array(),0, SEPARATOR });
        ways[3].push_back({ alphabet, 3 });
        ways[3].push_back({ numbers, 3 });
        ways[3].push_back({ { '/' }, 5 });
        ways[3].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[3].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //????? ??????

        ways[4].push_back({ separators.get_array(),0, SEPARATOR });
        ways[4].push_back({ numbers, 4 });
        ways[4].push_back({ { '/' }, 5 });
        ways[4].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[4].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //????? ??????

        ways[5].push_back({ { '/' },10 });
        ways[5].push_back({ { '*' },{11} });
        //????? ??????

        ways[6].push_back({ separators.get_array(),0, SEPARATOR });
        ways[6].push_back({ alphabet, 3 });
        ways[6].push_back({ numbers, 4 });
        ways[6].push_back({ { '/' }, 5 });
        //????? ??????


        ways[7].push_back({ separators.get_array(),0, SEPARATOR });
        ways[7].push_back({ alphabet, 3 });
        ways[7].push_back({ numbers, 4 });
        ways[7].push_back({ { '/' }, 5 });


        ways[8].push_back({ alphabet, 3 });


        ways[9].push_back({ separators.get_array(),0, SEPARATOR });

        ways[10].push_back({ all,10 });
        ways[10].push_back({ { '\n' },0 });

        ways[11].push_back({ all,"*",11 });
        ways[11].push_back({ { '*' },17 });

        ways[12].push_back({ separators.get_array(),0, SEPARATOR });//?????????? ?????? ??? ???? ???? ?? while ? int
        ways[12].push_back({ {'i'}, 13 });
        ways[12].push_back({ alphabet,"i", 3 });//??????? ?? ???????? i 
        ways[12].push_back({ numbers, 3 });
        ways[12].push_back({ { '/' }, 5 });
        ways[12].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[12].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //????? ??????

        ways[13].push_back({ separators.get_array(),0, SEPARATOR });//?????????? ?????? ??? ???? ???? ?? while ? int
        ways[13].push_back({ {'l'}, 14 });
        ways[13].push_back({ alphabet,"l", 3 });//??????? ?? ???????? l 
        ways[13].push_back({ numbers, 3 });
        ways[13].push_back({ { '/' }, 5 });
        ways[13].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[13].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //????? ??????

        ways[14].push_back({ separators.get_array(),0, SEPARATOR });//?????????? ?????? ??? ???? ???? ?? while ? int
        ways[14].push_back({ {'e'}, 9 });
        ways[14].push_back({ alphabet,"e", 3 });
        ways[14].push_back({ numbers, 3 });
        ways[14].push_back({ { '/' }, 5 });
        ways[14].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[14].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //????? ??????

        ways[16].push_back({ separators.get_array(),0, SEPARATOR });
        ways[16].push_back({ {'t'}, 8 });
        ways[16].push_back({ alphabet,"t", 3 });
        ways[16].push_back({ numbers, 3 });
        ways[16].push_back({ { '/' }, 5 });
        ways[16].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[16].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //????? ??????


        ways[17].push_back({ all,"/",11 });
        ways[17].push_back({ { '/' },0 });
    }
public:
    LexicalAnalyzer(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith)
    {
        run_dfa(keywords, separators, oper_signs_compare, oper_signs_arith);
    }
    int search_letter(char letter, int k)
    {
        for (int i = 0; i < ways[k].size(); i++)
            for (int j = 0; j < (ways[k][i].str).size(); j++)
                if (ways[k][i].str.at(j) == letter)
                {
                    k = ways[k][i].status;
                    return k;
                }
        return -1;
    }
    void run_dfa(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith)
    {
        state_diagram(keywords, separators, oper_signs_compare, oper_signs_arith);
        char word;
        ifstream code;
        code.open("code.txt");
        int  line = 1;
        string code_text;;
        string str;
        while (getline(code, str))
        {
            str += "\n";
            code_text += str;
        }
        str.clear();
        for (char c : code_text) if (c != ' ') str += c;//???????? ???????? ?? code_text
        code_text = str;
        int k = 0;
        for (int j = 0; j < code_text.size(); j++)
        {
            word = code_text[j];
            if (word == '\n') line++;//???? ?? ????????? '\n' ?????? ??????? ?? ????????? ??????-> ??????? ?????? = line
            k = search_letter(word, k);
            if (k == -1)
            {
                cout << "ERROR in line " << line;
                break;
            }

        }
    }

};