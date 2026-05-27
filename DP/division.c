#include <stdio.h>
#include <stdlib.h>

double max(double x, double y) {
    double res = x;
    if (x < y) {
        res = y;
    }
    return res;
}

double min(double x, double y) {
    double res = x;
    if (x > y) {
        res = y;
    }
    return res;
}

typedef struct table_s {
    double** data;
    int n;
}
Table;

Table* newTable(int n) {
    Table* self = (Table*)malloc(n*sizeof(Table));
    if (self) {
        self->n = n;
        self->data = (double**)malloc(n*sizeof(double*));
        for (int i = 0; i < n; ++i) {
            self->data[i] = (double*)malloc(n*sizeof(double));
            for (int j = 0; j < n; j++) {
                self->data[i][j] = -1.0;
            }
        }
    }
    return (self);
}

void freeTable(Table* t) {
    if (t && t->data) {
        for (int i = 0; i < t->n; i++) {
            if (t->data[i]) {
                free(t->data[i]);
            }
        }
        if (t->data) {
            free(t->data);
        }
    }
}

void printTable(Table* t) {
    for (int i = 0; i < t->n; i++) {
        for (int j = 0; j < t->n; j++) {
            printf("[%.2lf] ", t->data[i][j]);
        }
        printf("\n");
    }
}

double _fE (Table* t, int i, int j);

double fE (Table* t, int i, int j) {
    double res = 0.0;
    if (t) {
        if (t->data[i][j] != -1.0) {        // Already in the table
            res = t->data[i][j];
        }
        else if (i == j) {                  // Single value
            res = t->data[i][i];
        } else {
            double tmp = 0.0;
            for (int k = i; k < j; k++) {
                tmp = max(tmp, fE(t, i, k) / _fE(t, k+1, j));
            }
            res = tmp;
            t->data[i][j] = res;
        }
    }
    return res;
}

double _fE (Table* t, int i, int j) {
    double res = 0.0;
    if (t) {
        if (t->data[j][i] != -1.0) {        // Already in the table
            res = t->data[j][i];
        }
        else if (i == j) {                  // Single value
            res = t->data[i][i];
        } else {
            double tmp = (double)0x7fffffff;
            for (int k = i; k < j; k++) {
                tmp = min(tmp, _fE(t, i, k) / fE(t, k+1, j));
            }
            res = tmp;
            t->data[j][i] = res;
        }
    }
    return res;
}

void printResult(Table* t) {
    if (t) {
        double tmp = 0.0;
        for (int k = 0; k < t->n-1; k++) {
            tmp = max(tmp, t->data[0][k]);
        }
    }
}

int main (void) {

    double E [4] = {5, 0.5, 2, 10};
    int n = 4;

    Table* t = newTable(n);
    
    for (int i = 0; i < n; i++) {
        t->data[i][i] = E[i];
    }

    printTable(t);

    double res = fE(t, 0, n-1);

    printf("\n");
    printTable(t);
    printf("res = %lf\n", res);

    printResult(t);

    if (t) {
        freeTable(t);
        free(t);
    }

    return(0);
}
