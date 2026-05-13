#include <stdio.h>
#include <stdlib.h>

typedef struct value {
    int n;
    int* coins;
}
Value;

Value* newValue(int n, int* coins) {
    Value* self = (Value*)malloc(sizeof(Value));
    self->n = n;
    self->coins = coins;
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
    if (x && x->coins) {
        printf("%d | ", x->n);
        for (int i = 0; i < x->n; i++) {
            if (x->coins[i]) {
                printf("[%d] ", x->coins[i]);
            }
        }
    } else {
        printf("Invalid value!\n");
    }
}

int main (void) {
    
    int n = 3;
    int coins [3] = {1, 4, 5}; // Should be sorted

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
    if (D < coins[n-1]) {
        D = coins[n-1];
    }
    D++;

    // Create table of values
    Value** dp = (Value**)malloc(D*sizeof(Value*));

    dp[0] = NULL;       // Index starts at 1
    for (int i = 1; i < D; i++) {
        dp[i] = newValue(0x7fffffff, NULL);
    }

    // Preset coins into the table
    for (int i = 0; i < n; i++) {
        if (coins[i] < D) {
            int* new_coins = (int*)malloc(sizeof(int));
            new_coins[0] = coins[i];

            freeValue(dp[coins[i]]);

            dp[coins[i]] = newValue(1, new_coins);
        }
    }

    // Find values under, and target
    for (int k = 2; k <= target; k++) { // 1 is included

        for (int i = 0; i < n; i++) {   // Check options with every coin
            int coin = coins[i];

            if (k > coin) {             // Is a valid coin

                Value* other = dp[k-coin];
                int N = dp[k]->n;

                if (N > other->n+1) {

                    // Copy other's coins
                    int* new_coins = (int*)malloc((other->n+1)*sizeof(int));
                    for (int j = 0; j < other->n; j++) {
                        new_coins[j] = other->coins[j];
                    }

                    // Add one
                    new_coins[other->n] = coin;

                    Value* x = newValue(other->n+1, new_coins);
                    freeValue(dp[k]);
                    dp[k] = x;
                }
            }
        }

    }

    // Print table of values
    printf("dp -\n\n");
    for (int i = 1; i < target+1; i++) {
        printf("%d: ", i);
        printValue(dp[i]);
        printf("\n");
    }
    printf("\n");

    // Free everything
    for (int i = 1; i < target+1; i++) {
        if (dp[i]) {
            freeValue(dp[i]);
        }
    }
    if (dp) {
        free(dp);
    }
    return (0);
}
