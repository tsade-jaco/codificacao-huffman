#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int MAX = 500;
char Codigos[500];
bool limparNo = false;

struct no{
	int simbolo;
	int frequencia;
	struct no *proximo;
	struct no *esquerda;
	struct no *direita;
};

typedef struct no *No;

struct huffman{
	char *texto;
	int *VetorQTD;
	char *VetorChars;
	char **Tabela;
	int QTDBits;
	No Raiz;
};

typedef struct huffman *Huffman;


static void CriarMatriz(Huffman H, int TAM);
static void OrdenarVetores(Huffman H);
static void InicializarArvoreHuffman(Huffman H);
static void CriarArvoreHuffman(Huffman H);
int tamanhoDaLista(No N);
No CriarNo(int frequencia, No N, No proximo, bool estaNoFinal);
static void MostrarArvoreInOrdem(No Raiz);
static void MostrarTabelaDeCodigos(Huffman H);
static void ComputarNovoCodigo(No Raiz, int p, char **tabela, int *qtdBits, int linha, int coluna);
static void  LimparNo(No Raiz, int apagarNo);
static void CriarTabelaDeBinarios(Huffman H);
static void CalcularTaxaDeCompressao(Huffman H);

void main(){
	int i = -1,j;
	char caracteres[MAX], tmp;

	FILE *arquivo;
	arquivo = fopen("texto.txt", "r");

	while(!feof(arquivo) && i < MAX){
		i++;
		fscanf(arquivo, "%c", &caracteres[i]);
		
	}

	fclose(arquivo);

	Huffman MeuTexto;
	MeuTexto = malloc(sizeof(Huffman*));
	MeuTexto->texto = malloc(i * sizeof(char*));

	for(j = 0; j< i;++j)
		MeuTexto->texto[j] = caracteres[j];

	CriarMatriz(MeuTexto, i);
	InicializarArvoreHuffman(MeuTexto);
	CriarArvoreHuffman(MeuTexto);
	printf("Árvore Huffman:\n");
	MostrarArvoreInOrdem(MeuTexto->Raiz);
	MostrarTabelaDeCodigos(MeuTexto);
	CalcularTaxaDeCompressao(MeuTexto);

	printf("\n\nFim da execução do programa.\n");
}

static void CriarMatriz(Huffman H, int TAM){
	int i,j,k = 0;
	char chars[TAM], tmp;
	int count;
	bool jaEstaNaLista = false;
	
	for(i = 0; i< TAM; ++i){
		for(j = 0; j<TAM; ++j){
			if(chars[j] == H->texto[i]){
				jaEstaNaLista = true;
				break;	
			}

		}
		if(jaEstaNaLista == false){
			chars[k] = H->texto[i];
			k++;
		}

		jaEstaNaLista = false;
	}

	int qtds[k];

	for(i = 0; i< k; ++i){
		tmp = chars[i];
		count = 0;
		for(j = 0; j< TAM; ++j){
			if(tmp == H->texto[j]){
				count++;
			}
		}
		qtds[i] = count;
	}

	MAX = k;
	H->VetorQTD = malloc(MAX * sizeof(int*));
	H->VetorChars = malloc(MAX * sizeof(char*));

	for(i = 0; i<MAX; ++i)
		H->VetorChars[i] = chars[i];

	for(i = 0; i<MAX; ++i)
		H->VetorQTD[i] = qtds[i];

	OrdenarVetores(H);
}

static void OrdenarVetores(Huffman H){
	int i,j,key;
	char keyC;

	for(i=1; i<MAX; i++){
		key = H->VetorQTD[i];
		keyC = H->VetorChars[i];
		j = i-1;

		while(j >= 0 && H->VetorQTD[j] > key ){
			H->VetorQTD[j +1] = H->VetorQTD[j];
			H->VetorChars[j+1] = H->VetorChars[j];
			j = j-1;
		}
		H->VetorQTD[j+1] = key;
		H->VetorChars[j + 1] = keyC;
	}

}

static void InicializarArvoreHuffman(Huffman H){
	int i;

	No atual, anterior;


	for(i=0; i< MAX; i++){
		atual = malloc(sizeof(struct no));
		atual->simbolo = H->VetorChars[i];
		atual->frequencia = H->VetorQTD[i];
		atual->direita = NULL;
		atual->esquerda = NULL;

		if(i == 0){
			H->Raiz = atual;
		}else{
			anterior->proximo = atual;
		}

		anterior = atual;
	}
}

static void CriarArvoreHuffman(Huffman H){

	No NovoNo, tmp, tmp2;

	int frequencia, tam;
	
	
	tam = tamanhoDaLista(H->Raiz);
	if(tam == 1){
		H->Raiz->esquerda = NULL;
		H->Raiz->direita = NULL;
		H->Raiz->proximo = NULL;
	}else{
		tmp = H->Raiz;
		while(tam != 1){
			frequencia = tmp->frequencia + tmp->proximo->frequencia;
			tmp2 = tmp;
			while(tmp2->proximo != NULL && tmp2->proximo->frequencia <= frequencia){
				tmp2 = tmp2->proximo;
			}

			if(tmp2->proximo == NULL){
				tmp2->proximo = CriarNo(frequencia, tmp, NULL, true);
			}else{
				tmp2->proximo = CriarNo(frequencia, tmp, tmp2->proximo, false);
			}

			tmp = tmp->proximo->proximo;
			tam = tamanhoDaLista(tmp);
			
		}
		H->Raiz = tmp;
	}

}

int tamanhoDaLista(No N){
	No tmp = N;
	int i;
	for(i = 0; tmp != NULL; i++){
		tmp = tmp->proximo;
	}
	return i;
}


No CriarNo(int frequencia, No N, No proximo, bool estaNoFinal){
	No no;
	no = malloc(sizeof(struct no));
	no->esquerda = N;
	no->direita = N->proximo;
	no->frequencia = frequencia;
	no->simbolo = '#';

	
	if(estaNoFinal == false){
		no->proximo = proximo;
	}

	return no;
}

static void MostrarArvoreInOrdem(No Raiz){
	Raiz->proximo = NULL;
	if(Raiz->esquerda != NULL){
		MostrarArvoreInOrdem(Raiz->esquerda);
		if(Raiz->direita != NULL){
			MostrarArvoreInOrdem(Raiz->direita);
		}
	}else{
		int code = Raiz->simbolo;
		if(code == 10)
			printf("[LINE FEED] ");
		else if(code == 32)
			printf("[SPACE] ");
		else
			printf("[%c] ",Raiz->simbolo);
	}
}

static void MostrarTabelaDeCodigos(Huffman H){
	int i = 0, j = 0;
	char *codigo;

	CriarTabelaDeBinarios(H);
	if(H->Raiz->esquerda == NULL && H->Raiz->direita == NULL){
		H->Tabela[0][0]= H->Raiz->simbolo;
		H->Tabela[0][1] = '0';
	}else{
		while(H->Raiz ->direita != NULL || H->Raiz->esquerda != NULL){
			ComputarNovoCodigo(H->Raiz, i, H->Tabela, &H->QTDBits, i, 1);
			i++;
		}
	}

	printf("\n\nTABELA DE CÓDIGOS:\n");
	printf("Char | Código\n");

	for(i=0;i<MAX;++i){
		int code = H->Tabela[i][0];
		codigo = H->Tabela[i];
		if(code == 10)
			printf(" LF    ");
		else if(code == 32)
			printf(" SP    ");
		else
			printf("%2c     ", H->Tabela[i][0]);
		for (j = 1; codigo[j] != '\0'; ++j){
			printf("%c",codigo[j]);
			
		}
		printf("\n");
	}
}

static void ComputarNovoCodigo(No Raiz, int p, char **tabela, int *qtdBits, int linha, int coluna){
	int apagarNo;

	if(Raiz->esquerda != NULL){
		tabela[linha][coluna] = '0';
		apagarNo = 0;
		ComputarNovoCodigo(Raiz->esquerda, p, tabela, qtdBits, linha, coluna+1);
		if(limparNo)
			LimparNo(Raiz, apagarNo);

		return;
	}

	if(Raiz->direita != NULL){
		tabela[linha][coluna] = '1';
		apagarNo = 1;
		ComputarNovoCodigo(Raiz->direita, p, tabela, qtdBits, linha, coluna+1);
		if(limparNo)
			LimparNo(Raiz, apagarNo);

		return;
	}

	if(Raiz->esquerda == NULL && Raiz->direita == NULL && Raiz->simbolo != '#'){ 
		*qtdBits = Raiz->frequencia * (coluna - 1) + *qtdBits;
		tabela[linha][0] = Raiz->simbolo;
	}

	Raiz = NULL;
	free(Raiz);
	limparNo = true;
	
}

static void CriarTabelaDeBinarios(Huffman H){
	H->QTDBits = 0;

	char **tabela = malloc(MAX * sizeof(char*));

	for(int i = 0; i< MAX; ++i)
		tabela[i] = malloc(8 * sizeof(char*));

	H->Tabela = tabela;
}



static void LimparNo(No Raiz, int apagarNo){

	if(apagarNo == 0)
		Raiz->esquerda = NULL;

	if(apagarNo == 1)
		Raiz->direita = NULL;

	limparNo = false;

	
	if(Raiz->direita == NULL && Raiz->esquerda == NULL){

		Raiz = NULL;
		free(Raiz);
		limparNo = true;
	}
}

static void CalcularTaxaDeCompressao(Huffman H){
	float BitsOriginais = 0.0;
	float tx = 0.0;
	for(int i = 0; i< MAX; ++i)
		BitsOriginais = (H->VetorQTD[i] * 8) + BitsOriginais;

	tx = 1 - (H->QTDBits/BitsOriginais);
	printf("\nTaxa de Compressão: %.3f, aprox. %.2f%%", tx,(tx*100));

}