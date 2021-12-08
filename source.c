#include <stdio.h>
#include "mpi.h"
#define ROWS 300
#define COLS 500
#define ROWS1 12
#define COLS1 22

//void initGrid(float arr[ROWS][COLS]);
void print_A(float A[][COLS]);

void main() {
	int numprocs, myid;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

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
		

		MPI_Send(submap1,
			ROWS1*COLS1,
			MPI_FLOAT,
			1,				// destination process
			MPI_ANY_TAG,
			MPI_COMM_WORLD);

		//----------------------------------------------------------------------//

		float submap2;


		//int p2_startR = 100, p2_endR = 199, p2_startC = 0, p2_endC = 199;		//Sending  procs. 2 it's submap to calculate
		//float submap2[102][200] = { 0 };

		//for (int i = 0; i <= 99; i++) {
		//	for (int j = p2_startC; j <= p2_endC; j++)
		//		submap2[i][j] = map[p2_startR + i][j];
		//}
		//MPI_Send(submap2, 20400, MPI_FLOAT, 2, MPI_ANY_TAG, MPI_COMM_WORLD);

		////---------------------------------------------------------------------//

		//int p3_startR = 100, p3_endR = 199, p3_startC = 300, p3_endC = 499;		//Sending procs. 3 it's submap to calculate
		//float submap3[102][200] = {0};

		//for (int i = 0; i <= 99; i++) {
		//	for (int j = 0; j < 200; j++)
		//		submap3[i][j] = map[p3_startR + i][p3_startC + j];
		//}
		//MPI_Send(submap3, 20400, MPI_FLOAT, 3, MPI_ANY_TAG, MPI_COMM_WORLD);

		////---------------------------------------------------------------------//

		//int p4_startR = 200, p4_endR = 299, p4_startC = 150, p4_endC = 299;		//Sending  procs. 4 it's submap to calculate
		//float submap4[101][200];

		//for (int i = 0; i <= 99; i++) {
		//	for (int j = 0; j < 200; j++)
		//		submap4[i][j] = map[p4_startR + i][p4_startC + j];
		//}
		//MPI_Send(submap4, 20200, MPI_FLOAT, 4, MPI_ANY_TAG, MPI_COMM_WORLD);
		//}



	}
	//void initGrid(float arr[ROWS][COLS]) {
	//	for (int i = 0; i < 99; i++) {                           // Initialize frame valus
	//		for (int j = 150; j < 350; j++)
	//			arr[i][j] = (i == 0 || j == 150 || j == 349) ? 60 : 40;
	//	for (int j = 150; j < 350; j++)
	//		arr[99][j] = ( j >= 200 && j < 300) ? 20 : 40;
	//	}
	//	for (int i = 100; i < 200; i++) {
	//		for (int j = 0; j < 199; j++) 
	//			arr[i][j] = ((i == 100 && j < 150) || (i < 300 && j == 0) || (i == 299 && j < 150)) ? 60 : 40;
	//		arr[i][199] = 20; 
	//		arr[i][300] = 20; 
	//		for (int j = 301; j < 500; j++) 
	//			arr[i][j] = ((i == 100 && j >= 350) || j == 499 || (i == 199 && j >= 350)) ? 60 : 40;
	//		
	//	}
	//	for (int j = 150; j < 350; j++)
	//		arr[200][j] = (j >= 200 && j < 300) ? 20 : 40;
	//	for (int i = 201; i < 300; i++) {
	//		for (int j = 150; j < 350; j++)
	//			arr[i][j] = (j == 150 || i == 299 || j == 350) ? 60 : 40; 
	//			
	//		
	//	}
	//
	//}

	/*void print_A(float A[][COLS]) {
		for (int r = 0; r < ROWS; r++) {
			printf("%d --- ", r);
			for (int c = 0; c < COLS; c++) {
				printf("%.2f\t", A[r][c]);
			}
			printf("\n");
		}
	}*/

}