#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição do Produto
typedef struct {
    int id;
    char nome[50];
    float preco;
    int quantidade;
    int ativo; // 1 = Ativo, 0 = Excluído
} Produto;

// Variáveis Globais
Produto estoque[100];
int totalProduto = 0;
int proximoId = 1; // Gerador de IDs (Auto-Incremento)

// Protótipos das Funções
void menu();
void cadastrarProduto();
void listarProduto();
void salvarDados();
void carregarDados();
void excluirProduto();
void editarProduto(); 

// Função Principal
int main() {
    carregarDados(); // Carrega os dados ao iniciar
    menu();
    return 0;
}

// Menu Principal
void menu() {
    int opcao;
    do {
        printf("\n==== CONTROLE DE ESTOQUE COMPLETO ====\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Salvar Dados\n");
        printf("4. Excluir Produto\n");
        printf("5. Editar Produto\n"); 
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // Limpa buffer

        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: listarProduto(); break;
            case 3: salvarDados(); break;
            case 4: excluirProduto(); break;
            case 5: editarProduto(); break; 
            case 0: printf("\nSaindo...\n"); break;
            default: printf("\nOpcao invalida!\n"); break;
        }
    } while (opcao != 0);
}

// 1. CADASTRAR (CREATE)
void cadastrarProduto() {
    if (totalProduto >= 100) {
        printf("Estoque cheio!\n");
        return;
    }

    estoque[totalProduto].id = proximoId;
    proximoId++; // Garante que o ID nunca se repita

    printf("Digite o nome do produto: ");
    scanf(" %[^\n]", estoque[totalProduto].nome); // O espaço antes do % previne erros de leitura

    printf("Digite o preco: ");
    scanf("%f", &estoque[totalProduto].preco);

    printf("Digite a quantidade: ");
    scanf("%d", &estoque[totalProduto].quantidade);

    estoque[totalProduto].ativo = 1;
    totalProduto++;

    printf("\nProduto cadastrado! (ID: %d)\n", estoque[totalProduto-1].id);
}

// 2. LISTAR (READ)
void listarProduto() {
    if (totalProduto == 0) {
        printf("\nEstoque vazio!\n");
        return;
    }
    
    printf("\n--- Lista de Produtos ---\n");
    printf("ID   | Nome                 | Preco      | Qtd\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < totalProduto; i++) {
        if (estoque[i].ativo == 1) {
            printf("%-4d | %-20s | R$ %-7.2f | %d\n",
                estoque[i].id,
                estoque[i].nome,
                estoque[i].preco,
                estoque[i].quantidade
            );
        }
    }
}

// 3. SALVAR (PERSISTÊNCIA)
void salvarDados() {
    FILE *arquivo = fopen("estoque.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    for (int i = 0; i < totalProduto; i++) {
        if (estoque[i].ativo == 1) {
            fprintf(arquivo, "%d;%s;%.2f;%d\n",
                estoque[i].id,
                estoque[i].nome,
                estoque[i].preco,
                estoque[i].quantidade
            );
        }
    }
    fclose(arquivo);
    printf("Dados salvos com sucesso!\n");
}

// 4. EXCLUIR (DELETE)
void excluirProduto() {
    int idBusca;
    int encontrou = 0;

    printf("\n--- Exclusao de Produto ---\n");
    printf("Digite o ID para excluir: ");
    scanf("%d", &idBusca);

    for (int i = 0; i < totalProduto; i++) {
        if (estoque[i].id == idBusca && estoque[i].ativo == 1) {
            // "Excluímos" logicamente movendo a fila, mas mantendo os IDs originais
            for (int j = i; j < totalProduto - 1; j++) {
                estoque[j] = estoque[j+1];
            }
            totalProduto--;
            encontrou = 1;
            printf("Produto ID %d excluido com sucesso!\n", idBusca);
            break;
        }
    }
    if (!encontrou) printf("ID nao encontrado!\n");
}

// 5. EDITAR (UPDATE) - A NOVIDADE
void editarProduto() {
    int idBusca;
    int encontrou = 0;

    printf("\n--- Editar Produto ---\n");
    printf("Digite o ID do produto para editar: ");
    scanf("%d", &idBusca);

    for (int i = 0; i < totalProduto; i++) {
        if (estoque[i].id == idBusca && estoque[i].ativo == 1) {
            
            // Mostra os dados atuais para o usuário saber o que está mudando
            printf("\n--- Dados Atuais ---\n");
            printf("Nome: %s\n", estoque[i].nome);
            printf("Preco: R$ %.2f\n", estoque[i].preco);
            printf("Qtd: %d\n", estoque[i].quantidade);
            printf("--------------------\n");

            // Atualiza os dados
            printf("Digite o NOVO nome: ");
            scanf(" %[^\n]", estoque[i].nome);

            printf("Digite o NOVO preco: ");
            scanf("%f", &estoque[i].preco);

            printf("Digite a NOVA quantidade: ");
            scanf("%d", &estoque[i].quantidade);

            printf("\nProduto atualizado com sucesso!\n");
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) printf("ID nao encontrado!\n");
}

// CARREGAR (LOAD)
void carregarDados() {
    FILE *arquivo = fopen("estoque.txt", "r");
    if (arquivo == NULL) {
        printf("\n(Sistema iniciado. Nenhum dado anterior encontrado.)\n");
        return;
    }

    while (fscanf(arquivo, "%d;%[^;];%f;%d\n", 
            &estoque[totalProduto].id, 
            estoque[totalProduto].nome, 
            &estoque[totalProduto].preco, 
            &estoque[totalProduto].quantidade) == 4) {
        
        estoque[totalProduto].ativo = 1;
        totalProduto++;
    }
    fclose(arquivo);

    // Configura o próximo ID baseado no maior ID existente
    int maiorId = 0;
    for (int i = 0; i < totalProduto; i++) {
        if (estoque[i].id > maiorId) {
            maiorId = estoque[i].id;
        }
    }
    proximoId = maiorId + 1;

    printf("\nDados carregados! (%d produtos)\n", totalProduto);
}