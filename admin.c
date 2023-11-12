#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar informações sobre um produto
struct produto
{
    char Nome[50];
    float preco;
    int quantidade;
};

// Estrutura para armazenar informações sobre um usuário
struct usuario
{
    char nome[50];
    char senha[50];
    char nivelPrivilegio[5];
};

// Função para verificar a senha e privilégio de um usuário a partir de um arquivo de senhas
int verificaSenhaEPrivilegio(const char *senha)
{
    FILE *arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de senhas!\n");
        return 0;
    }

    char linha[100];

    // Verifica se a senha e o nível de privilégio correspondem a um usuário administrador
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if (strstr(linha, senha) != NULL && strstr(linha, "adm") != NULL)
        {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);

    return 0;
}

// Função para adicionar um produto ao estoque
void AdicaoProduto()
{
    FILE *arquivo;
    struct produto prod;

    char senha[50];
    printf("Informe sua senha:");
    scanf("%s", senha);

    // Verifica se a senha é válida para um usuário administrador
    if (verificaSenhaEPrivilegio(senha))
    {
        printf("Informe o nome do produto:\n");
        scanf("%s", prod.Nome);
        printf("Informe o preco do produto:\n");
        scanf("%f", &prod.preco);
        printf("Informe o estoque do produto:\n");
        scanf("%d", &prod.quantidade);

        arquivo = fopen("estoque.txt", "a");

        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo!\n");
            return;
        }

        // Registra as informações do produto no arquivo de estoque
        fprintf(arquivo, "Nome do Produto: %s, Preço: %.2f, Quantidade Disponível: %d\n", prod.Nome, prod.preco, prod.quantidade);

        printf("Produto registrado com sucesso!\n");

        fclose(arquivo);
    }
    else
    {
        printf("A senha informada está errada ou não é de um usuário administrador.");
    }
    return;
}

// Função para adicionar um usuário ao sistema
void AdicionarUsuario()
{
    FILE *arquivo;
    struct usuario user;

    char senha[50];
    printf("Informe sua senha:");
    scanf("%s", senha);
    if (verificaSenhaEPrivilegio(senha))
    {
        printf("Informe o nome do novo usuário:\n");
        scanf("%s", user.nome);
        printf("Informe o nível de privilégio:\n");
        scanf("%s", user.nivelPrivilegio);
        printf("Informe a senha para este usuário (A senha deve conter 8 caracteres):\n");

        // Loop para garantir que a senha tenha 7 caracteres
        while (strlen(user.senha) != 7)
        {
            scanf("%s", user.senha);
            if (strlen(user.senha) != 7)
            {
                printf("Senha informada é muito pequena ou muito grande. Informe outra senha:\n");
            }
        }

        arquivo = fopen("usuarios.txt", "a");

        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo!\n");
        }
        else
        {
            // Registra as informações do usuário no arquivo de usuários
            fprintf(arquivo, "Nome do Usuario: %s, Senha: %s, Nivel de Acesso: %s\n", user.nome, user.senha, user.nivelPrivilegio);
            printf("Usuário registrado com sucesso!\n");
            fclose(arquivo);
        }
    }
    else
    {
        printf("A senha informada está incorreta ou não é de um administrador.\n");
    }
}

// Função para excluir um usuário do sistema
void ExcluirUsuario()
{
    char senha[50];
    printf("Informe sua senha:");
    scanf("%s", senha);
    if (verificaSenhaEPrivilegio(senha))
    {
        char nome[50];
        printf("Informe o nome do usuário a ser excluído:\n");
        scanf("%s", nome);

        FILE *arquivo = fopen("usuarios.txt", "r");

        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo de usuários!\n");
            return;
        }

        FILE *temp = fopen("temp.txt", "w");

        if (temp == NULL)
        {
            printf("Erro ao criar arquivo temporário!\n");
            fclose(arquivo);
            return;
        }

        char linha[100];
        int usuarioExcluido = 0;

        // Copia os usuários para um arquivo temporário, excluindo o usuário especificado
        while (fgets(linha, sizeof(linha), arquivo) != NULL)
        {
            if (strstr(linha, nome) != NULL)
            {
                usuarioExcluido = 1;
                continue;
            }

            fprintf(temp, "%s", linha);
        }

        fclose(arquivo);
        fclose(temp);

        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");

        if (usuarioExcluido)
        {
            printf("Usuário '%s' foi excluído com sucesso.\n", nome);
        }
        else
        {
            printf("Usuário '%s' não encontrado para exclusão.\n", nome);
        }
    }
    else
    {
        printf("Senha incorreta ou não cadastrada como administrador.");
    }
}

// Estrutura para armazenar informações sobre um produto (utilizada em AtualizarEstoqueAdd)
struct prod
{
    char Nome[50];
    float Preco;
    int QuantidadeEstoque;
    int QuantidadePedido;
};

// Função para atualizar o estoque, adicionando produtos
void AtualizarEstoqueAdd(struct prod *p)
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

    // Atualiza a quantidade disponível de um produto no estoque
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if (strstr(linha, p->Nome) != NULL)
        {
            encontrado = 1;
            char temp[50];
            float preco;
            int quantidade;

            sscanf(linha, "Nome do Produto: %49[^,], Preço: %f, Quantidade Disponível: %d", temp, &preco, &quantidade);

            // Verifica se há quantidade suficiente no estoque para atender o pedido
            if (quantidade >= p->QuantidadePedido)
            {
                quantidade += p->QuantidadePedido; // Soma a quantidade do pedido ao estoque
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

// Estrutura para armazenar informações sobre um pedido
struct Pedido
{
    char NomeProduto[50];
    int QuantidadePedido;
};

// Função para excluir um pedido do sistema
int ExcluirPedido(const char *nomeProduto, int quantidadePedido)
{
    FILE *pedidoFile = fopen("pedido.txt", "r");
    FILE *tempFile = fopen("temp_pedidos.txt", "w");

    if (pedidoFile == NULL || tempFile == NULL)
    {
        printf("Erro ao abrir os arquivos de pedidos para atualização!\n");
        return 0;
    }

    char linha[100];
    int encontrado = 0;

    // Exclui um pedido do arquivo de pedidos
    while (fgets(linha, sizeof(linha), pedidoFile) != NULL)
    {
        char nome[50];
        int quantidade;

        if (sscanf(linha, "Produto: %49[^,], Quantidade: %d", nome, &quantidade) == 2)
        {
            if (strcmp(nome, nomeProduto) == 0 && quantidade == quantidadePedido)
            {
                encontrado = 1;
            }
            else
            {
                fprintf(tempFile, "%s", linha);
            }
        }
    }

    fclose(pedidoFile);
    fclose(tempFile);

    remove("pedido.txt");
    rename("temp_pedidos.txt", "pedido.txt");

    return encontrado;
}

// Função que chama as outras funções e exclui um pedido
void ExcluirPedidoComunicacao()
{
    struct prod p;

    char senha[50];
    printf("Informe sua senha:");
    scanf("%s", senha);

    if (verificaSenhaEPrivilegio(senha))
    {
        printf("Informe o nome do produto:");
        scanf("%s", p.Nome);
        printf("Informe a quantidade desejada:");
        scanf("%d", &p.QuantidadePedido);

        int controle = ExcluirPedido(p.Nome, p.QuantidadePedido);

        if (controle == 0)
        {
            AtualizarEstoqueAdd(&p);
        }
        else
        {
            printf("Pedido não encontrado ou quantidade não corresponde a um pedido existente.\n");
        }
    }
}
