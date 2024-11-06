#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
} Fruta;

typedef struct Nodo {
    Fruta fruta;
    struct Nodo* proximo;
} Nodo;

Nodo* criarNodo(Fruta fruta) {
    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));
    if (novo == NULL) {
        printf("Erro ao alocar memória para nova fruta.\n");
        exit(1);
    }
    novo->fruta = fruta;
    novo->proximo = NULL;
    return novo;
}

Nodo* cadastrarFruta(Nodo* lista, Fruta fruta) {
    printf("Cadastrando fruta: %s\n", fruta.nome);
    Nodo* novo = criarNodo(fruta);
    novo->proximo = lista;
    return novo;
}

void listarFrutas(Nodo* lista) {
    printf("Listando frutas...\n");
    Nodo* atual = lista;
    while (atual != NULL) {
        printf("Código: %d\nNome: %s\nQuantidade: %d\nPreço: %.2f\n", 
               atual->fruta.codigo, atual->fruta.nome, atual->fruta.quantidade, atual->fruta.preco);
        atual = atual->proximo;
    }
}

Fruta* buscarFruta(Nodo* lista, int codigo) {
    printf("Buscando fruta com código: %d\n", codigo);
    Nodo* atual = lista;
    while (atual != NULL) {
        if (atual->fruta.codigo == codigo) {
            return &atual->fruta;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void alterarFruta(Fruta* fruta, char* nome, int quantidade, float preco) {
    printf("Alterando fruta: %s\n", fruta->nome);
    strcpy(fruta->nome, nome);
    fruta->quantidade = quantidade;
    fruta->preco = preco;
}

Nodo* excluirFruta(Nodo* lista, int codigo) {
    printf("Excluindo fruta com código: %d\n", codigo);
    Nodo* atual = lista;
    Nodo* anterior = NULL;
    while (atual != NULL) {
        if (atual->fruta.codigo == codigo && atual->fruta.quantidade == 0) {
            if (anterior == NULL) {
                Nodo* temp = atual;
                lista = lista->proximo;
                free(temp);
            } else {
                anterior->proximo = atual->proximo;
                free(atual);
            }
            return lista;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return lista;
}

void venderFruta(Nodo* lista, int codigo, int quantidade) {
    printf("Vendendo fruta com código: %d\n", codigo);
    Fruta* fruta = buscarFruta(lista, codigo);
    if (fruta != NULL && fruta->quantidade >= quantidade) {
        fruta->quantidade -= quantidade;
        FILE* arquivo = fopen("vendas.txt", "a");
        if (arquivo != NULL) {
            fprintf(arquivo, "Código: %d, Quantidade Vendida: %d, Preço Total: %.2f\n",
                    fruta->codigo, quantidade, fruta->preco * quantidade);
            fclose(arquivo);
        }
    } else {
        printf("Erro na venda: Fruta não encontrada ou quantidade insuficiente.\n");
    }
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main() {
    Nodo* lista = NULL;
    int opcao;

    do {
        printf("Menu:\n");
        printf("1. Cadastrar fruta\n");
        printf("2. Listar frutas\n");
        printf("3. Buscar fruta\n");
        printf("4. Alterar fruta\n");
        printf("5. Excluir fruta\n");
        printf("6. Vender fruta\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida para opção.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 1) {
            Fruta novaFruta;
            printf("Digite o código da fruta: ");
            if (scanf("%d", &novaFruta.codigo) != 1) {
                printf("Entrada inválida para código.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();
            printf("Digite o nome da fruta: ");
            if (fgets(novaFruta.nome, sizeof(novaFruta.nome), stdin) == NULL) {
                printf("Erro ao ler o nome da fruta.\n");
                continue;
            }
            novaFruta.nome[strcspn(novaFruta.nome, "\n")] = 0;
            printf("Digite a quantidade: ");
            if (scanf("%d", &novaFruta.quantidade) != 1) {
                printf("Entrada inválida para quantidade.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();
            printf("Digite o preço: ");
            if (scanf("%f", &novaFruta.preco) != 1) {
                printf("Entrada inválida para preço.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();

            if (buscarFruta(lista, novaFruta.codigo) == NULL) {
                lista = cadastrarFruta(lista, novaFruta);
                printf("Fruta cadastrada com sucesso!\n");
            } else {
                printf("Código já cadastrado.\n");
            }

        } else if (opcao == 2) {
            listarFrutas(lista);

        } else if (opcao == 3) {
            int codigo;
            printf("Digite o código da fruta a buscar: ");
            if (scanf("%d", &codigo) != 1) {
                printf("Entrada inválida para código.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();

            Fruta* fruta = buscarFruta(lista, codigo);
            if (fruta != NULL) {
                printf("Código: %d\nNome: %s\nQuantidade: %d\nPreço: %.2f\n", 
                       fruta->codigo, fruta->nome, fruta->quantidade, fruta->preco);
            } else {
                printf("Fruta não encontrada.\n");
            }

        } else if (opcao == 4) {
            int codigo;
            printf("Digite o código da fruta a alterar: ");
            if (scanf("%d", &codigo) != 1) {
                printf("Entrada inválida para código.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();

            Fruta* fruta = buscarFruta(lista, codigo);
            if (fruta != NULL) {
                char nome[50];
                int quantidade;
                float preco;
                printf("Digite o novo nome: ");
                if (fgets(nome, sizeof(nome), stdin) == NULL) {
                    printf("Erro ao ler o nome.\n");
                    continue;
                }
                nome[strcspn(nome, "\n")] = 0;
                printf("Digite a nova quantidade: ");
                if (scanf("%d", &quantidade) != 1) {
                    printf("Entrada inválida para quantidade.\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                printf("Digite o novo preço: ");
                if (scanf("%f", &preco) != 1) {
                    printf("Entrada inválida para preço.\n");
                    limparBuffer();
                    continue;
                }
                limparBuffer();
                alterarFruta(fruta, nome, quantidade, preco);
                printf("Fruta alterada com sucesso!\n");
            } else {
                printf("Fruta não encontrada.\n");
            }

        } else if (opcao == 5) {
            int codigo;
            printf("Digite o código da fruta a excluir: ");
            if (scanf("%d", &codigo) != 1) {
                printf("Entrada inválida para código.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();

            lista = excluirFruta(lista, codigo);
            printf("Fruta excluída com sucesso!\n");

        } else if (opcao == 6) {
            int codigo, quantidade;
            printf("Digite o código da fruta a vender: ");
            if (scanf("%d", &codigo) != 1) {
                printf("Entrada inválida para código.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();
            printf("Digite a quantidade a vender: ");
            if (scanf("%d", &quantidade) != 1) {
                printf("Entrada inválida para quantidade.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();

            venderFruta(lista, codigo, quantidade);
            printf("Venda realizada com sucesso!\n");
        }

    } while(opcao != 7);

    return 0;
}
