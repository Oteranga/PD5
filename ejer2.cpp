double OpArr(double A[], double B[], double C[], int n){
    int i, j;
    double s1, s2, a, res;
    #pragma omp task depend (out: A)
    calculo_a(A,n); /* O(2nlog^2(n/p)) */
    #pragma omp task depend (out: B)
    calculo_b(B,n); /* O(2nlog^2(n/p)) */
    #pragma omp task depend (out: C)
    calculo_c(C,n); /* O(2nlog^2(n/p)) */
    
    #pragma omp task depend (in: A, B) depend (out: A, s1)
    for (i=0; i<n; i++) { /* primer bucle for O(n^2)*/
        s1=0;
        for (j=0; j<n; j++) 
            s1+=A[i]*B[i];
        for (j=0; j<n; j++) 
            A[i]*=s1;
    }
    #pragma omp task depend (in: C, B) depend (out: C, s2)
    for (i=0; i<n; i++) { /* segundo bucle for O(n^2)*/
        s2=0;
        for (j=0; j<n; j++) 
            s2+=B[i]*C[i];
        for (j=0; j<n; j++) 
            C[i]*=s2;
    }
    /* calculo final */
    #pragma omp task depend (in: s1, s2) depend (out: a)
    a=s1/s2; /* O(1) */
    res=0; /* O(1) */
    #pragma omp task depend (in: a, C)
    for (i=0; i<n; i++) /* O(n) */
        res+=a*C[i];
    return res;
}