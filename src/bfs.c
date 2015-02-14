#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "dict.h"
#include "arr.h"
#include "ll.h"

#include "bfs.h"

#include "queue.h"
#include "set.h"

void render_arr(LengthedArray*);

void* ll_remove_head(LinkedList* ll) {
    LinkedListNode* old_start = ll->start;
    ll->start = ll->start->next;
    return old_start->val;
}


void ll_prepend(LinkedList* ll, void* val) {
    LinkedListNode* old_head = ll->start;
    ll->start = ll_create_node(val);
    ll->start->next = old_head;

    if (ll->end == NULL) {
        ll->end = ll->start;
    }
}

void* ll_peek_tail(LinkedList* ll) {
    return ll->end->val;
}


char* lengthed_hasher(void* v_arr) {
    int i;
    char* appended;
    LengthedArray* arr = v_arr;
    size_t total_length = 0;

    for (i=0; i<arr->len; i++) {
        total_length += strlen(arr->array[i]);
    }

    // return arr_coalesce(arr, total_length)

    appended = malloc(sizeof(char*) * total_length);

    for (i=0; i<arr->len; i++) {
        strcat(appended, arr->array[i]);
    }

    return appended;
}


LengthedArray* arr_make(int num, ...) {
    int i;
    va_list arguments;
    LengthedArray* arr;

    arr = arr_create();

    va_start(arguments, num);
    for (i=0; i<num; i++) {
        arr_append(
            arr,
            strdup(va_arg(arguments, char*))
        );
    }
    va_end(arguments);

    return arr;
}


LengthedArray* arr_immutable_add(LengthedArray* arr, char* thing) {
    // returns a new array, with `thing` added to the end
    int i;
    LengthedArray* new_arr;

    new_arr = arr_create();

    for (i=0; i<arr->len; i++) {
        arr_append(new_arr, strdup(arr->array[i]));
    }
    arr_append(new_arr, strdup(thing));

    return new_arr;
}


void render_arr(LengthedArray* arr) {
    int i;
    printf("[");
    for (i=0; i<arr->len; i++) {
        printf("\"%s\"", arr->array[i]);
        if (i != arr->len - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}


LengthedArray* c_bfs(char* start, char* end, dict* lookup, int joints) {
    Queue* Q;
    Set* V;
    LengthedArray* start_arr;

    V = Set_create(lengthed_hasher);
    Q = Q_create();

    start_arr = arr_make(1, start);
    Set_add(V, start_arr);
    Q_enqueue(Q, start_arr);

    while (!Q_is_empty(Q)) {
        char* last;
        int i;
        LengthedArray *next_words, *t;

        t = Q_dequeue(Q);
        if (t->len > joints) {
            // free(t);
            continue; // cannot be valid, ignore
        }

        last = t->array[t->len - 1];

        if (strcmp(last, end) == 0) {
            if (joints == t->len) {
                char* joined;
                printf("found with %d possible paths remaining in the queue\n", Q->count);
                printf(
                    "%d possible paths seen, from %s to %s\n",
                    V->data->count,
                    start, end
                );

                // // easier to just join it here
                // joined = arr_coalesce(t, arr_total_length(t));

                // printf(
                //     "%d\n",
                //     Set_contains(V, t)
                // );

                // // save our precious result from being destroyed
                // Set_remove(V, t);

                // printf(
                //     "%d\n",
                //     Set_contains(V, t)
                // );

                // // Q_free(Q, arr_free);
                // printf("%s\n", t->array[0]);
                // Set_free(V, arr_free);
                // printf("%s\n", t->array[0]);

                return t;
            }
        }

        next_words = dict_get(lookup, last);
        if (next_words == NULL) continue; // not in lookup

        for (i=0; i<next_words->len; i++) {
            char* item;
            LengthedArray* pos;
            item = next_words->array[i];

            pos = arr_immutable_add(t, item);

            if (!Set_contains(V, pos)) {
                // not already seen
                Set_add(V, pos);
                Q_enqueue(Q, pos);
            } else {
                free(pos);
            }
        }

        // just free the container, not the contents, as it's contents
        // likely exist elsewhere
        // free(t);
    }

    // both the set and queue contain pointers to the same blocks of memory;
    // only free 'em once :P
    // Q_free(Q, arr_free);
    // Set_free(V, arr_free);

    return NULL;
}
