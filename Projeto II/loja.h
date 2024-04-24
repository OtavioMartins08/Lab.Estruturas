#include <stdbool.h>
#include <stdio.h>

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
  struct Item *prox;
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

typedef struct Node {
    Usuario usuario;
    struct Node *prox;
} Node;

void inicializar_itens(Usuario *usuario);
void cadastrar_usuario(Node **lista);
Item *encontrar_produto_por_id(int id);
void login(Usuario *usuario, Node *usuarios);
void mensagem_de_boas_vindas(Usuario *usuario);
void menu_de_opcoes_inicial();
void menu_de_opcoes(Usuario *usuario_logado);
void cadastrar_item(Usuario *usuario);
void ver_produtos();
void comprar();
void editar_item();
void excluir_item();
void adicionar_item_carrinho(int id_produto);
float calcular_total_carrinho(Usuario *usuario);
void exibir_carrinho(Usuario *usuario);
void inicializar_carrinho(Carrinho *carrinho);
Item *buscar_item_por_id(Usuario *usuario, int id);
void salvar_usuarios();
Node *carregar_usuarios();
void sair(Usuario *usuario);
void salvar_produtos(Item *produtos, int quantidade);
Item *carregar_produtos(int *quantidade);
void salvar_carrinho(Usuario *usuario);
void carregar_carrinho(Usuario *usuario);
void visualizar_informacoes_usuario(Node *inicio, char *email);
void cadastrar_endereco(Usuario *usuario);
void editar_idade(Usuario *usuario);
bool verificar_informacoes_completas(Usuario *usuario);
void editar_celular(Usuario *usuario);
bool verificar_endereco(Usuario *usuario);
Node *inserir_usuario(Node *lista, Usuario novo_usuario);
void trocar_usuarios(Usuario *a, Usuario *b);
void ordenar_usuarios_por_nome();

