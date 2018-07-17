/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, r, c, k, x;
  int tmp, tmp2, tmp3, tmp4, tmp5;
  int block;
  if (N==32 || N==64) {
    block = 8;
  } else {
    block = 16;
  }
  if (N!=64) {
    for (i = 0; i<M; i+=block) {
      for (j = 0; j<N; j+=block) {
	for (r = j; (r<j+block) && (r<N); r++) {
	  for (c = i; (c<i+block) && (c<M); c++) {
	    if (r==c) {
	      tmp = A[r][c];
	      x = r;
	    } else {
	      B[c][r] = A[r][c];
	    }
	  }
	  if (i==j) {
	    B[x][x] = tmp;
	  }
	}
      }
    }
  } else {
    for (i = 0; i<N; i+=block) {
      for (j = 0; j<M; j+=block) {
	for(k=0; k<4; k++) { 
	  r = A[i+k][j+0];
	  c = A[i+k][j+3];
	  x = A[i+k][j+4];
	  tmp = A[i+k][j+1];
	  tmp2 = A[i+k][j+2];
	  tmp3 = A[i+k][j+5];
	  tmp4 = A[i+k][j+6];
	  tmp5 = A[i+k][j+7];
	  B[j+0][i+k+0] = r;
	  B[j+1][i+k+0] = tmp;
	  B[j+2][i+k+0] = tmp2;
	  B[j+3][i+k+0] = c;
	  B[j+0][i+k+4] = tmp3;
	  B[j+1][i+k+4] = tmp4;
	  B[j+2][i+k+4] = tmp5;
	  B[j+3][i+k+4] = x;
	}
	r = A[i+4][j+4];
	c = A[i+7][j+4];
	x = A[i+4][j+3];
	tmp = A[i+5][j+4];
	tmp2 = A[i+6][j+4];
	tmp3 = A[i+5][j+3];
	tmp4 = A[i+6][j+3];
	tmp5 = A[i+7][j+3];
	B[j+4][i+0] = B[j+3][i+4];
	B[j+4][i+1] = B[j+3][i+5];
	B[j+4][i+2] = B[j+3][i+6];
	B[j+4][i+3] = B[j+3][i+7];
	B[j+4][i+4] = r;
	B[j+4][i+7] = c;
	B[j+4][i+5] = tmp;
	B[j+4][i+6] = tmp2;
	B[j+3][i+5] = tmp3;
	B[j+3][i+6] = tmp4;
	B[j+3][i+7] = tmp5;
	B[j+3][i+4] = x;
	for(k=0; k<3; k++) {
	  r = A[i+4][j+5+k];
	  c = A[i+7][j+5+k];
	  x = A[i+4][j+k];
	  tmp = A[i+5][j+5+k];
	  tmp2 = A[i+6][j+5+k];
	  tmp3 = A[i+5][j+k];
	  tmp4 = A[i+6][j+k];
	  tmp5 = A[i+7][j+k];
	  B[j+5+k][i+0] = B[j+k][i+4];
	  B[j+5+k][i+1] = B[j+k][i+5];
	  B[j+5+k][i+2] = B[j+k][i+6];
	  B[j+5+k][i+3] = B[j+k][i+7];
	  B[j+5+k][i+4] = r;
	  B[j+5+k][i+5] = tmp;
	  B[j+5+k][i+6] = tmp2;
	  B[j+5+k][i+7] = c;
	  B[j+k][i+4] = x;
	  B[j+k][i+5] = tmp3;
	  B[j+k][i+6] = tmp4;
	  B[j+k][i+7] = tmp5;
	}
      }
    }
  }
}


/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * trans_two - A multiple division baseline transpose function 
 */
char trans_two_desc[] = "Copying two elements at a time transpose";
void trans_two(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, tmp, tmp2; 
  for (i = 0; i<N; i++) {
    for (j = 0; j<M; j+=2) {
      tmp = A[i][j];
      tmp2 = A[i][j+1];
      B[j][i] = tmp;
      B[j+1][i] = tmp2;
    }
  }
  
}

/*
 * trans_col - A simple column-wise transpose function.       
 */
char trans_col_desc[] = "Simple column-wise scan transpose";
void trans_col(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, tmp;
  for (i = 0; i<M; i++) {
    for (j = 0; j < N; j++) {
      tmp = A[j][i];
      B[i][j] = tmp;
    }
  }

}

/*                                                                              
 * trans_eight - A multiple values at a time baseline transpose function    
 */
char trans_eight_desc[] = "Copying eight elements at a time transpose";
void trans_eight(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, tmp, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
  for (i = 0; i<N; i++) {
    for (j = 0; j<M; j+=8) {
      tmp = A[i][j];
      tmp2 = A[i][j+1];
      tmp3 = A[i][j+2];
      tmp4 = A[i][j+3];
      tmp5 = A[i][j+4];
      tmp6 = A[i][j+5];
      tmp7 = A[i][j+6];
      tmp8 = A[i][j+7];
      B[j][i] = tmp;
      B[j+1][i] = tmp2;
      B[j+2][i] = tmp3;
      B[j+3][i] = tmp4;
      B[j+4][i] = tmp5;
      B[j+5][i] = tmp6;
      B[j+6][i] = tmp7;
      B[j+7][i] = tmp8;
    }
  }

}

/*                                                                               * trans_block8 - A simple block-wise transpose function.           
 */
char trans_block8_desc[] = "Simple 8x8 block-wise scan transpose";
void trans_block8(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, r, c, tmp;
  int block = 8;
  for (i = 0; i<M; i+=block) {
    for (j = 0; j<N; j+=block) {
      for (r = j; (r<j+block) && (r<N); r++) {
	for (c = i; (c<i+block) && (c<M); c++) {
	  tmp = A[r][c];
	  B[c][r] = tmp;
	}
      }
    }
  }

}

/*                                                                               * trans_block4 - A simple block-wise transpose function.                         */
char trans_block4_desc[] = "Simple 4x4 block-wise scan transpose";
void trans_block4(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, r, c, x, tmp, diagTemp;
  int block = 4;
  for (i = 0; i<M; i+=block) {
    for (j = 0; j<N; j+=block) {
      for (r = j; (r<j+block) && (r<N); r++) {
	for (c = i; (c<i+block) && (c<M); c++) {
	  if (r==c) {
	    diagTemp = A[r][c];
	    x = r;
	  } else {
	    tmp = A[r][c];
	    B[c][r] = tmp;
	  }
	}
	if (i==j) {
	  B[x][x] = diagTemp;
	}
      }
    }
  }

}

/*                                                                               * trans_block16 - A simple block-wise transpose function.                         */
char trans_block16_desc[] = "Simple 16x16 block-wise scan transpose";
void trans_block16(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, r, c, tmp;
  int block = 16;
  for (i = 0; i<M; i+=block) {
    for (j = 0; j<N; j+=block) {
      for (r = j; (r<j+block) && (r<N); r++) {
	for (c = i; (c<i+block) && (c<M); c++) {
	  tmp = A[r][c];
	  B[c][r] = tmp;
	}
      }
    }
  }

}

/*                                                                               * trans_subblock - A more sophisticated block-wise transpose function.   
 */
char trans_subblock_desc[] = "8x8 and individual block-wise scan transpose";
void trans_subblock(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, r, c, k, x;
  int tmp, tmp2, tmp3, tmp4, tmp5;
  for (i = 0; i<N; i+=8) {
    for (j = 0; j<M; j+=8) {
      for(k=0; (k<4) && (k<N); k++) { 
	r = A[i+k][j+0];
	c = A[i+k][j+3];
	x = A[i+k][j+4];
	tmp = A[i+k][j+1];
	tmp2 = A[i+k][j+2];
	tmp3 = A[i+k][j+5];
	tmp4 = A[i+k][j+6];
	tmp5 = A[i+k][j+7];
	B[j+0][i+k+0] = r;
	B[j+1][i+k+0] = tmp;
	B[j+2][i+k+0] = tmp2;
	B[j+3][i+k+0] = c;
	B[j+0][i+k+4] = tmp3;
	B[j+1][i+k+4] = tmp4;
	B[j+2][i+k+4] = tmp5;
	B[j+3][i+k+4] = x;
      }
      r = A[i+4][j+4];
      c = A[i+7][j+4];
      x = A[i+4][j+3];
      tmp = A[i+5][j+4];
      tmp2 = A[i+6][j+4];
      tmp3 = A[i+5][j+3];
      tmp4 = A[i+6][j+3];
      tmp5 = A[i+7][j+3];
      B[j+4][i+0] = B[j+3][i+4];
      B[j+4][i+1] = B[j+3][i+5];
      B[j+4][i+2] = B[j+3][i+6];
      B[j+4][i+3] = B[j+3][i+7];
      B[j+4][i+4] = r;
      B[j+4][i+7] = c;
      B[j+4][i+5] = tmp;
      B[j+4][i+6] = tmp2;
      B[j+3][i+5] = tmp3;
      B[j+3][i+6] = tmp4;
      B[j+3][i+7] = tmp5;
      B[j+3][i+4] = x;
      for (k=0; (k<3) & (k<M); k++) {
	r = A[i+4][j+5+k];
	c = A[i+7][j+5+k];
	x = A[i+4][j+k];
	tmp = A[i+5][j+5+k];
	tmp2 = A[i+6][j+5+k];
	tmp3 = A[i+5][j+k];
	tmp4 = A[i+6][j+k];
	tmp5 = A[i+7][j+k];
	B[j+5+k][i+0] = B[j+k][i+4];
	B[j+5+k][i+1] = B[j+k][i+5];
	B[j+5+k][i+2] = B[j+k][i+6];
	B[j+5+k][i+3] = B[j+k][i+7];
	B[j+5+k][i+4] = r;
	B[j+5+k][i+5] = tmp;
	B[j+5+k][i+6] = tmp2;
	B[j+5+k][i+7] = c;
	B[j+k][i+4] = x;
	B[j+k][i+5] = tmp3;
	B[j+k][i+6] = tmp4;
	B[j+k][i+7] = tmp5;
      }
    }
  }
}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
    registerTransFunction(trans_two, trans_two_desc);
    registerTransFunction(trans_col, trans_col_desc);
    registerTransFunction(trans_eight, trans_eight_desc);
    registerTransFunction(trans_block8, trans_block8_desc);
    registerTransFunction(trans_block4, trans_block4_desc);
    registerTransFunction(trans_block16, trans_block16_desc);
    registerTransFunction(trans_subblock, trans_subblock_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

