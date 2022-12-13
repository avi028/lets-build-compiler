#include <iostream>
#include <vector>

#include "statSem.h"
std::vector<int> counter_stack;

class stack stck;
stack::stack(){
    // do something
    itr=0;
}

struct data stack::top(){
    struct data d;
    if(itr>0){
        d.id = arr[itr-1].id;
        d.line = arr[itr-1].line;
        d.charNo = arr[itr-1].charNo;
    }
    else{
        d.id = "";
        d.line=-1;
        d.charNo=-1;
    }

    return d;
}

int stack::push(std::string in,int lineNo,int charNo){
    if((itr+1)>STACK_SIZE)
        return -1;
    this->itr++;
    this->arr[itr-1].id = in;
    this->arr[itr-1].line=lineNo;
    this->arr[itr-1].charNo = charNo;

    return 0;
}

int stack::pop(){
    if(itr==0)
        return -1;
    this->arr[itr-1].id = "";
    this->arr[itr-1].line=-1;
    this->arr[itr-1].charNo = -1;

    this->itr--;

    return 1;
}

int stack::find(std::string s){
    for(int i=(itr-1);i>=0;i--){
        if(s==this->arr[i].id)
            return itr-1-i;
    }
    return -1;
}

void stack::printf(){
    for(int i=this->itr-1;i>=0;i--){
        std::cout<<arr[i].id<<"\t"<<arr[i].line<<":"<<arr[i].charNo<<"\n";
    }
 }

bool pg=false;

struct g_v{
    char name[11];
    int val;
};

std::vector<struct g_v> g_set;

int find_in_g_set(char * var){
    for (int i=0;i<g_set.size();i++){
        if(strcmp(g_set[i].name,var)==0)
            return i;
    }
    return -1;
}
int status =0;

int statSem(node* root)
{
    if(strcmp(root->fname,"<vars>")==0){
        if(pg==false){
            int val = find_in_g_set(root->tokenInstance[0]);
            if(val==-1){
                struct g_v tmp;
                strcpy(tmp.name,root->tokenInstance[0]);
                tmp.val = atoi(root->tokenInstance[2]);
                g_set.push_back(tmp);
            }                
            else{
                printf("Redefination of Global variable %s at %d,%d\n",root->tokenInstance[0],root->lineNo[0],root->charNo[0]);
                status = -1;
            }
        }
        else{
            if(counter_stack.empty()==true)
                stck.push(string(root->tokenInstance[0]),root->lineNo[0],root->charNo[0]);
            else{
                if(stck.find(string(root->tokenInstance[0])) ==-1 && find_in_g_set(root->tokenInstance[0])==-1){
                    counter_stack[counter_stack.size()-1]++;
                    stck.push(string(root->tokenInstance[0]),root->lineNo[0],root->charNo[0]);
                }
                else{
                    printf("Redefination of Local variable %s at %d,%d\n",root->tokenInstance[0],root->lineNo[0],root->charNo[0]);
                    status = -1;
                }
            }
        }
    }
    else if(strcmp(root->fname,"<program>")==0){
        if(root->childCount==1){
            pg = true;
            statSem((node*)(root->child[0]));
        }
        else{
            statSem((node*)(root->child[0]));
            pg = true;
            statSem((node*)(root->child[1]));                
        }
        return status;
    }
    else if(strcmp(root->fname,"<block>")==0){
        counter_stack.push_back(0);            
    }
    else 
    {
        for(int i=0;i<root->tsetCount;i++)
        {
            if(root->tokenId[i]==IDENT_tk){
                if(stck.find(string(root->tokenInstance[i])) ==-1 && find_in_g_set(root->tokenInstance[i])==-1){
                    printf("Cannnot find variable %s at %d,%d\n",root->tokenInstance[i],root->lineNo[i],root->charNo[i]);
                    status =  -1;
                }
            }
        }
    }
    for(int i=0;i<root->childCount;i++)
    {
        statSem((node*)(root->child[i]));
    }
    // stck.printf();
    if(strcmp(root->fname,"<block>")==0){
        for(int i=0;i<counter_stack[counter_stack.size()-1];i++){
            stck.pop();
        }
        counter_stack.pop_back();
    }
    return 0;
}
