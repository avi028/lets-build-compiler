#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
using namespace std;


#define NUM0 48
#define NUM9 57

#define ALFA 65
#define ALFZ 90

#define ALFa 97
#define ALFz 122
#define MAX_LINE_LEN 500

#define NULL_STR ""
#define STATES  12
#define LABELS  10

#define labelNumChar 0
#define labelSingleOp 1
#define label_ 2
#define labelEquals 3
#define labelEx 4
#define labelCol 5
#define labelpipe 6
#define labelAnd 7
#define labelUnknown 8
#define labelEOF 9


#define stateNumChar 0
#define stateSingleOp 1
#define state_ 2
#define stateEquals 3
#define stateEx 4
#define stateCol 5
#define statepipe 6
#define stateAnd 7
#define state2And 8
#define state2Pipe 9
#define state2Equals 10
#define stateInit 11


#define FINAL_NUMCHAR 1001
#define FINAL_OP 1003

#define FINAL_ERROR_UND -1
#define FINAL_ERROR_OP  -2
#define FINAL_ERROR_UNKNOWN_CHAR -3

#define SPACE ' '
#define COMMENT '#'

#define UNDERSCORE '_'
#define EQUALS '='
#define Ex '!'
#define COL ':'
#define PIPE '|'
#define AND '&'

#define print(x) cout<<#x<<"\t"<<x<<endl;
enum tokenID { IDENT_tk, NUM_tk, KW_tk, OP_tk,EOF_tk};
extern std::string tokenNames[5];

typedef struct t {
    enum tokenID tokenId;
    std::string tokenInstance;
    int lineNo;
    int charNo;
} token;

extern std::ifstream * fileReader;
extern unsigned int charRead ;
extern unsigned int lineItr;
extern unsigned int lineNumber;
extern int tokItr;
extern int tokLen;

token scanner();
void setupScanner();
void testScanner();

#endif
