#ifndef _TOKEN_H

#include <string>

std::string keywords []= {"begin", "end" ,"do", "while", "whole", "label", "return", "input", "output", \
                            "program", "warp", "if", "then", "pick", "declare", "assign", "func"};
unsigned int keywordsCount = 17;

std::string operators [] = {"=", "<", " >", "==", "!=", ":", ":=", "+", "-", "*", "/", "^", ".", "(", ")", ",","{", "}", \
                            ";", "[", "]","||", "&&"};
unsigned int operatorsCount = 23;

#endif
