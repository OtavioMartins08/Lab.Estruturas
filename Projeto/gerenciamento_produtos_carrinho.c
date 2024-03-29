#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int item_id;
  int quantidade;
} CarrinhoItem;

typedef struct {
  int quantidade_itens;
  CarrinhoItem *itens;
} Carrinho;

typedef struct {
  int id;
  char nome[80];
  char descricao[200];
  float preco;
  int promocao;
} Item;

typedef struct {
  char email[80];
  char senha[80];
  char nome[80];
  char endereco[200];
  int idade;
  char celular[15];
  int quantidade_itens;
  Item *itens;
  int is_admin;
  Carrinho carrinho;
  bool autenticado;
} Usuario;

void inicializar_itens(Usuario *usuario);
void cadastrar_usuario(Usuario *usuario);
Item *encontrar_produto_por_id(int id);
void login(Usuario *usuario, Usuario *usuarios, int quantidade_usuarios);
void mensagem_de_boas_vindas(Usuario *usuario);
void menu_de_opcoes_inicial();
void menu_de_opcoes(Usuario *usuario_logado);
void cadastrar_item(Usuario *usuario);
void ver_produtos();
void comprar();
void editar_item();
void excluir_item();
void adicionar_item_carrinho(int id_produto);
void exibir_carrinho();
void inicializar_carrinho(Carrinho *carrinho);
Item *buscar_item_por_id(Usuario *usuario, int id);
void salvar_usuarios(Usuario *usuarios, int quantidade);
Usuario *carregar_usuarios(int *quantidade);
void sair(Usuario *usuario);
void salvar_produtos(Item *produtos, int quantidade);
Item *carregar_produtos(int *quantidade);
void salvar_carrinho(Usuario *usuario);
void carregar_carrinho(Usuario *usuario);
void editar_informacoes_usuario(Usuario *usuario);
void cadastrar_endereco(Usuario *usuario);
void editar_idade(Usuario *usuario);
bool verificar_informacoes_completas(Usuario *usuario);
void editar_celular(Usuario *usuario);
bool verificar_endereco(Usuario *usuario);

int main() {
  Usuario *usuarios;
  int quantidade_usuarios;
  Item *produtos;
  int quantidade_produtos;

  char nome_loja[100] = "Loja";
  printf("Bem-vindo(a) a %s!\n", nome_loja);

  FILE *usuarios_arquivo = fopen("usuarios.txt", "r");
  if (usuarios_arquivo == NULL) {
    printf("Arquivo de usuarios nao encontrado. Criando um novo...\n");
    FILE *novo_usuarios_arquivo = fopen("usuarios.txt", "w");
    if (novo_usuarios_arquivo == NULL) {
      printf("Erro ao criar arquivo de usuarios.\n");
      return 1;
    }
    fclose(novo_usuarios_arquivo);
  } else {
    fclose(usuarios_arquivo);
  }

  FILE *produtos_arquivo = fopen("produtos.txt", "r");
  if (produtos_arquivo == NULL) {
    printf("Arquivo de produtos nao encontrado. Criando um novo...\n");
    FILE *novo_produtos_arquivo = fopen("produtos.txt", "w");
    if (novo_produtos_arquivo == NULL) {
      printf("Erro ao criar arquivo de produtos.\n");
      return 1;
    }
    fclose(novo_produtos_arquivo);
  } else {
    fclose(produtos_arquivo);
  }

  usuarios = carregar_usuarios(&quantidade_usuarios);
  produtos = carregar_produtos(&quantidade_produtos);

  Usuario usuario_logado;

  int opcao;
  do {
    menu_de_opcoes_inicial();
    scanf("%d", &opcao);
    switch (opcao) {
    case 1:
      cadastrar_usuario(&usuarios[quantidade_usuarios]);
      quantidade_usuarios++;
      break;
    case 2:
      login(&usuario_logado, usuarios, quantidade_usuarios);
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
      printf("Opcao invalida.\n");
    }
  } while (opcao != 3);
  salvar_usuarios(usuarios, quantidade_usuarios);
  salvar_produtos(produtos, quantidade_produtos);

  free(usuarios);
  free(produtos);

  return 0;
}

void cadastrar_usuario(Usuario *usuario) {
  printf("\nRegistre-se para continuar:\n");
  printf("Digite seu nome: ");
  scanf("%79s", usuario->nome);

  do {
    printf("Digite seu e-mail (exemplo@dominio.com): ");
    scanf("%79s", usuario->email);
    if (strchr(usuario->email, '@') == NULL) {
      printf("E-mail invalido. Por favor, inclua o caractere '@'.\n");
    }
  } while (strchr(usuario->email, '@') == NULL);

  printf("Digite sua senha: ");
  scanf("%79s", usuario->senha);

  printf("Digite seu endereco (ex: Rua Exemplo, 123): ");
  scanf(" %199[^\n]", usuario->endereco);
  printf("Digite sua idade: ");
  scanf("%d", &usuario->idade);
  printf("Digite seu numero de celular (ex: (00) 1234-5678): ");
  scanf(" %14s", usuario->celular);

  if (strcmp(usuario->email, "admin@admin.com") == 0 &&
      strcmp(usuario->senha, "admin") == 0) {
    usuario->is_admin = 1;
  } else {
    usuario->is_admin = 0;
  }

  usuario->quantidade_itens = 0;
  inicializar_carrinho(&usuario->carrinho);
  usuario->autenticado = true;

  FILE *arquivo = fopen("usuarios.txt", "a");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de usuarios para escrita.\n");
    return;
  }
  fprintf(arquivo, "%s;%s;%s;%s;%d;%s;%d;%d\n", usuario->email, usuario->senha,
          usuario->nome, usuario->endereco, usuario->idade, usuario->celular,
          usuario->is_admin, usuario->carrinho.quantidade_itens);
  fclose(arquivo);

  printf("Registro bem-sucedido.\n");
  printf("Retornando ao menu inicial...\n");
}

void editar_informacoes_usuario(Usuario *usuario) {
  printf("\nEditar informacoes do usuario:\n");
  printf("1. Nome: %s\n", usuario->nome);
  printf("2. E-mail: %s\n", usuario->email);
  printf("3. Endereco: %s\n", usuario->endereco);
  printf("4. Idade: %d\n", usuario->idade);
  printf("5. Celular: %s\n", usuario->celular);
  printf("\n0. Sair\n");

  int opcao;
  printf("Escolha o numero da informacao que deseja editar: ");
  scanf("%d", &opcao);

  switch (opcao) {
  case 1:
    printf("Novo nome: ");
    scanf("%79s", usuario->nome);
    break;
  case 2:
    printf("Novo e-mail: ");
    scanf("%79s", usuario->email);
    break;
  case 3:
    printf("Novo endereco: ");
    scanf(" %199[^\n]", usuario->endereco);
    break;
  case 4:
    printf("Nova idade: ");
    scanf("%d", &usuario->idade);
    break;
  case 5:
    printf("Novo celular: ");
    scanf(" %14s", usuario->celular);
    break;
  case 0:
    printf("Retornando ao menu anterior...\n");
    return;
  default:
    printf("Opcao invalida.\n");
  }

  FILE *arquivo = fopen("usuarios.txt", "r+");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de usuarios para escrita.\n");
    return;
  }
  char linha[256];
  char tempFile[] = "tempFile.txt";
  FILE *temp = fopen(tempFile, "w");

  while (fgets(linha, sizeof(linha), arquivo)) {
    char email[80];
    sscanf(linha, "%[^;];", email);
    if (strcmp(email, usuario->email) == 0) {
      fprintf(temp, "%s;%s;%s;%s;%d;%s;%d;%d\n", usuario->email, usuario->senha,
              usuario->nome, usuario->endereco, usuario->idade,
              usuario->celular, usuario->is_admin,
              usuario->carrinho.quantidade_itens);
    } else {
      fprintf(temp, "%s", linha);
    }
  }
  fclose(arquivo);
  fclose(temp);
  remove("usuarios.txt");
  rename(tempFile, "usuarios.txt");

  printf("As informacoes do usuaio foram atualizadas.\n");
}
void cadastrar_endereco(Usuario *usuario) {
  printf("Digite seu endereco: ");
  scanf("%199s", usuario->endereco);
}

void editar_idade(Usuario *usuario) {
  printf("\nEditar idade do usuario:\n");
  printf("Idade atual: %d\n", usuario->idade);
  printf("Nova idade: ");
  scanf("%d", &usuario->idade);
  printf("A idade do usuario foi atualizada.\n");
}

void editar_celular(Usuario *usuario) {
  printf("\nEditar numero de celular do usuario:\n");
  printf("Numero de celular atual: %s\n", usuario->celular);
  printf("Novo numero de celular: ");
  scanf("%s", usuario->celular);
  printf("O numero de celular do usuario foi atualizado.\n");
}

bool verificar_endereco(Usuario *usuario) {
  return strlen(usuario->endereco) > 0;
}

void login(Usuario *usuario, Usuario *usuarios, int quantidade_usuarios) {
  char email[80], senha[80];
  printf("\nFaca login:\n");
  printf("Digite seu e-mail: ");
  scanf("%79s", email);
  printf("Digite sua senha: ");
  scanf("%79s", senha);
  int encontrado = 0;

  for (int i = 0; i < quantidade_usuarios; i++) {
    if (strcmp(email, usuarios[i].email) == 0 &&
        strcmp(senha, usuarios[i].senha) == 0) {
      if (usuarios[i].is_admin) {
        *usuario = usuarios[i];
        usuario->autenticado = true;
        printf("Login bem-sucedido como administrador.\n");
        encontrado = 1;
        break;
      } else if (verificar_informacoes_completas(&usuarios[i])) {
        *usuario = usuarios[i];
        usuario->autenticado = true;
        printf("Login bem-sucedido.\n");
        encontrado = 1;
        break;
      } else {
        printf("Alguns detalhes do seu perfil estao incompletos. Por favor, "
               "complete todas as informacoes para fazer login.\n");
        editar_informacoes_usuario(&usuarios[i]);
        salvar_usuarios(usuarios, quantidade_usuarios);
        return;
      }
    }
  }
  if (!encontrado) {
    char opcao;
    printf("E-mail ou senha incorretos.\n");
    printf("Deseja se cadastrar? (S/N): ");
    scanf(" %c", &opcao);
    if (opcao == 'S' || opcao == 's') {
      cadastrar_usuario(usuario);
    } else {
      usuario->autenticado = false;
    }
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
      editar_informacoes_usuario(usuario_logado);
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

  while (fscanf(arquivo_carrinho, "%[^;];%d;%f\n", nome_produto, &id_produto,
                &preco_produto) == 3) {
    printf("Nome: %s\n", nome_produto);
    printf("ID: %d\n", id_produto);
    printf("Preco: R$%.2f\n", preco_produto);
    printf("--------------------\n");
    total += preco_produto;
  }

  fclose(arquivo_carrinho);

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
  carrinho->quantidade_itens = 0;
  carrinho->itens = NULL;
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

void salvar_usuarios(Usuario *usuarios, int quantidade) {
  FILE *arquivo = fopen("usuarios.txt", "w");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo de usuarios para escrita.\n");
    return;
  }
  for (int i = 0; i < quantidade; i++) {
    fprintf(arquivo, "%s;%s;%s;%s;%d;%s;%d;%d\n", usuarios[i].email,
            usuarios[i].senha, usuarios[i].nome, usuarios[i].endereco,
            usuarios[i].idade, usuarios[i].celular, usuarios[i].is_admin,
            usuarios[i].carrinho.quantidade_itens);
    for (int j = 0; j < usuarios[i].carrinho.quantidade_itens; j++) {
      fprintf(arquivo, "%d;%d\n", usuarios[i].carrinho.itens[j].item_id,
              usuarios[i].carrinho.itens[j].quantidade);
    }
  }
  fclose(arquivo);
}

Usuario *carregar_usuarios(int *quantidade) {
  FILE *arquivo = fopen("usuarios.txt", "r");
  if (arquivo == NULL) {
    printf("Arquivo de usuarios nao encontrado. Criando um novo...\n");
    *quantidade = 0;
    return NULL;
  }
  Usuario *usuarios = malloc(sizeof(Usuario));
  int contador = 0;
  char linha[256];
  while (fgets(linha, sizeof(linha), arquivo)) {
    sscanf(linha, "%[^;];%[^;];%[^;];%[^;];%d;%[^;];%d;%d",
           usuarios[contador].email, usuarios[contador].senha,
           usuarios[contador].nome, usuarios[contador].endereco,
           &usuarios[contador].idade, usuarios[contador].celular,
           &usuarios[contador].is_admin,
           &usuarios[contador].carrinho.quantidade_itens);
    contador++;
    usuarios = realloc(usuarios, (contador + 1) * sizeof(Usuario));
  }
  fclose(arquivo);
  *quantidade = contador;
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
          usuario->carrinho.quantidade_itens);
  for (int i = 0; i < usuario->carrinho.quantidade_itens; i++) {
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