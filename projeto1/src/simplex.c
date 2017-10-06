#include <stdio.h>
#include <stdlib.h>

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
	for (i=0; i<n; i++){
		printf("Linha %d", i+1);
		A[i] = preencheVetor(n);
	}

	return 0;	
}
