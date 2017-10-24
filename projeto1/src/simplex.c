#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/////// utils

int *alocaVetor(int tam) {
	return (int*) calloc(0, tam * sizeof(int));
}

int **alocaMatriz(int lin, int col) {
	int **matriz = (int**) malloc(lin * sizeof(int*));

	for (int i=0; i<lin; i++) {
		matriz[i] = (int*) calloc(0, col * sizeof(int));
	}

	return matriz;
}

///////



//calcula custo relativo
int pegaIndiceEntraNaBase(int *custoNB, int *lambda, int **N, int m, int n) {
	int indiceMin = 0;
	int valMin = INT_MAX;
	
	for (int i=0; i<n-m; i++) {
		int aux =0;

		for (int j=0; j<m; j++) {
			aux += lambda[i] *N[j][i];
		}

		if (valMin > custoNB[i]-aux) {
			valMin = custoNB[i]-aux;
			indiceMin = i;
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

	for (int i=0; i<m; i++)
		for (int j=0; j<n-m; j++){
			naoBasica[i][j] = A[i][j];
			(*custoNB)[j] = custo[j];
		}

	return naoBasica;
}

// Essa funcao le os valores de um vetor da entrada padrao
int *lerVetor(int tam){
	int *vet = alocaVetor(tam);

	for (int i=0; i<tam; i++) {
		scanf("%d", &vet[i]);
	}

	return vet;
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
	int i;			// variavel temporaria para criacao dos vetores
	int j;			// variavel temporaria para criacao dos vetores
	
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
	A = (int**)malloc(n * sizeof(int));
	printf("Digite a matriz de coeficientes de restricao.\n");
	for (i=0; i<m; i++){
		printf("Linha %d\n", i+1);
		A[i] = lerVetor(n);
	}
	//preenche o vetor custoBasico e custoNaoBasico
	cb = (int)calloc (0, m*sizeof(int));
	cn = (int)calloc (0, (n-m)*sizeof(int));
	//preenchendo a matriz basica e nao basica
	B = defineB(m,n,A,&cb,c);
	N = defineNaoB(m,n,A,&cn,c);
	

	while(true){
		int indEntraBase;
		//calcula Xb
		xb = resolveSistema(m,B,b);
		lambda = resolveSistema(m,B,c);
		indEntraBase = pegaIndiceEntraNaBase(cn, lambda, N, m, n);
		if (indEntraBase == -1)
			break;
		break;
	}

	return 0;	
}
