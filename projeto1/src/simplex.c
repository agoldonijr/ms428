#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdarg.h>
#include <gsl/gsl_linalg.h>

/////// utils

int DEBUGGING = 0;

void DEBUG_NO_ARG(const char *string) {
	if (DEBUGGING) {
		printf("%s", string);
	}
}

void DEBUG(const char *string, int n, ...) {
	if (DEBUGGING) {
		va_list ap;
		va_start(ap, n);
		vprintf(string, ap);
	}
}

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
double *lerVetor(int tam, double *vet) {
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

void trocaIndiceInt(int *A, int *B, int indA, int indB) {
	int aux = A[indA];
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
			DEBUG("%lf ", 1, matriz[i][j]);
		}

		DEBUG_NO_ARG("\n");
	}
	DEBUG_NO_ARG("\n");
}

void printaVetor(int tam, double *vetor) {
	for (int i=0; i<tam; i++) {
		DEBUG("%lf ", 1, vetor[i]);
	}
	DEBUG_NO_ARG("\n\n");
}

void printaVetorInt(int tam, int *vetor) {
	for (int i=0; i<tam; i++) {
		DEBUG("%d ", 1, vetor[i]);
	}
	DEBUG_NO_ARG("\n\n");
}

void printState(int m, int n, double **A, double **B, double **N, double *c, double *cb, double *cn) {
	//printState(m, n, A, B, N, c, cb, cn);

	DEBUG_NO_ARG("A\n");
	printaMatriz(m,n,A);

	DEBUG_NO_ARG("B\n");
	printaMatriz(m,m,B);

	DEBUG_NO_ARG("N\n");
	printaMatriz(m,n-m,N);

	DEBUG_NO_ARG("c\n");
	printaVetor(n, c);

	DEBUG_NO_ARG("cb\n");
	printaVetor(m, cb);

	DEBUG_NO_ARG("cn\n");
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

void gslCloneBack(gsl_vector *vector, double *destino) {
	for (int i=0; i < vector->size; i++) {
		destino[i] = gsl_vector_get(vector, i);
	}
}


void resolveSistemaGSL(int tam, gsl_matrix *m, gsl_vector *v, double *solucaoSL) {
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

	gslCloneBack(x, solucaoSL);

	// printf ("x = \n");
	// gsl_vector_fprintf (stdout, x, "%g");
	gsl_vector_free(x);
}

//////////



// calcula os custos relativos e retorna o indice do menor custo negativo
// se todos os custos relativos forem >= 0 entao retorna -1 indicando condição de otimalidade
int pegaIndiceEntraNaBase(double *custoNB, double *lambda, double **N, int m, int n) {
	int indiceMin = 0;
	double valMin = DBL_MAX;
	
	DEBUG_NO_ARG("custos relativos:\n");

	//para cada coluna
	for (int j=0; j<n-m; j++) {
		double lambdaEscalarNj=0;

		//calcula o custo relativo
		for (int i=0; i<m; i++) {
			lambdaEscalarNj += lambda[i] * N[i][j];
		}

		double custoRelativo = custoNB[j]-lambdaEscalarNj;
		DEBUG("%lf ", 1, custoRelativo);

		if (valMin > custoRelativo) {
			valMin = custoRelativo;
			indiceMin = j;
		}
	}

	DEBUG_NO_ARG ("\n\n");

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

	for (int i=0; i<tam; i++) {
		printf("%lf ", pontoOtimo[i]);
	}
	printf("\n\n");

	printf("f(x) = %lf\n", valorOtimo);
}


//resolver sistema B * Xb =  b
void resolveSistema(int tam, double **matriz, double *vetor, double *solucaoSL) {
	gsl_matrix *m = gslMatrixClone(tam, matriz);
	gsl_vector *v = gslVectorClone(tam, vetor);

	resolveSistemaGSL(tam, m, v, solucaoSL);
}

void resolveSistemaTransposta(int tam, double **matriz, double *vetor, double *solucaoSL) {
	gsl_matrix *m = gslMatrixTransposeClone(tam, matriz);
	gsl_vector *v = gslVectorClone(tam, vetor);

	return resolveSistemaGSL(tam, m, v, solucaoSL);
}

void resolveSistemaUsandoColunaDaMatrizComoVetor(int tam, double **matriz, double **matrizOrigemDoVetor, int colunaDoVetor, double *solucaoSL) {
	gsl_matrix *m = gslMatrixClone(tam, matriz);
	gsl_vector *v = gslCloneMatrixColumnAsVector(tam, matrizOrigemDoVetor, colunaDoVetor);

	return resolveSistemaGSL(tam, m, v, solucaoSL);
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

void adicionaBigM(double *custos, double **A, int lin, int col) {
	for (int k=col; k<col+lin; k++) {
		A[k-col][k] = 1;
		custos[k] = 1000000000000000000;
	}
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

	int *indicesBaseENaoBase; // vetor para dizer quais variaveis sao basicas e quais sao nao basicas
	// as priemiras m posicoes do vetor representam os indices relativos a matriz A das colunas de N
	// as ultimas n-m posicoes do vetor representam os indices relativos a matriz A das colunas de B

	printf("\nEste programa resolve problemas de otimizacao na forma padrao usando o algoritimo primal simplex.\n");
	printf("Siga as instrucoes abaixo para inserir os dados do problema na forma padrao.\n\n");

	//Numero de linhas e colunas 
	printf("Digite o numero de linhas e colunas da matriz de restricoes.\n");
	scanf("%d %d", &m, &n);
	
	//vetor de custos
	c = alocaVetor(n+m);
	printf("Digite os coeficientes da funcao de custo a ser minimizada (na ordem x1 x2 x3 ... xn).\n");
	lerVetor(n, c);
	
	//vetor de recursos
	b = alocaVetor(m);
	printf("Digite os valores das restricoes (Ax = b, digite o b).\n");
	lerVetor(m, b);
	
	//matriz de coeficientes
	double** matriz = alocaMatriz(m, n+m);
	printf("Digite a matriz de coeficientes de restricao (Ax = b, digite a matriz A).\n");
	A = lerMatriz(m, n, matriz);


	// a partir desse momento o programa considera o problema com BigM
	adicionaBigM(c, A, m, n);
	n = n+m;

	indicesBaseENaoBase = (int*) calloc(n, sizeof(int));
	for (int i=0; i<n; i++) {
		indicesBaseENaoBase[i] = i;
	}


	//preenche o vetor custoBasico e custoNaoBasico
	cb = alocaVetor(m);
	cn = alocaVetor(n-m);

	//preenchendo a matriz basica e nao basica
	B = defineB(m,n,A,&cb,c);
	N = defineNaoB(m,n,A,&cn,c);


	xb = alocaVetor(m);
	lambda = alocaVetor(m);
	y = alocaVetor(m);
	//loop de iteracoes do simplex
	for (int iteracao = 0; iteracao<1000; iteracao++) {
		DEBUG("ITERACAO %d \n\n", 1, iteracao);
		printState(m, n, A, B, N, c, cb, cn);

		// PASSO 1: {cálculo da solução básica}
		//calcula Xb
		resolveSistema(m, B, b, xb);

		DEBUG_NO_ARG("Xb: \n");
		printaVetor(m, xb);


		// PASSO 2: {cálculo dos custos relativos} 
		// calcula vetor multiplicador simplex (lambda)
		resolveSistemaTransposta(m, B, cb, lambda);

		DEBUG_NO_ARG("lambda: \n");
		printaVetor(m, lambda);

		//calcula custos relativos e pega indice do menor custo relativo
		int indEntraBase = pegaIndiceEntraNaBase(cn, lambda, N, m, n);

		// PASSO 3: {teste de otimalidade}
		// quando retorna -1 quer dizer que todos os custos relativos eram >= 0
		// portanto este ponto é o otimizador da funcao
		if (indEntraBase == -1) {
			double fxb = 0;
			for (int i=0; i<m; i++) {
				fxb += cb[i] * xb[i];
			}

			double *xOtimo = alocaVetor(n);
			for (int i=n-m; i<n; i++) {
				xOtimo[indicesBaseENaoBase[i]] = xb[i-n+m];
			}

			// detecta infactibilidade se alguma variavel artificial for diferente de zero na solucao
			for (int i=n-m; i<n; i++) {
				if (xOtimo[i] != 0) {
					printf("\n\nProblema infactivel!!!\nAinda existem variaveis artificiais diferentes de zero na solucao encontrada com BigM\n\n");
					return 0;
				}
			}

			reportaOtimo(n-m, xOtimo, fxb);
			return 0;
		}

		// PASSO 4: {cálculo da direção simplex}
		// calculo da direcao simplex
		resolveSistemaUsandoColunaDaMatrizComoVetor(m, B, N, indEntraBase, y);

		DEBUG_NO_ARG("y: \n");
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

		// se for qualquer Yi <= 0
		// for (int i=0; i<m; i++) {
		// 	if (y[i] <= 0) {
		// 		printf ("\n\nProblema nao tem solucao finita!!!\n\n");
		// 		return 0;
		// 	}
		// }

		// determina a variavel a sair da base
		int indSaiDaBase = pegaIndiceSaiDaBase(m, y, xb);


		// PASSO 6: {atualização: nova partição básica, troque a l-ésima coluna de B pela k-ésima coluna de N}.
		trocaColuna(m, B, N, indSaiDaBase, indEntraBase);
		trocaIndice(cb, cn, indSaiDaBase, indEntraBase);

		// printf("antes e dps troca de indice\n");
		// printaVetorInt(n, indicesBaseENaoBase);
		trocaIndiceInt(indicesBaseENaoBase, indicesBaseENaoBase, n-m+indSaiDaBase, indEntraBase);
		// printaVetorInt(n, indicesBaseENaoBase);
	}

	return 0;	
}
