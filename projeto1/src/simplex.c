#include <stdio.h>
#include <stdlib.h>

//multiplicacao de matriz por vetor
int **multiplicaMatrix(int **M, int **N, int){
	return mult;
}

//resolver sistema Xb = (B^-1) b
int *resolveSistema(int tam, int **matriz, int *vetor){
	int *xb;
	return xb;
}

//resolve lambda
int *resolveLambda(int *custo, int **B){

	return lambda;
}

//calcula custo relativo
int *custosRelativo(int *custo, int *lambda, int **N){

	return custos;
}

//teste de otimalidade
int testeOtimalidade(int *custos, int tam){
	int i;

	for(i=0; i< tam; i++)
		if(custos[i] < 0)
			return 0;
}
//Definicao das matrizes Basica
int **defineB(int m, int n, int **A){
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
		}

	return basica;
}
//Define Cb
int *custoBasico(int *custos){

}
//Define Cn
int *custoNaoBasico(int *custos){
	
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

	for (i=0; i<m; i++)
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
		printf("Linha %d", i+1);
		A[i] = preencheVetor(n);
	}
	//preenchendo a matriz basica e nao basica
	B = defineB(m,n,A);
	N = defineNaoB(m,n,A);
	
	//preenche o vetor custoBasico e custoNaoBasico
	cn = custoNaoBasico();
	cb = custoBasico();

	while(true){
		//calcula Xb
		xb = resolveSistema(m,B,b);
		lambda = resolveSistema(m,B,c);
		cRelativo = custosRelativo()
		break;
	}

	return 0;	
}
