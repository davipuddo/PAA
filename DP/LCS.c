#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* LCS (char*** table, char* x, char* y, int n, int m) {

    char* res = "";
    if (n > 0 && m > 0) {
        char** cur = &table[n-1][m-1];

        if (*cur) {
            res = *cur;

        } else {
            res = (char*)calloc(2, sizeof(char)); // 1 char + \0

            char cx = x[n-1];
            char cy = y[m-1];

            if (cx == cy) {
                res[0] = cx;
                strcat(res, LCS(table, x, y, n-1, m-1));

            } else {
               char* t1 = LCS(table, x, y, n-1, m);
               char* t2 = LCS(table, x, y, n, m-1);

               if (strlen(t1) > strlen(t2)) {
                    strcat(res, t1);
               } else {
                    strcat(res, t2);
               }
            }

            *cur = res;
        }
    }

    return res;
}

void invertS (char* x) {
    if (x) {
        int n = strlen(x);
        int step = (int)((double)n/2.0);

        for (int i = 0; i < step; i++) {
            char tmp = x[i];
            x[i] = x[n-i-1];
            x[n-i-1] = tmp;
        }
    }
}

int main (void) {
    
    char x [6] = "12345";
    char y [6] = "92349";

    int n = 5;
    int m = 5;

    char*** table = (char***)calloc(n,sizeof(char**));

    for (int i = 0; i < n; i++) {
        table[i] = (char**)calloc(m,sizeof(char*));
        for (int j = 0; j < m; j++) {
            table[i][j] = NULL;
        }
    }

    char* res = LCS(table, x, y, n, m);

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < m; j++) {
            if (table[i][j]) {
                printf("[%s] ", table[i][j]);
            }
        }
        printf("\n");
    }

    invertS(res);
    printf("LCS: %s\n", res);

    if (table) {
        for (int i = 0; i < n; i++) {
            if (table[i]) {
                for (int j = 0; j < m; j++) {
                    if (table[i][j]) {
                        free(table[i][j]);
                    }
                }
                free(table[i]);
            }
        }
        free(table);
    }

    return(0);
}
