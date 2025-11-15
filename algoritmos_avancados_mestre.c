#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 17  // numero primo pra hash funcionar melhor

//estrutura pra cada comodo da mansao
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// estrutura pra arvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// estrutura pra tabela hash - pista -> suspeito
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

// tabela hash global
HashNode *tabelaHash[TAMANHO_HASH];

/*
funcao pra criar uma sala nova
*/
Sala* criarSala(char *nome) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro: nao conseguiu alocar memoria!\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// funcao hash simples
int funcaoHash(char *chave) {
    int hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash += chave[i];
    }
    return hash % TAMANHO_HASH;
}

// funcao pra inserir na tabela hash
void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);

    HashNode *novo = (HashNode*)malloc(sizeof(HashNode));
    if (novo == NULL) return;

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// funcao pra encontrar suspeito pela pista
char* encontrarSuspeito(char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;  // nao encontrou
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
        char *suspeito = encontrarSuspeito(raiz->pista);
        printf("- %s", raiz->pista);
        if (suspeito != NULL) {
            printf(" (suspeito: %s)", suspeito);
        }
        printf("\n");
        exibirPistas(raiz->direita);
    }
}

// funcao pra obter pista baseada no nome da sala
char* obterPistaSala(char *nomeSala) {
    // aqui defino as pistas baseado no nome da sala
    if (strcmp(nomeSala, "Biblioteca") == 0) {
        return "Livro sobre venenos aberto";
    } else if (strcmp(nomeSala, "Sala de Estar") == 0) {
        return "Cigarro ainda aceso no cinzeiro";
    } else if (strcmp(nomeSala, "Escritorio do Mordomo") == 0) {
        return "Carta ameacadora no lixo";
    } else if (strcmp(nomeSala, "Cozinha") == 0) {
        return "Faca com sangue escondida";
    } else if (strcmp(nomeSala, "Conservatorio") == 0) {
        return "Vaso quebrado com impressao digital";
    } else if (strcmp(nomeSala, "Sala de Jantar") == 0) {
        return "Copo com residuo de veneno";
    } else if (strcmp(nomeSala, "Jardim de Inverno") == 0) {
        return "Pegadas masculinas na lama";
    } else if (strcmp(nomeSala, "Cofre Secreto") == 0) {
        return "Documento forjado com assinatura";
    } else if (strcmp(nomeSala, "Estufa Abandonada") == 0) {
        return "Frasco vazio de arsenic";
    }
    return NULL;  // sem pista
}

// funcao pra contar pistas de um suspeito
int contarPistasSuspeito(PistaNode *raiz, char *suspeito) {
    if (raiz == NULL) return 0;

    int count = 0;
    char *suspeitoPista = encontrarSuspeito(raiz->pista);
    if (suspeitoPista != NULL && strcmp(suspeitoPista, suspeito) == 0) {
        count = 1;
    }

    return count + contarPistasSuspeito(raiz->esquerda, suspeito) +
                  contarPistasSuspeito(raiz->direita, suspeito);
}

// funcao pra verificar suspeito final
void verificarSuspeitoFinal(PistaNode *pistasColetadas) {
    char acusado[50];

    printf("\n=== HORA DA ACUSACAO ===\n");
    printf("Baseado nas evidencias coletadas, quem voce acusa?\n");
    printf("Digite o nome do suspeito: ");
    scanf(" %49[^\n]", acusado);  // le com espacos

    int numPistas = contarPistasSuspeito(pistasColetadas, acusado);

    printf("\n=== VEREDICTO ===\n");
    printf("Suspeito acusado: %s\n", acusado);
    printf("Numero de pistas contra ele: %d\n", numPistas);

    if (numPistas >= 2) {
        printf("\n*** PARABENS! ***\n");
        printf("Voce conseguiu evidencias suficientes!\n");
        printf("O caso foi resolvido com sucesso!\n");
        printf("%s foi preso e confessou o crime.\n", acusado);
    } else {
        printf("\n*** CASO NAO RESOLVIDO ***\n");
        printf("Evidencias insuficientes contra %s.\n", acusado);
        printf("O suspeito foi liberado por falta de provas.\n");
        printf("O verdadeiro culpado ainda esta solto...\n");
    }
}

// funcao pra explorar as salas com coleta de pistas
PistaNode* explorarSalas(Sala *salaAtual, PistaNode *pistasColetadas) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\n=== DETECTIVE QUEST ===\n");
        printf("Voce esta em: %s\n", salaAtual->nome);

        // verifica se tem pista na sala
        char *pista = obterPistaSala(salaAtual->nome);
        if (pista != NULL) {
            printf("\n*** PISTA ENCONTRADA! ***\n");
            printf("Evidencia: %s\n", pista);
            pistasColetadas = inserirPista(pistasColetadas, pista);

            char *suspeito = encontrarSuspeito(pista);
            if (suspeito != NULL) {
                printf("Esta pista aponta para: %s\n", suspeito);
            }
            printf("Pista adicionada ao caderno de investigacao!\n");
        } else {
            printf("Nenhuma evidencia relevante encontrada aqui.\n");
        }

        //mostra as opcoes
        printf("\nOpcoes disponiveis:\n");
        if (salaAtual->esquerda != NULL) {
            printf("(e) - Ir pra esquerda\n");
        }
        if (salaAtual->direita != NULL) {
            printf("(d) - Ir pra direita\n");
        }
        printf("(s) - Finalizar investigacao\n");

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
                printf("\nFinalizando investigacao...\n");
                return pistasColetadas;

            default:
                printf("\nOpcao invalida! Tenta de novo.\n");
        }
    }
    return pistasColetadas;
}

int main() {
    printf("=== DETECTIVE QUEST - CAPITULO FINAL ===\n");
    printf("Um crime foi cometido na mansao!\n");
    printf("Colete pistas e descubra o culpado...\n");

    // inicializa tabela hash
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }

    // monta as associacoes pista -> suspeito na hash
    inserirNaHash("Livro sobre venenos aberto", "Jose");
    inserirNaHash("Cigarro ainda aceso no cinzeiro", "Joao");
    inserirNaHash("Carta ameacadora no lixo", "Antonio");
    inserirNaHash("Faca com sangue escondida", "Josue");
    inserirNaHash("Vaso quebrado com impressao digital", "Maria");
    inserirNaHash("Copo com residuo de veneno", "Jose");
    inserirNaHash("Pegadas masculinas na lama", "Joao");
    inserirNaHash("Documento forjado com assinatura", "Antonio");
    inserirNaHash("Frasco vazio de arsenic", "Jose");

    PistaNode *pistasColetadas = NULL;  // arvore de pistas

    // montando a arvore da mansao
    Sala *hall = criarSala("Hall de Entrada");

    //segundo nivel
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Sala de Estar");

    // terceiro nivel - biblioteca
    hall->esquerda->esquerda = criarSala("Escritorio do Mordomo");
    hall->esquerda->direita = criarSala("Sala de Leitura");

    //terceiro nivel - sala de estar
    hall->direita->esquerda = criarSala("Cozinha");
    hall->direita->direita = criarSala("Conservatorio");

    //quarto nivel
    hall->direita->esquerda->esquerda = criarSala("Despensa");
    hall->direita->esquerda->direita = criarSala("Sala de Jantar");

    hall->direita->direita->direita = criarSala("Jardim de Inverno");

    // ultimo nivel
    hall->esquerda->esquerda->esquerda = criarSala("Cofre Secreto");
    hall->direita->direita->direita->direita = criarSala("Estufa Abandonada");

    printf("\nCena do crime mapeada!\n");
    printf("Iniciando investigacao no Hall de Entrada.\n");
    printf("\nSUSPEITOS: Jose, Joao, Antonio, Josue e Maria\n");

    //comeca exploracao com coleta de pistas
    pistasColetadas = explorarSalas(hall, pistasColetadas);

    // exibe todas as pistas coletadas
    printf("\n=== RELATORIO DE EVIDENCIAS ===\n");
    printf("Pistas coletadas durante a investigacao:\n");
    if (pistasColetadas != NULL) {
        exibirPistas(pistasColetadas);

        // fase final - acusacao
        verificarSuspeitoFinal(pistasColetadas);
    } else {
        printf("Nenhuma pista foi coletada.\n");
        printf("Impossivel resolver o caso sem evidencias!\n");
    }

    printf("\n=== FIM DE JOGO ===\n");

    return 0;
}