#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

//multiplicacao de matriz por vetor
int **multiplicaMatrizPorVetor(int **matriz, int *vetor, int tam) {
	int *mult;

	mult = (int) calloc(0, sizeof(int));
	puts("validar");
	for (int i=0; i<tam; i++)
		for (int j=0; j<tam; j++)
			mult = matriz[i][j] * vetor[i];
	return mult;
}

//calcula custo relativo
int pegaIndiceEntraNaBase(int *custoNB, int *lambda, int **N, int m, int n) {
	int i;
	int j;
	int indiceMin = 0;
	int valMin = INT_MAX;
	
	for (i=0; i<n-m; i++){
		int aux =0;
		for (j=0; j<m; j++)
			aux += lambda[i] *N[j][i];
		if (valMin > custoNB[i]-aux){
			valMin = custoNB[i]-aux;
			indiceMin = i;
		}
	}
	if(valMin > 0)
		return -1;
	else
		return indiceMin;
}

//Definicao das matrizes Basica e do custo das matrizes basicas
int **defineB(int m, int n, int **A, int **custoB, int *custo){
	int **basica;
	int i;
	int j;


	//Alocando a matriz B
	basica = (int*)malloc(m *sizeof (int));
	for (i=0; i<m; i++)
		basica[i] = (int) calloc(0, m*sizeof(int));
	for (i=0; i<m; i++)
		for (j=0; j<m; j++) {
			basica[i][j] = A[i][n-m+j];
			(*custoB)[j] = custo[n-m+j]; 
		}

	return basica;
}
//Definicao das matrizes Nao-Basica
int **defineNaoB(int m, int n, int **A, int **custoNB, int *custo){
	int **Naobasica;
	int i;
	int j;

	//Alocando a matriz B
	Naobasica = (int*)malloc(m *sizeof (int));
	for (i=0; i<m; i++)
		Naobasica[i] = (int) calloc(0, (n-m)*sizeof(int));

	for (i=0; i<m; i++)
		for (j=0; j<n-m; j++){
			Naobasica[i][j] = A[i][j];
			(*custoNB)[j] = custo[j];
		}

	return Naobasica;
}

// Essa funcao faz o preenchimento de vetores de forma dinamica
int *preencheVetor(int tam){
	int i;
	int *vet;

	vet = (int*) malloc(tam * sizeof(int));
	for (i=0; i<tam; i++)
		scanf("%d", &vet[i]);

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
	c = preencheVetor(n);
	
	//vetor de recursos
	printf("Digite o vetor de recursos.\n");
	b = preencheVetor(m);
	
	//matriz de coeficientes
	A = (int**)malloc(n * sizeof(int));
	printf("Digite a matriz de coeficientes de restricao.\n");
	for (i=0; i<m; i++){
		printf("Linha %d\n", i+1);
		A[i] = preencheVetor(n);
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
