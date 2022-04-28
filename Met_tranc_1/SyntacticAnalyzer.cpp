#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include<fstream>
#include <stack>
#include "Tables.h"
using namespace std;

struct element_parser
{
	vector <string> tokens;
	int jump;
	int accept;
	int sta;
	int ret;
	int error;
};

class SyntacticAnalyzer
{
private:
	int now_state;//������� ���������
	stack <int> st_int;//���������
	vector<Token> TokensInput;
	vector <element_parser> table_parser;//������� 
	void TokenInput()
	{
		int i, j;
		ifstream table_file;
		table_file.open("Tokens.txt");
		while (table_file >> i >> j)
		{			
			TokensInput.push_back({ i,j });
		}
	}
	bool check(Token this_token, ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants &constants, VariableTableIdentifier &identifier)//���� ��������� ����� ���� � ������� ������� ������� �� ��� ����� 
	{
		string command;
		command= keywords.search_num(this_token);
		if (command == "NULL") command = separators.search_num(this_token);
		if (command == "NULL") command = oper_signs_compare.search_num(this_token);
		if (command == "NULL") command = oper_signs_arith.search_num(this_token);
		if (command == "NULL")
		{
			command = constants.search_num(this_token);
			if (command != "NULL") command = "const";
		}
		if (command == "NULL")
		{
			command = identifier.search_num(this_token);
			if (command != "NULL" && command!="main") command = "var";
		}
		for (string token : table_parser[now_state].tokens)//�� ������-��������� table_parser[now_state].tokens �� ������ ����-�������� ������� ��������� ������� ����� ������ � ������������� ���������� token
		{
			if (command == token)    return true;
		}
		return false;
	}
	int analizer(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants &constants, VariableTableIdentifier &identifier, ofstream &errors_file)
	{
		now_state = 0;
		st_int.push(-1);
		int stat = 0;
		while (stat < TokensInput.size())
		{
			if (check(TokensInput[stat], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier))//������� �������� ������ ���� � ��������? ���� �� �� ��� �����, ���� ��� � error=1 �� ��� �����
			{
				if (table_parser[now_state].accept)//���� ��������
				{
					++stat;
				}
				if (table_parser[now_state].sta)
				{
					st_int.push(now_state + 1);//��������� ��������� ������ ������� � ����
				}
				if (table_parser[now_state].jump >= 0)//���� -1 �� ���� ����� �� �����, � ��� ��� ��� ������
				{
					now_state = table_parser[now_state].jump;//��������� � ������� ���������
				}
				else if (table_parser[now_state].ret)//� ��� ��� ��� jump=-1
				{
					now_state = st_int.top();//����� �������� �� �����
					st_int.pop();
				}
			

			}


			else
			{
				if (table_parser[now_state].error)//���� ������ �� ����� � error=1 �� ��� �� �� ����������
				{
					errors_file << "Error. Unexpected token: (" << TokensInput[stat].i << "," << TokensInput[stat].j << ")" << endl;
					cout << "Error. Unexpected token: ("<< TokensInput[stat].i<<","<< TokensInput[stat].j<<")"<< endl;
					return -1;
				}
				else //��� ������ �� ����� � error=0 �� ������ ������� ��������� ������ �������, ��� ����� ���� ��� ��
				{
					++now_state;
				}

			}
		}
		return 0;
	}

	string get_name(Token this_token, ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier)
	{
		string command;
		command = keywords.search_num(this_token);
		if (command == "NULL") command = separators.search_num(this_token);
		if (command == "NULL") command = oper_signs_compare.search_num(this_token);
		if (command == "NULL") command = oper_signs_arith.search_num(this_token);
		if (command == "NULL")
		{
			command = constants.search_num(this_token);
			if (command != "NULL") command = "const";
		}
		if (command == "NULL")
		{
			command = identifier.search_num(this_token);
			if (command != "NULL" && command != "main") command = "var";
		}
		return command;
	}

	void check_situation(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier, ofstream& errors_file)
	{
		bool is_it_while = false;
		vector <int> var_in_while;
		int round_brackets_balance = 0;
		int curly_brackets_balance = 0;
		int curly_brackets_balance_while = 0;
		for (int i = 0; i < TokensInput.size(); i++)
		{
			string command;
			Token this_token = TokensInput[i];
			command = get_name(this_token, keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
			//����� ��� ��������������/���� �� �� �� ���� � ��� �������� � ������� ��������
			if (command == "while")
			{
				round_brackets_balance = 0;
				curly_brackets_balance_while=0;
				is_it_while = true;
			}
			
			if (is_it_while)//���� �� � �����
			{
				if (command == "(")	round_brackets_balance++;
				if (command == ")")	round_brackets_balance--;
				if (command == "{")	curly_brackets_balance_while++;
				if (command == "}" && round_brackets_balance == 0) 
				{ 
					is_it_while = false; 
					curly_brackets_balance_while--; 
					for(int s=0; s< var_in_while.size(); s++)
					{
						identifier.add_ident(var_in_while[s], 0, 0);
					}
					var_in_while.clear();
				}
			}
			else//���� ��� �����
			{
				if (command == "{") curly_brackets_balance++;
				if (command == "}") curly_brackets_balance--;
			}
			if (command == "int")
			{
				
				command = get_name(TokensInput[i+2], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
				if (command == "=")
				{
					command = get_name(TokensInput[i + 3], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
					identifier.add_ident(TokensInput[i + 1].i, 1, 1, atoi(command.c_str()));
				}
				else
					identifier.add_ident(TokensInput[i + 1].i, 0, 1);
				if (is_it_while) var_in_while.push_back(TokensInput[i + 1].i);//���� �� ������ ����� �� ������ ���������� � ����
			}

			if (command == "var")//�������� ���������������� �� �� ����������
			{
				if (identifier.check_int(TokensInput[i]) == 0)
				{
					errors_file << "Error. Variable '" << identifier.search_num(TokensInput[i]) << "' is not initialized." << endl;
					cout << "Error. Variable '" << identifier.search_num(TokensInput[i]) << "' is not initialized."<<endl;
				}
			}
		}
		if (curly_brackets_balance_while != 0 || curly_brackets_balance != 0)
		{
			errors_file<< "Error. The bracket is lost. Probably cycle wasn't closed." << endl;
			cout << "Error. The bracket is lost. Probably cycle wasn't closed." << endl;
		}

		
	}
public:
	SyntacticAnalyzer(string file_name, ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants &constants, VariableTableIdentifier &identifier)//������������� �������
	{
		ofstream errors_file;
		errors_file.open("Errors.txt", ios::app);//�������� � ����
		TokenInput();//��� �������� ������� �������
		string el[7];//��� ������� ���������� ����� ������ (��������, ���� ��������, accept � � �)
		ifstream table_file;
		table_file.open(file_name);
		while (!table_file.eof())
		{
			element_parser elem;
			table_file >> el[0] >> el[1] >> el[2] >> el[3] >> el[4] >> el[5];
			for (int i = 0; i < 6; i++)
			{
				switch (i)
				{
				case 0:
				{
					std::stringstream ss(el[0]);
					std::string item;
					while (getline(ss, item, '|'))
						elem.tokens.push_back(item);
					break;
				}

				case 1:
					elem.jump= std::stoi(el[1]);//�������������� � int 
					break;

				case 2:
					elem.accept = std::stoi(el[2]);
					break;

				case 3:
					elem.sta = std::stoi(el[3]);
					break;

				case 4:
					elem.ret = std::stoi(el[4]);
					break;

				case 5:
					elem.error = std::stoi(el[5]);
					break;

				}
			}
			table_parser.push_back(elem);
		}
		if (!analizer(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, errors_file))//�������� �� �������
		{
			check_situation(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, errors_file);
		}

		errors_file.close();
	}

	
};
