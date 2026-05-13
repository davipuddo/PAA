#include <stdio.h>
#include <stdlib.h>

int* copyVec (int* arr, int n) {
    int* copy = NULL;
    if (arr && n > 0) {
        copy = (int*)malloc(n*sizeof(int));
        for (int i = 0; i < n; i++) {
            copy[i] = arr[i];
        }
    }
    return copy;
}

typedef struct value {
    int cn;
    int n;
    int cost;
    int* coins;
}
Value;

Value* newValue(int n, int cost, int cn, int* coins) {
    Value* self = (Value*)malloc(sizeof(Value));
    if (self) {
        self->cost = cost;
        self->cn = cn;
        self->n = n;
        self->coins = coins;
    }
    return self;
}

void freeValue(Value* x) {
    if (x) {
        if (x->coins) {
            free(x->coins);
            x->coins = NULL;
        }
        free(x);
    }
}

void printValue(Value* x) {
    if (x) {
        printf("%d | ", (x->cost != 0x7fffffff ? x->cost : -1));
        for (int i = 1; i < x->cn; i++) {
            printf("[%d] ", x->coins[i]);
        }
    } else {
        printf("Invalid value!\n");
    }
}

int* eval (int* base, int* coins, int n) {
    int* res = NULL;
    if (base && coins && n > 0) {
        res = (int*)malloc(n*sizeof(int));
        for (int i = 0; i < n; i++) {
            res[i] = 0;
        }
    
        int j = 0;
        for (int i = 0; i < n; i++) {
            int diff = base[i] - coins[i];
            if (diff > 0) {
                for (int k = 0; k < diff; k++) {
                    res[k+j] = i;
                }
                j+=diff;
            }
        }
    }
    return res;
}

int main (void) {
    
    int n = 4;
    int coins [4] = {1, 1, 4, 5}; // Should be sorted

    int biggest = coins[n-1];

    printf("coins: {");
    for (int i = 0; i < n; i++) {
        if (i < n-1) {
            printf("%d, ", coins[i]);
        } else {
            printf("%d}\n", coins[i]);
        }
    }

    int target = 0;
    printf("target = ");
    scanf("%d", &target);
    getchar();

    // Define table size
    int D = target;
    if (D < biggest) {
        D = biggest;
    }
    D++;

    // Create table of values
    Value** dp = (Value**)malloc(D*sizeof(Value*));

    int Bn = biggest+1;
    int* base_coins = (int*)malloc(Bn*sizeof(int));
    for (int i = 0; i < Bn; i++) {
        base_coins[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        base_coins[coins[i]]++;
    }

    for (int i = 0; i < D; i++) {
        int* copy = copyVec(base_coins, Bn);
        dp[i] = newValue(n, 0x7fffffff, Bn, copy);
    }
    dp[0]->cost = 0;

    free(base_coins);

    // Find values under, and target
    for (int k = 1; k <= target; k++) { // 1 is included

        for (int i = 0; i < n; i++) {   // Check options with every coin
            int coin = coins[i];

            if (k >= coin) {             // Is a valid coin

                Value* previous = dp[k-coin];
                int C = previous->cost+coin;

                if (C > 0 && previous->coins[coin] > 0 &&
                        (
                            (C < dp[k]->cost) || 
                            (C == dp[k]->cost && previous->n > dp[k]->n)) 
                        ) 
                {
                    int* tmp = dp[k]->coins;

                    dp[k]->coins = copyVec(previous->coins, previous->cn);
                    dp[k]->coins[coin]--;
                    dp[k]->cost = C;
                    dp[k]->n = previous->n-1;

                    free(tmp);
                }
            }
        }

    }

    // Print table of values
    printf("\nDP -\n");
    for (int i = 0; i < target+1; i++) {
        printf("%d: ", i);
        printValue(dp[i]);
        printf("\n");
    }
    printf("\n");

    if (dp[target]->cost != 0x7fffffff) {
        printf("The target can be reached with: ");
        int* w = eval(dp[0]->coins, dp[target]->coins, Bn);
        for (int i = 0; i < Bn; i++) {
            if (w[i]) {
                printf("[%d] ", w[i]);
            }
        }
        printf("\n");
        free(w);

    } else {
        printf("The target can't be reached from this set of coins\n");
    }

    // Free everything
    for (int i = 0; i < target+1; i++) {
        if (dp[i]) {
            freeValue(dp[i]);
        }
    }
    if (dp) {
        free(dp);
    }
    return (0);
}
