#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arr.h"
#include "dict.h"
#include "bfs.c"
#include "markov/tokenise.h"

typedef struct {
    bool decode;
    char* input;
    int skip_num;
    LengthedArray* files;
} options;


char* build_skip_code(LengthedArray* la_words, dict* lookup, unsigned int joints) {
    int i, len;
    LengthedArray* sentence = arr_create();

    for (i=0; i<(la_words->len-1); i++) {
        char *first, *second;
        LengthedArray* res;

        first = la_words->array[i];
        second = la_words->array[i+1];

        res = c_bfs(first, second, lookup, joints);
        if (res == NULL) {
            return NULL;
        }

        if (i != 0) {
            char* discarded;
            // every chunk after the first, we slice the first word off
            res->len -= 1;
            discarded = res->array[0];
            res->array += 1;
            free(discarded);
        }

        printf("len: %d, size: %d\n", sentence->len, sentence->size);

        sentence = arr_merge(
            sentence,
            res
        );
    }

    len = arr_total_length(sentence);
    printf("%d\n", len);

    return arr_coalesce(sentence, len);
}


void decode(char* sentence, int interval) {
    int i=0;
    char* word = strtok(sentence, " ");

    while (word != NULL) {
        if ((i % interval) == 0) {
            printf("%s ", word);
        }
        word = strtok(NULL, " ");
        i++;
    }
    printf("\n");
}


void print_help(char const *argv[]) {
    fprintf(
        stderr,
        "%s [-n skip_num] [-d (decode)] \"Input sentence\" [lookup files...] \n",
        argv[0]
    );
}


options* parse_args(int argc, char const *argv[]) {
    int i;
    options* ops = calloc(sizeof(*ops), 1);

    // default values
    ops->skip_num = 3;
    ops->decode = FALSE;
    ops->files = arr_create();

    for (i=1; i<argc; i++) {
               if (strcmp(argv[i], "-n") == 0) {
            ops->skip_num = atoi(argv[++i]);

        } else if (strcmp(argv[i], "-d") == 0) {
            ops->decode = TRUE;

        } else {
            if (ops->input != NULL) {
                arr_append(ops->files, (char*)argv[i]);
            } else {
                ops->input = strdup(argv[i]);
            }
        }
    }

    if (ops->input == NULL) {
        print_help(argv);
        fprintf(stderr, "No input provided\n");
        exit(-1);
    }

    return ops;
}


LengthedArray* tokenise(char* input) {
    char* word = NULL;
    LengthedArray* arr = arr_create();

    word = strtok(input, " ");

    while (word != NULL) {
        arr_append(arr, word);
        word = strtok(NULL, " ");
    }

    return arr;
}


void lookup_table_free(dict* lookup_table) {}


void encode(char* input, int skip_num, LengthedArray* files) {
    dict* lookup;
    char* sentence;
    LengthedArray* words;

    words = tokenise(input);
    lookup = tokenise_files(files, FALSE);

    sentence = build_skip_code(words, lookup, skip_num);
    if (sentence == NULL) {
        fprintf(stderr, "Unable to build skip code with given input\n");
    } else {
        printf("%s\n", sentence);
    }
}


int main(int argc, char const *argv[]) {
    options* ops = parse_args(argc, argv);

    "Save souls Now! John or James Watson!";
    "Saint or Sinner? James or John? The more is less?";

    if (ops->decode) {
        decode(ops->input, ops->skip_num);

    } else {
        encode(ops->input, ops->skip_num, ops->files);
    }
}
