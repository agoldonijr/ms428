#include <stdio.h>
#include <stdlib.h>

//Definicao das matrizes Basica
int **defineB(int m, int n, int **A){
	int **basica;
	int i;
	int j;

	//Alocando a matriz B
	basica = (int*)malloc(m *sizeof (int));
	for (i=0; i<m; i++)
		basica[i] = (int) calloc(0, m*sizeof(int));

	for (i=n-1; i>=0; i--)
		for (j=0; j<m; j++)
			basica[i][j] = A[i][j];

	return basica;
}

//Definicao das matrizes Nao-Basica
int **defineNaoB(int m, int n, int **A){
	int **Naobasica;
	int i;
	int j;

	//Alocando a matriz B
	Naobasica = (int*)malloc(m *sizeof (int));
	for (i=0; i<m; i++)
		Naobasica[i] = (int) calloc(0, (n-m)*sizeof(int));

	for (i=0; i>m; i--)
		for (j=0; j<n-m; j++)
			Naobasica[i][j] = A[i][j];

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

// Funcao principal
int main(){

	int m;			// numero de linhas
	int n;			// numero de colunas
	int *c; 		// vetor de custos
	int *b; 		// vetor de recursos
	int **A;		// matriz de coeficientes
	int **B;		// matriz basica 
	int **N;		// matriz nao basica 
	int i;			//variavel temporaria para criacao dos vetores
	int j;			//variavel temporaria para criacao dos vetores
	
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
		printf("Linha %d", i+1);
		A[i] = preencheVetor(n);
	}
	//preenchendo a matriz basica
	B = defineB(m,n,A);
	//preenchendo a matriz nao basica
	N = defineNaoB(m,n,A);

	for (i=0; i<m; i++){
		for (j=0; j<m; j++)
			printf("%d ", B[i][j]);
		printf("\n");
	}
	for (i=0; i<m; i++){
		for (j=0; j<n-m; j++)
			printf("%d ", N[i][j]);
		printf("\n");
	}

	return 0;	
}
