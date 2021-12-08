#include <stdio.h>
//#include "mpi.h"
#define ROWSX 300
#define COLSX 500
#define ROWS 12
#define COLS 22

//void initGrid(float arr[ROWS][COLS]);


void main() {
	int numprocs, myid;

	//MPI_Init(NULL, NULL);
	//MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	//MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	float map[ROWS][COLS] = { 0 };

	if (myid == 0)
	{											//Sending procs. 1 it's submap to calculate
		float submap[ROWS][COLS] = { 0 };

		
			for (int j = 0; j < COLS; j++) {				// initializing horizontal boundary condition
				submap[0][j] = 60;
				submap[ROWS - 1][j] = (j < 5 || j >= 15) ? 40 : 20;

			}
			for (int i = 0; i < ROWS; i++) {				// initializing vertical boundary condition
				submap[i][0] = 60;
				submap[i][COLS - 1] = 60;
			}

			for (int i = 1; i < ROWS - 1; i++) { // initializing inner values
				for (int j = 1; j < COLS - 1; j++)
					submap[i][j] = 40;
			}

			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLS; j++) {
					printf("%.2f\t", submap[i][j]);
				}
				printf("\n");

			}
		

		/*MPI_Send(submap,
			ROWS * COLS,
			MPI_FLOAT,
			1,				// destination process
			MPI_ANY_TAG,
			MPI_COMM_WORLD);
			*/
		printf("\nsubmap1 was sent to process 1\n");
		//----------------------------------------------------------------------//

		//float submap2[ROWS][COLS] = { 0 };

		int currentBoundary2 = 15;

		for (int i = 0; i < ROWS; i++) {				// initializing vertical boundary condition
			submap[i][0] = 60;
			submap[i][COLS - 1] = 20;
		}

		for (int j = 0; j <= COLS; j++) {				// initializing horizontal boundary condition
			submap[0][j] = (j <= currentBoundary2) ? 60 : 40;
			submap[ROWS - 1][j] = (j <= currentBoundary2) ? 60 : 40;

		}

		for (int i = 1; i < ROWS - 1; i++) { // initializing inner values
			for (int j = 1; j < COLS - 1; j++)
				submap[i][j] = 40;
		}

		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				printf("%.2f\t", submap[i][j]);
			}
			printf("\n");

		}


		/*MPI_Send(submap,
			ROWS * COLS,
			MPI_FLOAT,
			2,				// destination process
			MPI_ANY_TAG,
			MPI_COMM_WORLD);*/



		printf("\nsubmap2 was sent to process 2\n");
		//--------------------------------------------------------------------------------------//

		//float submap3[ROWS][COLS] = { 0 };

		int currentBoundary3 = 5;

		for (int i = 0; i < ROWS; i++) {				// initializing vertical boundary condition
			submap[i][0] = 20;
			submap[i][COLS - 1] = 60;
		}

		for (int j = 0; j <= COLS; j++) {				// initializing horizontal boundary condition
			submap[0][j] = (j <= currentBoundary3) ? 40 : 60;
			submap[ROWS - 1][j] = (j <= currentBoundary3) ? 40 : 60;

		}

		for (int i = 1; i < ROWS - 1; i++) { // initializing inner values
			for (int j = 1; j < COLS - 1; j++)
				submap[i][j] = 40;
		}

		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				printf("%.2f\t", submap[i][j]);
			}
			printf("\n");

		}


		/*MPI_Send(submap,
			ROWS * COLS,
			MPI_FLOAT,
			3,				// destination process
			MPI_ANY_TAG,
			MPI_COMM_WORLD);*/

		printf("\nsubmap3 was sent to process 3\n");
	// -----------------------------------------------------------------------//

		//float submap0[ROWS][COLS] = { 0 };

		for (int j = 0; j < COLS; j++) {				// initializing horizontal boundary condition
			submap[ROWS - 1][j] = 60;
			submap[0][j] = (j < 5 || j >= 15) ? 40 : 20;

		}
		for (int i = 0; i < ROWS; i++) {				// initializing vertical boundary condition
			submap[i][0] = 60;
			submap[i][COLS - 1] = 60;
		}

		for (int i = 1; i < ROWS - 1; i++) { // initializing inner values
			for (int j = 1; j < COLS - 1; j++)
				submap[i][j] = 40;
		}

		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				printf("%.2f\t", submap[i][j]);
			}
			printf("\n");

		}
		


	}
}

