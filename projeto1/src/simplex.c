#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <gsl/gsl_linalg.h>

/////// utils

double *alocaVetor(int tam) {
	return (double*) calloc(tam, sizeof(double));
}

double **alocaMatriz(int lin, int col) {
	double **matriz = (double**) malloc(lin * sizeof(double *));

	for (int i=0; i<lin; i++) {
		matriz[i] = alocaVetor(col);
	}

	return matriz;
}

// Essa funcao le os valores de um vetor da entrada padrao
double *lerVetor(int tam){
	double *vet = alocaVetor(tam);

	for (int i=0; i<tam; i++) {
		scanf("%lf", &vet[i]);
	}

	return vet;
}

double **lerMatriz(int lin, int col) {
	double** matriz = alocaMatriz(lin, col);

	for (int i=0; i<lin; i++) {
		printf("Digite a linha %d:\n", i+1);

		for (int j=0; j<col; j++) {
			scanf("%lf", &matriz[i][j]);

			//printf("just read %d	at i=%d, j=%d, addr %d\n", matriz[i][j], i, j, &matriz[i][j]);
		}
	}

	return matriz;
}

void printaMatriz(int lin, int col, double **matriz) {
	for(int i=0; i<lin; i++) {

		for(int j=0; j<col; j++) {
			printf("%lf ", matriz[i][j]);
		}

		printf("\n");
	}
	printf("\n");
}

void printaVetor(int tam, double *vetor) {
	for (int i=0; i<tam; i++) {
		printf("%lf ", vetor[i]);
	}
	printf("\n\n");
}

void printState(int m, int n, double **A, double **B, double **N, double *c, double *cb, double *cn) {
	//printState(m, n, A, B, N, c, cb, cn);

	printf("A\n");
	printaMatriz(m,n,A);

	printf("B\n");
	printaMatriz(m,m,B);

	printf("N\n");
	printaMatriz(m,n-m,N);

	printf("c\n");
	printaVetor(n, c);

	printf("cb\n");
	printaVetor(m, cb);

	printf("cn\n");
	printaVetor(n-m, cn);
}

///////

/////// gsl utils

gsl_matrix *gslMatrixClone(int size, double ** matrix) {
	gsl_matrix* m = gsl_matrix_alloc(size, size);

	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			gsl_matrix_set(m, i, j, matrix[i][j]);
		}
	}

	return m;
}

gsl_matrix *gslMatrixTransposeClone(int size, double ** matrix) {
	gsl_matrix* m = gsl_matrix_alloc(size, size);

	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			gsl_matrix_set(m, i, j, matrix[j][i]);
		}
	}

	return m;
}

gsl_vector *gslVectorClone(int size, double *vector) {
	gsl_vector *v = gsl_vector_alloc(size);

	for (int i=0; i<size; i++) {
		gsl_vector_set(v,i,vector[i]);
	}

	return v;
}

double *gslCloneBack(gsl_vector *vector) {
	double *arr = alocaVetor(vector->size);

	for (int i=0; i < vector->size; i++) {
		arr[i] = gsl_vector_get(vector, i);
	}

	return arr;
}


double *resolveSistemaGSL(int tam, gsl_matrix *m, gsl_vector *v) {
	// gsl_matrix_view m = gsl_matrix_view_array (matriz, tam, tam);
	// gsl_vector_view b = gsl_vector_view_array (vetor, tam);
	gsl_vector *x = gsl_vector_alloc (tam);
	
	int s;

	gsl_permutation * p = gsl_permutation_alloc (tam);
	gsl_linalg_LU_decomp (m, p, &s);
	gsl_linalg_LU_solve (m, p, v, x);

	gsl_permutation_free (p);

	gsl_matrix_free(m);
	gsl_vector_free(v);

	double *solucaoSL = gslCloneBack(x);

	// printf ("x = \n");
	// gsl_vector_fprintf (stdout, x, "%g");
	gsl_vector_free(x);

	return solucaoSL;
}

//////////



//calcula custo relativo
int pegaIndiceEntraNaBase(double *custoNB, double *lambda, double **N, int m, int n) {
	int indiceMin = 0;
	double valMin = DBL_MAX;
	
	//para cada coluna
	for (int j=0; j<n-m; j++) {
		double aux=0;

		//calcula o custo relativo
		for (int i=0; i<m; i++) {
			aux += lambda[j] * N[i][j];
		}

		if (valMin > custoNB[j]-aux) {
			valMin = custoNB[j]-aux;
			indiceMin = j;
		}
	}

	if (valMin > 0) {
		return -1;
	} else {
		return indiceMin;
	}
}

//Definicao das matrizes Basica e do custo das matrizes basicas
double **defineB(int m, int n, double **A, double **custoB, double *custo) {
	double **basica = alocaMatriz(m, m);

	for (int i=0; i<m; i++) {
		for (int j=0; j<m; j++) {
			basica[i][j] = A[i][n-m+j];
			(*custoB)[j] = custo[n-m+j]; 
		}
	}

	return basica;
}

//Definicao das matrizes Nao-Basica
double **defineNaoB(int m, int n, double **A, double **custoNB, double *custo) {
	double **naoBasica = alocaMatriz(m, n-m);

	for (int i=0; i<m; i++) {
		for (int j=0; j<n-m; j++){
			naoBasica[i][j] = A[i][j];
			(*custoNB)[j] = custo[j];
		}
	}

	return naoBasica;
}

void reportaOtimo(int tam, double *pontoOtimo, double valorOtimo) {
	printf("Ponto otimo encontrado:\n");
	printf("x` = ");
	printaVetor(tam, pontoOtimo);

	printf("f(x) = %lf\n", valorOtimo);
}

//////// TODO!!!!


//resolver sistema B * Xb =  b
double *resolveSistema(int tam, double **matriz, double *vetor) {
	gsl_matrix *m = gslMatrixClone(tam, matriz);
	gsl_vector *v = gslVectorClone(tam, vetor);

	return resolveSistemaGSL(tam, m, v);
}

double *resolveSistemaTransposta(int tam, double **matriz, double *vetor) {
	gsl_matrix *m = gslMatrixTransposeClone(tam, matriz);
	gsl_vector *v = gslVectorClone(tam, vetor);

	return resolveSistemaGSL(tam, m, v);
}

// Funcao principal
int main(){

	int m;				// numero de linhas
	int n;				// numero de colunas
	double *c; 			// vetor de custos
	double *b; 			// vetor de recursos
	double **A;			// matriz de coeficientes
	double **B;			// matriz basica 
	double **N;			// matriz nao basica 
	double *cb;			// custo basico
	double *cn;			// custo nao basico
	double *xb;			// vetor de solucao basica
	double *lambda;		// vetor multiplicador simplex
	double *cRelativo;	// custos relativos
	
	//Numero de linhas e colunas 
	printf("Digite o numero de linhas e colunas.\n");
	scanf("%d %d", &m, &n);
	
	//vetor de custos
	printf("Digite o vetor de custos.\n");
	c = lerVetor(n);
	
	//vetor de recursos
	printf("Digite o vetor de recursos.\n");
	b = lerVetor(m);
	
	//matriz de coeficientes
	printf("Digite a matriz de coeficientes de restricao.\n");
	A = lerMatriz(m, n);

	//preenche o vetor custoBasico e custoNaoBasico
	cb = alocaVetor(m);
	cn = alocaVetor(n-m);

	//preenchendo a matriz basica e nao basica
	B = defineB(m,n,A,&cb,c);
	N = defineNaoB(m,n,A,&cn,c);
	

	printState(m, n, A, B, N, c, cb, cn);



	//loop de iteracoes do simplex
	while(true) {
		//calcula Xb
		xb = resolveSistema(m,B,b);

		printf("Xb: \n");
		printaVetor(m, xb);

		printf("lambda: \n");
		lambda = resolveSistemaTransposta(m,B,c);

		printaVetor(m, lambda);

		int indEntraBase = pegaIndiceEntraNaBase(cn, lambda, N, m, n);
		if (indEntraBase == -1) {
			//TODO chamar funcao mas passando as coisas certas
			//reportaOtimo(m, xb, fxb)
			return 0;
		}
		break;
	}

	return 0;	
}
