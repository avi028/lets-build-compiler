#include "token.h"
#include "scanner.h"

string tokenNames[] ={"ID_tk","NUM_tk", "KW_tk","OP_tk","EOF_tk"};
char SingleOp []= {'<','>','+','-','*','/','^','(',')','[',']','{','}','.',',',';'};
uint sizeofSingleOp = 16;
char DoubleOp []={'_','=','!',':','|','&'};

int fsa[STATES][LABELS];

ifstream* fileReader;
char lineString[MAX_LINE_LEN];
uint charRead ;
uint lineItr;
uint lineNumber;
int tokItr;
int tokLen;
string tokenString;
string tok;


void fsaTable(){

    // initialize fsa table
    for(int i=0;i<STATES;i++)
        for(int j=0;j<LABELS;j++)
            fsa[i][j]=FINAL_OP;

    // initialState
    for(int i=0;i<LABELS;i++)
        fsa[stateInit][i]=i;

    // numChar
    for(int i=0;i<LABELS;i++)
    {
        if(i!=0)
            fsa [labelNumChar][i] = FINAL_NUMCHAR;
        else
            fsa[stateNumChar][labelNumChar] = stateNumChar;
    }

    // singleOp
    for(int i=0;i<LABELS;i++)
            fsa [stateSingleOp][i] = FINAL_OP;

    // _
    for(int i=0;i<LABELS;i++)
            fsa [2][i] = FINAL_ERROR_UND;

    fsa[state_][label_] = state_;
    fsa[state_][labelNumChar] = stateNumChar;

    // =
    for(int i=0;i<LABELS;i++)
        fsa[stateEquals][i] = FINAL_OP;
    fsa[stateEquals][labelEquals] =  state2Equals;

    // !
    for(int i=0;i<LABELS;i++)
        fsa[stateEx][i]= FINAL_ERROR_OP;
    fsa[stateEx][labelEquals]= state2Equals;

    // :
    for(int i=0;i<LABELS;i++)
        fsa[stateCol][i]= FINAL_OP;
    fsa[stateCol][labelEquals]= state2Equals;

    // |
    for(int i=0;i<LABELS;i++)
        fsa[statepipe][i]= FINAL_ERROR_OP;
    fsa[statepipe][labelpipe]= state2Pipe;

    //  &
    for(int i=0;i<LABELS;i++)
        fsa[stateAnd][i]= FINAL_ERROR_OP;
    fsa[stateAnd][labelAnd]= state2And;



    // unknown label

    for(int i=0;i<STATES;i++)
        fsa[i][labelUnknown]=FINAL_ERROR_UNKNOWN_CHAR;

}

bool checkForALphaNum(char c){
    if( (c>=NUM0 && c<=NUM9) || (c>=ALFA && c<=ALFZ) || (c>=ALFa && c<=ALFz) ){
        return true;
    }
    return false;
}

bool checkKeyWord(string s){
    for(int i=0;i<keywordsCount;i++)
        if(s==keywords[i])
            return true;
    return false;
}

bool checkForSingleOp(char c){
    for(int i=0;i<sizeofSingleOp;i++)
        if(c == SingleOp[i])
            return true;
    return false;
}

bool checkConstant(string s)
{
    for(int i=0;i<s.size();i++){
        if(!(s[i]>=NUM0 && s[i]<=NUM9))
            return false;
    }
    return true;
}

string filter(){

    if( lineItr==charRead){
        // if this is false then its end of file
        charRead=0;
        bzero(lineString,500);
        while(fileReader->getline(lineString,MAX_LINE_LEN,'\n')){
                charRead = strlen(lineString);
                
                if(charRead!=0)
                {
                    if((int)lineString [charRead - 1] == 13)
                    {
                        charRead = strlen(lineString) - 1;
                    }
                }
                //cout << charRead << endl;
                lineNumber++;
                lineItr=0;
                if(charRead>0){
                    break;
                }
                else
                    continue;
        }
        if(charRead==0){
    //        lineNumber++;
            return NULL_STR;
        }
    }
    // clear out the comments
    if(lineString[lineItr]==COMMENT){
        do{
            lineItr = lineItr<charRead ? ++lineItr:charRead;
            if(lineItr==charRead)
            {
                charRead=0;
                bzero(lineString,500);
                while(fileReader->getline(lineString,MAX_LINE_LEN,'\n')){
                        charRead = strlen(lineString);

                        if ((int)lineString [charRead - 1] == 13)
                        {

                        	charRead = strlen(lineString) - 1;
                        }
                        lineNumber++;
                        lineItr=0;
                        if(charRead>0){
                            break;
                        }
                        else
                            continue;
                }
                if(charRead==0){
  //                  lineNumber++;
                    return NULL_STR;
                }
            }
        }while(lineString[lineItr]!=COMMENT);

        lineItr = lineItr<charRead ? ++lineItr:charRead;
    }
    // read till hit space or end of the line
    if(lineItr==charRead){
        charRead=0;
        bzero(lineString,500);
        while(fileReader->getline(lineString,MAX_LINE_LEN,'\n')){
                charRead = strlen(lineString);

                if ((int)lineString [charRead - 1] == 13)
                {

                	charRead = strlen(lineString) - 1;
                }
                lineNumber++;
                lineItr=0;
                if(charRead>0){
                    break;
                }
                else
                    continue;
        }
        if(charRead==0){
//            lineNumber++;
            return NULL_STR;
        }
    }

    while(lineString[lineItr]==SPACE && lineItr<charRead){
        lineItr++;
    }

    tokenString = NULL_STR;
    while(lineString[lineItr]!=SPACE && lineString[lineItr]!='#' && lineItr<charRead){
        tokenString+=lineString[lineItr++];
    }
    if(lineString[lineItr]=='#'){
      lineItr = lineItr;
      if(tokenString == NULL_STR)
        tokenString = filter();
    }
    else{
      lineItr = lineItr<charRead ? ++lineItr:charRead;
      if(tokenString == NULL_STR)
        tokenString = filter();
    }
    return tokenString;
}

bool isFinal(int state){
    if(state == FINAL_OP || state == FINAL_NUMCHAR)
        return true;

    return false;
}

bool isError(int state){
    if(state == FINAL_ERROR_UND || state == FINAL_ERROR_OP || state == FINAL_ERROR_UNKNOWN_CHAR)
        return true;

    return false;
}

void appendString(string *s)
{
    *s+=tok[tokItr];
    tokItr = tokItr<tokLen ? tokItr+1 : tokLen;
}

void printError(int state){
    if(state == FINAL_ERROR_UNKNOWN_CHAR)
        printf("Unknown Character at %d line number %d\n",(int)(lineItr+tokItr-tok.size()),lineNumber);
    else if(state == FINAL_ERROR_UND)
        printf("only underscore not allowed");
    else if(state == FINAL_ERROR_OP)
        printf("Unknown operator at %d line number %d\n",(int)(lineItr+tokItr-tok.size()),lineNumber);
}

void closeFileRead(){
    fileReader->close();
}

int getNextLabel()
{
    if(tokItr==tokLen)
        return labelEOF;

    char c = tok[tokItr];
    tokItr = tokItr<tokLen ? tokItr+1 : tokLen;

    if(c==UNDERSCORE)
        return label_;
    else if(checkForALphaNum(c))
        return labelNumChar;
    else if(checkForSingleOp(c))
        return labelSingleOp;
    else if(c==EQUALS)
        return labelEquals;
    else if(c==COL)
        return labelCol;
    else if(c==AND)
        return labelAnd;
    else if(c==Ex)
        return labelEx;
    else if(c==PIPE)
        return labelpipe;
    else
        return labelUnknown;
}


token driver(){
    token t;

    t.lineNo = lineNumber;
    t.charNo = lineItr+tokItr-tok.size();

    int state = stateInit;
    int nextState ;
    int label = getNextLabel();
    // print(label);
    while(!isFinal(state)){
        nextState = fsa[state][label];
        // print(nextState);
        if(isError(nextState)){
            printError(nextState) ;
            closeFileRead();
            exit(0);
        }
        else if(isFinal(nextState)){
                // check for the keyword or instance or num
            if(nextState==FINAL_NUMCHAR){
                if(checkKeyWord(t.tokenInstance)==true)
                    t.tokenId = KW_tk;
                else if(checkConstant(t.tokenInstance))
                    t.tokenId = NUM_tk;
                else
                    t.tokenId = IDENT_tk;
            }
                //  operator
            else {
                    t.tokenId = OP_tk;
            }
            if(label!=labelEOF)
                tokItr--;
            return t;
        }
        else{
            state = nextState;
            t.tokenInstance +=tok[tokItr-1];
            label = getNextLabel();
        }
    }
    return t;
}

// void printFsa(){
//     for(int i=0;i<STATES;i++){
//         for(int j=0;j<LABELS;j++)
//             cout<<fsa[i][j]<<'\t';
//         cout<<endl;
//     }
// }

token scanner(){
    if(tokItr == tokLen){
        tok = filter();

        if(tok==NULL_STR)
        {
            token t;
            t.tokenId = EOF_tk;
            t.lineNo = lineNumber;
            t.charNo=0;
            return t;
        }
        tokItr=0;
        tokLen=tok.size();
    }
    //cout<<tok<<endl;
    return  driver();
}

void setupScanner(){
    fsaTable();
}


void testScanner(){
    token t;

    t=scanner();

    while (t.tokenId!=EOF_tk){
        cout<<tokenNames[t.tokenId]<<"\t"<<t.tokenInstance<<"\t"<<t.lineNo<<"."<<t.charNo<<"\n";
        t=scanner();
    }
    cout<<tokenNames[t.tokenId]<<"\t''"<<"\t"<<t.lineNo<<"."<<t.charNo<<"\n";
}


// int main(int argc , char ** argv){

//     string filename;
//     if(argc < 2){
//         cin>>filename;
//     }
//     else if(argc==2){
//         filename = string(argv[1]);
//     }


//     ifstream f(filename.c_str(), std::ios_base::in);
//     fileReader = &f;

//     lineItr=0;
//     charRead=0;
//     lineNumber=0;
//     tokItr=0;
//     tokLen=0;

//  //   printFsa();

//     testScanner();

//     f.close();
//      return 0;
// }
