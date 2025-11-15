#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------
// Estruturas de dados principais
// ------------------------------

// Sala da mansão (árvore binária)
typedef struct Sala {
    char nome[60];           // nome do cômodo
    char pista[120];         // pista associada (pode ser vazia)
    int pistaColetada;       // 0 = ainda não adicionada na BST, 1 = já coletada
    struct Sala* esquerda;   // caminho à esquerda
    struct Sala* direita;    // caminho à direita
} Sala;

// Nó da árvore de busca binária de pistas (BST)
typedef struct PistaNode {
    char pista[120];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Nó da tabela hash (lista encadeada para tratar colisões)
typedef struct HashNode {
    char pista[120];
    char suspeito[60];
    struct HashNode* prox;
} HashNode;

#define TAM_HASH 11  // tamanho da tabela hash

// ------------------------------
// Variáveis globais
// ------------------------------
PistaNode* raizPistas = NULL;          // raiz da BST de pistas
HashNode* tabelaHash[TAM_HASH] = {0};  // tabela hash (inicialmente toda NULL)

// ------------------------------
// Funções auxiliares de string
// ------------------------------
void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Lê uma linha (sem quebrar o programa por causa do \n)
void lerLinha(char* destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        }
    }
}

// ------------------------------
// Criação das estruturas
// ------------------------------

// Cria dinamicamente uma sala com nome e pista
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("[ERRO] Falha ao alocar memoria para sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0'; // vazio = sem pista
    nova->pistaColetada = 0;
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Cria dinamicamente um nó de pista para a BST
PistaNode* criarNoPista(const char* pista) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (!novo) {
        printf("[ERRO] Falha ao alocar memoria para pista.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Cria dinamicamente um nó da hash (pista -> suspeito)
HashNode* criarHashNode(const char* pista, const char* suspeito) {
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    if (!novo) {
        printf("[ERRO] Falha ao alocar memoria para hash.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = NULL;
    return novo;
}

// ------------------------------
// ÁRVORE DE PISTAS (BST)
// ------------------------------

// Insere uma pista na BST (não insere duplicata)
void inserirPista(PistaNode** raiz, const char* pista) {
    if (*raz = = NULL) {
        *raz = criarNoPista(pista);
        return;
    }
    int cmp = strcmp(pista, (*raz)->pista);
    if (cmp < 0) {
        inserirPista(&((*raz)->esquerda), pista);
    } else if (cmp > 0) {
        inserirPista(&((*raz)->direita), pista);
    } else {
        // pista já existe, não faz nada
    }
}

// Exibe pistas em ordem alfabética (in-order)
void exibirPistasEmOrdem(PistaNode* raiz) {
    if (raiz == NULL) return;
    exibirPistasEmOrdem(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistasEmOrdem(raiz->direita);
}

// Libera toda a BST de pistas
void liberarBST(PistaNode* raiz) {
    if (raiz == NULL) return;
    liberarBST(raiz->esquerda);
    liberarBST(raiz->direita);
    free(raiz);
}

// ------------------------------
// TABELA HASH (pista -> suspeito)
// ------------------------------

// Função simples de hash: soma dos caracteres % TAM_HASH
unsigned int hashFunc(const char* str) {
    unsigned int soma = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        soma += (unsigned char)str[i];
    }
    return soma % TAM_HASH;
}

// Insere (pista, suspeito) na hash (encadeamento externo)
void inserirNaHash(const char* pista, const char* suspeito) {
    unsigned int h = hashFunc(pista);
    HashNode* novo = criarHashNode(pista, suspeito);
    // insere no início da lista
    novo->prox = tabelaHash[h];
    tabelaHash[h] = novo;
}

// Busca um suspeito a partir da pista
HashNode* encontrarSuspeito(const char* pista) {
    unsigned int h = hashFunc(pista);
    HashNode* atual = tabelaHash[h];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Libera a tabela hash inteira
void liberarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->prox;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

// Conta quantas pistas na BST apontam para um suspeito específico
void contarPistasPorSuspeito(PistaNode* raiz, const char* suspeito, int* contador) {
    if (raiz == NULL) return;

    contarPistasPorSuspeito(raiz->esquerda, suspeito, contador);

    HashNode* h = encontrarSuspeito(raiz->pista);
    if (h != NULL && strcmp(h->suspeito, suspeito) == 0) {
        (*contador)++;
    }

    contarPistasPorSuspeito(raiz->direita, suspeito, contador);
}

// Fase final: jogador acusa um suspeito e o sistema verifica se há >= 2 pistas
void verificarSuspeitoFinal(PistaNode* raiz) {
    if (raiz == NULL) {
        printf("\nNenhuma pista foi coletada. Voce nao tem base para acusar ninguem!\n");
        return;
    }

    char suspeito[60];
    printf("\n=== FASE FINAL: JULGAMENTO ===\n");
    printf("Pistas coletadas (em ordem alfabetica):\n");
    exibirPistasEmOrdem(raiz);

    printf("\nSuspeitos disponiveis (exemplos): Sr Mostarda, Sra Branca, Profa Violeta\n");
    printf("Digite o nome EXATO do suspeito que deseja acusar: ");
    lerLinha(suspeito, sizeof(suspeito));

    int qtd = 0;
    contarPistasPorSuspeito(raiz, suspeito, &qtd);

    printf("\nQuantidade de pistas que apontam para '%s': %d\n", suspeito, qtd);

    if (qtd >= 2) {
        printf("Veredito: HA pistas suficientes. Sua acusacao e consistente!\n");
    } else if (qtd == 1) {
        printf("Veredito: Ha apenas UMA pista apontando para esse suspeito. Caso fraco.\n");
    } else {
        printf("Veredito: Nenhuma pista collected aponta para esse suspeito. Acusacao infundada!\n");
    }
}

// ------------------------------
// Mansão: árvore binária de salas
// ------------------------------

// Monta um mapa fixo da mansão (árvore binária)
Sala* montarMansao() {
    // Alguns exemplos de salas e pistas
    // (nomes e pistas fixos para o jogo)
    Sala* hall = criarSala("Hall de Entrada",       "Luvas sujas de sangue");
    Sala* salaEstar = criarSala("Sala de Estar",    "Relogio quebrado");
    Sala* biblioteca = criarSala("Biblioteca",      "Livro de venenos");
    Sala* salaJantar = criarSala("Sala de Jantar",  "Taca com residuo de veneno");
    Sala* corredor = criarSala("Corredor",          "Pegadas recentes no tapete");
    Sala* cozinha = criarSala("Cozinha",            "Faca desaparecida");
    Sala* jardim = criarSala("Jardim",              "Pegadas na lama");

    // Montando a árvore:
    //
    //              Hall de Entrada
    //             /                \
    //      Sala de Estar         Corredor
    //       /        \           /      \
    //  Biblioteca  Sala Jantar  Cozinha  Jardim
    //
    hall->esquerda = salaEstar;
    hall->direita  = corredor;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita  = salaJantar;

    corredor->esquerda  = cozinha;
    corredor->direita   = jardim;

    return hall;
}

// Libera a árvore de salas (mansão)
void liberarMansao(Sala* raiz) {
    if (raiz == NULL) return;
    liberarMansao(raiz->esquerda);
    liberarMansao(raiz->direita);
    free(raiz);
}

// ------------------------------
// Exploração da mansão
// ------------------------------

void mostrarOpcoesNavegacao(Sala* atual) {
    printf("\nVoce esta em: %s\n", atual->nome);
    if (atual->pista[0] != '\0' && !atual->pistaColetada) {
        printf("Pista encontrada neste comodo: \"%s\"\n", atual->pista);
    } else if (atual->pista[0] != '\0') {
        printf("Voce ja coletou a pista deste comodo.\n");
    } else {
        printf("Nao ha pista neste comodo.\n");
    }

    printf("Caminhos disponiveis:\n");
    if (atual->esquerda != NULL) {
        printf("- (e) Esquerda -> %s\n", atual->esquerda->nome);
    } else {
        printf("- (e) Esquerda -> [sem caminho]\n");
    }
    if (atual->direita != NULL) {
        printf("- (d) Direita  -> %s\n", atual->direita->nome);
    } else {
        printf("- (d) Direita  -> [sem caminho]\n");
    }
    printf("- (s) Sair da exploracao\n");
    printf("Escolha (e/d/s): ");
}

// Função principal de exploração: navega pela árvore, coleta pistas e preenche a BST
void explorarSalas(Sala* raiz) {
    if (raiz == NULL) {
        printf("[ERRO] Mansao vazia.\n");
        return;
    }

    Sala* atual = raiz;
    char opcao[10];

    printf("=== DETECTIVE QUEST: EXPLORACAO DA MANSAO ===\n");

    int continuar = 1;
    while (continuar) {
        // Ao entrar na sala, se houver pista ainda não coletada, coleta e insere na BST
        if (atual->pista[0] != '\0' && !atual->pistaColetada) {
            printf("\n>> PISTA COLETADA: \"%s\"\n", atual->pista);
            inserirPista(&raizPistas, atual->pista);
            atual->pistaColetada = 1;
        }

        mostrarOpcoesNavegacao(atual);
        lerLinha(opcao, sizeof(opcao));

        if (strcmp(opcao, "e") == 0 || strcmp(opcao, "E") == 0) {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Nao ha sala a esquerda a partir daqui. Tente outro caminho.\n");
            }
        } else if (strcmp(opcao, "d") == 0 || strcmp(opcao, "D") == 0) {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Nao ha sala a direita a partir daqui. Tente outro caminho.\n");
            }
        } else if (strcmp(opcao, "s") == 0 || strcmp(opcao, "S") == 0) {
            printf("\nVoce decidiu encerrar a exploracao da mansao.\n");
            continuar = 0;
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }

    printf("\nExploracao encerrada.\n");
}

// ------------------------------
// Inicialização da tabela hash
// ------------------------------

// Define, no código, quais pistas apontam para quais suspeitos
void montarTabelaHashSuspeitos() {
    // Exemplo de mapeamento:
    // - Sr Mostarda
    // - Sra Branca
    // - Profa Violeta
    inserirNaHash("Luvas sujas de sangue",          "Sr Mostarda");
    inserirNaHash("Relogio quebrado",               "Sra Branca");
    inserirNaHash("Livro de venenos",               "Profa Violeta");
    inserirNaHash("Taca com residuo de veneno",     "Profa Violeta");
    inserirNaHash("Pegadas recentes no tapete",     "Sra Branca");
    inserirNaHash("Faca desaparecida",              "Sr Mostarda");
    inserirNaHash("Pegadas na lama",                "Sra Branca");
}

// ------------------------------
// Função main
// ------------------------------

int main() {
    // Monta o mapa fixo da mansão (árvore binária)
    Sala* mansao = montarMansao();

    // Monta a tabela hash com a associação pista -> suspeito
    montarTabelaHashSuspeitos();

    // Exploração interativa da mansão, coletando pistas na BST
    explorarSalas(mansao);

    // Fase final: julgamento e verificação de suspeito
    verificarSuspeitoFinal(raizPistas);

    // Libera memória das estruturas
    liberarMansao(mansao);
    liberarBST(raizPistas);
    liberarHash();

    printf("\nRecursos liberados. Fim do Detective Quest.\n");
    return 0;
}
