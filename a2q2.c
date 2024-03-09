#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_ROWS 100
#define MAX_COLS 100

double matrix_a[MAX_ROWS][MAX_COLS] = {0};
double matrix_b[MAX_ROWS][MAX_COLS] = {0};
double tb_matrix_b[MAX_ROWS][MAX_COLS] = {0}; // Transpose of matrix_b
int row_a, col_a, row_b, col_b;               // Rows and Columns for matrix_a and matrix_b

void read_matrix(double matrix[MAX_ROWS][MAX_COLS], int *row, int *col);
void print_matrix(double matrix[MAX_ROWS][MAX_COLS], int row, int col);
void transpose_matrix(double source[MAX_ROWS][MAX_COLS], double target[MAX_ROWS][MAX_COLS], int row, int col);
void add_matrices(double matrix_a[MAX_ROWS][MAX_COLS], double matrix_b[MAX_ROWS][MAX_COLS], double result[MAX_ROWS][MAX_COLS], int row_a, int col_a, int row_b, int col_b);
double get_element(const double matrix[MAX_ROWS][MAX_COLS], int row, int col, int max_r, int max_c);
int max(int a, int b); 

void read_matrix(double matrix[MAX_ROWS][MAX_COLS], int *row, int *col)
{
    // check if row and col is empty
    assert(row != NULL && col != NULL);
    printf("Please enter row and col:\n");
    scanf("%d %d", row, col);
    // Validate matrix dimensions is in bound
    assert(*row > 0 && *col > 0 && *row <= MAX_ROWS && *col <= MAX_COLS);

    // fill in matrix
    printf("Please enter matrices input:\n");
    for (int i = 0; i < *row; i++)
    {
        for (int j = 0; j < *col; j++)
        {
            scanf("%lf", &matrix[i][j]);
        }
    }
}

void print_matrix(double matrix[MAX_ROWS][MAX_COLS], int row, int col)
{
    // check if row and col are within bound
    assert(row > 0 && col > 0 && row <= MAX_ROWS && col <= MAX_COLS);
    printf("%d %d\n", row, col);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            // make sure theres 7 spaces in between each element and 1 decimal
            printf("%7.1f", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(double source[MAX_ROWS][MAX_COLS], double target[MAX_ROWS][MAX_COLS], int row, int col)
{
    // check if row and col are within bound
    assert(row > 0 && col > 0 && row <= MAX_ROWS && col <= MAX_COLS);

    // transpose the matrix
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            target[j][i] = source[i][j];
        }
    }
}

void add_matrices(double matrix_a[MAX_ROWS][MAX_COLS], double matrix_b[MAX_ROWS][MAX_COLS], double result[MAX_ROWS][MAX_COLS], int row_a, int col_a, int row_b, int col_b)
{
    // Determine the maximum number of rows and columns between the two matrices
    int max_r, max_c;

    max_r = max(row_a, row_b); 
    max_c = max(col_a, col_b);

    for (int i = 0; i < max_r; i++)
    {
        for (int j = 0; j < max_c; j++)
        {
            // Sum the corresponding elements from matrix_a and matrix_b.
            // If an element does not exist in one of the matrices, it is assumed to be 0
            result[i][j] = get_element(matrix_a, i, j, row_a, col_a) + get_element(matrix_b, i, j, row_b, col_b);
        }
    }
}

double get_element(const double matrix[MAX_ROWS][MAX_COLS], int row, int col, int max_r, int max_c)
{
    // check if row and element are within bound
    assert(row >= 0 && row < max_r && col >= 0 && col < max_c);
    // return the element at row and col in the matrix
    return matrix[row][col];
}

int max(int a, int b) 
{
    if(a<b)
    {
        a = b; 
    }
    return a;
}
int main()
{
    read_matrix(matrix_a, &row_a, &col_a);
    read_matrix(matrix_b, &row_b, &col_b);

    double sum_ab[MAX_ROWS][MAX_COLS] = {0};
    double sum_abt[MAX_ROWS][MAX_COLS] = {0};
    int max_row, max_col;

    printf("adding matrix");
    add_matrices(matrix_a, matrix_b, sum_ab, row_a, col_a, row_b, col_b);

    max_row = max(row_a,row_b); 
    max_col = max(col_a, col_b); 
    // Call print_matrix with the determined maximum row and column sizes
    print_matrix(sum_ab, max_row, max_col);
    printf("transposing matrix\n");
    transpose_matrix(matrix_b, tb_matrix_b, row_b, col_b);
    max_row = max(row_a,row_b); 
    max_col = max(col_a, col_b); 

    printf("adding matrix after transpose\n"); 
    add_matrices(matrix_a, tb_matrix_b, sum_abt, row_a, col_a, col_b, row_b);
    max_row = max(row_a,row_b); 
    max_col = max(col_a, col_b); 
    // Call print_matrix with the determined dimensions
    print_matrix(sum_abt, max_row, max_col);
    return 0;
}
