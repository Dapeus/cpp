#pragma once
#ifndef USER_H
#define USER_H
#include<iostream>
using namespace std;
class User
{
public:
    User() {};
    ~User() {};
    char acc[50] = {0}; //�˻�
    char pass[50] = {0}; //����
};

#endif