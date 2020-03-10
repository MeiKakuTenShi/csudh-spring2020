#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ctype.h"

int lineNum =1; //counter for line number
char errorMessage[50] = "improper formatting. check your syntax carefully.";


int mStart;
typedef enum {read, write, id, literal, becomes,
                addOp, subOp, mulOp, divOp, lparen, rparen, eof} token;

char token_image[100];

FILE *fileSrc;
char buffer[80];

void setSource(FILE *src)
{
    fileSrc = src;
}

void setError(const char* s){
    strcpy(errorMessage, s);
}

const char* getError(){
    return errorMessage;
}

int getNext()
{
    char c;

    if (fileSrc == NULL)
        c = getchar();
    else {
        c = fgetc(fileSrc);
        if (c == '\n')
        {
            ++lineNum;
        }
    }


    // printf("next char: %c\n", c);
    return c;
}

int getLine()
{
    if(lineNum==1) return 1;
    else return --lineNum;
}


token scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */
    //int x;
    //x = endofline(fileSrc, lineNum);
    //printf("x= %d \n",x);
    /* skip white space */
    while (isspace(c)) {
        c = getNext();
    }
    if (c == EOF)
        return eof;
    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            c = getNext();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return read;
        else if (!strcmp(token_image, "write")) return write;
        else return id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = getNext();
        } while (isdigit(c));
        token_image[i] = '\0';
        return literal;
    } else switch (c) {
        case ':':
            if (c = getNext() != '=') {
                fprintf(stderr, "lexical error on line #%d\n",lineNum);
                exit(1);
            } else {
                c = getNext();
                return becomes;
            }
            break;
        case '+': c = getNext(); return addOp;
        case '-': c = getNext(); return subOp;
        case '*': c = getNext(); return mulOp;
        case '/':
            c = getNext();
            switch (c) {
                case '/':
                    printf("line comment found on line #%d\n",lineNum);
                    do {
                        c = getNext();
                    }while(c != '\n');
                    c = getNext();
                    return scan();
                case '*':
                    mStart=lineNum;
                    do {
                        c = getNext();
                        if(c == '*') {
                            c = getNext();
                            if (c == '/') {
                                c = getNext();
                                return scan();
                            }
                        }
                    }while(c != EOF);
                    printf("lexical error: invalid multi-line comment starting on line #%d\n",mStart);
                    exit(1);
                default:
                    return divOp;
            }
        case '(': c = getNext(); return lparen;
        case ')': c = getNext(); return rparen;
        default:
            printf("lexical error on line #%d\n",lineNum);
            exit(1);
    }
}
