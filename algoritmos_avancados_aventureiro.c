
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estrutura pra cada comodo da mansao
typedef struct Sala {
    char nome[50];
    char pista[100];  // adicionei campo pra pista
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// estrutura pra arvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/*
funcao pra criar uma sala nova - agora com pista
*/
Sala* criarSala(char *nome, char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro: nao conseguiu alocar memoria!\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    if (pista != NULL) {
        strcpy(novaSala->pista, pista);
    } else {
        strcpy(novaSala->pista, "");  // sem pista
    }
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// funcao pra inserir pista na arvore BST
PistaNode* inserirPista(PistaNode *raiz, char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
        if (novo == NULL) {
            printf("Erro na alocacao!\n");
            return NULL;
        }
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    
    // insere baseado na ordem alfabetica
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // se for igual, nao insere duplicata
    
    return raiz;
}

// funcao pra exibir pistas em ordem alfabetica
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// funcao pra explorar as salas com coleta de pistas
PistaNode* explorarSalasComPistas(Sala *salaAtual, PistaNode *pistasColetadas) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\n=== DETECTIVE QUEST ===\n");
        printf("Voce esta em: %s\n", salaAtual->nome);
        
        // verifica se tem pista na sala
        if (strlen(salaAtual->pista) > 0) {
            printf("\n*** PISTA ENCONTRADA! ***\n");
            printf("Pista: %s\n", salaAtual->pista);
            pistasColetadas = inserirPista(pistasColetadas, salaAtual->pista);
            printf("Pista adicionada ao seu caderno!\n");
        }

        //mostra as opcoes
        printf("\nOpcoes disponiveis:\n");
        if (salaAtual->esquerda != NULL) {
            printf("(e) - Ir pra esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf("(d) - Ir pra direita\n");
        }
        printf("(s) - Sair\n");

        printf("\nEscolha: ");
        scanf(" %c", &opcao);

        //processa escolha
        switch (opcao) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda;
                    printf("\nVoce foi pra esquerda...\n");
                } else {
                    printf("\nNao da pra ir pra esquerda!\n");
                }
                break;

            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                    printf("\nVoce foi pra direita...\n");
                } else {
                    printf("\nNao da pra ir pra direita!\n");
                }
                break;

            case 's':
            case 'S':
                printf("\nFinalizando exploracao...\n");
                return pistasColetadas;

            default:
                printf("\nOpcao invalida! Tenta de novo.\n");
        }
    }
    return pistasColetadas;
}

int main() {
    printf("Bem vindo ao Detective Quest!\n");
    printf("Voce vai explorar uma mansao e coletar pistas...\n");
    
    PistaNode *pistasColetadas = NULL;  // arvore de pistas

    // montando a arvore da mansao com pistas
    Sala *hall = criarSala("Hall de Entrada", NULL);

    //segundo nivel
    hall->esquerda = criarSala("Biblioteca", "Livro com pagina marcada");
    hall->direita = criarSala("Sala de Estar", "Cinzas na lareira ainda quentes");

    // terceiro nivel - biblioteca
    hall->esquerda->esquerda = criarSala("Escritorio do Mordomo", "Carta rasgada no lixo");
    hall->esquerda->direita = criarSala("Sala de Leitura", NULL);

    //terceiro nivel - sala de estar
    hall->direita->esquerda = criarSala("Cozinha", "Faca suja de sangue");
    hall->direita->direita = criarSala("Conservatorio", "Vaso quebrado no chao");

    //quarto nivel
    hall->direita->esquerda->esquerda = criarSala("Despensa", NULL);
    hall->direita->esquerda->direita = criarSala("Sala de Jantar", "Mesa posta para tres pessoas");

    hall->direita->direita->direita = criarSala("Jardim de Inverno", "Pegadas na terra");

    // ultimo nivel
    hall->esquerda->esquerda->esquerda = criarSala("Cofre Secreto", "Documento falso escondido");
    hall->direita->direita->direita->direita = criarSala("Estufa Abandonada", "Veneno entre as plantas");

    printf("\nMapa carregado com pistas espalhadas!\n");
    printf("Comecando investigacao no Hall de Entrada.\n");

    //comeca exploracao com coleta de pistas
    pistasColetadas = explorarSalasComPistas(hall, pistasColetadas);
    
    // exibe todas as pistas coletadas em ordem alfabetica
    printf("\n=== RELATORIO FINAL ===\n");
    printf("Pistas coletadas durante a investigacao:\n");
    if (pistasColetadas != NULL) {
        exibirPistas(pistasColetadas);
    } else {
        printf("Nenhuma pista foi coletada.\n");
    }
    printf("\nInvestigacao concluida!\n");

    //TODO: fazer funcao pra liberar memoria depois

    return 0;
}