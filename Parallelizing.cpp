#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;
#define MAX 100
#define MAX_THREAD 100


int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];
int matCo[MAX][MAX];
int step_i = 0;
 
void* multi(void* arg)
{
    int i = step_i++; 
   
    for (int j = 0; j < MAX; j++)
      for (int k = 0; k < MAX; k++)
        matC[i][j] += matA[i][k] * matB[k][j];
}
 

int main()
{

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }
 
    auto start_pThread = chrono::high_resolution_clock::now();
    // declaring four threads
    pthread_t threads[MAX_THREAD];
 

    for (int i = 0; i < MAX_THREAD; i++) {
        int* p;
        pthread_create(&threads[i], NULL, multi, (void*)(p));
    }

    for (int i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);   
    auto end_pThread = chrono::high_resolution_clock::now();
    auto duration_pThread = chrono::duration_cast<chrono::milliseconds>(end_pThread - start_pThread);





   auto start_parallel = chrono::high_resolution_clock::now();
   #pragma omp parallel for
   for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            int sum = 0;
            for (int k = 0; k < MAX; k++) {
                sum += matA[i][k] * matB[k][j];
            }
            matCo[i][j] = sum;
        }
    }
    auto end_parallel = chrono::high_resolution_clock::now();
    auto duration_parallel = chrono::duration_cast<chrono::milliseconds>(end_parallel - start_parallel);


    cout << "Time taken for matrix multiplication using pthread: " << duration_pThread.count() << " milliseconds" << endl;
    cout << "Time taken for matrix multiplication using openMP: " << duration_parallel.count() << " milliseconds" << endl;
    return 0;
}