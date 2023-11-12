#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgc.h"

void menu() {
    int usuario;

    while (1) {
        printf("\n=============BEM-VINDO AO SISTEMA COMERCIAL=============\n\n");
        printf("\n----------Escolha uma ação no Sistema Comercial----------\n\n");

        printf("Qual seu nível de usuário?\n\n");
        printf("1-Comum\n");
        printf("2-Administrador\n");
        printf("3-Sair do Sistema\n");

        scanf("%d", &usuario);

        switch (usuario) {
            case 1:
                printf(" 1- Faca o pedido de algum produto.\n 2-  Mostrar a lista de produtos disponíveis no estoque.\n 3- Exibir a lista de pedidos feitos pelos usuários\n");
                int escolha1;
                scanf("%d", &escolha1);

                switch (escolha1) {
                    case 1:
                        FazerPedido();
                        break;
                    case 2:
                        ExibirListaProdutos();
                        break;
                    case 3:
                        ExibirPedidosUsuarios();
                        break;
                    default:
                        break;
                }

                break;

            case 2:
                printf(" 1- Adicionar produtos novos ao estoque.\n 2- Exibir a lista de produtos disponíveis no estoque.\n 3- Adicionar novos usuários ao sistema.\n 4- Excluir usuário do sistema.\n 5- Faca pedidos de produtos.\n 6- Cancele o pedido de produtos.\n 7- Exibir lista de pedidos feitas por usuário.\n");
                int escolha2;
                scanf("%d", &escolha2);
                switch (escolha2) {
                    case 1:
                        AdicaoProduto();
                        break;
                    case 2:
                        ExibirListaProdutos();
                        break;
                    case 3:
                        AdicionarUsuario();
                        break;
                    case 4:
                        ExcluirUsuario();
                        break;
                    case 5:
                        FazerPedido();
                        break;
                    case 6:
                        ExcluirPedidoComunicacao();
                        break;
                    case 7:
                        ExibirPedidosUsuarios();
                        break;
                    default:
                        break;
                }
                break;

            case 3:
                printf("Saída do Sistema.\n");
                exit(0);

            default:
                break;
        }
    }
}

int main() {
    // Inicializa o menu
    menu();

    return 0;
}
