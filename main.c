#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//#define ROWS 4
//#define COLS 6
#define ITERS 10

#define dx 5
#define dy 5

#define numprocs 4


#define ROWS 12
#define COLS 22

// overlap size 
#define OVS (COLS-2)/4 + 1 


// global vars 

int my_id;
MPI_Status status;
int ROWS, COLS;



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

void get_overlaps_address(int A[ROWS][COLS], int indexes_arr[numprocs][2], int * overlaps_address[numprocs]){
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

void iterate(float A[][COLS], int * overlaps_recv_address[numprocs], int * overlaps_send_address[numprocs], int overlaps_recv_sizes[numprocs], int overlaps_send_sizes[numprocs]){
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
        
        for(int p_id = 0; p_id < numprocs; p_id++){    // p_id represent the p_id+1 process
            if(overlaps_recv_sizes[p_id] != 0 || overlaps_send_sizes[p_id] != 0) {
                // recv/send boundary values from/to process p_id+1 with overlaps_sizes[p_id]

                // when running on mpi:
                
                int tag = i*numprocs*numprocs*2 + my_id+p_id+my_id*p_id; // hash table with symmetric for my_id & p_id

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
                                MPI_COMM_WORLD,
                                status);
                
            }
        }
    }
}

int main(int argc, char** argv) {

    // for debug only
    int p_id = 1;
    while(p_id);
    

    int numprocs2;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs2);
    if(numprocs2 != numprocs){
        printf("num of process is not 4!");
        return 1;
    }



    float map[ROWS][COLS] = { 0 };

	if (myid == 0)
	{											//Sending procs. 1 it's submap to calculate
		float submap1[ROWS1][COLS1] = { 0 };

		for (int j = 0; j < COLS1; j++) {				// initializing horizontal boundary condition
			submap1[0][j] = 60;
			submap1[ROWS1-1][j] = (j < 5 || j >= 15) ? 40 : 20;
			//print("%d, %d", submap1[0][j], submap1[101][j]);
		}
		for (int i = 0; i < ROWS1; i++) {				// initializing vertical boundary condition
			submap1[i][0] = 60;
			submap1[i][COLS1-1] = 60;
		}
		
		for (int i = 1; i < ROWS1 - 1; i++) { // initializing inner values
			for (int j = 1; j < COLS1-1; j++)
				submap1[i][j] = 40;
		}

		for (int i = 0; i < ROWS1; i++) {
			for (int j = 0; j < COLS1; j++) {
				printf("%.2f\t", submap1[i][j]);
			}
			printf("\n");
			
		}

        float A[ROWS][COLS] = {{60 ,60 ,60 ,60 ,60 ,60 },
                           {60 ,40 ,40 ,40 ,40 ,60 },
                           {60 ,40 ,40 ,40 ,40 ,60 },
                           {40 ,40 ,20 ,20 ,40 ,40 }};
        
        int overlaps_recv_sizes[numprocs][numprocs] = { {0,0,OVS,OVS},      // process 0 has OVS with process 2 & 3
                                                        {0,0,OVS,OVS},
                                                        {OVS,OVS,0,0},
                                                        {OVS,OVS,0,0}
                                                        };  

        int overlaps_recv_indexes[numprocs][numprocs][2] = { { {-1,-1}, {-1,-1}, {0,0}, {0,COLS-1- OVS}},   // no overlaps with process 2 start at [0][0] and with process 3 at [0][COLS-1- OVS]
                                                             { {-1,-1}, {-1,-1}, {ROWS-1,0}, {ROWS-1,COLS-1- OVS}},
                                                             { {ROWS-1,COLS-1- OVS}, {0,COLS-1- OVS}, {-1,-1},{-1,-1}},
                                                             { {ROWS-1,0}, {0,0}, {-1,-1}, {-1,-1}}
                                                            };

        int overlaps_send_sizes[numprocs][numprocs] = { {0,0,OVS,OVS},      // process 0 has OVS with process 2 & 3
                                                        {0,0,OVS,OVS},
                                                        {OVS,OVS,2,0},
                                                        {OVS,OVS,2,0}
                                                        };  
                                                        
        int overlaps_send_indexes[numprocs][numprocs][2] = { { {-1,-1}, {-1,-1}, {1,1}, {1, COLS - 1 - OVS}},   // no overlaps with process 2 start at [0][0] and with process 3 at [0][COLS-1- OVS]
                                                             { {-1,-1}, {-1,-1}, {ROWS-1-1,0}, {1, COLS - 1 - OVS}},
                                                             { {ROWS-1-1,COLS-1- OVS}, {1,COLS-1- OVS}, {-1,-1},{-1,-1}},
                                                             { {ROWS-1,0}, {1,0}, {-1,-1}, {-1,-1}}
                                                            };
		for(p_id = 1; p_id < numprocs; p_id++){

            MPI_Send(submap1,
                ROWS1*COLS1,
                MPI_FLOAT,
                p_id,				// destination process
                1,
                MPI_COMM_WORLD);

            MPI_Send(overlaps_recv_sizes[p_id],
                numprocs,
                MPI_INT,
                p_id,				// destination process
                2,
                MPI_COMM_WORLD);

            MPI_Send(overlaps_recv_indexes[p_id],
                numprocs*2,
                MPI_INT,
                p_id,				// destination process
                3,
                MPI_COMM_WORLD);

            MPI_Send(overlaps_send_sizes[p_id],
                numprocs,
                MPI_INT,
                p_id,				// destination process
                4,
                MPI_COMM_WORLD);

            MPI_Send(overlaps_send_indexes[p_id],
                numprocs*2,
                MPI_INT,
                p_id,				// destination process
                5,
                MPI_COMM_WORLD);
        }




    /// all the process do the following
    else{   // my_id !=  0
        
        float A[ROWS][COLS];

		MPI_Recv(A,
			ROWS*COLS,
			MPI_FLOAT,
			0,				// destination process
			1,
			MPI_COMM_WORLD,
            status);


		MPI_Recv(overlaps_recv_sizes,
			numprocs,
			MPI_INT,
			0,				// destination process
			2,
			MPI_COMM_WORLD,
            status);


		MPI_Recv(overlaps_recv_indexes,
			numprocs*2,
			MPI_INT,
			0,				// destination process
			3,
			MPI_COMM_WORLD,
            status);


		MPI_Recv(overlaps_send_sizes,
			numprocs,
			MPI_INT,
			0,				// destination process
			4,
			MPI_COMM_WORLD,
            status);


		MPI_Recv(overlaps_send_indexes,
			numprocs*2,
			MPI_INT,
			0,				// destination process
			5,
			MPI_COMM_WORLD,
            status);
    }

    int * overlaps_recv_address[numprocs];
    get_overlaps_address(A, overlaps_recv_address, overlaps_recv_indexes);
    int * overlaps_send_address[numprocs];
    get_overlaps_address(A, overlaps_send_address, overlaps_send_indexes);

    void iterate(A, overlaps_recv_address, overlaps_send_address, overlaps_recv_sizes, overlaps_send_sizes);

    print_A(A);
    return 0;
}
