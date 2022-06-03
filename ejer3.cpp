# define N_DIM 4

#include <assert.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cstdlib> 
#include <stdio.h>
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

vector<double> times;

void RowMatrixVectorMultiply(int dim, double *matrix_data, double *vector_data, double *result){
    int rank,size;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size); 
    double* localresult = new double[dim / size];
    double matrix [dim][dim];
    double timer = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(matrix_data, (dim*dim)/size, MPI_DOUBLE, matrix, (dim*dim)/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector_data, dim, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0;i<(dim/size);i++)
        for (int j = 0;j<dim;j++)
            localresult[i]+=vector_data[j]*matrix[i][j];
    
    MPI_Gather(localresult, (dim)/size, MPI_DOUBLE, result, (dim)/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    timer = MPI_Wtime()-timer;

    cout << "Final time = "<<timer<<endl;
    times.push_back(timer);
}

void read_mat_from_file(const char *s, int n_row, int n_col, double  *in_matrix){
	ifstream fin(s);
	string line;
	double data_in;

	if(!fin){
		cout << "Unable to open " << s << " for reading.\n";
		exit (0);
	}

 	for (int i = 0; i < n_row; i++){
        getline(fin, line);
        stringstream stream(line);
		for (int j = 0 ;  j < n_col; j++)
		{
	        stream >> data_in;
			*(in_matrix+(i*n_col)+j) = data_in;
		}
	}

}

int main( int argc, char *argv[]){
    int rank, size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    
    if (N_DIM%size){
        MPI_Finalize();
        return(0);
    }
    
    double matrix_data[N_DIM][N_DIM];
    double vector_data[N_DIM];
    double result[N_DIM] = {0.0};
    
    if (rank==0){
        read_mat_from_file("matrix4.txt", N_DIM, N_DIM, (double *)matrix_data);
        read_mat_from_file("vector4.txt", N_DIM, 1, vector_data);
    }
    
    RowMatrixVectorMultiply(N_DIM, (double *)matrix_data, vector_data,result);
    
    if (rank==0){
        printf("Matrix  :\n");
        for (int i=0;i<N_DIM;i++){
            for (int j=0;j<N_DIM;j++)
                printf("%.5f ", matrix_data[i][j]);
            printf("\n");
        }
        printf("Vector :\n");
        for (int i=0;i<N_DIM;i++)
            printf("%.5f ", vector_data[i]);
        printf("\n\n");
        
        printf("Result :\n");
        for (int i=0;i<N_DIM;i++)
            printf("%.5f ", result[i]);
        printf("\n\n");
    }

    if (rank == 0) {
        fstream f;
        f.open("test_P4_4.txt", ios::app);
        f << accumulate(times.begin(), times.end(), 0.0) / times.size() << endl;
        f << '\n';
    f.close();
    }
    
    MPI_Finalize();
    return(0);
}