#include <stdio.h>
#include <stdlib.h>

#define ROWS 4
#define COLS 6
#define ITERS 10

#define dx 1
#define dy 1

#define numprocs 5


void print_A(float A[][COLS]){
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            printf("%.2f\t", A[r][c]);
        }
        printf("\n");
    }
}

float * recv_OL_values(int * rows, int * cols){}

void copy_mat(float from[][COLS], float to[][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            to[r][c] = from[r][c];
        }
    }
}

void get_overlaps_address(int indexes_arr[numprocs-1][2], int * overlaps_recv_address[numprocs-1], int * overlaps_send_address[numprocs-1]){

}

void iterate(float A[][COLS], int * overlaps_recv_address[numprocs-1], int * overlaps_send_address[numprocs-1], int overlaps_recv_sizes[numprocs-1], int overlaps_send_sizes[numprocs-1]){
    float A_copy[ROWS][COLS];
    for(int i = 0; i < ITERS; i++){
        copy_mat(A, A_copy);
        for(int r = 1; r < ROWS-1; r++){
            for(int c = 1; c < COLS-1; c++){
                A[r][c] = ((dx*dx)*(A_copy[r+1][c] + A_copy[r-1][c]) +
                           (dy*dy)*(A_copy[r][c+1] + A_copy[r][c-1]))
                            /(2*(dx*dx+dy*dy));
            }
        }
        
        for(int p_id = 0; p_id < numprocs-1; p_id++){    // p_id represent the p_id+1 process
            if(overlaps_recv_sizes[p_id] != 0 || overlaps_send_sizes[p_id] != 0) {
                // recv/send boundary values from/to process p_id+1 with overlaps_sizes[p_id]
                *overlaps_recv_address[p_id] = 50;     // represent the received value for now

                // when running on mpi:
                /*
                int tag = i*numprocs*numprocs*2 + my_id+p_id+my_id*p_id // hash table with symmetric for my_id & p_id

                MPI_Sendrecv(// send details
                                overlaps_send_address[p_id],
                                overlaps_send_sizes[p_id], //count_send,
                                MPI_FLOAT,
                                p_id, // recipient,
                                tag, //tag_send, should be function of iter and p_id
                                // recv details
                                overlaps_recv_address[p_id],  //  buffer_recv,
                                overlaps_recv_sizes[p_id], // count_recv,
                                MPI_FLOAT,
                                p_id, // sender,
                                tag, //tag_recv,
                                comm,
                                status);
                */
            }
        }
    }
}
/*
int * count_overlaps(float A[][COLS]){
    int counter[numprocs] = {0};
    for(int i = 0; i < ROWS; i++){
        if(A[i][0] < 0)
            counter[-(int)A[i][0]]++;
        if(A[i][COLS-1] < 0)
            counter[-(int)A[i][COLS-1]]++;
    }
    for(int i = 1; i < COLS-1; i++){
        if(A[0][i] < 0)
            counter[-(int)A[0][i]]++;
        if(A[ROWS-1][i] < 0)
            counter[-(int)A[ROWS-1][i]]++;
    }
    return counter;
}

void get_overlaps(float A[][COLS]){
    int arr[numprocs][][2];
}
*/

int main() {

    int my_id = 1;
    float A[ROWS][COLS] = {{60 ,60 ,60 ,60 ,60 ,60 },
                           {60 ,40 ,40 ,40 ,40 ,60 },
                           {60 ,40 ,40 ,40 ,40 ,60 },
                           {40 ,40 ,20 ,20 ,40 ,40 }};


    int overlaps_sizes[numprocs-1] = {0,2,2,0};  // represent the num of cells that the process has in commen
    int overlaps_recv[numprocs - 1][2] = {{-1,-1},    // no overlaps with itself
                                             {3,0},   // over laps with process 2 start at index [3][0] until [3][1]
                                             {3,4},   // over laps with process 3 start at index [3][4] until [3][5]
                                             {-1,-1}   // no overlaps with process 4
                                            };
    int recv_from[][3] = {{}};

    iterate(A, overlaps_recv, overlaps_sizes);
    print_A(A);
    return 0;
}
