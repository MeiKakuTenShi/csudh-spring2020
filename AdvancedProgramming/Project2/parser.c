/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.17.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {read, write, id, literal, becomes,
                addOp, subOp, mulOp, divOp, lparen, rparen, eof} token;

extern char token_image[];
int lineNum;
char errorSend[50] = "improper formatting. check your syntax carefully.";
char* names[] = {"read", "write", "id", "literal", "becomes",
                "add", "sub", "mul", "div", "lparen", "rparen", "eof"};

static token input_token;

FILE *src;

void error() {
    lineNum=getLine();
    printf("syntax error on line #%d \n", lineNum);
    printf("\tgetting error:  **  %s **\n", getError());
    exit(1);
}

void match(token expected)
{
    if (input_token == expected)
    {
//        printf("matched %s", names[input_token]);
//        if (input_token == id || input_token == literal)
//            printf(": %s", token_image);
//        printf("\n");
        if(expected != eof)
        {
            input_token = scan();
            printf ("the token is %s \n",names[input_token]);
        }
    }
    else error();

    if (input_token == eof && expected == eof)
    {
        printf("No lexical and syntax error.");
    }
}

void program();
void stmt_list();
void stmt();
void expr();
void term_tail();
void term();
void factor_tail();
void factor();
void add_op();
void mult_op();

void program() {
   // printf("predict program\n");
    switch (input_token) {
        case id:
        case read:
        case write:
        case eof:
            stmt_list();
            match(eof);
            break;
        default:
            strcpy(errorSend, "check your comment syntax");
            setError(errorSend);
            error();
    }
}

void stmt_list() {
    //printf("predict stmt_list\n");
    switch (input_token) {
        case id:
        case read:
        case write:
            stmt();
            stmt_list();
            break;
        case eof:
            break;          /*  epsilon production */
        default:
            error();
    }
}

void stmt() {
    //printf("predict stmt\n");
    switch (input_token) {
        case id:
            match(id);
            //errorSend[50]="incorrect assignment statement";
            strcpy(errorSend, "identifier or assignment formatted incorrectly");
            setError(errorSend);
            match(becomes);
            expr();
            break;
        case read:
            match(read);
            match(id);
            break;
        case write:
            match(write);

            expr();
            break;
        default:
            error();
    }
}

void expr() {
    //printf("predict expr\n");
    switch (input_token) {
        case id:
        case literal:
        case lparen:
            term();
            term_tail();
            break;
        default:
            error();
    }
}

void term_tail() {
    //printf("predict term_tail\n");
    switch (input_token) {
        case addOp:
        case subOp:
            add_op();
            term();
            term_tail();
            break;
        case rparen:
        case id:
        case read:
        case write:
        case eof:
            break;          /*  epsilon production */
        default: error();
    }
}

void term() {
  //  printf("predict term\n");
    switch (input_token) {
        case id:
        case literal:
        case lparen:
            factor();
            factor_tail();
            break;
        default:

            error();
    }
}

void factor_tail() {
//    printf("predict factor_tail\n");
    switch (input_token) {
        case mulOp:
        case divOp:
            mult_op();
            factor();
            factor_tail();
            break;
        case addOp:
        case subOp:
        case rparen:
        case id:
        case read:
        case write:
        case eof:
            break;          /*  epsilon production */
        default:
            error();
    }
}

void factor() {
//    printf("predict factor\n");
    switch (input_token) {
        case id :
            match(id);
            break;
        case literal:
            match(literal);
            break;
        case lparen:
            match(lparen);
            expr();
            strcpy(errorSend, "right parenthesis missing");
            setError(errorSend);
            match(rparen);
            break;
        default:
            strcpy(errorSend, "incorrect character inside parenthesis");
            setError(errorSend);
            error();
    }
}

void add_op() {
//    printf("predict add_op\n");
    switch (input_token) {
        case addOp:
            match(addOp);
            break;
        case subOp:
            match(subOp);
            break;
        default: error();
    }
}

void mult_op() {
//    printf("predict mult_op\n");
    switch (input_token) {
        case mulOp:
            match(mulOp);
            break;
        case divOp:
            match(divOp);
            break;
        default: error();
    }
}

int main(int argc, char* argv[])
{
    char* prog_prefix;
    char file_name[32];

    prog_prefix = "./programs/";

    strcpy(file_name, prog_prefix);

    // program name was passed in as cl arg
    if (argc > 1)
    {
        strcat(file_name, argv[1]);
        printf("opening file: %s\n", file_name);
        src = fopen(file_name, "r");

        // failed to open file
        if (src == NULL)
        {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
        }
    }else
        src = NULL;

    setSource(src);

    input_token = scan();
    printf ("the token is %s \n", names[input_token]);
    program();

    if (src != NULL)
        fclose(src);

    return(0);
}
