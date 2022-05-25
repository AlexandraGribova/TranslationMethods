#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include<stack>
#include<fstream>
int errors = 0;
#include "Tables.h"
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.cpp"
#include "Generator.cpp"
using namespace std;



int main()
{
    stack <string> postfix_stack;
    ConstantTable keywords("keywords.txt", 2, 0);
    ConstantTable separators("separators.txt", 6, 1);
    ConstantTable oper_signs_compare("oper_signs_compare.txt", 5, 2);
    ConstantTable oper_signs_arith("oper_signs_arith.txt", 2, 3);    
    VariableTableConstants constants;
    VariableTableIdentifier identifier;
    LexicalAnalyzer lexical_analyzer(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
    if (errors == 0)
    {
        SyntacticAnalyzer syntactic_analyzer("table.txt", keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, postfix_stack);
        Generator generator(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, postfix_stack);
    }
}