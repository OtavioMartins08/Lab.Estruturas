#include <stdio.h>

typedef struct {
    int mat;
    char nome[10];
    float media;
} aluno;

void menu() {
    printf("Escolha uma opcao:\n");
    printf("1 - cadastar aluno\n");
    printf("2 - listar alunos\n");
    printf("3 - buscar aluno\n");
    printf("4 - quantidade de alunos\n");
    printf("5 - excluir um aluno\n");
    printf("6 - editar um aluno\n");
    printf("0 - sair\n");
    printf("===================\n");
}

void cadastrar() {
    FILE* file = fopen("aluno.b", "ab");
    aluno a;
    printf("Informe os dados do aluno:\n");
    scanf("%d %s %f", &a.mat, a.nome, &a.media);
    fwrite(&a, sizeof(aluno), 1, file);
    fclose(file);
}

void buscar (int mat) {
    FILE* file = fopen("aluno.b", "rb");
    aluno a;
    int entrou = 0;
    while (fread(&a, sizeof(aluno), 1, file)) {
        if (a.mat == mat) {
            printf("%d: %s %f\n", a.mat, a.nome, a.media);
            entrou = 1;
        }
    }
    if (entrou == 0) {
        printf("Aluno nao encontrado\n");
    }
    fclose(file);
}

void listar() {
    FILE* file = fopen("aluno.b", "rb");
    aluno a;
    while (fread(&a, sizeof(aluno), 1, file)) {
        printf("%d: %s %.2f\n", a.mat, a.nome, a.media);
    }
    fclose(file);
}

int tamanho() {
    FILE* file = fopen("aluno.b", "rb");
    aluno a;
    int cont = 0;
    while (fread(&a, sizeof(aluno), 1, file)) {
        cont++;
    }
    fclose(file);
    return cont;
}

void excluir(int mat) {
    int n = tamanho();
    aluno v[n];
    FILE *file = fopen("aluno.b", "rb");
    int i = 0;
    while (fread(&v[i], sizeof(aluno), 1, file)) {
        i++;
    }
    fclose(file);
    file = fopen("aluno.b", "wb");
    for (i = 0; i < n; i++) {
        if (v[i].mat != mat) {
            fwrite(&v[i], sizeof(aluno), 1, file);
        }
    }
    fclose(file);
}

void editar(int mat) {
    int n = tamanho();
    aluno v[n];
    FILE *file = fopen("aluno.b", "rb");
    int i = 0;
    while (fread(&v[i], sizeof(aluno), 1, file)) {
        i++;
    }
    fclose(file);
    file = fopen("aluno.b", "wb");
    for (i = 0; i < n; i++) {
        if (v[i].mat == mat) {
            float media;
            printf("Informe a nota nota:\n");
            scanf("%f", &media);
            v[i].media = media;
            printf("media: %f", media);
        }
        printf("media: %f", v[i].media);
        fwrite(&v[i], sizeof(aluno), 1, file);
    }
    fclose(file);
}

int main() {
    int op;
    menu();
    scanf("%d", &op);
    while (op != 0) {
        if (op == 1) {
           cadastrar();
        } else if (op == 2) {
            listar();
        } else if (op == 3) {
            int mat;
            printf("Informe a matricula:\n");
            scanf("%d", &mat);
            buscar(mat);
        } else if (op == 4) {
            int n = tamanho();
            printf("Quantidade alunos: %d\n", n);
        } else if (op == 5) {
            int mat;
            printf("Informe a matricula para exclusao:\n");
            scanf("%d", &mat);
            excluir(mat);
        } else if (op == 6) {
            int mat;
            printf("Informe a matricula do aluno para edicao:\n");
            scanf("%d", &mat);
            editar(mat);
        }
        menu();
        scanf("%d", &op);
    }
    return 0;
}