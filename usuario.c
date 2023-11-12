#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
struct pedido
{
    char PesquisaNome[50];
    int QuantidadePedido;
};

struct produto
{
    char Nome[50];
    float preco;
    int quantidade;
};

struct prod
{
    char Nome[50];
    float Preco;
    int QuantidadeEstoque;
    int QuantidadePedido;
};

// Função para atualizar 
void AtualizarEstoque(struct prod *p)
{
    FILE *arquivo = fopen("estoque.txt", "r");
    FILE *temporario = fopen("estoque_temp.txt", "w");

    if (arquivo == NULL || temporario == NULL)
    {
        printf("Erro ao abrir os arquivos de estoque para atualização!\n");
        return;
    }

    char linha[100];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if (strstr(linha, p->Nome) != NULL)
        {
            encontrado = 1;
            char temp[50];
            float preco;
            int quantidade;
            
            

            sscanf(linha, "Nome do Produto: %49[^,], Preço: %f, Quantidade Disponível: %d", temp, &preco, &quantidade);

            if (p->QuantidadePedido <= quantidade)
            {
                quantidade -= p->QuantidadePedido;
                p->QuantidadeEstoque = quantidade;
            }
            else
            {
                printf("Quantidade insuficiente em estoque!\n");
            }

            fprintf(temporario, "Nome do Produto: %s, Preço: %.2f, Quantidade Disponível: %d\n", p->Nome, preco, quantidade);
        }
        else
        {
            fprintf(temporario, "%s", linha);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    remove("estoque.txt");
    rename("estoque_temp.txt", "estoque.txt");

    if (encontrado)
    {
        printf("Estoque atualizado com sucesso!\n");
    }
    else
    {
        printf("Produto não encontrado no estoque!\n");
    }
}

// Funçao que salva o pedido
void SalvarPedido(struct prod *p)
{
    FILE *pedido = fopen("pedido.txt", "a");

    if (pedido == NULL)
    {
        printf("Erro ao abrir o arquivo de pedidos para atualização!\n");
        return;
    }

    fprintf(pedido, "Nome do Produto: %s, Quantidade Pedido: %d\n", p->Nome, p->QuantidadePedido);
    fclose(pedido);
    printf("Pedido registrado com sucesso!\n");
}

// função que chama as outras funçoes e faz o pedido
void FazerPedido()
{
    struct prod p;

    printf("Informe o nome do produto:");
    scanf("%s", p.Nome);
    printf("Informe a quantidade desejada:");
    scanf("%d", &p.QuantidadePedido);

    AtualizarEstoque(&p);
    SalvarPedido(&p);
}

// função que exibir pedidos de todos os usuarios
void ExibirPedidosUsuarios()
{
    FILE *arquivo;
    struct pedido p;

    arquivo = fopen("pedido.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Lista de Pedidos Feitos por Usuarios Comuns:\n");

    while (fscanf(arquivo, "Nome do Produto: %49[^,], Quantidade Pedido: %d\n", p.PesquisaNome, &p.QuantidadePedido) != EOF)
    {
        printf("Nome do Produto: %s\n", p.PesquisaNome);
        printf("Quantidade: %d\n\n", p.QuantidadePedido);
    }

    fclose(arquivo);
}

// função que exibe a lista de produtos no estoque
void ExibirListaProdutos()
{
    FILE *arquivo;
    struct produto prod;

    arquivo = fopen("estoque.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Lista de Produtos Disponiveis:\n");

    while (fscanf(arquivo, "Nome do Produto: %49[^,], Preço: %f, Quantidade Disponível: %d\n", prod.Nome, &prod.preco, &prod.quantidade) != EOF)
    {
        printf("Nome: %s\n", prod.Nome);
        printf("Preço: %.2f\n", prod.preco);
        printf("Quantidade Disponível: %d", prod.quantidade);
    }

    fclose(arquivo);
}
