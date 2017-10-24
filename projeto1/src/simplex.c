#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/////// utils

int *alocaVetor(int tam) {
	return (int*) calloc(tam, sizeof(int));
}

int **alocaMatriz(int lin, int col) {
	int **matriz = (int**) malloc(lin * sizeof(int *));

	for (int i=0; i<lin; i++) {
		matriz[i] = alocaVetor(col);
	}

	return matriz;
}

// Essa funcao le os valores de um vetor da entrada padrao
int *lerVetor(int tam){
	int *vet = alocaVetor(tam);

	for (int i=0; i<tam; i++) {
		scanf("%d", &vet[i]);
	}

	return vet;
}

int **lerMatriz(int lin, int col) {
	int** matriz = alocaMatriz(lin, col);

	for (int i=0; i<lin; i++) {
		printf("Digite a linha %d:\n", i+1);

		for (int j=0; j<col; j++) {
			scanf("%d", &matriz[i][j]);

			//printf("just read %d  at i=%d, j=%d, addr %d\n", matriz[i][j], i, j, &matriz[i][j]);
		}
	}

	return matriz;
}

void printaMatriz(int lin, int col, int **matriz) {
	for(int i=0; i<lin; i++) {

		for(int j=0; j<col; j++) {
			printf("%d ", matriz[i][j]);
		}

		printf("\n");
	}
	printf("\n");
}

void printaVetor(int tam, int *vetor) {
	for (int i=0; i<tam; i++) {
		printf("%d ", vetor[i]);
	}
	printf("\n\n");
}

void printState(int m, int n, int **A, int **B, int **N, int *c, int *cb, int *cn) {
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



//calcula custo relativo
int pegaIndiceEntraNaBase(int *custoNB, int *lambda, int **N, int m, int n) {
	int indiceMin = 0;
	int valMin = INT_MAX;
	
	//para cada coluna
	for (int j=0; j<n-m; j++) {
		int aux =0;

		//calcula o custo relativo
		for (int i=0; i<m; i++) {
			aux += lambda[j] *N[i][j];
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
int **defineB(int m, int n, int **A, int **custoB, int *custo) {
	int **basica = alocaMatriz(m, m);

	for (int i=0; i<m; i++) {
		for (int j=0; j<m; j++) {
			basica[i][j] = A[i][n-m+j];
			(*custoB)[j] = custo[n-m+j]; 
		}
	}

	return basica;
}
//Definicao das matrizes Nao-Basica
int **defineNaoB(int m, int n, int **A, int **custoNB, int *custo) {
	int **naoBasica = alocaMatriz(m, n-m);

	for (int i=0; i<m; i++) {
		for (int j=0; j<n-m; j++){
			naoBasica[i][j] = A[i][j];
			(*custoNB)[j] = custo[j];
		}
	}

	return naoBasica;
}


//////// TODO!!!!

//resolver sistema Xb = (B^-1) b
int *resolveSistema(int tam, int **matriz, int *vetor){
	int *xb;
	return xb;
}



// Funcao principal
int main(){

	int m;			// numero de linhas
	int n;			// numero de colunas
	int *c; 		// vetor de custos
	int *b; 		// vetor de recursos
	int **A;		// matriz de coeficientes
	int **B;		// matriz basica 
	int **N;		// matriz nao basica 
	int *cb;		// custo basico
	int *cn;		// custo nao basico
	int *xb;		// vetor de solucao basica
	int *lambda;	// vetor multiplicador simplex
	int *cRelativo;	// custos relativos
	
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
		lambda = resolveSistema(m,B,c);
		int indEntraBase = pegaIndiceEntraNaBase(cn, lambda, N, m, n);
		if (indEntraBase == -1)
			break;
		break;
	}

	return 0;	
}
