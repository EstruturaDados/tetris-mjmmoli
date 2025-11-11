#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Definição das Estruturas de Dados ---

// Estrutura que representa uma Peça de Tetris Stack
typedef struct {
    int id; // Identificador único da peça
    char nome[2]; // Nome da peça ('I', 'O', 'T', 'L', 'S', 'Z', 'J')
} Peca;

// Estrutura que representa a Fila Circular de Peças
#define CAPACIDADE_FILA 5
typedef struct {
    Peca elementos[CAPACIDADE_FILA];
    int frente; // Índice do primeiro elemento
    int tras;   // Índice do próximo local vazio (ou do último elemento + 1)
    int tamanho; // Número atual de elementos na fila
} FilaCircular;

// --- Protótipos das Funções ---

void inicializarFila(FilaCircular *fila);
void gerarPeca(Peca *novaPeca);
void enfileirar(FilaCircular *fila, Peca novaPeca);
Peca desenfileirar(FilaCircular *fila);
void visualizarFila(const FilaCircular *fila);
void preencherFilaInicial(FilaCircular *fila);
void exibirMenu();

// --- Implementação das Funções ---

/**
 * @brief Inicializa a estrutura da Fila Circular.
 * @param fila Ponteiro para a FilaCircular a ser inicializada.
 */
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
}

/**
 * @brief Gera automaticamente uma nova peça com ID sequencial e nome aleatório.
 * @param novaPeca Ponteiro para a Peca onde os dados serão armazenados.
 */
void gerarPeca(Peca *novaPeca) {
    // static para manter o ID único a cada chamada
    static int proximoId = 1;
    const char *tipos[] = {"I", "O", "T", "L", "J", "S", "Z"};
    int numTipos = sizeof(tipos) / sizeof(tipos[0]);

    // Inicializa o gerador de números aleatórios se não foi feito
    if (proximoId == 1) {
        srand(time(NULL));
    }
    
    // Atribui o ID
    novaPeca->id = proximoId++;
    
    // Escolhe um tipo aleatório
    int indiceTipo = rand() % numTipos;
    strcpy(novaPeca->nome, tipos[indiceTipo]);
}

/**
 * @brief Insere uma nova peça no final (trás) da fila (Enqueue).
 * @param fila Ponteiro para a FilaCircular.
 * @param novaPeca A peça a ser inserida.
 */
void enfileirar(FilaCircular *fila, Peca novaPeca) {
    if (fila->tamanho == CAPACIDADE_FILA) {
        // Segundo as regras do Nível Novato, esta condição não deve ocorrer após 
        // a inicialização, pois enfileirar só é chamado após um desenfileirar.
        printf("🚨 Erro: A fila está cheia. Impossível enfileirar.\n");
        return;
    }
    
    fila->elementos[fila->tras] = novaPeca;
    // O operador % CAPACIDADE_FILA garante o comportamento circular
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA; 
    fila->tamanho++;
}

/**
 * @brief Remove e retorna a peça da frente da fila (Dequeue).
 * @param fila Ponteiro para a FilaCircular.
 * @return A peça removida. Retorna uma peça nula (ID 0) se a fila estiver vazia.
 */
Peca desenfileirar(FilaCircular *fila) {
    if (fila->tamanho == 0) {
        printf("🚨 Erro: A fila está vazia. Impossível desenfileirar.\n");
        Peca nula = {0, ""};
        return nula;
    }
    
    Peca pecaRemovida = fila->elementos[fila->frente];
    // O operador % CAPACIDADE_FILA garante o comportamento circular
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->tamanho--;
    
    return pecaRemovida;
}

/**
 * @brief Exibe o estado atual da fila de peças.
 * @param fila Ponteiro constante para a FilaCircular.
 */
void visualizarFila(const FilaCircular *fila) {
    printf("\n--- 🧩 Fila de Peças Futuras (Tamanho: %d/%d) ---\n", fila->tamanho, CAPACIDADE_FILA);
    if (fila->tamanho == 0) {
        printf("[VAZIA]\n");
        printf("--------------------------------------------------\n");
        return;
    }

    printf("Frente -> ");
    // Percorre a fila a partir da 'frente'
    for (int i = 0; i < fila->tamanho; i++) {
        int indice = (fila->frente + i) % CAPACIDADE_FILA;
        Peca p = fila->elementos[indice];
        printf("| %s (ID:%d) ", p.nome, p.id);
    }
    printf(" <- Fim (Trás)\n");
    printf("--------------------------------------------------\n");
}

/**
 * @brief Preenche a fila inicial com 5 peças geradas automaticamente.
 * @param fila Ponteiro para a FilaCircular.
 */
void preencherFilaInicial(FilaCircular *fila) {
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        Peca novaPeca;
        gerarPeca(&novaPeca);
        enfileirar(fila, novaPeca);
    }
    printf("✅ Fila inicial preenchida com %d peças.\n", CAPACIDADE_FILA);
}

/**
 * @brief Exibe o menu de opções para o usuário.
 */
void exibirMenu() {
    printf("\n--- 🎮 Menu Tetris Stack - Nível Novato ---\n");
    printf("1 - Jogar a peça da frente (Dequeue + Enqueue automático)\n");
    printf("2 - Visualizar Fila\n");
    printf("0 - Sair\n");
    printf("--------------------------------------------------\n");
    printf("Escolha uma opção: ");
}

// --- Função Principal ---

int main() {
    FilaCircular fila;
    int opcao;

    inicializarFila(&fila);
    preencherFilaInicial(&fila);
    visualizarFila(&fila);

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            // Lidar com entrada inválida (não numérica)
            while (getchar() != '\n');
            opcao = -1; // Garante que o loop continue ou saia com a mensagem
            continue;
        }

        switch (opcao) {
            case 1: {
                // 1. JOGAR (REMOVER) A PEÇA DA FRENTE (Dequeue)
                if (fila.tamanho > 0) {
                    Peca pecaJogada = desenfileirar(&fila);
                    printf("➡️ Peça jogada: **%s (ID:%d)**. ", pecaJogada.nome, pecaJogada.id);

                    // 2. INSERIR AUTOMATICAMENTE UMA NOVA PEÇA (Enqueue)
                    Peca novaPeca;
                    gerarPeca(&novaPeca);
                    enfileirar(&fila, novaPeca);
                    printf("Nova peça **%s (ID:%d)** adicionada ao final da fila.\n", novaPeca.nome, novaPeca.id);
                } else {
                    printf("Ação não realizada. A fila está vazia (erro na lógica).\n");
                }
                visualizarFila(&fila);
                break;
            }
            case 2:
                // 3. VISUALIZAR A FILA ATUAL
                visualizarFila(&fila);
                break;
            case 0:
                printf("\n👋 Saindo do Tetris Stack. Obrigado por jogar!\n");
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}