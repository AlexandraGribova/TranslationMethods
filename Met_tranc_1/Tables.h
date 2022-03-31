#pragma once
#include <string>
#include <vector>
#include <iostream>
#include<fstream>
using namespace std;

const int KEY_WORD = 0, SEPARATOR = 1, OPER_ARITH = 2, OPER_COMPARE = 3, VARIABLE = 4, CONSTANT = 5, NULL_TOKEN = 6, UNDEFIND = 7;
const string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string numbers = "0123456789";
const string all = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-<>=*/";

struct Identifier
{
    string name;//��� ����������
    bool type;//1-int ��� 0 - ��� �� ������
    bool value_exist;//1-������ ��� 0 - �� ������ ��������
    int value;//��������
    Identifier(string name_, bool type_, bool value_exist_, int value_) {
        name = name_;
        type = type_;
        value_exist = value_exist_;
        value = value_;
    }
    Identifier() {
        name = "";
        type = false;
        value_exist = false;
        value = 0;
    }
};


struct Token
{
    int j;//����� �������
    int i;//
    Token() //����������� �� ��������� (i � j ���� �� �� ��������� ��������)
    {
        i = 0;      j = -1;
    }
    Token(int i_, int j_)//����������� � �����������
    {
        i = i_;      j = j_;
    }
};


class Table {};


class ConstantTable :public Table {
private:
    int N;
    int j;
    vector <string> array;

    void create_table(string file_name)
    {
        ifstream const_file;
        const_file.open(file_name);
        array.resize(N);
        for (int i = 0; i < N; i++)
            const_file >> array[i];
    }

public:

    Token search_str(string str)//����� �������� ������ - ������� ������
    {
        int i = 0;
        Token token;
        string word;
        for (i = 0; i < N; i++)
        {
            word = array[i];
            if (word == str)
                return Token(i, j);
        }
        return token;
    }

    string search_num(Token token)
    {
        if (token.j == j && token.i < array.size())
            return array[token.i];
        return "NULL";
    }

    ConstantTable(string file_name, int N_, int j_)
    {
        N = N_;
        j = j_;
        create_table(file_name);
    }
    ConstantTable()
    {}

    vector <string> get_array() {
        return array;
    }
};


class VariableTableConstants {
private:
    vector <string> constants;//������, ���������� ���������
public:
    Token search_str(string data)//����� ����� �������� �� �����, ���� �� �������, �� ����������
    {
        Token token;
        for (int i = 0; i < constants.size(); i++)
            if (constants[i] == data)
                token = Token(i, CONSTANT);
        if (token.j == -1)
        {
            constants.push_back(data);
            token.i = constants.size() - 1;
        }
        token.j = CONSTANT;
        return token;
    }
    string search_num(Token token)
    {
        if (token.j == CONSTANT && token.i < constants.size())
            return constants[token.i];
        return "NULL";
    }

};


class VariableTableIdentifier {
private:
    vector <Identifier> identifier;//��������� �������� ��� ����������, ���(0-��� �� �����, 1- ��� int) � ��������� �������� ����������

public:
    Token search_str(string data)//����� ����� �������� �� �����, ���� �� �������, �� ����������
    {
        Token token;
        for (int i = 0; i < identifier.size(); i++)
            if (identifier[i].name == data)
                token = Token(i, VARIABLE);
        if (token.j == -1)
        {
            identifier.push_back(Identifier(data, 0, 0, 0));//���������� ��� �� 0 ��� ��� ������ �� ����� ��� ����������
            token.i = identifier.size() - 1;
        }
        token.j = VARIABLE;
        return token;
    }
    string search_num(Token token)
    {
        if (token.j == 5 && token.i < identifier.size())
            return identifier[token.i].name;
        return "NULL";
    }
    void add_ident(int i, bool value_exist, bool type, int value = 0)//�������� ����� ��-�� � ������� i
    {
        if (value_exist)
        {
            identifier[i].value_exist = 1;
            identifier[i].value = value;
        }
        if (type) identifier[i].type = 1;
    }
    Identifier get(int i)
    {
        return identifier[i];
    }
};
