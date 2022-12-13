#ifndef _STATSEM_H
#define _STATSEM_H
#include <string>
#include <vector>
#include "parser.h"


#define STACK_SIZE 100

struct data{
    std::string id;
    int line ;
    int charNo;
};

class stack{

    struct data arr[STACK_SIZE];
    int itr;
    public:

    stack();
    int push(std::string input,int lineNo , int charNo);
    struct data top();
    int pop();
    int find(std::string input);
    void printf();
};
extern class stack stck;

int statSem(node* root);

#endif