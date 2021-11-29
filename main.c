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

void copy_mat(float from[][COLS], float to[][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            to[r][c] = from[r][c];
        }
    }
}

void get_overlaps_address(int A[ROWS][COLS], int indexes_arr[numprocs-1][2], int * overlaps_address[numprocs-1]){
    /* example for indexes_arr
     * indexes_arr = [  [row_index, col_index],         -match with process 1
                        [row_index, col_index],         -match with process 2
                        ...
     */
    for(int i = 0; i < numprocs-1; i++){
        if(indexes_arr[i][0] != -1)
            overlaps_address[i] = &A[indexes_arr[i][0]][indexes_arr[i][1]];
        else
            overlaps_address[i] = NULL;
    }
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

int main() {

    int my_id = 1;


    // receive from master
    float A[ROWS][COLS] = {{60 ,60 ,60 ,60 ,60 ,60 },
                           {60 ,40 ,40 ,40 ,40 ,60 },
                           {60 ,40 ,40 ,40 ,40 ,60 },
                           {40 ,40 ,20 ,20 ,40 ,40 }};


    // receive from master
    int overlaps_recv_sizes[numprocs-1] = {0,2,2,0};     // represent the num of cells that the process has in common

    // receive from master
    int overlaps_recv_indexes[numprocs - 1][2] = {{-1,-1},  // no overlaps with itself
                                                  {3,0},    // over laps with process 2 start at index [3][0] until [3][1]
                                                  {3,4},    // over laps with process 3 start at index [3][4] until [3][5]
                                                  {-1,-1}   // no overlaps with process 4
                                                  };

    // receive from master
    int overlaps_send_sizes[numprocs-1] = {0,1,1,0};     // represent the num of cells that the process has in common

    // receive from master
    int overlaps_send_indexes[numprocs - 1][2] = {{-1,-1},  // no overlaps with itself
                                                  {2,1},    // over laps with process 2 start at index [3][0] until [3][1]
                                                  {2,4},    // over laps with process 3 start at index [3][4] until [3][5]
                                                  {-1,-1}   // no overlaps with process 4
                                                  };

    int * overlaps_recv_address[numprocs-1];
    get_overlaps_address(A, overlaps_recv_address, overlaps_recv_indexes);
    int * overlaps_send_address[numprocs-1];
    get_overlaps_address(A, overlaps_send_address, overlaps_send_indexes);

    void iterate(A, overlaps_recv_address, overlaps_send_address, overlaps_recv_sizes, overlaps_send_sizes);

    print_A(A);
    return 0;
}
