#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estrutura pra cada comodo da mansao
typedef struct Sala {
    char nome[50];          
    struct Sala *esquerda;   
    struct Sala *direita;    
} Sala;

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

// funcao pra explorar as salas - aqui que o jogo acontece
void explorarSalas(Sala *salaAtual) {
    char opcao;
    
    while (salaAtual != NULL) {
        printf("\n=== DETECTIVE QUEST ===\n");
        printf("Voce esta em: %s\n", salaAtual->nome);
        
        // verifica se eh sala final 
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\nVoce chegou no fim desta parte da mansao!\n");
            printf("Nao tem mais caminhos aqui.\n");
            printf("Talvez seja hora de investigar mais...\n");
            break;
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
                printf("\nValeu por jogar!\n");
                return;
                
            default:
                printf("\nOpcao invalida! Tenta de novo.\n");
        }
    }
}

int main() {
    printf("Bem vindo ao Detective Quest!\n");
    printf("Voce vai explorar uma mansao cheia de misterios...\n");
    
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
    
    printf("\nMapa carregado!\n");
    printf("Comecando no Hall de Entrada.\n");
    
    //comeca exploracao
    explorarSalas(hall);
    
    //TODO: fazer funcao pra liberar memoria depois
    
    return 0;
}