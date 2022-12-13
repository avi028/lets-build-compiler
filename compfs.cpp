#include <iostream>
#include <vector>

#include "compfs.h"

bool pg_check=false;

struct g_v{
    char name[11];
    int val;
};

std::vector<struct g_v> globals;

int find_in_globals(char * var){
    for (int i=0;i<globals.size();i++){
        if(strcmp(globals[i].name,var)==0)
            return i;
    }
    return -1;
}
std::vector<int> c_stck;
int counter_var=0;
int counter_label=0;

void compfs(node* root,FILE *out)
{
    char tmpName[11];
    char tmpLabel[11];
    if(strcmp(root->fname,"<vars>")==0){
        if(pg_check==false){
            int val = find_in_globals(root->tokenInstance[0]);
            if(val==-1){
                struct g_v tmp;
                strcpy(tmp.name,root->tokenInstance[0]);
                tmp.val = atoi(root->tokenInstance[2]);
                globals.push_back(tmp);
                fprintf(out,"LOAD %s\n",root->tokenInstance[2]);
                fprintf(out,"STORE %s\n",root->tokenInstance[0]);
            }                
            else{
                printf("Redefination of Global variable %s at %d,%d\n",root->tokenInstance[0],root->lineNo[0],root->charNo[0]);
            }
        }
        else{
            if(c_stck.empty()==true){
                int val = stck.push(string(root->tokenInstance[0]),root->lineNo[0],root->charNo[0]);
                fprintf(out,"LOAD %s\n",root->tokenInstance[2]);
                fprintf(out,"PUSH\n");
                fprintf(out,"STACKW %d\n",val);
            }
            else{
                if(stck.find(string(root->tokenInstance[0])) ==-1 && find_in_globals(root->tokenInstance[0])==-1){
                    c_stck[c_stck.size()-1]++;
                    int val = stck.push(string(root->tokenInstance[0]),root->lineNo[0],root->charNo[0]);
                    fprintf(out,"LOAD %s\n",root->tokenInstance[2]);
                    fprintf(out,"PUSH\n");
                    fprintf(out,"STACKW %d\n",val);
                }
                else{
                   printf("Redefination of Local variable %s at %d,%d\n",root->tokenInstance[0],root->lineNo[0],root->charNo[0]);
                }
            }
        }
        if(root->childCount>0)
            compfs((node*)(root->child[0]),out);    
    }
    else if(strcmp(root->fname,"<program>")==0){
        if(root->childCount==1){
            pg_check = true;
            compfs((node*)(root->child[0]),out);
        }
        else{
            compfs((node*)(root->child[0]),out);
            pg_check = true;
            compfs((node*)(root->child[1]),out);    
        }
        fprintf(out,"STOP\n");           
        for(int i=0;i<globals.size();i++){
            fprintf(out,"%s %d\n",globals[i].name,globals[i].val);
        }
        while(counter_var>0){
            fprintf(out,"T_%d 0\n",counter_var-1);
            counter_var--;
        }
        return;
    }
    else if(strcmp(root->fname,"<block>")==0){
        c_stck.push_back(0); 
        for(int i=0;i<root->childCount;i++){
            compfs((node*)(root->child[i]),out);
        }    
        for(int i=0;i<c_stck[c_stck.size()-1];i++){
            stck.pop();
            fprintf(out,"POP\n"); 
        }
        c_stck.pop_back();
    }
    else if(strcmp(root->fname,"<in>")==0){
        int val = stck.find(string(root->tokenInstance[0]));
        if(val==-1){
            val = find_in_globals(root->tokenInstance[0]);
            if(val==-1){
                printf("Cannnot find variable %s at %d,%d\n",root->tokenInstance[0],root->lineNo[0],root->charNo[0]);
            }
            // global variable
            else{
                fprintf(out,"READ %s\n",root->tokenInstance[0]);
            }
        }
        //local variable 
        else{
            snprintf(tmpName,11,"T_%d",counter_var++);
            fprintf(out,"READ %s\n",tmpName);
            fprintf(out,"LOAD %s\n",tmpName);
            fprintf(out,"STACKW %d\n",val);
        }
    }
    else if(strcmp(root->fname,"<out>")==0) 
    {
        compfs((node*)(root->child[0]),out);
        snprintf(tmpName,11,"T_%d",counter_var++);
        fprintf(out,"STORE %s\n",tmpName);
        fprintf(out,"WRITE %s\n",tmpName);
    }
    else if(strcmp(root->fname,"<expr>")==0) {
        if(root->tsetCount==0){ 
            compfs((node*)(root->child[0]),out);
        }
        else{
            compfs((node*)(root->child[1]),out);

            snprintf(tmpName,11,"T_%d",counter_var++);
            fprintf(out,"STORE %s\n",tmpName);

            compfs((node*)(root->child[0]),out);

            fprintf(out,"SUB %s\n",tmpName);
        }
    }
    else if(strcmp(root->fname,"<N>")==0) {
        if(root->tsetCount==0){ 
            compfs((node*)(root->child[0]),out);
        }
        else if(root->tokenInstance[0][0]=='+'){
            compfs((node*)(root->child[0]),out);

            snprintf(tmpName,11,"T_%d",counter_var++);
            fprintf(out,"STORE %s\n",tmpName);

            compfs((node*)(root->child[1]),out);

            fprintf(out,"ADD %s\n",tmpName);
        }
        else if(root->tokenInstance[0][0]=='*'){
            compfs((node*)(root->child[0]),out);

            snprintf(tmpName,11,"T_%d",counter_var++);
            fprintf(out,"STORE %s\n",tmpName);

            compfs((node*)(root->child[1]),out);

            fprintf(out,"MULT %s\n",tmpName);          
        }
    }
    else if(strcmp(root->fname,"<A>")==0){
        if(root->childCount==1){
            compfs((node*)(root->child[0]),out);
        }
        else{
            compfs((node*)(root->child[1]),out);

            snprintf(tmpName,11,"T_%d",counter_var++);
            fprintf(out,"STORE %s\n",tmpName);

            compfs((node*)(root->child[0]),out);

            fprintf(out,"DIV %s\n",tmpName);          
        }
    }
    else if(strcmp(root->fname,"<A_>")==0){
            compfs((node*)(root->child[0]),out);    
    }
    else if(strcmp(root->fname,"<M>")==0){
        if(root->tsetCount==0){
            compfs((node*)(root->child[0]),out);    
        }
        else{
            compfs((node*)(root->child[0]),out);    
            //negate
            fprintf(out,"MULT -1\n");          
        }
    }
    else if(strcmp(root->fname,"<R>")==0){
        if(root->childCount==0){
            // load variable from the stack or store 
            if(root->tokenId[0]==IDENT_tk){
                int val = stck.find(string(root->tokenInstance[0]));
                if(val==-1){
                    val = find_in_globals(root->tokenInstance[0]);
                    if(val==-1){
                        printf("Cannnot find variable %s at %d,%d\n",root->tokenInstance[0],root->lineNo[0],root->charNo[0]);
                    }
                    // global variable
                    else{
                        fprintf(out,"LOAD %s\n",root->tokenInstance[0]);
                    }
                }
                //local variable 
                else{
                    fprintf(out,"STACKR %d\n",val);
                }
            }
            // integer load directly
            else{
                fprintf(out,"LOAD %s\n",root->tokenInstance[0]);
            }
        }
        else{
            compfs((node*)(root->child[0]),out);    
        }
    }
    else if(strcmp(root->fname,"<assign>")==0){
            compfs((node*)(root->child[0]),out);    

            int val = stck.find(string(root->tokenInstance[0]));
            if(val==-1){
                val = find_in_globals(root->tokenInstance[0]);
                if(val==-1){
                    printf("Cannnot find variable %s at %d,%d\n",root->tokenInstance[0],root->lineNo[0],root->charNo[0]);
                }
                // global variable
                else{
                    fprintf(out,"STORE %s\n",root->tokenInstance[0]);                      
                }
            }
            //local variable 
            else{
                fprintf(out,"STACKW %d\n",val);
            }
    }
    else if(strcmp(root->fname,"<if>")==0){
        compfs((node*)(root->child[2]),out);    
        char pickLabel[11];
        snprintf(tmpName,11,"T_%d",counter_var++);
        fprintf(out,"STORE %s\n",tmpName);
        
        compfs((node*)(root->child[0]),out);    
        node * RO =(node *) root->child[1];
        snprintf(tmpLabel,11,"V_%d",counter_label++);
        snprintf(pickLabel,11,"V_%d",counter_label++);


        if(strcmp(RO->tokenInstance[0],"[")==0){
            fprintf(out,"MULT %s\n",tmpName);   
            // move to label if of different sign
            fprintf(out,"BRNEG %s\n",tmpLabel);                        
        }
        else{
            fprintf(out,"SUB %s\n",tmpName);

            if(strcmp(RO->tokenInstance[0],"<") ==0){
                // move to label if >=
                fprintf(out,"BRZPOS %s\n",tmpLabel);            
            }
            else if(strcmp(RO->tokenInstance[0],">")==0){
                // move to label if <=
                fprintf(out,"BRZNEG %s\n",tmpLabel);                        
            }
            else if(strcmp(RO->tokenInstance[0],"==")==0){
                // move to label if >
                fprintf(out,"BRPOS %s\n",tmpLabel);                        
                // move to label if <
                fprintf(out,"BRNEG %s\n",tmpLabel);                        
            }
            else if(strcmp(RO->tokenInstance[0],"!=")==0){
                // move to label if ==
                fprintf(out,"BRZERO %s\n",tmpLabel);                                    
            }
        }
        compfs((node*)(root->child[3]),out); 
        if(root->childCount==4){            
            fprintf(out,"%s: NOOP\n",tmpLabel);
        }
        else{
            fprintf(out,"BR %s\n",pickLabel);           
            fprintf(out,"%s: NOOP\n",tmpLabel);
            compfs((node*)(root->child[4]),out); 
            fprintf(out,"%s: NOOP\n",pickLabel);
        }

    }    
    else if(strcmp(root->fname,"<loop>")==0){
        char labelIn[11];
        char labelOut[11];

        snprintf(labelIn,11,"V_%d",counter_label++);
        fprintf(out,"%s: NOOP\n",labelIn);                                    

        compfs((node*)(root->child[2]),out);    

        snprintf(tmpName,11,"T_%d",counter_var++);
        fprintf(out,"STORE %s\n",tmpName);
        
        compfs((node*)(root->child[0]),out);    


        node * RO = (node *) root->child[1];
        snprintf(labelOut,11,"V_%d",counter_label++);
        if(strcmp(RO->tokenInstance[0],"[")==0){
            fprintf(out,"MULT %s\n",tmpName);
            // move to label if of different sign
            fprintf(out,"BRNEG %s\n",labelOut);                        
        }
        else{

            fprintf(out,"SUB %s\n",tmpName);

            if(strcmp(RO->tokenInstance[0],"<") ==0){
                // move to label if >=
                fprintf(out,"BRZPOS %s\n",labelOut);            
            }
            else if(strcmp(RO->tokenInstance[0],">")==0){
                // move to label if <=
                fprintf(out,"BRZNEG %s\n",labelOut);                        
            }
            else if(strcmp(RO->tokenInstance[0],"==")==0){
                // move to label if >
                fprintf(out,"BRPOS %s\n",labelOut);                        
                // move to label if <
                fprintf(out,"BRNEG %s\n",labelOut);                        
            }
            else if(strcmp(RO->tokenInstance[0],"!=")==0){
                // move to label if ==
                fprintf(out,"BRZERO %s\n",labelOut);                                    
            }
        }
        compfs((node*)(root->child[3]),out);    
        fprintf(out,"BR %s\n",labelIn);                                            
        fprintf(out,"%s: NOOP\n",labelOut);                                    
    }   
    else if(strcmp(root->fname,"<label>")==0){
        fprintf(out,"%s: NOOP\n",root->tokenInstance[0]);
    }
    else if(strcmp(root->fname,"<goto>")==0){
        fprintf(out,"BR %s\n",root->tokenInstance[0]);
    } 
    // <stats> <stat> <mStat> 
    else{
        for(int i=0;i<root->childCount;i++){
            compfs((node*)(root->child[i]),out);
        }
    }    
}
