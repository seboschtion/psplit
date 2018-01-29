#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// forward declaration
void parse_opts(int argc, char *argv[]);
void parse_args(int argc, char *argv[]);
void print_help(char *helper_msg);
void split();
void split_sentence();
_Bool skip_char_due_w(char c, char last_c);

// p options
enum pattern_options { SENTENCE };

// set options
int p_option;
char *t_option = ".";
_Bool w_option;

// input file
char *input_file;

int main(int argc, char *argv[]) {
    int p;
    parse_opts(argc, argv);
    parse_args(argc, argv);
    if(input_file == NULL) {
        print_help("No input file given");
        exit(2);
    }

    split();

    exit(0);
}

void split() {
    FILE *file = fopen(input_file, "r");
    switch(p_option) {
        case SENTENCE:
            split_sentence(*file);
            break;
        default:
            break;
    }
    fclose(file);
}

void split_sentence(FILE *file) {
    int current_filenumber = -1;
    FILE *current_out_file;
    char c;
    char last_c;
    _Bool in_a_quoted_text = 0;
    while((c = fgetc(file)) != EOF) {
        if(c == '"' && !in_a_quoted_text) {
            in_a_quoted_text = 1;
        } else if(c == '"' && in_a_quoted_text) {
            in_a_quoted_text = 0;
        }
        if(current_out_file == NULL && (in_a_quoted_text || isupper(c))){
            current_filenumber++;
            char current_filenumber_string[12];
            sprintf(current_filenumber_string, "%s/%x", t_option, current_filenumber);
            current_out_file = fopen(current_filenumber_string, "w");
        }
        if(current_out_file != NULL) {
            if(w_option && skip_char_due_w(c, last_c)) {
                continue;
            }
            fputc(c, current_out_file);
        }
        if(current_out_file != NULL && !in_a_quoted_text && c == '.') {
            fclose(current_out_file);
            current_out_file = NULL;
        }
        last_c = c;
    }
    if(current_out_file != NULL){
        fclose(current_out_file);
    }
}

_Bool skip_char_due_w(char c, char last_c) {
    if(c == '\n') {
        return 1;
    }
    if(c == ' ' && last_c == ' ') {
        return 1;
    }
    return 0;
}

void parse_opts(int argc, char *argv[]) {
    if(argc <= 1) {
        print_help("No option set");
        exit(1);
    }

    int opt;
    while((opt = getopt(argc, argv, "hwp:t:")) != -1) {
        switch(opt) {
            case 'p':
                if(strcmp("sentence", optarg) == 0) {
                    p_option = SENTENCE;
                }
                break;
            case 't':
                t_option = optarg;
                break;
            case 'w':
                w_option = 1;
                break;
            case 'h':
                print_help(NULL);
                exit(0);
            default:
                print_help("Please check your options");
                exit(1);
        }
    }
}

void parse_args(int argc, char *argv[]){
    int i;
    for(i = optind; i < argc; i++) {
        input_file = argv[i];
    }
}

void print_help(char *helper_msg) {
    if(helper_msg != NULL) {
        printf("Error: %s\n\n", helper_msg);
    }
    printf("psplit, a command line tool to split a (text)file into patterns.\n"
            "\n"
            "Usage:\n"
            "psplit -p \"pattern\" \"myfile.txt\"\n"
            "\n"
            "Currently supported patterns:\n"
            "sentence    Split text by sentences\n"
            "\n"
            "All options:\n"
            "-p          Pattern, default is \"sentence\"\n"
            "-t          Where to store the files to\n"
            "-w          Skip CR and LF and double whitespaces\n"
            "-h          Display this help.\n"
          );
}
