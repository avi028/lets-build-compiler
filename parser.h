#ifndef _PARSER_H
#define _PARSER_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include "scanner.h"
using namespace std;

#define NODE_MAX_CHILD 5
#define MAX_TOKEN_PER_NODE 10


//tree structure
typedef struct n {
//    struct  token2 *tset[MAX_TOKEN_PER_NODE];
    int tsetCount;
    char fname [100];
    enum tokenID tokenId[MAX_TOKEN_PER_NODE];
    char tokenInstance[100][MAX_TOKEN_PER_NODE];
    int lineNo[MAX_TOKEN_PER_NODE];
    int charNo[MAX_TOKEN_PER_NODE];
    void * child[NODE_MAX_CHILD];
    int childCount;
}node ;

//add tokens to node
void addToken(node* ptr,token tk);


/*display the parse tree in pre-order traversal
  print data
  tree->left
  tree->right
*/
void parseTree(node* root,int level);

//addChild with parent and child parameters
void  addChild(node* parent, node* child);

node * getNode(std::string fname);

//nonterminals
node *  goto_();
node *  label();
node *  RO();
node *  assign();
node *  loop();
node *  if_();
node *  out();
node *  in();
node *  stat_();
node *  mStat();
node *  stats();
node *  R();
node *  M();
node *  A_();
node *  A();
node *  N();
node *  expr();
node *  vars();
node *  block();
node *  program();
node * parser();

#endif
