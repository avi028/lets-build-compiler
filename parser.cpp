#include "parser.h"

token tk;

//ParseError: Expected " ", received " "
void error(string str)
{
    printf("Parse Error\t: LINE: %d  CHAR: %d\n",tk.lineNo,tk.charNo);
    printf("Expected \"%s\"  , Received  \"%s\" \n",str.c_str(),tk.tokenInstance.c_str());
    exit(0);
}

//<expr> -> <N> - <expr>  | <N>
node* expr()
{
    node* ptr = getNode("<expr>");
    // printf("expr %s\n",tk.tokenInstance.c_str());
    addChild(ptr,N());
    if (tk.tokenId == OP_tk && tk.tokenInstance == "-" )
    {
        addToken(ptr,tk);
        tk = scanner();
        addChild(ptr,expr());
    }
    return ptr;
}

//<N> -> <A> + <N> | <A> * <N> | <A>
node* N()
{
    node* ptr = getNode("<N>");
    // printf("N %s\n",tk.tokenInstance.c_str());
    addChild(ptr,A());
    if (tk.tokenId == OP_tk && tk.tokenInstance == "+" )
    {
        addToken(ptr,tk);
        tk = scanner();
        addChild(ptr,N());
    }
    else if (tk.tokenId == OP_tk && tk.tokenInstance == "*" )
    {
        addToken(ptr,tk);
        tk = scanner();
        addChild(ptr,N());
    }
    return ptr;
}

//<A> -> <M><A_>
node* A()
{
    node* ptr = getNode("<A>");
    // printf("A %s\n",tk.tokenInstance.c_str());
    addChild(ptr,M());
    addChild(ptr,A_());
    return ptr;
}

//<A_> -> / <M> | empty
node* A_()
{
    node * ptr = getNode("<A_>");
    // printf("A_ %s\n",tk.tokenInstance.c_str());
    if (tk.tokenId == OP_tk && tk.tokenInstance == "/"){
        addToken(ptr,tk);
        tk = scanner();
        addChild(ptr,M());
        return ptr;
    }
    // empty case handled
    else
        return NULL;

    return NULL;
}

//<M> -> :<M> | <R>
node* M(){
    node* ptr = getNode("<M>");
    // printf("M %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == OP_tk && tk.tokenInstance == ":")
    {
        addToken(ptr,tk);
        tk = scanner();
        addChild(ptr,M());
        return ptr;
    }
    else if (tk.tokenId == OP_tk && tk.tokenInstance == "(")
    {
        addChild(ptr,R());
        return ptr;
    }
    else if (tk.tokenId == IDENT_tk )
    {
        addChild(ptr,R());
        return ptr;
    }
    else if (tk.tokenId == NUM_tk)
    {
        addChild(ptr,R());
        return ptr;
    }
    else
        error(": or ( or NUM or IDENTIFIER");
    return NULL;
}

//<R> -> (<expr>) | Identifier | Integer
node* R(){
    node* ptr = getNode("<R>");
    // printf("R %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == OP_tk && tk.tokenInstance == "(")
    {
        addToken(ptr,tk);
        tk = scanner();
        addChild(ptr,expr());
        if(tk.tokenId == OP_tk && tk.tokenInstance == ")")
        {
            addToken(ptr,tk);
            tk = scanner();
            return ptr;
        }
        else
            error(")");
    }
    else  if(tk.tokenId == IDENT_tk)
    {
        addToken(ptr,tk);
        tk = scanner();
        return ptr;
    }
    else  if(tk.tokenId == NUM_tk)
    {
        addToken(ptr,tk);
        tk = scanner();
        return ptr;
    }
    else
        error("( or IDENTIFIER or INTEGER");
    return NULL;
}

//<mStat> -> empty | <stat> <mStat>
node* mStat()
{
    node* ptr = getNode("<mStat>");
    // printf("mStat %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == KW_tk && tk.tokenInstance == "input")
    {
        addChild(ptr,stat_());
        addChild(ptr,mStat());
        return ptr;
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "output")
    {
        addChild(ptr,stat_());
        addChild(ptr,mStat());
        return ptr;
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "if")
    {
        addChild(ptr,stat_());
        addChild(ptr,mStat());
        return ptr;
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "while")
    {
        addChild(ptr,stat_());
        addChild(ptr,mStat());
        return ptr;
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "assign")
    {
        addChild(ptr,stat_());
        addChild(ptr,mStat());
        return ptr;
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "warp")
    {
        addChild(ptr,stat_());
        addChild(ptr,mStat());
        return ptr;
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "label")
    {
        addChild(ptr,stat_());
        addChild(ptr,mStat());
        return ptr;
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "begin")
    {
        addChild(ptr,stat_());
        addChild(ptr,mStat());
        return ptr;
    }
    else
        return NULL;
    return NULL;
}

//<stat> -> <in> ; | <out> ; | <block> | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ;
node* stat_()
{
    node* ptr = getNode("<stat_>");
    // printf("stat %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == KW_tk && tk.tokenInstance == "input")
    {
        addChild(ptr,in());
        if(tk.tokenId == OP_tk && tk.tokenInstance == ";")
        {
            tk = scanner();
            return ptr;
        }
        else
            error(";");
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "output")
    {
        addChild(ptr,out());
        if(tk.tokenId == OP_tk && tk.tokenInstance == ";")
        {
            tk = scanner();
            return ptr;
        }
        else
            error(";");
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "if")
    {
        addChild(ptr,if_());
        if(tk.tokenId == OP_tk && tk.tokenInstance == ";")
        {
            tk = scanner();
            return ptr;
        }
        else
            error(";");
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "while")
    {
        addChild(ptr,loop());
        if(tk.tokenId == OP_tk && tk.tokenInstance == ";")
        {
            tk = scanner();
            return ptr;
        }
        else
            error(";");
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "assign")
    {
        addChild(ptr,assign());
        if(tk.tokenId == OP_tk && tk.tokenInstance == ";")
        {
            tk = scanner();
            return ptr;
        }
        else
            error(";");
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "warp")
    {
        addChild(ptr,goto_());
        if(tk.tokenId == OP_tk && tk.tokenInstance == ";")
        {
            tk = scanner();
            return ptr;
        }
        else
            error(";");
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "label")
    {
        addChild(ptr,label());
        if(tk.tokenId == OP_tk && tk.tokenInstance == ";")
        {
            tk = scanner();
            return ptr;
        }
        else
            error(";");
    }
    else if(tk.tokenId == KW_tk && tk.tokenInstance == "begin")
    {
        addChild(ptr,block());
        return ptr;
    }
    else
        error("input OR output OR if OR while OR assign OR label OR warp OR begin");
    return NULL;
}

//<in> -> input Identifier
node* in()
{
    node* ptr = getNode("<in>");
    // printf("in %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == KW_tk && tk.tokenInstance == "input")
    {
        tk = scanner();
        if(tk.tokenId == IDENT_tk)
        {
            // ptr->tokenId = tk.tokenId;
            // strcpy(ptr->tokenInstance, tk.tokenInstance.c_str());
            // ptr->tokenInstance = tk.tokenInstance;
            // ptr->charNo = tk.charNo;
            // ptr->lineNo = tk.lineNo;
            addToken(ptr,tk);
            tk = scanner();
            return ptr;
        }
        else
            error("Identifier");
    }
    else
        error("input");
    return NULL;
}

//<out> -> output <expr>
node* out()
{
    node* ptr = getNode("<out>");
    // printf("out %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == KW_tk && tk.tokenInstance == "output")
    {
        tk = scanner();
        addChild(ptr,expr());
        return ptr;
    }
    else
        error("output");
    return NULL;
}

//<if> -> if [<expr> <RO> <expr>] then <stat> | if [<expr> <RO> <expr>] then <stat> pick <stat>
node * if_()
{
    node* ptr = getNode("<if>");
    // printf("if %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == KW_tk && tk.tokenInstance == "if")
    {
        tk = scanner();
        if(tk.tokenId == OP_tk && tk.tokenInstance == "[")
        {
            // addToken(ptr,tk);
            tk = scanner();
            addChild(ptr,expr());
            addChild(ptr,RO());
            addChild(ptr,expr());
            if(tk.tokenId == OP_tk && tk.tokenInstance == "]")
            {
                // addToken(ptr,tk);
                tk = scanner();
                if(tk.tokenId == KW_tk && tk.tokenInstance == "then")
                {
                    tk = scanner();
                    addChild(ptr,stat_());
                    if(tk.tokenId == KW_tk && tk.tokenInstance == "pick")
                    {
                        tk =scanner();
                        addChild(ptr,stat_());
                        return ptr;
                    }
                    else
                        //other case of if
                        return ptr;
                }
                else
                    error("then");
            }
            else
                error("]");
        }
        else
            error("[");
    }
    else
        error("if");
    return NULL;
}

//<loop> -> while [<expr> <RO> <expr>] <stat>
node* loop()
{
    node* ptr = getNode("<loop>");
    //printf("loop %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == KW_tk && tk.tokenInstance == "while")
    {
        tk = scanner();
        if(tk.tokenId == OP_tk && tk.tokenInstance == "[")
        {
            // addToken(ptr,tk);
            tk = scanner();
            addChild(ptr,expr());
            addChild(ptr,RO());
            addChild(ptr,expr());
            if(tk.tokenId == OP_tk && tk.tokenInstance == "]")
            {
                // addToken(ptr,tk);
                tk = scanner();
                addChild(ptr,stat_());
                return ptr;
            }
            else
                error("]");
        }
        else
            error("[");
    }
    else
        error("while");
    return NULL;
}

//<assign> -> assign Identifier = <expr>
node* assign()
{
    node* ptr = getNode("<assign>");
    // printf("Assign %s\n",tk.tokenInstance.c_str());
    if(tk.tokenId == KW_tk && tk.tokenInstance == "assign")
    {
        tk = scanner();
        if(tk.tokenId == IDENT_tk){
            addToken(ptr,tk);
            tk = scanner();
            if(tk.tokenId == OP_tk && tk.tokenInstance == "=")
            {
                addToken(ptr,tk);
                tk = scanner();
                addChild(ptr,expr());
                return ptr;
            }
            else
                error("=");
        }
        else
            error("Identifier");
    }
    else
        error("assign");
    return NULL;
}

//<RO> -> > | < | == | [ = ] (three tokens) | !=
node* RO()
{
    node* ptr = getNode("<RO>");
    //printf("RO %s\n",tk.tokenInstance.c_str());

    if(tk.tokenId == OP_tk && tk.tokenInstance == "<")
    {
        addToken(ptr,tk);
        tk =scanner();
        return ptr;
    }
    else if(tk.tokenId == OP_tk && tk.tokenInstance == ">")
    {
        addToken(ptr,tk);
        tk =scanner();
        return ptr;
    }
    else if(tk.tokenId == OP_tk && tk.tokenInstance == "==")
    {
        addToken(ptr,tk);
        tk =scanner();
        return ptr;
    }
    else if(tk.tokenId == OP_tk && tk.tokenInstance == "!=")
    {
        addToken(ptr,tk);
        tk =scanner();
        return ptr;
    }
    else if(tk.tokenId == OP_tk && tk.tokenInstance == "[")
    {
        addToken(ptr,tk);
        tk =scanner();
        if(tk.tokenId == OP_tk && tk.tokenInstance == "=")
        {
            addToken(ptr,tk);
              tk =scanner();
              if(tk.tokenId == OP_tk && tk.tokenInstance == "]")
              {
                addToken(ptr,tk);
                tk = scanner();
                return ptr;
              }
              else
            	  error("]");
        }
        else
            error("=");
    }
    else
        error("< OR > OR != OR == OR [");
    return NULL;
}

//<label> -> label Identifier
node* label()
{
	node* ptr = getNode("<label>");
    //printf("label %s\n",tk.tokenInstance.c_str());

    if(tk.tokenId == KW_tk && tk.tokenInstance == "label")
    {
        tk = scanner();
        if(tk.tokenId == IDENT_tk)
        {
            // ptr->tokenId = tk.tokenId;
            // strcpy(ptr->tokenInstance, tk.tokenInstance.c_str());
            //ptr->tokenInstance = tk.tokenInstance;
            // ptr->charNo = tk.charNo;
            // ptr->lineNo = tk.lineNo;
            addToken(ptr,tk);
            tk = scanner();
            return ptr;
        }
        else
            error("Indentifier");
    }
    else
        error("label");
    return NULL;
}

//<goto> -> warp Identifier
node* goto_()
{
    node* ptr = getNode("<goto>");
    // printf("goto %s\n",tk.tokenInstance.c_str());

    if(tk.tokenId == KW_tk && tk.tokenInstance == "warp")
    {
        tk = scanner();
        if(tk.tokenId == IDENT_tk)
        {
            // ptr->tokenId = tk.tokenId;
            // strcpy(ptr->tokenInstance, tk.tokenInstance.c_str());
            //ptr->tokenInstance = tk.tokenInstance;
            // ptr->charNo = tk.charNo;
            // ptr->lineNo = tk.lineNo;
            addToken(ptr,tk);
            tk = scanner();
            return ptr;
        }
        else
            error("Identifier");
    }
    else
        error("warp");
    return NULL;
}

//<stats> -> <stat> <mStat>
node* stats()
{
    // printf("stats %s\n",tk.tokenInstance.c_str());
    node* ptr = getNode("<stats>");
    addChild(ptr,stat_());
    addChild(ptr,mStat());
    return ptr;
}

node* block()
{
    // printf("block %s\n",tk.tokenInstance.c_str());
    node* ptr = getNode("<block>");
    if(tk.tokenId == KW_tk && tk.tokenInstance == "begin")
    {
        tk = scanner();
        addChild(ptr,vars());
        addChild(ptr,stats());
        // printf("after stats %s\n",tk.tokenInstance.c_str());
        if(tk.tokenId == KW_tk && tk.tokenInstance == "end")
        {
            tk = scanner();
            return ptr;
        }
        else
            error("end");
    }
    else
        error("begin");
    return NULL;
}

//<vars> -> empty | whole Identifier := Integer ; <vars>
node* vars()
{
    node* ptr = getNode("<vars>");
    if(tk.tokenId == KW_tk && tk.tokenInstance == "whole")
    {
        tk = scanner();
        if(tk.tokenId == IDENT_tk)
        {
            addToken(ptr,tk);
            tk = scanner();
            if(tk.tokenId == OP_tk && tk.tokenInstance == ":=")
            {
                addToken(ptr,tk);
                tk = scanner();
                if(tk.tokenId == NUM_tk)
                {
                    addToken(ptr,tk);
                    tk = scanner ();
                    if(tk.tokenId == OP_tk && tk.tokenInstance == ";")
                    {
                        tk = scanner ();
                        addChild(ptr,vars());
                        return ptr;
                    }
                    else
                        error(";");
                }
                else
                    error("Number");
            }
            else
                error(":=");
        }
        else
            error("Identifier");
    }
    //since empty also there
    else
        return NULL;
    return NULL;
}

//<program> -- whole , program
//<program> -><vars> program <block>
node* program(){
    node * ptr = getNode("<program>");
    addChild(ptr,vars());

    //if no error in var
    if(tk.tokenId == KW_tk && tk.tokenInstance == "program")
    {
        tk = scanner();
        addChild(ptr,block());
        return ptr;
    }
    else
        error("program");
    return NULL;
}

node* parser(){
    tk = scanner();
    //std::cout << tk.tokenInstance;
    node * root = program();
    if(tk.tokenId==EOF_tk){
        //printf("PARSE SUCCESSFUL\n");
        return root;
    }
    else{
    	//edge case
        printf("Unknown Error\n");
    }
    return root;
}

node* getNode(string fname)
{
    node * t = (node *) malloc(sizeof(node));
    //std::cout << sizeof(node);
    if (t == NULL)
    {
    	//debugging purposes
    	cout << "test";
    }
    int tset=0;
    //t->tokenId = tk.tokenId;
    //t->tokenInstance = tk.tokenInstance;
    //t->fname = fname;
    strcpy(t->fname, fname.c_str());
    // strcpy(t->tokenInstance, tk.tokenInstance.c_str());
    t->childCount=0;
    // t->lineNo = tk.lineNo;
    // t->charNo = tk.charNo;
    for(int i=0;i<NODE_MAX_CHILD;i++)
        t->child[i]=NULL;

    return t;
}

void addToken(node* ptr,token tk)
{
		ptr->tokenId[ptr->tsetCount] = tk.tokenId;
	    ptr->lineNo[ptr->tsetCount] = tk.lineNo;
	    ptr->charNo[ptr->tsetCount] = tk.charNo;
	    strcpy(ptr->tokenInstance[ptr->tsetCount], tk.tokenInstance.c_str());
	    ptr->tsetCount++;
}

void addChild(node * parent, node *child)
{
    if(child !=NULL)
    {
        parent->child[parent->childCount] = child;
        parent->childCount++;
    }
}

//print tree
void printNode(node* ptr,int level)
{
    for(int i=1;i<=level;i++)
        printf("  ");
    printf("%s\n",ptr->fname);
    for(int i=0;i<ptr->tsetCount;i++)
    {
        for(int i=1;i<=level;i++)
            printf("  ");

        printf("[%s %s LINE:%d, CHAR:%d]\n",\
        tokenNames[ptr->tokenId[i]].c_str(),\
        ptr->tokenInstance[i],\
        ptr->lineNo[i],\
        ptr->charNo[i]);
    }
}

void parseTree(node* root,int level)
{
        printNode(root,level);
        //child nodes;
        level++;
        for(int i=0;i<root->childCount;i++)
        {
            parseTree((node*)(root->child[i]), level);
        }
}
