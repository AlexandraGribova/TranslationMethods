#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include<fstream>
#include "Tables.h"
#include "LexicalAnalyzer.h"
using namespace std;



int main()
{

    ConstantTable keywords("keywords.txt", 2, 0);
    ConstantTable separators("separators.txt", 6, 1);
    ConstantTable oper_signs_compare("oper_signs_compare.txt", 5, 2);
    ConstantTable oper_signs_arith("oper_signs_arith.txt", 2, 3);
    LexicalAnalyzer lexical_analyzer(keywords, separators, oper_signs_compare, oper_signs_arith);

}