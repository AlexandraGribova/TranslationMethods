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
    int status;//текущее состояние
    int current_table; //текущий токен
    string remove(const string& s1, string s2)//удаление всех символов s2 из строки s1
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
    probable_way(string alphabet, string del, int status)//убирает из алфавита заданные буквы
    {
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
        ways.resize(num_states);//num_states - число состояний
        ways[0].push_back({ separators.get_array(),0, SEPARATOR });
        ways[0].push_back({ {'w'}, 1 });
        ways[0].push_back({ { 'i' }, 2 });
        ways[0].push_back({ alphabet,{"wi"}, 3 });//удаляет из алфавита w и i
        ways[0].push_back({ numbers, 4 });
        ways[0].push_back({ { '/' }, 5 });
        ways[0].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[0].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        ways[0].push_back({ { '\n' }, 0 });
        //иначе ошибка 

        ways[1].push_back({ separators.get_array(),0, SEPARATOR });//аналогично вообще для всех букв из while и int
        ways[1].push_back({ {'h'}, 12 });
        ways[1].push_back({ alphabet,"h", 3 });//удаляет из алфавита h 
        ways[1].push_back({ numbers, 3 });
        ways[1].push_back({ { '/' }, 5 });
        ways[1].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[1].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //иначе ошибка

        ways[2].push_back({ separators.get_array(),0, SEPARATOR });//аналогично вообще для всех букв из while и int
        ways[2].push_back({ {'n'}, 16 });
        ways[2].push_back({ alphabet,"n", 3 });//удаляет из алфавита n 
        ways[2].push_back({ numbers, 3 });
        ways[2].push_back({ { '/' }, 5 });
        ways[2].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[2].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //иначе ошибка

        ways[3].push_back({ separators.get_array(),0, SEPARATOR });
        ways[3].push_back({ alphabet, 3 });
        ways[3].push_back({ numbers, 3 });
        ways[3].push_back({ { '/' }, 5 });
        ways[3].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[3].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //иначе ошибка

        ways[4].push_back({ separators.get_array(),0, SEPARATOR });
        ways[4].push_back({ numbers, 4 });
        ways[4].push_back({ { '/' }, 5 });
        ways[4].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[4].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //иначе ошибка

        ways[5].push_back({ { '/' },10 });
        ways[5].push_back({ { '*' },{11} });
        //иначе ошибка

        ways[6].push_back({ separators.get_array(),0, SEPARATOR });
        ways[6].push_back({ alphabet, 3 });
        ways[6].push_back({ numbers, 4 });
        ways[6].push_back({ { '/' }, 5 });
        ways[6].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        //иначе ошибка


        ways[7].push_back({ separators.get_array(),0, SEPARATOR });
        ways[7].push_back({ alphabet, 3 });
        ways[7].push_back({ numbers, 4 });
        ways[7].push_back({ { '/' }, 5 });


        ways[8].push_back({ separators.get_array(),0, SEPARATOR });
        ways[8].push_back({ {'w'}, 1 });
        ways[8].push_back({ { 'i' }, 2 });
        ways[8].push_back({ alphabet,{"wi"}, 3 });//удаляет из алфавита w и i
        ways[8].push_back({ numbers, 4 });
        ways[8].push_back({ { '/' }, 5 });
        ways[8].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[8].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        ways[8].push_back({ { '\n' }, 0 });

        ways[9].push_back({ separators.get_array(),0, SEPARATOR });
        ways[9].push_back({ {'w'}, 1 });
        ways[9].push_back({ { 'i' }, 2 });
        ways[9].push_back({ alphabet,{"wi"}, 3 });//удаляет из алфавита w и i
        ways[9].push_back({ numbers, 4 });
        ways[9].push_back({ { '/' }, 5 });
        ways[9].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[9].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        ways[9].push_back({ { '\n' }, 0 });

        ways[10].push_back({ all,10 });
        ways[10].push_back({ { '\n' },0 });

        ways[11].push_back({ all,"*",11 });
        ways[11].push_back({ { '*' },17 });

        ways[12].push_back({ separators.get_array(),0, SEPARATOR });//аналогично вообще для всех букв из while и int
        ways[12].push_back({ {'i'}, 13 });
        ways[12].push_back({ alphabet,"i", 3 });//удаляет из алфавита i 
        ways[12].push_back({ numbers, 3 });
        ways[12].push_back({ { '/' }, 5 });
        ways[12].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[12].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //иначе ошибка

        ways[13].push_back({ separators.get_array(),0, SEPARATOR });//аналогично вообще для всех букв из while и int
        ways[13].push_back({ {'l'}, 14 });
        ways[13].push_back({ alphabet,"l", 3 });//удаляет из алфавита l 
        ways[13].push_back({ numbers, 3 });
        ways[13].push_back({ { '/' }, 5 });
        ways[13].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[13].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //иначе ошибка

        ways[14].push_back({ separators.get_array(),0, SEPARATOR });//аналогично вообще для всех букв из while и int
        ways[14].push_back({ {'e'}, 9 });
        ways[14].push_back({ alphabet,"e", 3 });
        ways[14].push_back({ numbers, 3 });
        ways[14].push_back({ { '/' }, 5 });
        ways[14].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[14].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //иначе ошибка

        ways[16].push_back({ separators.get_array(),0, SEPARATOR });
        ways[16].push_back({ {'t'}, 8 });
        ways[16].push_back({ alphabet,"t", 3 });
        ways[16].push_back({ numbers, 3 });
        ways[16].push_back({ { '/' }, 5 });
        ways[16].push_back({ oper_signs_compare.get_array(), 6,OPER_COMPARE });
        ways[16].push_back({ oper_signs_arith.get_array(), 7,OPER_ARITH });
        //иначе ошибка


        ways[17].push_back({ all,"/",11 });
        ways[17].push_back({ { '/' },0 });
    }
public:
    LexicalAnalyzer(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants &constants,VariableTableIdentifier &identifier)
    {
        run_dfa(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
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
    void take_token(ostream& token_out,char letter, char next, string &sign_comp, string &word, ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants &constants, VariableTableIdentifier &identifier)
    {
        Token token, token1;
        string str;//str строка содержащая символ
        str.clear();
        str = str + letter;
        while (true)
        {
            if (letter == '/') break;
            token = separators.search_str(str);//Если нашли символ среди разделителей
            if (token.j != -1) 
            { 
                if (word.length() != 0)//На случай если в word уже накопилось слово или константа
                {
                    for (int j = 0; j < alphabet.size(); j++)
                        if (alphabet.at(j) == word[0])//Если мы нашли в алфавите первый символ слова то это точно идентификатор
                        {
                            token1 = identifier.search_str(word);
                            cout<< word << " (" << token1.i << "," << token1.j << ")" << endl;
                            token_out << token1.i << " " << token1.j << endl;
                            word.clear();
                        }
                    for (int j = 0; j < numbers.size(); j++)
                        if (numbers.at(j) == word[0])//Если мы нашли в перчне циферок первый символ слова то это точно константа
                        {
                            token1 = constants.search_str(word);
                            cout << word << " (" << token1.i << "," << token1.j << ")" << endl;
                            token_out << token1.i << " " << token1.j <<endl;
                            word.clear();
                        }
                    
                }
                cout << str << " (" << token.i << "," << token.j << ")" << endl;
                token_out <<token.i<<" "<<token.j<< endl;
                break; 
            }

            token = oper_signs_compare.search_str(str);//Если нашли символ знаков операций (сравнение)
            if (token.j != -1) 
            { 
                if (word.length() != 0)//На случай если в word уже накопилось слово или константа
                {
                    for (int j = 0; j < alphabet.size(); j++)
                        if (alphabet.at(j) == word[0])//Если мы нашли в алфавите первый символ слова то это точно идентификатор
                        {
                            token1 = identifier.search_str(word);
                            cout << word << " (" << token1.i << "," << token1.j << ")" << endl;
                            token_out << token1.i << " " << token1.j << endl;
                            word.clear();
                        }
                    for (int j = 0; j < numbers.size(); j++)
                        if (numbers.at(j) == word[0])//Если мы нашли в перчне циферок первый символ слова то это точно константа
                        {
                            token1 = constants.search_str(word);
                            cout << word << " (" << token1.i << "," << token1.j << ")" << endl;
                            token_out << token1.i << " " << token1.j << endl;
                            word.clear();
                        }

                }
                if (sign_comp.length() == 0)
                {
                    token1 = oper_signs_compare.search_str(str+next);//если следующий символ тоже знак сравнения 
                    if (token1.j == -1)
                    {
                        cout << str << " (" << token.i << "," << token.j << ")" << endl;
                        token_out << token.i << " " << token.j << endl;
                    }
                    else 
                        sign_comp = letter;
                }
                else
                {
                    token = oper_signs_compare.search_str(sign_comp+letter);
                    cout << sign_comp + letter << " (" << token.i << "," << token.j << ")" << endl;
                    token_out  << " (" << token.i << " " << token.j <<endl;
                    sign_comp.clear();
                }
                break; 

            }


            token = oper_signs_arith.search_str(str);//Если нашли символ знаков операций (арифметические)
            if (token.j != -1)
            {
                if (word.length() != 0)//На случай если в word уже накопилось слово или константа
                {
                    for (int j = 0; j < alphabet.size(); j++)
                        if (alphabet.at(j) == word[0])//Если мы нашли в алфавите первый символ слова то это точно идентификатор
                        {
                            token1 = identifier.search_str(word);
                            cout << word << " (" << token1.i << "," << token1.j << ")" << endl;
                            token_out  << token1.i << " " << token1.j << endl;
                            word.clear();
                        }
                    for (int j = 0; j < numbers.size(); j++)
                        if (numbers.at(j) == word[0])//Если мы нашли в перчне циферок первый символ слова то это точно константа
                        {
                            token1 = constants.search_str(word);
                            cout << word << " (" << token1.i << "," << token1.j << ")" << endl;
                            token_out << token1.i << " " << token1.j << endl;
                            word.clear();
                        }

                }
                cout << str << " (" << token.i << "," << token.j << ")" << endl;
                token_out << token.i << " " << token.j  << endl;
                break;
            }
            
            //если это уже точно не разделитель и не знак операции
            if(letter!='\n') word = word + letter;

            token = keywords.search_str(word);
            if (token.j != -1){ cout << word << " (" << token.i << "," << token.j << ")" << endl; token_out << token.i << " " << token.j<< endl; word.clear(); break;  }//Если нашли слово среди ключевых            
            
            break;
        }
    }

    void run_dfa(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants &constants, VariableTableIdentifier &identifier)
    {
        state_diagram(keywords, separators, oper_signs_compare, oper_signs_arith);
        char letter;
        string sign_comp;
        string word;
        ifstream code;
        code.open("code.txt");
        int  line = 1;
        string code_text;
        string str;
        while (getline(code, str))
        {
            str += "\n";
            code_text += str;
        }
        str.clear();
        for (char c : code_text) if (c != ' ' &&  c!='\t') str += c;//удаление пробелов и табуляций из code_text
        code_text = str;
        int k = 0;
        ofstream token_out;
        token_out.open("Tokens.txt");
        for (int j = 0; j < code_text.size(); j++)
        {
            letter = code_text[j];
            if (letter == '\n') line++;//если мы встретили '\n' значит перешли на следующую строку-> текущая строка = line
            k = search_letter(letter, k);
            //поверка на токен и ошибку
            if (k != 5 && k != 10 && k != 11 && k != 17 )
            {
                if (k == -1)
                {
                    cout << "ERROR in line " << line;
                    break;
                }
                take_token(token_out, letter, code_text[j + 1], sign_comp, word, keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
            }
        }
        token_out.close();
    }

};