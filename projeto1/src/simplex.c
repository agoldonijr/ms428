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

double **lerMatriz(int lin, int col, double **matriz) {
	for (int i=0; i<lin; i++) {
		printf("Digite a linha %d:\n", i+1);

		for (int j=0; j<col; j++) {
			scanf("%lf", &matriz[i][j]);

			//printf("just read %d	at i=%d, j=%d, addr %d\n", matriz[i][j], i, j, &matriz[i][j]);
		}
	}

	return matriz;
}

// troca a posicao indA do vetor A com a posicao indB do vetor B
void trocaIndice(double *A, double *B, int indA, int indB) {
	double aux = A[indA];
	A[indA] = B[indB];
	B[indB] = aux;
}

// troca a coluna colA da matriz A com a coluna colB da matriz B
void trocaColuna(int qtdLinhas, double **A, double **B, int colA, int colB) {
	for (int i=0; i<qtdLinhas; i++) {
		trocaIndice(A[i], B[i], colA, colB);
	}
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

gsl_vector *gslCloneMatrixColumnAsVector(int size, double **sourceMatrix, int colIndex) {
	gsl_vector *v = gsl_vector_alloc(size);

	for (int i=0; i<size; i++) {
		gsl_vector_set(v, i, sourceMatrix[i][colIndex]);
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



// calcula os custos relativos e retorna o indice do menor custo negativo
// se todos os custos relativos forem >= 0 entao retorna -1 indicando condição de otimalidade
int pegaIndiceEntraNaBase(double *custoNB, double *lambda, double **N, int m, int n) {
	int indiceMin = 0;
	double valMin = DBL_MAX;
	
	printf("custos relativos:\n");

	//para cada coluna
	for (int j=0; j<n-m; j++) {
		double lambdaEscalarNj=0;

		//calcula o custo relativo
		for (int i=0; i<m; i++) {
			lambdaEscalarNj += lambda[i] * N[i][j];
		}

		double custoRelativo = custoNB[j]-lambdaEscalarNj;
		printf("%lf ", custoRelativo);

		if (valMin > custoRelativo) {
			valMin = custoRelativo;
			indiceMin = j;
		}
	}

	printf ("\n\n");

	if (valMin >= 0) {
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

double *resolveSistemaUsandoColunaDaMatrizComoVetor(int tam, double **matriz, double **matrizOrigemDoVetor, int colunaDoVetor) {
	gsl_matrix *m = gslMatrixClone(tam, matriz);
	gsl_vector *v = gslCloneMatrixColumnAsVector(tam, matrizOrigemDoVetor, colunaDoVetor);

	return resolveSistemaGSL(tam, m, v);
}

int pegaIndiceSaiDaBase(int tam, double *y, double *xb) {
	double min = DBL_MAX;
	int minIndex = -1;

	for(int i=0; i<tam; i++) {
		if (y[i] > 0 && xb[i]/y[i] < min) {
			min = xb[i]/y[i];
			minIndex = i;
		}
	}

	return minIndex;
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
	double *y;			// direcao simplex

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
	double** matriz = alocaMatriz(m, n);
	A = lerMatriz(m, n, matriz);

	//preenche o vetor custoBasico e custoNaoBasico
	cb = alocaVetor(m);
	cn = alocaVetor(n-m);

	//preenchendo a matriz basica e nao basica
	B = defineB(m,n,A,&cb,c);
	N = defineNaoB(m,n,A,&cn,c);



	//loop de iteracoes do simplex
	for (int iteracao = 0; iteracao<1000; iteracao++) {
		printf("ITERACAO %d \n\n", iteracao);
		printState(m, n, A, B, N, c, cb, cn);

		// PASSO 1: {cálculo da solução básica}
		//calcula Xb
		xb = resolveSistema(m,B,b);
		//SET ME FREE!!

		printf("Xb: \n");
		printaVetor(m, xb);


		// PASSO 2: {cálculo dos custos relativos} 
		// calcula vetor multiplicador simplex
		lambda = resolveSistemaTransposta(m,B,cb);
		//SET ME FREE!!

		printf("lambda: \n");
		printaVetor(m, lambda);

		//calcula custos relativos e pega indice do menor custo relativo
		int indEntraBase = pegaIndiceEntraNaBase(cn, lambda, N, m, n);

		// PASSO 3: {teste de otimalidade}
		// quando retorna -1 quer dizer que todos os custos relativos eram >= 0
		// portanto este ponto é o otimizador da funcao
		if (indEntraBase == -1) {
			//TODO chamar funcao mas passando as coisas certas
			double fxb = 1337;
			reportaOtimo(m, xb, fxb);
			return 0;
		}

		// PASSO 4: {cálculo da direção simplex}
		// calculo da direcao simplex
		y = resolveSistemaUsandoColunaDaMatrizComoVetor(m, B, N, indEntraBase);

		printf("y: \n");
		printaVetor(m, y);


		// PASSO 5: {determinação do passo e variável a sair da base}

		// Se y ≤ 0,então: PARE=VERDADE {problema não tem solução ótima finita}.
		int temSolucaoFinita = 0;
		for (int i=0; i<m; i++) {
			if (y[i] > 0) {
				temSolucaoFinita = 1;
				break;
			}
		}

		if (!temSolucaoFinita) {
			printf ("Problema nao tem solucao finita!!\n");
			return 0;
		}

		// determina a variavel a sair da base
		int indSaiDaBase = pegaIndiceSaiDaBase(m, y, xb);


		// PASSO 6: {atualização: nova partição básica, troque a l-ésima coluna de B pela k-ésima coluna de N}.
		trocaColuna(m, B, N, indSaiDaBase, indEntraBase);
		trocaIndice(cb, cn, indSaiDaBase, indEntraBase);
	}

	return 0;	
}
