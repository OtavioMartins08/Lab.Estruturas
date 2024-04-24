#include "loja.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *inicio_da_lista;

void cadastrar_usuario(Node **lista) {
    Usuario novo_usuario;
    printf("\nRegistre-se para continuar:\n");
    printf("Digite seu nome: ");
    scanf("%79s", novo_usuario.nome);

    do {
        printf("Digite seu e-mail (exemplo@dominio.com): ");
        scanf("%79s", novo_usuario.email);
        if (strchr(novo_usuario.email, '@') == NULL) {
            printf("E-mail invalido. Por favor, inclua o caractere '@'.\n");
        }
    } while (strchr(novo_usuario.email, '@') == NULL);

    printf("Digite sua senha: ");
    scanf("%79s", novo_usuario.senha);

    printf("Digite seu endereco (ex: Rua Exemplo, 123): ");
    scanf(" %199[^\n]", novo_usuario.endereco);
    printf("Digite sua idade: ");
    scanf("%d", &novo_usuario.idade);
    printf("Digite seu numero de celular (ex: (00) 1234-5678): ");
    scanf(" %14s", novo_usuario.celular);

    if (strcmp(novo_usuario.email, "admin@admin.com") == 0 &&
        strcmp(novo_usuario.senha, "admin") == 0) {
        novo_usuario.is_admin = 1;
    } else {
        novo_usuario.is_admin = 0;
    }

    novo_usuario.quantidade_itens = 0;
    inicializar_carrinho(&novo_usuario.carrinho);
    novo_usuario.autenticado = true;

    Node *novo_no = (Node *)malloc(sizeof(Node));
    if (novo_no == NULL) {
        printf("Erro: falha ao alocar memória para novo usuário.\n");
        return;
    }

    novo_no->usuario = novo_usuario;
    novo_no->prox = NULL;

    if (*lista == NULL) {
        *lista = novo_no;
    } else {
        Node *atual = *lista;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo_no;
    }

    FILE *arquivo = fopen("usuarios.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuarios para escrita.\n");
        return;
    }

    fprintf(arquivo, "%s;%s;%s;%s;%d;%s;%d;%hhd;%d\n", novo_usuario.email, novo_usuario.senha,
            novo_usuario.nome, novo_usuario.endereco, novo_usuario.idade, novo_usuario.celular,
            novo_usuario.is_admin, novo_usuario.quantidade_itens, novo_usuario.autenticado);
    fclose(arquivo);

    printf("Registro bem-sucedido.\n");
    printf("Retornando ao menu inicial...\n");
}

void visualizar_informacoes_usuario(Node *inicio, char *email) {
    Node *atual = inicio;
    while (atual != NULL) {
        if (strcmp(atual->usuario.email, email) == 0) {
            printf("\nInformacoes do usuario:\n");
            printf("Nome: %s\n", atual->usuario.nome);
            printf("Endereco: %s\n", atual->usuario.endereco);
            printf("Idade: %d\n", atual->usuario.idade);
            printf("Celular: %s\n", atual->usuario.celular);
            return;
        }
        atual = atual->prox;
    }
    printf("Usuario nao encontrado.\n");
}

bool verificar_endereco(Usuario *usuario) {
  return strlen(usuario->endereco) > 0;
}

void login(Usuario *usuario, Node *usuarios) {
    char email[80], senha[80];
    printf("\nFaca login:\n");
    printf("Digite seu e-mail: ");
    scanf("%79s", email);
    printf("Digite sua senha: ");
    scanf("%79s", senha);
    int encontrado = 0;

    Node *atual = usuarios;
    while (atual != NULL) {
        if (strcmp(email, atual->usuario.email) == 0 &&
            strcmp(senha, atual->usuario.senha) == 0) {
            if (atual->usuario.is_admin) {
                *usuario = atual->usuario;
                usuario->autenticado = true;
                printf("Login bem-sucedido como administrador.\n");
                encontrado = 1;
                break;
            } else if (verificar_informacoes_completas(&atual->usuario)) {
                *usuario = atual->usuario;
                usuario->autenticado = true;
                printf("Login bem-sucedido.\n");
                encontrado = 1;
                break;
            } else {
                printf("Alguns detalhes do seu perfil estão incompletos. Por favor, "
                       "complete todas as informações para fazer login.\n");
                 visualizar_informacoes_usuario(inicio_da_lista, atual->usuario.email);
                return;
            }
        }
        atual = atual->prox;
    }

    if (!encontrado) {
        printf("Usuário ou senha incorretos.\n");
    }
}

bool verificar_informacoes_completas(Usuario *usuario) {
  return strlen(usuario->nome) > 0 && strlen(usuario->email) > 0 &&
         strlen(usuario->senha) > 0 && strlen(usuario->endereco) > 0 &&
         usuario->idade > 0 && strlen(usuario->celular) > 0;
}

void mensagem_de_boas_vindas(Usuario *usuario) {
  printf("\nBem-vindo(a) %s a nossa loja!\n", usuario->nome);
}

void menu_de_opcoes_inicial() {
  printf("\nSelecione uma opcao:\n");
  printf("1. Registro\n");
  printf("2. Login\n");
  printf("3. Sair\n");
}

void menu_de_opcoes(Usuario *usuario_logado) {
  Node *inicio_da_lista = carregar_usuarios();
  int opcao;
  do {
    printf("\nMenu de Opcoes:\n");
    printf("1. Perfil\n");
    printf("2. Cadastrar Produto\n");
    printf("3. Ver Produtos e Adicionar ao Carrinho\n");
    printf("4. Editar Produto\n");
    printf("5. Excluir Produto\n");
    printf("6. Exibir Carrinho\n");
    printf("7. Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    switch (opcao) {
    case 1:
     visualizar_informacoes_usuario(inicio_da_lista, usuario_logado->email);
      break;
    case 2:
      if (usuario_logado->is_admin) {
        cadastrar_item(usuario_logado);
      } else {
        printf("Apenas administradores podem cadastrar produtos.\n");
      }
      break;
    case 3:
      ver_produtos(usuario_logado);
      break;
    case 4:
      if (usuario_logado->is_admin) {
        editar_item(usuario_logado);
      } else {
        printf("Apenas administradores podem editar produtos.\n");
      }
      break;
    case 5:
      if (usuario_logado->is_admin) {
        excluir_item(usuario_logado);
      } else {
        printf("Apenas administradores podem excluir produtos.\n");
      }
      break;
    case 6:
      if (verificar_endereco(usuario_logado)) {
        exibir_carrinho(usuario_logado);
      } else {
        printf(
            "Voca precisa cadastrar um endereco antes de ver seu carrinho.\n");
      }
      break;
    case 7:
      sair(usuario_logado);
      return;
    default:
      printf("Opcao invalida.\n");
    }
  } while (opcao != 7);
}

void comprar() {
  printf("\nCompra efetuada com sucesso!\n");
  FILE *arquivo_carrinho = fopen("carrinho.txt", "w");
  if (arquivo_carrinho == NULL) {
    printf("Erro ao limpar o carrinho de compras.\n");
    return;
  }
  fclose(arquivo_carrinho);
}

void cadastrar_item(Usuario *usuario) {
  Item novo_item;
  printf("\nCadastrar novo produto:\n");
  printf("Nome: ");
  scanf("%79s", novo_item.nome);
  printf("Descricao: ");
  scanf("%199s", novo_item.descricao);
  printf("Preco: ");
  scanf("%f", &novo_item.preco);
  printf("Em promocao? (1 - Sim, 0 - Nao): ");
  scanf("%d", &novo_item.promocao);

  novo_item.id = usuario->quantidade_itens + 1;

  usuario->quantidade_itens++;
  usuario->itens =
      realloc(usuario->itens, usuario->quantidade_itens * sizeof(Item));
  usuario->itens[usuario->quantidade_itens - 1] = novo_item;

  salvar_produtos(usuario->itens, usuario->quantidade_itens);

  printf("Produto cadastrado com sucesso!\n");
}

void ver_produtos() {
  printf("\nLista de produtos:\n");

  Item *produtos;
  int quantidade_produtos;
  produtos = carregar_produtos(&quantidade_produtos);

  if (produtos == NULL) {
    printf("Nenhum produto encontrado.\n");
    return;
  }

  for (int i = 0; i < quantidade_produtos; i++) {
    printf("ID: %d\n", produtos[i].id);
    printf("Nome: %s\n", produtos[i].nome);
    printf("Descricao: %s\n", produtos[i].descricao);
    printf("Preco: R$%.2f\n", produtos[i].preco);
    printf("Promocao: %s\n", produtos[i].promocao ? "Sim" : "Nao");
    printf("--------------------\n");
  }

  free(produtos);

  char escolha;
  do {
    printf("Deseja adicionar um produto ao carrinho? (S/N): ");
    scanf(" %c", &escolha);
    if (escolha == 'S' || escolha == 's') {
      int id_produto;
      printf("Digite o ID do produto que deseja adicionar ao carrinho: ");
      scanf("%d", &id_produto);
      adicionar_item_carrinho(id_produto);
    }
  } while (escolha == 'S' || escolha == 's');
}


void editar_item() {
  int id;
  printf("Digite o ID do produto que deseja editar: ");
  scanf("%d", &id);

  FILE *arquivo = fopen("produtos.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de produtos.\n");
    return;
  }

  FILE *temp = fopen("temp_produtos.txt", "w");
  if (temp == NULL) {
    printf("Erro ao criar arquivo temporaio.\n");
    fclose(arquivo);
    return;
  }

  int encontrado = 0;
  char linha[256];

  while (fgets(linha, sizeof(linha), arquivo)) {
    int produto_id, em_promocao;
    char nome[100], descricao[200];
    float preco;

    sscanf(linha, "%d;%[^;];%[^;];%f;%d\n", &produto_id, nome, descricao,
           &preco, &em_promocao);

    if (produto_id == id) {
      encontrado = 1;
      printf("Produto encontrado. Edite as informacoes conforme necessario.\n");
      printf("Digite o novo nome do produto: ");
      scanf("%s", nome);
      printf("Digite o novo preco do produto: ");
      scanf("%f", &preco);
      printf("Digite a nova descricao do produto: ");
      scanf(" %[^\n]", descricao);
      printf("O produto esta em promocao? (0 para nao, 1 para sim): ");
      scanf("%d", &em_promocao);

      fprintf(temp, "%d;%s;%s;%.2f;%d\n", produto_id, nome, descricao, preco,
              em_promocao);
      continue;
    }

    fprintf(temp, "%s", linha);
  }

  fclose(arquivo);
  fclose(temp);

  if (remove("produtos.txt") != 0) {
    printf("Erro ao remover o arquivo original.\n");
    return;
  }

  if (rename("temp_produtos.txt", "produtos.txt") != 0) {
    printf("Erro ao renomear o arquivo temporaio.\n");
    return;
  }

  if (!encontrado) {
    printf("Produto nao encontrado.\n");
  } else {
    printf("Produto editado com sucesso.\n");
  }
}

void excluir_item() {
  int id;
  printf("Digite o ID do produto que deseja excluir: ");
  scanf("%d", &id);

  FILE *arquivo = fopen("produtos.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de produtos.\n");
    return;
  }

  FILE *temp = fopen("temp_produtos.txt", "w");
  if (temp == NULL) {
    printf("Erro ao criar arquivo temporario.\n");
    fclose(arquivo);
    return;
  }

  int encontrado = 0;
  char linha[256];

  while (fgets(linha, sizeof(linha), arquivo)) {
    int produto_id;
    sscanf(linha, "%d;", &produto_id);

    if (produto_id == id) {
      encontrado = 1;
      continue;
    }

    fprintf(temp, "%s", linha);
  }

  fclose(arquivo);
  fclose(temp);

  if (!encontrado) {
    printf("Produto nao encontrado.\n");
    remove("temp_produtos.txt");
    return;
  }

  remove("produtos.txt");
  rename("temp_produtos.txt", "produtos.txt");

  printf("Produto excluido com sucesso!\n");
}

void exibir_carrinho(Usuario *usuario) {
    printf("Carrinho de Compras:\n");

    FILE *arquivo_carrinho = fopen("carrinho.txt", "r");
    if (arquivo_carrinho == NULL) {
        printf("Erro ao acessar o carrinho de compras.\n");
        return;
    }

    float total = 0.0;
    char nome_produto[100];
    int id_produto;
    float preco_produto;
    int items_read = 0;

    while (fscanf(arquivo_carrinho, "%[^;];%d;%f\n", nome_produto, &id_produto,
                  &preco_produto) == 3) {
        printf("Nome: %s\n", nome_produto);
        printf("ID: %d\n", id_produto);
        printf("Preco: R$%.2f\n", preco_produto);
        printf("--------------------\n");
        total += preco_produto;
        items_read++;
    }

    fclose(arquivo_carrinho);

     if (items_read == 0) {
        printf("Nao ha nada no carrinho de compras.\n");
    } else {
        printf("Total: R$%.2f\n", total);
    }

    printf("Total: R$%.2f\n", total);

    char escolha;
    do {
        printf("\nSelecione uma opcao:\n");
        printf("1. Comprar\n");
        printf("2. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf(" %c", &escolha);
        switch (escolha) {
            case '1':
                comprar(usuario);
                return;
            case '2':
                return;
            default:
                printf("Opcao invalida.\n");
        }
    } while (escolha != '2');
}

Item *buscar_item_por_id(Usuario *usuario, int id) {
  for (int i = 0; i < usuario->quantidade_itens; i++) {
    if (usuario->itens[i].id == id) {
      return &usuario->itens[i];
    }
  }
  return NULL;
}

void inicializar_carrinho(Carrinho *carrinho) {
    carrinho->itens = malloc(100 * sizeof(CarrinhoItem)); 
    if (carrinho->itens == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para os itens do carrinho.\n");
        exit(EXIT_FAILURE);
    }
    carrinho->quantidade_itens = 0;
}


void sair(Usuario *usuario) {
  printf("\nAte breve, %s!\n", usuario->nome);
  usuario->autenticado = false;
  FILE *arquivo_carrinho = fopen("carrinho.txt", "w");
  if (arquivo_carrinho == NULL) {
    printf("Erro ao limpar o carrinho de compras.\n");
    return;
  }
  fclose(arquivo_carrinho);
}

void salvar_usuarios(Node *usuarios) {
    FILE *arquivo = fopen("usuarios.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários para escrita.\n");
        return;
    }

    Node *atual = usuarios;
    while (atual != NULL) {
        fprintf(arquivo, "%s;%s;%s;%s;%d;%s;%d;%hhd;%d\n", 
                atual->usuario.email, 
                atual->usuario.senha, 
                atual->usuario.nome,
                atual->usuario.endereco, 
                atual->usuario.idade, 
                atual->usuario.celular, 
                atual->usuario.is_admin, 
                atual->usuario.quantidade_itens,
                atual->usuario.autenticado);
        atual = atual->prox;
    }

    fclose(arquivo);
}

Node *inserir_usuario(Node *lista, Usuario novo_usuario) {
    Node *novo_no = (Node *)malloc(sizeof(Node));
    if (novo_no == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para novo usuário.\n");
        exit(EXIT_FAILURE);
    }

    novo_no->usuario = novo_usuario;
    novo_no->prox = NULL;

    if (lista == NULL) {
        return novo_no;
    }

    Node *atual = lista;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }

    atual->prox = novo_no;

    return lista;
}

Node *carregar_usuarios() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Arquivo de usuários não encontrado. Criando novo arquivo.\n");
        return NULL;
    }

    Node *usuarios = NULL;
    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        Usuario usuario;
        int autenticado;
        sscanf(linha, "%[^;];%[^;];%[^;];%[^;];%d;%[^;];%d;%d;%d\n", usuario.email, usuario.senha, usuario.nome,
               usuario.endereco, &usuario.idade, usuario.celular, &usuario.quantidade_itens,
               &usuario.is_admin, &autenticado);
        usuario.autenticado = autenticado;

        usuarios = inserir_usuario(usuarios, usuario);
    }

    fclose(arquivo);
    return usuarios;
}

void salvar_produtos(Item *produtos, int quantidade) {
    FILE *arquivo = fopen("produtos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos para escrita.\n");
        return;
    }
    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d;%s;%s;%.2f;%d\n", produtos[i].id, produtos[i].nome,
                produtos[i].descricao, produtos[i].preco, produtos[i].promocao);
    }
    fclose(arquivo);
}

Item *carregar_produtos(int *quantidade) {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Arquivo de produtos nao encontrado. Criando um novo...\n");
        *quantidade = 0;
        return NULL;
    }
    Item *produtos = malloc(sizeof(Item));
    int contador = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Item novo_item;
        sscanf(linha, "%d;%[^;];%[^;];%f;%d", &novo_item.id, novo_item.nome,
               novo_item.descricao, &novo_item.preco, &novo_item.promocao);
        produtos[contador] = novo_item;
        contador++;
        produtos = realloc(produtos, (contador + 1) * sizeof(Item));
    }
    fclose(arquivo);
    *quantidade = contador;
    return produtos;
}

void salvar_carrinho(Usuario *usuario) {
    FILE *arquivo = fopen("carrinho.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de carrinho para escrita.\n");
        return;
    }
    fprintf(arquivo, "%s;%d\n", usuario->email,
            usuario->quantidade_itens);
    for (int i = 0; i < usuario->quantidade_itens; i++) {
        fprintf(arquivo, "%d;%d\n", usuario->carrinho.itens[i].item_id,
                usuario->carrinho.itens[i].quantidade);
    }
    fclose(arquivo);
}

void carregar_carrinho(Usuario *usuario) {
  FILE *arquivo = fopen("carrinho.txt", "r");
  if (arquivo == NULL) {
    printf("Arquivo de carrinho nao encontrado. Criando um novo...\n");
    return;
  }
  char email[80];
  int quantidade_itens;
  while (fscanf(arquivo, "%[^;];%d\n", email, &quantidade_itens) != EOF) {
    if (strcmp(email, usuario->email) == 0) {
      usuario->carrinho.quantidade_itens = quantidade_itens;
      for (int i = 0; i < quantidade_itens; i++) {
        fscanf(arquivo, "%d;%d\n", &usuario->carrinho.itens[i].item_id,
               &usuario->carrinho.itens[i].quantidade);
      }
      fclose(arquivo);
      return;
    }
  }
  fclose(arquivo);
}

void adicionar_item_carrinho(int id_produto) {
  Item *produto = encontrar_produto_por_id(id_produto);
  if (produto == NULL) {
    printf("Produto nao encontrado.\n");
    return;
  }

  FILE *arquivo_carrinho = fopen("carrinho.txt", "a+");

  if (arquivo_carrinho == NULL) {
    printf("Erro ao acessar o carrinho de compras.\n");
    return;
  }

  fprintf(arquivo_carrinho, "%s;%d;%.2f\n", produto->nome, produto->id,
          produto->preco);

  fclose(arquivo_carrinho);
  printf("Item adicionado ao carrinho.\n");
}

Item *encontrar_produto_por_id(int id) {
  Item *produtos;
  int quantidade_produtos;
  produtos = carregar_produtos(&quantidade_produtos);

  if (produtos == NULL) {
    printf("Erro ao carregar produtos.\n");
    return NULL;
  }

  for (int i = 0; i < quantidade_produtos; i++) {
    if (produtos[i].id == id) {
      Item *produto_encontrado = (Item *)malloc(sizeof(Item));
      if (produto_encontrado == NULL) {
        printf("Erro ao alocar memoria.\n");
        return NULL;
      }
      *produto_encontrado = produtos[i];
      free(produtos);
      return produto_encontrado;
    }
  }

  free(produtos);
  return NULL;
}

void trocar_usuarios(Usuario *a, Usuario *b) {
    Usuario temp = *a;
    *a = *b;
    *b = temp;
}

void ordenar_usuarios_por_nome() {
    FILE *arquivo = fopen("usuarios.txt", "r+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de usuários para leitura e escrita.\n");
        return;
    }

    Usuario usuarios[100];
    int num_usuarios = 0;
int autenticado_int;
while (fscanf(arquivo, "%[^;];%[^;];%[^;];%[^;];%d;%[^;];%d;%d;%d\n",
              usuarios[num_usuarios].email, usuarios[num_usuarios].senha,
              usuarios[num_usuarios].nome, usuarios[num_usuarios].endereco,
              &usuarios[num_usuarios].idade, usuarios[num_usuarios].celular,
              &usuarios[num_usuarios].is_admin, &usuarios[num_usuarios].quantidade_itens,
              &autenticado_int) != EOF) {
    usuarios[num_usuarios].autenticado = autenticado_int != 0;
        num_usuarios++;
    }

    for (int i = 0; i < num_usuarios - 1; i++) {
        for (int j = 0; j < num_usuarios - i - 1; j++) {
            if (strcmp(usuarios[j].nome, usuarios[j + 1].nome) > 0) {
                trocar_usuarios(&usuarios[j], &usuarios[j + 1]);
            }
        }
    }

    rewind(arquivo);

    for (int i = 0; i < num_usuarios; i++) {
        fprintf(arquivo, "%s;%s;%s;%s;%d;%s;%d;%d;%d\n", usuarios[i].email, usuarios[i].senha,
                usuarios[i].nome, usuarios[i].endereco, usuarios[i].idade, usuarios[i].celular,
                usuarios[i].is_admin, usuarios[i].quantidade_itens, usuarios[i].autenticado);
    }

    fclose(arquivo);
}
