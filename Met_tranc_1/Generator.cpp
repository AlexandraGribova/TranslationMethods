#pragma once
#include <sstream>
#include <stack>
#include <string>
#include "Tables.h"
using namespace std;
static class Generator
{
private:
	void init_main_process(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier, stringstream& result)
	{
		bool less;
		bool eq;
		stack<string> process;
		ifstream input;
		input.open("postfix.txt");
		string str;
		string word;//òóò õðàíèòñÿ ñëîâî èç ñòðîêè
		result << ".CODE" << endl;
		result << "MAIN PROC" << endl;
		while (!input.eof())
		{
			getline(input, str);
			istringstream ist(str);
			int k = 0;
			for (std::string word; ist >> word; )
			{
				if (identifier.search_str_without_add(word).j != -1 || constants.search_str_without_add(word).j != -1 || word.back() == ':')
				{
					if (word.back() != ':')
						process.push(word);
					else
						result << word << endl;
				}
				else
				{
					string a, b, c;
					if (oper_signs_arith.search_str(word).j != -1)
					{
						a = process.top(); process.pop();
						b = process.top(); process.pop();
						if (a.find("DWORD") != std::string::npos) {
							k -= 4;
						}
						if (b.find("DWORD") != std::string::npos) {
							k -= 4;
						}
						result << "MOV EAX, " << b << endl;
						if (word == "+")
						{
							result << "ADD EAX, " << a << endl;
							result << "MOV DWORD PTR[_ST + " << k << "], EAX" << endl;
							stringstream str;
							str << "DWORD PTR[_ST + " << k << "]";
							process.push(str.str());
							k += 4;
						}
						else if (word == "-")
						{
							result << "SUB EAX, " << a << endl;
							result << "MOV DWORD PTR[_ST + " << k << "], EAX" << endl;
							stringstream str;
							str << "DWORD PTR[_ST + " << k << "]";
							process.push(str.str());
							k += 4;
						}

					}
					else if (oper_signs_compare.search_str(word).j != -1)
					{
						if (word == "=")
						{
							a = process.top(); process.pop();
							b = process.top(); process.pop();
							if (a.find("DWORD") != std::string::npos) {
								k -= 4;
							}
							if (b.find("DWORD") != std::string::npos) {
								k -= 4;
							}
							result << "MOV EAX, " << a << endl;
							result << "MOV " << b << ", EAX" << endl;

						}
						else if (word == "<") {
							less = true;
							eq = false;
						}
						else if (word == ">") {
							less = false;
							eq = false;
						}
						else if (word == "<=") {
							less = true;
							eq = true;
						}
						else if (word == ">=") {
							less = false;
							eq = true;
						}
					}
					else if (word == "UPL") {
						a = process.top(); process.pop();
						b = process.top(); process.pop();
						c = process.top(); process.pop();
						if (b.find("DWORD") != std::string::npos) {
							k -= 4;
						}
						if (c.find("DWORD") != std::string::npos) {
							k -= 4;
						}
						result << "MOV EAX, " << c << endl;
						result << "MOV EBX, " << b << endl;
						result << "CMP EAX, EBX" << endl;
						if (less == false && eq == false)
							result << "JLE " << a << endl;
						else if (less == true && eq == false)
							result << "JGE " << a << endl;
						else if (less == false && eq == true)
							result << "JL " << a << endl;
						else
							result << "JG " << a << endl;
					}

					else if (word == "BP") {
						a = process.top(); process.pop();
						result << "JMP " << a << endl;
					}
					else
						process.push(word);
				}
			}

		}
		result << "PUSH 0" << endl;
		result << "CALL ExitProcess@4" << endl;
		result << "MAIN ENDP" << endl;
		result << "END MAIN" << endl;
	}
	void init_data_memory(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier, stringstream& result)
	{

		result << ".DATA" << endl;
		result << "_ST DD 2 dup(0)" << endl; 
		for (int i = 0; i < identifier.GetSize(); i++)
		{
			if (identifier.get(i).name != "main")
				result << identifier.get(i).name << " DD ?" << endl;
		}

	}

	string GetAsmCodeBeg(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier, stack <string>& postfix_stack)
	{
		stringstream result;
		result << ".386" << endl;
		result << ".MODEL FLAT, STDCALL" << endl;
		result << "EXTRN ExitProcess@4:PROC" << endl;
		init_data_memory(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, result);
		init_main_process(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, result);
		ofstream out;
		out.open("ASM.txt");
		out << result.str();
		out.close();
		return result.str();

	}
public:
	Generator(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier, stack <string>& postfix_stack)
	{
		GetAsmCodeBeg(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, postfix_stack);
	}

};
