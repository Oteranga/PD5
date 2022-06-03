#include <iostream>
#include <random>
#include <mpi.h>

using namespace std;

int main(){
    int rank, guess, guess_chance, guesses[5];
    MPI_Init();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0){
        guess = rand()%100 + 1;
        for(guess_chance = 0; guess_chance < 1000; guess_chance++){
            MPI_Recv(&guesses[0], 1, MPI_INT, 1, guess_chance, MPI_COMM_WORLD, MPI_IGNORE);
            MPI_Recv(&guesses[1], 1, MPI_INT, 2, guess_chance, MPI_COMM_WORLD, MPI_IGNORE);
            MPI_Recv(&guesses[2], 1, MPI_INT, 3, guess_chance, MPI_COMM_WORLD, MPI_IGNORE);
            MPI_Recv(&guesses[3], 1, MPI_INT, 4, guess_chance, MPI_COMM_WORLD, MPI_IGNORE);
            MPI_Recv(&guesses[4], 1, MPI_INT, 5, guess_chance, MPI_COMM_WORLD, MPI_IGNORE);
            if(guess == guesses[0] == guesses[1] == guesses[2] == guesses[3] == guesses[4]){
                printf("Pese a las posiblidades casi nulas, se logró adivinar. Compre un boleto del
                pozo millonario de la Tinka si puede.");
            }
        }
    }else{
        for(guess_chance = 0; guess_chance < 1000; guess_chance++){
            guess = rand()%100 + 1;
            MPI_Send(&guess, 1, MPI_INT, 0, guess_chance, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}