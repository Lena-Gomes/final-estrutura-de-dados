#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define TAMANHO_ALFABETO 26    	
#define MAX_TENTATIVAS 5       	
#define TAMANHO_MAXIMO_PALAVRA 30
#define MAX_ERROS_PARA_DICA 3  	

// Estrutura de um nó da árvore TRIE, com os ponteiros para nós filhos.
typedef struct NodoTrie {
    struct NodoTrie *letrasFilhas[TAMANHO_ALFABETO];  // Uma posição para cada letra
    bool finalDaPalavra;                          	
} NodoTrie;

// Função para criar um novo nó na TRIE
NodoTrie *criarNovoNodo() {
    NodoTrie *novoNodo = (NodoTrie *)malloc(sizeof(NodoTrie));  // Aloca memória para o novo nó
    novoNodo->finalDaPalavra = false;  // Inicializa o nó como não sendo final de uma palavra
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        novoNodo->letrasFilhas[i] = NULL;  // Inicializa todos os filhos como NULL
    }
    return novoNodo;  // Retorna o novo nó criado
}

// Função para adicionar uma palavra à TRIE
void adicionarPalavra(NodoTrie *raiz, const char *palavra) {
    NodoTrie *nodoAtual = raiz;
    for (int i = 0; palavra[i] != '\0'; i++) {
        int posicaoLetra = palavra[i] - 'a';  // Converte a letra para um índice (a = 0, b = 1, etc.)
        if (nodoAtual->letrasFilhas[posicaoLetra] == NULL) {
            nodoAtual->letrasFilhas[posicaoLetra] = criarNovoNodo();  // Cria um novo nó se ele não existir
        }
        nodoAtual = nodoAtual->letrasFilhas[posicaoLetra];  // Avança para o próximo nó
    }
    nodoAtual->finalDaPalavra = true;  // Marca o nó atual como o final de uma palavra
}

// Função para selecionar uma palavra aleatoriamente do banco de palavras
void selecionarPalavraAleatoria(NodoTrie *raiz, char *palavraEscolhida, char listaDePalavras[][TAMANHO_MAXIMO_PALAVRA], int totalDePalavras) {
    srand(time(NULL));  // Inicializa o gerador de números aleatórios
    int indiceAleatorio = rand() % totalDePalavras;  // Gera um índice aleatório para escolher a palavra
    strcpy(palavraEscolhida, listaDePalavras[indiceAleatorio]);  // Copia a palavra escolhida para a variável "palavraEscolhida"
}

// Função para mostrar o estado atual da palavra no jogo (letras adivinhadas ou "_")
void mostrarProgressoDaPalavra(const char *palavra, const bool *letrasAdivinhadas) {
    for (int i = 0; palavra[i] != '\0'; i++) {
        if (letrasAdivinhadas[i]) {
            printf("%c ", palavra[i]);  // Mostra a letra se ela já foi adivinhada
        } else {
            printf("_ ");  // Mostra "_" para letras ainda não adivinhadas
        }
    }
    printf("\n");
}

// Função para exibir uma dica após 3 erros
void mostrarDica(const char *classeDaPalavra, const char *dica) {

    printf("Dica: A palavra pertence à classe '%s'.\n Dica: %s\n", classeDaPalavra, dica);
}

// Função para o jogador adivinhar a palavra completa
bool adivinharPalavra(const char *palavraEscolhida) {
    char tentativa[TAMANHO_MAXIMO_PALAVRA];
    printf("Você pode tentar adivinhar a palavra completa. \n Digite sua tentativa: ");
    scanf("%s", tentativa);

    if (strcmp(tentativa, palavraEscolhida) == 0) {
        printf("Parabéns! Você acertou a palavra: %s\n", palavraEscolhida);
        return true;
    } else {
        printf("POXA!! Você Errou, a palavra correta era: %s\n", palavraEscolhida);
        return false;
    }
}

// Função que gerencia a lógica principal do jogo da forca
void jogarForca(NodoTrie *raiz, char listaDePalavras[][TAMANHO_MAXIMO_PALAVRA], char listaDeClasses[][30], char listaDeDicas[][TAMANHO_MAXIMO_PALAVRA], int totalDePalavras) {
    char palavraEscolhida[TAMANHO_MAXIMO_PALAVRA];  // Armazena a palavra que será usada no jogo
    selecionarPalavraAleatoria(raiz, palavraEscolhida, listaDePalavras, totalDePalavras);  // Seleciona uma palavra aleatória

    bool letrasAdivinhadas[TAMANHO_MAXIMO_PALAVRA] = {false};  // Marca as letras adivinhadas
    int tentativasRestantes = MAX_TENTATIVAS;  // Número de tentativas que o jogador ainda tem
    bool palavraCompleta = false;  // Flag para verificar se o jogador adivinhou a palavra inteira
    int tamanhoDaPalavra = strlen(palavraEscolhida);  // Tamanho da palavra escolhida
    int erros = 0;  // Contador de erros

    // Determinar a classe da palavra e a dica (os índices são correspondentes nas listas)
    int indiceDaPalavra = -1;
    for (int i = 0; i < totalDePalavras; i++) {
        if (strcmp(listaDePalavras[i], palavraEscolhida) == 0) {
            indiceDaPalavra = i;
            break;
        }
    }

    printf("Olá meninas e meninos tudo bom? \n Bem-vindo ao JOGO DA FORCA!\n");


    while (tentativasRestantes > 0 && !palavraCompleta) {  // Continua até o jogador vencer ou perder
        printf("\nNúmero de Tentativas: %d\n", tentativasRestantes);
        mostrarProgressoDaPalavra(palavraEscolhida, letrasAdivinhadas);  

        printf("Digite uma letra: ");
        char letraDigitada;
        scanf(" %c", &letraDigitada);  
        bool letraCorreta = false; 
        for (int i = 0; i < tamanhoDaPalavra; i++) {
            if (palavraEscolhida[i] == letraDigitada && !letrasAdivinhadas[i]) {
                letrasAdivinhadas[i] = true;  // Marca a letra como adivinhada
                letraCorreta = true;  // Indica que o jogador acertou a letra
            }
        }

        if (!letraCorreta) {
            tentativasRestantes--;  // Se a letra estiver errada, diminui o número de tentativas
            erros++;
            if (erros == MAX_ERROS_PARA_DICA) {
                mostrarDica(listaDeClasses[indiceDaPalavra], listaDeDicas[indiceDaPalavra]);  // Mostra a dica após 3 erros
            }

            if (tentativasRestantes == 0) {
                // Ao final o jogador pode tentar adivinhar a palavra completa
                if (!adivinharPalavra(palavraEscolhida)) {
                    return;  // Se o jogador erra a palavra completa, o jogo termina.
                }
            }
        }

        // Verifica se a palavra foi completamente adivinhada
        palavraCompleta = true;
        for (int i = 0; i < tamanhoDaPalavra; i++) {
            if (!letrasAdivinhadas[i]) {
                palavraCompleta = false;
                break;
            }
        }
    }

    // Verifica se o jogador venceu ou perdeu
    if (palavraCompleta) {
        printf("Parabéns! Você adivinhou a palavra: %s\n", palavraEscolhida);  
    } else if (tentativasRestantes == 0) {
        printf("Iiiii Você perdeu! A palavra era: %s\n", palavraEscolhida); 
    }
}

// Função principal que inicia o jogo
int main() {
    // Banco de palavras (dicionário) com suas respectivas classes e dicas
    char listaDePalavras[40][TAMANHO_MAXIMO_PALAVRA] = {
        // Animais domésticos
        "coelho", "cachorro", "gato", "hamster", "papagaio", "periquito", "galinha", "canario", "pato", "porquinho",
        // Frutas
        "kiwi", "maca", "uva", "manga", "laranja", "limonada", "abacate", "goiaba", "pera", "caju",
        // Cores
        "preto", "azul", "vermelho", "verde", "rosa", "amarelo", "marrom", "roxo", "branco", "laranja",
        // Objetos escolares
        "lapis", "caneta", "borracha", "caderno", "livro", "estojo", "tesoura", "régua", "grampeador", "mochila"
    };

    char listaDeClasses[40][30] = {
        "Animal", "Animal", "Animal", "Animal", "Animal", "Animal", "Animal", "Animal", "Animal", "Animal",
        "Fruta", "Fruta", "Fruta", "Fruta", "Fruta", "Fruta", "Fruta", "Fruta", "Fruta", "Fruta",
        "Cor", "Cor", "Cor", "Cor", "Cor", "Cor", "Cor", "Cor", "Cor", "Cor",
        "Objeto", "Objeto", "Objeto", "Objeto", "Objeto", "Objeto", "Objeto", "Objeto", "Objeto", "Objeto"
    };

    char listaDeDicas[40][TAMANHO_MAXIMO_PALAVRA] = {
        "Orelhas grandes e gosta de cenouras.", "Melhor amigo do homem.", "Adora caçar ratos.", "Pequeno e fofinho.", "Pode imitar a fala humana.", 
        "Gosta de cantar em gaiolas.", "Acorda cedo para cantar.", "Canta em gaiolas e é pequeno.", "Nada em lagoas.", "Pequeno animal que grita muito.",
        "Fruta marrom por fora e verde por dentro.", "Fruta vermelha e crocante.", "Fruta pequena e roxa.", "Fruta grande e suculenta.", "Cítrica e rica em vitamina C.",
        "Bebida azeda e doce.", "Fruta verde com caroço grande.", "Fruta brasileira, de cor amarelada.", "Fruta macia e doce.", "Fruta tropical e azeda.",
        "Cor de noite.", "Cor do céu e do mar.", "Cor do sangue.", "Cor da grama.", "Cor das flores mais delicadas.", 
        "Cor dos girassóis.", "Cor da madeira.", "Cor da uva.", "Cor da neve.", "Cor de fruta cítrica.",
        "Ferramenta para escrever.", "Ferramenta de tinta para escrever.", "Usada para apagar erros.", "Usado para tomar notas.", "Contém muito conhecimento.",
        "Guarda itens escolares.", "Ferramenta para cortar.", "Ferramenta para medir.", "Ferramenta para unir papéis.", "Carrega todos os itens escolares."
    };

    NodoTrie *raiz = criarNovoNodo();

    // Adiciona todas as palavras do banco de palavras na árvore TRIE
    for (int i = 0; i < 40; i++) {
        adicionarPalavra(raiz, listaDePalavras[i]);
    }

    jogarForca(raiz, listaDePalavras, listaDeClasses, listaDeDicas, 40);

    // Libera a memória da árvore TRIE (a função de liberar a árvore ainda precisa ser implementada)
    return 0;
}
