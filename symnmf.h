#ifndef SYMNMF_H_
#define SYMNMF_H_


/* These functions implement matrix computations for symmetric NMF and related operations, as described in the accompanying PDF documentation */
double** symnmf_c(double **H,double **W, int n, int k);

double** sym_c(double **X, int n, int d);

double** ddg_c(double **X, int n, int d);

double** norm_c(double **X, int n, int d);


#endif
