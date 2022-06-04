#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#include <time.h>

#define N 1000

void escribir(int resultados[N]){
  int i;
  for(i = 0; i < N; i++){
    printf("%d ", resultados[i]); 
  }
  printf("\n");
}

int main(){
  srand(time(NULL));

  int A[N][N], v[N], resultados[N];
  
  int a, b;

  for( a=0; a<N; a++){
    v[a]=rand() % 100;
    for( b=0; b<N; b++){
      int r = rand() % 100;
      A[a][b] = r;
    }
  }
  
  int power_two;
  for(power_two=2; power_two<=16; power_two*=2){
    clock_t start = clock();
    printf("Number of threads: %d \n", power_two);
    #pragma omp parallel num_threads(power_two)
    { 
      int i, j;
  
      int* results_private = (int*)calloc(N, sizeof(int));

      for (i=0;i<N;i++) {
        #pragma omp for
        for (j=0;j<N;j++) 
          results_private[i] += A[i][j]*v[j];
      }
      #pragma omp critical
      {
        for(j=0; j<N; j++) resultados[j] += results_private[j];
      }
      free(results_private);
    }

    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Tiempo Total: %f\n\n", time_spent);
    
  }

  return 0;
}
