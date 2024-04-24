#include "loja.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  int quantidade_usuarios = 0;
  Item *produtos;
  int quantidade_produtos = 0;

  char nome_loja[100] = "Loja";
  printf("Bem-vindo(a) a %s!\n", nome_loja);

  FILE *usuarios_arquivo = fopen("usuarios.txt", "r");
  if (usuarios_arquivo == NULL) {
    printf("Arquivo de usuários não encontrado. Criando um novo...\n");
    FILE *novo_usuarios_arquivo = fopen("usuarios.txt", "w");
    if (novo_usuarios_arquivo == NULL) {
      printf("Erro ao criar arquivo de usuários.\n");
      return 1;
    }
    fclose(novo_usuarios_arquivo);
  } else {
    fclose(usuarios_arquivo);
  }

  FILE *produtos_arquivo = fopen("produtos.txt", "r");
  if (produtos_arquivo == NULL) {
    printf("Arquivo de produtos não encontrado. Criando um novo...\n");
    FILE *novo_produtos_arquivo = fopen("produtos.txt", "w");
    if (novo_produtos_arquivo == NULL) {
      printf("Erro ao criar arquivo de produtos.\n");
      return 1;
    }
    fclose(novo_produtos_arquivo);
  } else {
    fclose(produtos_arquivo);
  }

  Node *usuarios_lista = carregar_usuarios();
  produtos = carregar_produtos(&quantidade_produtos);

  Usuario usuario_logado;

  int opcao;
  do {
    menu_de_opcoes_inicial();
    scanf("%d", &opcao);
    switch (opcao) {
    case 1:
      cadastrar_usuario(&usuarios_lista);
      quantidade_usuarios++;
      break;
    case 2:
      login(&usuario_logado, usuarios_lista);
      if (usuario_logado.autenticado) {
        carregar_carrinho(&usuario_logado);
        mensagem_de_boas_vindas(&usuario_logado);
        menu_de_opcoes(&usuario_logado);
      }
      break;
    case 3:
      sair(&usuario_logado);
      break;
    default:
      printf("Opção inválida.\n");
    }
  } while (opcao != 3);
  void salvar_usuarios(Node *usuarios);
  salvar_produtos(produtos, quantidade_produtos);

  free(usuarios_lista);
  free(produtos);
  ordenar_usuarios_por_nome();
  Node *inicio_da_lista = carregar_usuarios();
  return 0;
}
