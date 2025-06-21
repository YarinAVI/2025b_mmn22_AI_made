# Matrix Calculator Assignment - Systems Programming Lab

## Overview
This assignment requires you to create an interactive matrix calculator program in C that performs various operations on 4x4 matrices containing real numbers.

## Program Structure

### Files Required
1. **mymat.h** - Contains the matrix type definition and function declarations
2. **mymat.c** - Contains all matrix computation functions (no I/O operations here)
3. **mainmat.c** - Contains the main function, user interface, and command parsing

### Matrix Definition
- Define a type `mat` using `typedef` that holds a 4x4 matrix
- The program must define 6 matrices: `A_MAT`, `B_MAT`, `C_MAT`, `D_MAT`, `E_MAT`, `F_MAT`
- All matrices should be initialized with zeros at program start

## Supported Commands

### 1. **read_mat** - Set values in a matrix
```
read_mat MAT_A, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
```
- Fills the matrix row by row
- If fewer than 16 values provided, remaining cells are set to 0
- If more than 16 values provided, extras are ignored

Example:
```
read_mat MAT_A, 23, 5, 6.253, -7, -200.5
```
Results in:
```
[0,0]=23  [0,1]=5     [0,2]=6.253  [0,3]=-7
[1,0]=-200.5  [1,1]=0  [1,2]=0      [1,3]=0
[2,0]=0   [2,1]=0     [2,2]=0      [2,3]=0
[3,0]=0   [3,1]=0     [3,2]=0      [3,3]=0
```

### 2. **print_mat** - Display a matrix
```
print_mat MAT_A
```
Output format (with 2 decimal places):
```
  23.00   5.00   6.25  -7.00
-200.50   0.00   0.00   0.00
   0.00   0.00   0.00   0.00
   0.00   0.00   0.00   0.00
```

### 3. **add_mat** - Add two matrices
```
add_mat MAT_A, MAT_B, MAT_C
```
Calculates: MAT_C = MAT_A + MAT_B

### 4. **sub_mat** - Subtract matrices
```
sub_mat MAT_A, MAT_B, MAT_C
```
Calculates: MAT_C = MAT_A - MAT_B

### 5. **mul_mat** - Multiply matrices
```
mul_mat MAT_A, MAT_B, MAT_C
```
Calculates: MAT_C = MAT_A × MAT_B

### 6. **mul_scalar** - Multiply matrix by scalar
```
mul_scalar MAT_A, 12.5, MAT_B
```
Calculates: MAT_B = MAT_A × 12.5

### 7. **trans_mat** - Transpose a matrix
```
trans_mat MAT_A, MAT_B
```
Stores the transpose of MAT_A in MAT_B

### 8. **stop** - Exit the program
```
stop
```

## Input Format Rules
- Each command must be on a single line
- Command names are lowercase, matrix names are uppercase
- Arguments are separated by commas
- Spaces/tabs allowed before and after commas
- No comma after the last argument
- Empty lines are allowed

## Error Handling Examples

### 1. Undefined matrix name
```
Input: read_mat G_MAT, 3.2, 8
Error: Undefined matrix name
```

### 2. Invalid matrix name (wrong case)
```
Input: read_mat a_mat, 3.2, -5.3
Error: Undefined matrix name
```

### 3. Undefined command
```
Input: do_it MAT_A, MAT_B, MAT_C
Error: Undefined command name
```

### 4. Non-numeric argument
```
Input: read_mat MAT_A, abc, 567
Error: Argument is not a real number
```

### 5. Extra text after command
```
Input: read_mat MAT_A, 3, -4.2, 6,
Error: Extraneous text after end of command
```

### 6. Missing argument
```
Input: read_mat MAT_A
Error: Missing argument
```

### 7. Illegal comma
```
Input: print_mat MAT_A,
Error: Illegal comma
```

### 8. Missing comma
```
Input: trans_mat MAT_A MAT_B
Error: Missing comma
```

### 9. Multiple consecutive commas
```
Input: sub_mat MAT_A, , MAT_B, MAT_C
Error: Multiple consecutive commas
```

### 10. Scalar argument where matrix expected
```
Input: mul_scalar MAT_A, MAT_B, MAT_C
Error: Argument is not a scalar
```

## Valid Command Sequence Example
```
print_mat MAT_A
print_mat MAT_B
print_mat MAT_C
read_mat MAT_A, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6
read_mat MAT_B, 1, 2.3456, -7.89
read_mat MAT_C, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
print_mat MAT_A
print_mat MAT_B
print_mat MAT_C
add_mat MAT_A, MAT_B, MAT_D
print_mat MAT_D
sub_mat MAT_B, MAT_A, MAT_E
print_mat MAT_E
mul_mat MAT_B, MAT_C, MAT_F
print_mat MAT_F
mul_scalar MAT_A, 12.5, MAT_A
print_mat MAT_A
trans_mat MAT_C, MAT_C
print_mat MAT_C
read_mat MAT_B, 0
print_mat MAT_B
mul_mat MAT_A, MAT_A, MAT_A
print_mat MAT_A
stop
```

## Special Notes
- The same matrix can appear multiple times in a command (e.g., `mul_mat MAT_A, MAT_B, MAT_A`)
- Handle EOF (End of File) condition properly
- Display user-friendly prompts and messages
- Compile with maximum warning flags: `-Wall -ansi -pedantic`
- Submit with makefile and test input/output files

## Matrix Operations Examples

### Addition
```
[1 2]   [2 2]   [3 4]
[3 4] + [2 0] = [5 4]
```

### Subtraction
```
[2 2]   [2 2]   [0 0]
[1 1] - [1 0] = [0 1]
```

### Multiplication
```
[1 1]   [1 0]   [3 2]
[2 2] × [2 2] = [6 4]
```

### Scalar Multiplication
```
[2 2]         [1 1]
[1 1] × 0.5 = [0.5 0.5]
```

### Transpose
```
[1 2 3 4]T   [1 5 9  13]
[5 6 7 8]  = [2 6 10 14]
[9 10 11 12] [3 7 11 15]
[13 14 15 16][4 8 12 16]
```

## Implementation Guidelines

### Data Structure Design
The `mat` type should be memory-efficient and provide easy access to elements. Consider:
```c
typedef struct {
    double data[4][4];
} mat;
```

### Function Prototypes (mymat.h)
```c
#ifndef MYMAT_H
#define MYMAT_H

typedef struct {
    double data[4][4];
} mat;

void read_mat(mat *m, double values[], int count);
void print_mat(const mat *m);
void add_mat(const mat *a, const mat *b, mat *result);
void sub_mat(const mat *a, const mat *b, mat *result);
void mul_mat(const mat *a, const mat *b, mat *result);
void mul_scalar(const mat *m, double scalar, mat *result);
void trans_mat(const mat *m, mat *result);

#endif
```

### Command Parsing Tips

1. **Reading Commands**: Use `fgets()` to read entire lines, then parse them
2. **Tokenization**: Consider using `strtok()` or manual parsing
3. **Matrix Name Validation**: Create a function to map string names to matrix pointers
4. **Number Parsing**: Use `strtod()` for converting strings to doubles

### Example Command Parser Structure
```c
// In mainmat.c
typedef struct {
    char *name;
    mat *matrix;
} MatrixEntry;

MatrixEntry matrices[] = {
    {"MAT_A", &MAT_A},
    {"MAT_B", &MAT_B},
    {"MAT_C", &MAT_C},
    {"MAT_D", &MAT_D},
    {"MAT_E", &MAT_E},
    {"MAT_F", &MAT_F}
};

mat* get_matrix_by_name(const char *name) {
    for (int i = 0; i < 6; i++) {
        if (strcmp(matrices[i].name, name) == 0) {
            return matrices[i].matrix;
        }
    }
    return NULL;
}
```

## Common Pitfalls and Solutions

### 1. **Matrix Aliasing**
When the same matrix appears as both input and output:
```c
// Wrong approach:
void mul_mat(const mat *a, const mat *b, mat *result) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result->data[i][j] = 0;  // This zeros out input if result == a!
            for (int k = 0; k < 4; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
}

// Correct approach:
void mul_mat(const mat *a, const mat *b, mat *result) {
    mat temp;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp.data[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                temp.data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    *result = temp;  // Copy back
}
```

### 2. **Input Validation Checklist**
- ✓ Check for empty lines (skip them)
- ✓ Verify command exists
- ✓ Validate matrix names (uppercase, correct names)
- ✓ Check argument count for each command
- ✓ Verify numeric values are valid
- ✓ Look for illegal commas
- ✓ Detect missing commas
- ✓ Find consecutive commas
- ✓ Identify trailing text

### 3. **Memory Management**
Since all matrices are statically allocated, no dynamic memory management is needed. However, be careful with:
- Buffer overflows when reading input
- String manipulation boundaries

## Testing Strategy

### Create Test Files
1. **test_valid.txt** - All valid commands
2. **test_errors.txt** - Each type of error
3. **test_edge.txt** - Edge cases (same matrix multiple times, zero matrices, etc.)
4. **test_computation.txt** - Verify mathematical correctness

### Sample Test File Structure
```
# Test basic reading and printing
read_mat MAT_A, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1
print_mat MAT_A

# Test with fewer than 16 values
read_mat MAT_B, 5, 10, 15
print_mat MAT_B

# Test matrix operations
add_mat MAT_A, MAT_B, MAT_C
print_mat MAT_C

# Test with same matrix as input and output
mul_scalar MAT_A, 2.5, MAT_A
print_mat MAT_A

# Test transpose
trans_mat MAT_A, MAT_D
print_mat MAT_D

stop
```

## Makefile Example
```makefile
CC = gcc
CFLAGS = -Wall -ansi -pedantic
OBJS = mainmat.o mymat.o
TARGET = mainmat

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

mainmat.o: mainmat.c mymat.h
	$(CC) $(CFLAGS) -c mainmat.c

mymat.o: mymat.c mymat.h
	$(CC) $(CFLAGS) -c mymat.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) < test_valid.txt > output_valid.txt
	./$(TARGET) < test_errors.txt > output_errors.txt
```

## Debugging Tips

1. **Print Debug Information**
   - Add debug prints to show parsed commands and arguments
   - Display intermediate calculation results
   - Show which function is being called

2. **Use Assertions**
   ```c
   #include <assert.h>
   assert(matrix != NULL);
   assert(row >= 0 && row < 4);
   ```

3. **Test Incrementally**
   - First implement and test `read_mat` and `print_mat`
   - Then add one operation at a time
   - Test error handling last

4. **Common Bugs to Watch For**
   - Off-by-one errors in loops
   - Incorrect matrix multiplication algorithm
   - Not handling whitespace properly
   - Buffer overflow in string operations
   - Forgetting to initialize matrices to zero

## Submission Checklist
- [ ] All source files (mainmat.c, mymat.c, mymat.h)
- [ ] Makefile
- [ ] Test input files
- [ ] Output files showing program results
- [ ] Code compiles with `-Wall -ansi -pedantic` without warnings
- [ ] All commands work correctly
- [ ] All error cases handled properly
- [ ] Program terminates correctly with `stop` and EOF
- [ ] User-friendly prompts and error messages
- [ ] Code is well-commented and organized

## Final Notes
- The assignment is due on **25.05.2025**
- Worth **8 points** (optional assignment)
- Submit via the course's online submission system
- Create a zip file with all required files
- Download and verify your submission after uploading

Remember: The goal is to create a robust, user-friendly matrix calculator that handles all operations correctly and provides clear error messages for invalid input.
