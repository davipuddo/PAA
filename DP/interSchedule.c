#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct interval {
    int start;
    int end;
} Interval;

Interval* newInterval(int start, int end) {
    Interval* self = (Interval*)malloc(sizeof(Interval));
    self->start = start;
    self->end = end;
    return self;
}

bool conflict(Interval* x, Interval* y) {
    bool res = false;
    if (x && y) {
        if (y->end < x->end) {
            Interval* tmp = y;
            y = x;
            x = tmp;
        }
        if (x->end > y->start && x->end <= y->end) {
            res = true;
        }
    }
    return res;
}

bool condition (Interval* x, Interval* y) {
    bool res = false;
    if ( (x->end < y->end) ) {
        res = true;
    } else if ( (x->end == y->end) && (x->start < y->start) ) {
        res = true;
    }
    return res;
}

void sort(Interval** S, int n) {
    for (int i = 1; i < n; i++) {
        Interval* tmp = S[i];
        int j = i-1;
        while ( (j >= 0) && condition(tmp, S[j]) ) {
            S[j+1] = S[j];
            j--;
        }
        S[j+1] = tmp;
    }
}

void printInterval (Interval* x) {
    printf("[%d, %d] ", x->start, x->end);
}

void printIntervals (Interval** S, int n) {
    for (int i = 0; i < n; i++) {
        printInterval(S[i]);
    }
    printf("\n");
}

Interval* pop (Interval** S, int* n) {
    Interval* res = NULL;
    if (S) {
        res = S[0];
        for (int i = 0; i < (*n)-1; i++) {
            S[i] = S[i+1];
        }
        S[(*n)-1] = NULL;
        (*n)--;
    }
    return res;
}

int main (void) {

    int n = 5;

    Interval** S = (Interval**)malloc(n*sizeof(Interval*));
    Interval** H = (Interval**)malloc(n*sizeof(Interval*));

    for (int i = 0; i < n; i++) {
        H[i] = NULL;
    }

    S[0] = newInterval(1, 4);
    S[1] = newInterval(2, 4);
    S[2] = newInterval(3, 4);
    S[3] = newInterval(4, 6);
    S[4] = newInterval(5, 7);

    printf("Raw intervals: ");
    printIntervals(S, n);

    sort(S, n);

    printf("Sorted intervals: ");
    printIntervals(S, n);

    int y = 0;

    while (n > 0) {
        Interval* x = pop(S, &n);
    
        if (x) {
            int i = 0;
            while ((i < n) && conflict(x, S[i])) {
                free(pop(S, &n));
            }
            H[y++] = x;
        }
    }

    printf("\nResp: ");
    printIntervals(H, y);

    if (H) {
        for (int i = 0; i < y; i++) {
            if (H[i]) {
                free(H[i]);
            }
        }
        if (H) {
            free(H);
        }
    }
    if (S) {
        free(S);
    }
    
    return(0);
}
