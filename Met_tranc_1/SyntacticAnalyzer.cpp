#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
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
	int now_state;//òåêóùåå ñîñòîÿíèå
	stack <int> st_int;//ñîñòîÿíèå
	vector<Token> TokensInput;
	vector <element_parser> table_parser;//òàáëèöà 
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
	bool check(Token this_token, ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier)//åñëè ââåäåííûé òîêåí åñòü â òåêóùåé ïîçèöèè òàáëèöû òî âñå êðóòî 
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
		for (string token : table_parser[now_state].tokens)//Èç ñïèñêà-êîëëåêöèè table_parser[now_state].tokens íà êàæäîì øàãå-èòåðàöèè áåðåòñÿ î÷åðåäíîé ýëåìåíò ýòîãî ñïèñêà è ïðèñâàèâàåòñÿ ïåðåìåííîé token
		{
			if (command == token)    return true;
		}
		return false;
	}
	int analizer(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier, ofstream& errors_file)
	{
		now_state = 0;
		st_int.push(-1);
		int stat = 0;
		while (stat < TokensInput.size())
		{
			if (check(TokensInput[stat], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier))//òåêóùèé òåðìèíàë âîîáùå åñòü â òàáëè÷êå? Åñëè äà òî âñå êðóòî, åñëè íåò è error=1 òî âñå ïëîõî
			{
				if (table_parser[now_state].accept)//Åñëè òåðìèíàë
				{
					++stat;
				}
				if (table_parser[now_state].sta)
				{
					st_int.push(now_state + 1);//äîáàâëÿåì ñëåäóþùóþ ñòðîêó òàáëèöû â ñòýê
				}
				if (table_parser[now_state].jump >= 0)//åñëè -1 òî íàäî áðàòü èç ñòåêà, à òàê âåñ êàê îáû÷íî
				{
					now_state = table_parser[now_state].jump;//ïåðåõîäèì ê äðóãîìó ñîñòîÿíèþ
				}
				else if (table_parser[now_state].ret)//à ýòî êàê ðàç jump=-1
				{
					now_state = st_int.top();//áåðåì çíà÷åíèå èç ñòåêà
					st_int.pop();
				}


			}


			else
			{
				if (table_parser[now_state].error)//Åñëè íè÷åãî íå íàøëè à error=1 òî ÷åò íå òî ïðîèñõîäèò
				{
					errors_file << "Error. Unexpected token: (" << TokensInput[stat].i << "," << TokensInput[stat].j << ")" << endl;
					cout << "Error. Unexpected token: (" << TokensInput[stat].i << "," << TokensInput[stat].j << ")" << endl;
					return -1;
				}
				else //Åñë íè÷åãî íå íàøëè à error=0 òî ïðîñòî ñìîòðèì ñëåäóþùóþ ñòðîêó òàáëèöû, òàì ìîæåò áûòü âñå îê
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

	bool check_situation(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier, ofstream& errors_file)
	{
		bool isError = false;
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
			//Íàøëè èìÿ èäåíòèôèêàòîðà/÷åãî áû òî íè áûëî ñ ÷åì ðàáîòàåì è ïóñòèëè ïðîâåðêó
			if (command == "while")
			{
				round_brackets_balance = 0;
				curly_brackets_balance_while = 0;
				is_it_while = true;
			}

			if (is_it_while)//åñëè ìû â öèêëå
			{
				if (command == "(")	round_brackets_balance++;
				if (command == ")")	round_brackets_balance--;
				if (command == "{")	curly_brackets_balance_while++;
				if (command == "}" && round_brackets_balance == 0)
				{
					is_it_while = false;
					curly_brackets_balance_while--;
					for (int s = 0; s < var_in_while.size(); s++)
					{
						identifier.add_ident(var_in_while[s], 0, 0);
					}
					var_in_while.clear();
				}
			}
			else//åñëè âíå öèêëà
			{
				if (command == "{") curly_brackets_balance++;
				if (command == "}") curly_brackets_balance--;
			}
			if (command == "int")
			{

				command = get_name(TokensInput[i + 2], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
				if (command == "=")
				{
					command = get_name(TokensInput[i + 3], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
					if (identifier.get_type(TokensInput[i + 1]) == false)
					 identifier.add_ident(TokensInput[i + 1].i, 1, 1, atoi(command.c_str()));
					else cout << "Error. Variable has already initialised. " << endl;
				}
				else
				{
					if (identifier.get_type(TokensInput[i + 1]) == false)
						identifier.add_ident(TokensInput[i + 1].i, 0, 1);
					else cout << "Error. Variable has already initialised. " << endl;
				}
				if (is_it_while) var_in_while.push_back(TokensInput[i + 1].i);//åñëè ìû âíóòðè öèêëà òî ïèõàåì ïåðåìåííóþ â ñòåê
			}

			if (command == "var")//ïðîâåðêà èíèöèàëèçèðîâàëè ëè ìû ïåðåìåííóþ
			{
				if (identifier.check_int(TokensInput[i]) == 0)
				{
					isError = true;
					errors_file << "Error. Variable '" << identifier.search_num(TokensInput[i]) << "' is not initialized." << endl;
					cout << "Error. Variable '" << identifier.search_num(TokensInput[i]) << "' is not initialized." << endl;
				}
			}
		}
		if (curly_brackets_balance_while != 0 || curly_brackets_balance != 0)
		{
			isError = true;
			errors_file << "Error. The bracket is lost. Probably cycle wasn't closed." << endl;
			cout << "Error. The bracket is lost. Probably cycle wasn't closed." << endl;
		}

		return isError;
	}


	void generate_postfix_file(ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier, ofstream& postfix_file)
	{
		bool is_it_while = false;
		int label_num = 1;
		for (int i = 0; i < TokensInput.size(); i++)
		{
			string command;
			string command2;
			string command3;
			Token this_token = TokensInput[i];
			command = get_name(this_token, keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
			if (command == "while")
			{
				is_it_while = true;
				postfix_file << "m" << to_string(label_num) << ":" << " ";
			}

			if (is_it_while)
			{
				vector <string> var_in_while;
				bool isLocal = false;
				command = get_name(TokensInput[i + 1], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
				while (command != "}") {
					if (command == "(") {
						command = get_name(TokensInput[i + 2], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
						if (command == "const" or command == "var")
						{
							int k = i + 2;
							stack <string> postfix_stack;
							string out_str = "";
							while (command != ")") {
								if (command == "var") {
									out_str += identifier.search_num(TokensInput[k]);
									out_str += " ";
								}
								else if (command == "const") {
									out_str += constants.search_num(TokensInput[k]);
									out_str += " ";
								}
								else if (command == "+" or command == "-") {
									while (!postfix_stack.empty() && (postfix_stack.top() == "+" or postfix_stack.top() == "-")) {
										out_str += postfix_stack.top();
										out_str += " ";
										postfix_stack.pop();
									}
									postfix_stack.push(command);
								}
								else if (command == "<" or command == ">" or command == ">=" or command == "<=") {
									while (!postfix_stack.empty() &&
										(postfix_stack.top() == "+" or postfix_stack.top() == "-"
											or postfix_stack.top() == "<" or postfix_stack.top() == ">" or
											postfix_stack.top() == "<=" or postfix_stack.top() == ">=")) {
										out_str += postfix_stack.top();
										out_str += " ";
										postfix_stack.pop();
									}
									postfix_stack.push(command);
								}
								k++;
								command = get_name(TokensInput[k], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
							}
							while (!postfix_stack.empty()) {
								out_str += postfix_stack.top();
								out_str += " ";
								postfix_stack.pop();
							}
							postfix_file << out_str << "m" << to_string(label_num + 1) << " " << "UPL" << "\n";
							i = k + 1;
							command = command = get_name(TokensInput[i], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
						}
					}
					else
					{
						if (command == "int" or command == "var")
						{
							int k;
							if (command == "var") {
								k = i;
							}
							else {
								isLocal = true;
								k = i + 1;
							}
							command2 = get_name(TokensInput[i + 1], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
							command3 = get_name(TokensInput[i + 2], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
							if (command2 != "main" && command3 != ";") {
								stack <string> postfix_stack;
								string out_str = "";
								command = get_name(TokensInput[k], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
								while (command != ";") {
									if (command == "var") {
										string post_local = "";
										if (isLocal && count(var_in_while.begin(), var_in_while.end(), identifier.search_num(TokensInput[k])) == 0) {
											var_in_while.push_back(identifier.search_num(TokensInput[k]));
											isLocal = false;
										}
										if (count(var_in_while.begin(), var_in_while.end(), identifier.search_num(TokensInput[k])))
											post_local = "_l";
										out_str += post_local + identifier.search_num(TokensInput[k]);
										out_str += " ";
									}
									else if (command == "const") {
										out_str += constants.search_num(TokensInput[k]);
										out_str += " ";
									}
									else if (command == "+" or command == "-") {
										while (!postfix_stack.empty() && (postfix_stack.top() == "+" or postfix_stack.top() == "-")) {
											out_str += postfix_stack.top();
											out_str += " ";
											postfix_stack.pop();
										}
										postfix_stack.push(command);
									}
									else if (command == "=") {
										while (!postfix_stack.empty() && (postfix_stack.top() == "+" or postfix_stack.top() == "-" or postfix_stack.top() == "=")) {
											out_str += postfix_stack.top();
											out_str += " ";
											postfix_stack.pop();
										}
										postfix_stack.push(command);
									}
									k++;
									command = get_name(TokensInput[k], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
								}
								while (!postfix_stack.empty()) {
									out_str += postfix_stack.top();
									out_str += " ";
									postfix_stack.pop();
								}
								postfix_file << out_str << "\n";
								i = k + 1;
								command = get_name(TokensInput[i], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
							}
						}
						else
						{
							++i;
							command = get_name(TokensInput[i], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
						}
					}
				}
				postfix_file << "m" << to_string(label_num) << " " << "BP" << " " << "m" << to_string(label_num + 1) << ":" << "\n";
				is_it_while = false;
				label_num += 2;
			}
			else if (command == "int" or command == "var")
			{
				int k;
				if (command == "var") {
					k = i;
				}
				else {
					k = i + 1;
				}
				command2 = get_name(TokensInput[i + 1], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
				command3 = get_name(TokensInput[i + 2], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
				if (command2 != "main" && command3 != ";") {
					stack <string> postfix_stack;
					string out_str = "";
					command = get_name(TokensInput[k], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
					while (command != ";") {
						if (command == "var") {
							out_str += identifier.search_num(TokensInput[k]);
							out_str += " ";
						}
						else if (command == "const") {
							out_str += constants.search_num(TokensInput[k]);
							out_str += " ";
						}
						/*else if (command == "(") {
							postfix_stack.push(command);
						}
						else if (command == ")") {
							while (postfix_stack.top() != "(") {
								out_str += postfix_stack.top();
								out_str += " ";
								postfix_stack.pop();
								if (postfix_stack.empty()) {
									cout << endl << "ERROR???" << endl;
									exit(-1);
								}
							}
							postfix_stack.pop();
						}*/
						else if (command == "+" or command == "-") {
							while (!postfix_stack.empty() && (postfix_stack.top() == "+" or postfix_stack.top() == "-")) {
								out_str += postfix_stack.top();
								out_str += " ";
								postfix_stack.pop();
							}
							postfix_stack.push(command);
						}
						else if (command == "=") {
							while (!postfix_stack.empty() && (postfix_stack.top() == "+" or postfix_stack.top() == "-" or postfix_stack.top() == "=")) {
								out_str += postfix_stack.top();
								out_str += " ";
								postfix_stack.pop();
							}
							postfix_stack.push(command);
						}
						k++;
						command = get_name(TokensInput[k], keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier);
					}
					while (!postfix_stack.empty()) {
						out_str += postfix_stack.top();
						out_str += " ";
						postfix_stack.pop();
					}
					postfix_file << out_str << "\n";
					i = k;
				}
			}
		}
	}


public:
	SyntacticAnalyzer(string file_name, ConstantTable keywords, ConstantTable separators, ConstantTable oper_signs_compare, ConstantTable oper_signs_arith, VariableTableConstants& constants, VariableTableIdentifier& identifier)//Èíèöèàëèçàöèÿ òàáëèöû
	{
		ofstream errors_file;
		errors_file.open("Errors.txt", ios::app);//Äîïèñàòü â ôàéë
		ofstream postfix_file;
		postfix_file.open("postfix.txt");
		TokenInput();//äëÿ âõîäíîãî ìàññèâà òîêåíîâ
		string el[7];//äëÿ êàæäîãî îòäåëüíîãî êóñêà ñòðîêè (òåðìèíàë, êóäà ïðûãíóòü, accept è ò ä)
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
					elem.jump = std::stoi(el[1]);//ïðåîáðàçîâàíèå â int 
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
		if (!analizer(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, errors_file))//ïðîâåðêà ïî òàáëèöå
		{
			bool isError = check_situation(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, errors_file);
			if (!isError) {
				generate_postfix_file(keywords, separators, oper_signs_compare, oper_signs_arith, constants, identifier, postfix_file);
				postfix_file.close();
			}
		}
		postfix_file.close();
		errors_file.close();
	}


};