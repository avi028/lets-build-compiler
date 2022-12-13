#include "compfs.h"

int main(int argc , char ** argv){

    string filename;
    if(argc < 2){
        ofstream createdFile;
        string userInput;
        createdFile.open("keyboardinput.txt");
        filename = "keyboardinput.txt";
			if (createdFile) {
				while (std::getline(std::cin, userInput)) {
					createdFile << userInput << "\n";
				}
			}
			else {
				cout << "ERROR: failed to create file...\n";
				exit(1);
			}
			createdFile.close();
        }
    else if(argc==2){
        filename = string(argv[1]);
    }

    ifstream f(filename.c_str(), std::ios_base::in);
    fileReader = &f;

    lineItr=0;
    charRead=0;
    lineNumber=0;
    tokItr=0;
    tokLen=0;

    //printFsa();
    setupScanner(); //building FSA table
    //testScanner();
    node* root =  parser();
    // parseTree(root,0);
    if(statSem(root)>=0){
        FILE * out;   
        char out_file [100];
        snprintf(out_file,100,"%s.asm",filename.c_str());     
        out = fopen(out_file,"w+");
        compfs(root,out);
        fclose(out);
    }
    f.close();
    return 0;
}
