# Symmetric Non-negative Matrix Factorization (SymNMF) Project

This project implements the SymNMF clustering algorithm using Python and C. The goal is to create a modular, efficient clustering tool, and compare its performance to K-means.

## Project Structure

- `symnmf.py`: Main Python interface for executing SymNMF and intermediate goals (similarity matrix, degree matrix, normalized matrix).
- `symnmf.c`: C implementation of the algorithm logic.
- `symnmfmodule.c`: C extension API to connect C code with Python.
- `symnmf.h`: Header file with C function declarations.
- `analysis.py`: Compares SymNMF and K-means using silhouette score.
- `setup.py`: Python setup script to build the C extension.
- `Makefile`: Makefile to compile the C program.

## How to Build

### C extension (for Python interface)
```bash
python3 setup.py build_ext --inplace
```

### C program (for standalone execution)
```bash
make
```

## How to Run

### Using Python
```bash
python3 symnmf.py <k> <goal> <input_file.txt>
```
- `goal` options: `symnmf`, `sym`, `ddg`, `norm`

Example:
```bash
python3 symnmf.py 2 symnmf input_1.txt
```

### Using C directly
```bash
./symnmf <goal> <input_file.txt>
```
Example:
```bash
./symnmf norm input_1.txt
```

### Running Analysis
```bash
python3 analysis.py <k> <input_file.txt>
```
Example:
```bash
python3 analysis.py 5 input_k5_d7.txt
```

## Output Format
- Matrices are printed row-by-row, values separated by commas.
- All values are rounded to 4 decimal places.

## Notes
- Seed for random generation is fixed using `np.random.seed(1234)`.
- Only valid inputs are assumed; errors should print "An Error Has Occurred".
- Do not forget to free memory in C code.
- Matrix `H` is initialized with uniform values in the range `[0, 2 * sqrt(mean(W)/k)]`.

## Comparison
- `analysis.py` calculates silhouette scores for both SymNMF and K-means.
- The highest value in each row of matrix `H` determines cluster assignment.

## Requirements
- Python 3.x
- numpy
- sklearn (for analysis)
- gcc (with `-ansi -Wall -Wextra -Werror -pedantic-errors` flags)

---
