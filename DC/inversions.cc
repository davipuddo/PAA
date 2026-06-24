#include <iostream>
#include <vector>

void print(std::vector<int> V) {
    int n = V.size();
    for (int i = 0; i < n; i++) {
        std::cout << "[" << V[i] << "] ";
    }
    std::cout << std::endl;
}

std::vector<int> randV(int n) {
    std::vector<int> V = std::vector<int>(n);
    for (int i = 0; i < n; i++) {
        V[i] = rand() % 20;
    }
    return V;
}

int optimal (std::vector<int> V) {
    int n = V.size();
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (V[i] > V[j]) {
                count++;
            }
        }
    }
    return count;
}

int intercalate(std::vector<int>* V, int L, int M, int R) {
    int count = 0;

    int nL = M + 1 - L;
    int nR = R - M;

    int bufL [nL+1];
    int bufR [nR+1];

    bufL[nL] = bufR[nR] = 0x7fffffff;

    for (int i = 0; i < nL; i++) {
        bufL[i] = (*V)[i+L];
    }

    for (int i = 0; i < nR; i++) {
        bufR[i] = (*V)[i+M+1];
    }

    int iL = 0;
    int iR = 0;

    for (int i = L; i <= R; i++) {
        if (bufL[iL] <= bufR[iR]) {
            (*V)[i] = bufL[iL++];
        } else {
            if (bufL[iL] != 0x7fffffff) {
                count += nL - iL;
            }
            (*V)[i] = bufR[iR++];
        }
    }

    return count;
}

int merge(std::vector<int>* V, int L, int R) {
    int count = 0;
    if (L < R) {
        int M = (int)((double)(L+R)/2.0);
        count += merge(V, L, M);
        count += merge(V, M+1, R);
        count += intercalate(V, L, M, R);
    }
    return count;
}

int main (void) {
    
    // std::vector<int> V = { 7, 0, 6, 4, 5, 1, 8, 3 };
    std::vector<int> V = randV(10);
    print(V);

    std::cout << "optimal: " << optimal(V) << std::endl;

    int res = merge(&V, 0, V.size()-1);
    std::cout << "DC: " << res << std::endl;

    std::cout << "\nsort: " << std::endl; print(V);

    return(0);
}
