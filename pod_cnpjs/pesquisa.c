#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 100003
#define MAX_CNPJ_LENGTH 11
#define MAX_NOME_LENGTH 100

typedef struct Empresa {
    char cnpj[MAX_CNPJ_LENGTH];
    char nome[MAX_NOME_LENGTH];
    char natureza_juridica[10];
    char qualificacao_responsavel[10];
    char capital_social[20];
    char porte_empresa[5];
    char ente_federativo[50];
} Empresa;

typedef struct Node {
    Empresa data;
    struct Node* next;
} Node;

Node* hashTableCNPJ[TABLE_SIZE];
Node* hashTableNome[TABLE_SIZE];

unsigned int hash(const char* key) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % TABLE_SIZE;
}

void inicializarTabelas() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTableCNPJ[i] = NULL;
        hashTableNome[i] = NULL;
    }
}

void inserirTabela(Node** table, const Empresa* emp, unsigned int (*hashFunc)(const char*)) {
    unsigned int index = hashFunc(emp->cnpj);
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erro ao alocar memória para novo nó");
        exit(EXIT_FAILURE);
    }
    newNode->data = *emp;
    newNode->next = table[index];
    table[index] = newNode;
}

Node* buscarTabela(Node** table, const char* key, unsigned int (*hashFunc)(const char*), int isNameSearch) {
    unsigned int index = hashFunc(key);
    Node* node = table[index];

    while (node != NULL) {
        if (isNameSearch == 1) {
            if (strcasecmp(node->data.nome, key) == 0) {
                return node;
            }
        } else {
            if (strcmp(node->data.cnpj, key) == 0) {
                return node;
            }
        }
        node = node->next;
    }

    return NULL;
}

void carregarDados(FILE* file) {
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        Empresa emp;
        char* token = strtok(line, ";");
        if (token == NULL) continue;
        strncpy(emp.cnpj, token, MAX_CNPJ_LENGTH - 1);
        emp.cnpj[MAX_CNPJ_LENGTH - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(emp.nome, token, MAX_NOME_LENGTH - 1);
        emp.nome[MAX_NOME_LENGTH - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(emp.natureza_juridica, token, sizeof(emp.natureza_juridica) - 1);
        emp.natureza_juridica[sizeof(emp.natureza_juridica) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(emp.qualificacao_responsavel, token, sizeof(emp.qualificacao_responsavel) - 1);
        emp.qualificacao_responsavel[sizeof(emp.qualificacao_responsavel) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(emp.capital_social, token, sizeof(emp.capital_social) - 1);
        emp.capital_social[sizeof(emp.capital_social) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(emp.porte_empresa, token, sizeof(emp.porte_empresa) - 1);
        emp.porte_empresa[sizeof(emp.porte_empresa) - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(emp.ente_federativo, token, sizeof(emp.ente_federativo) - 1);
        emp.ente_federativo[sizeof(emp.ente_federativo) - 1] = '\0';

        inserirTabela(hashTableCNPJ, &emp, hash);
        inserirTabela(hashTableNome, &emp, hash);
    }
}

void liberarTabela(Node** table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* node = table[i];
        while (node != NULL) {
            Node* temp = node;
            node = node->next;
            free(temp);
        }
    }
}

void exibirMenu() {
    printf("Escolha uma opcao:\n");
    printf("1. Buscar por CNPJ\n");
    printf("2. Buscar por Nome\n");
    printf("3. Sair\n");
}

int capturarOpcao() {
    int opcao;
    printf("Opcao: ");
    scanf("%d", &opcao);
    while (getchar() != '\n');  // Limpa o buffer do stdin
    return opcao;
}

void inicializarECarregarDados(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    inicializarTabelas();
    carregarDados(file);
    fclose(file);
}

void processarBusca(int opcao) {
    char input[MAX_NOME_LENGTH];
    Node* resultado = NULL;

    if (opcao == 1) {
        printf("Digite o CNPJ (8 digitos): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        resultado = buscarTabela(hashTableCNPJ, input, hash, 0);  // 0 para busca por CNPJ
    } else if (opcao == 2) {
        printf("Digite o Nome: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        resultado = buscarTabela(hashTableNome, input, hash, 1);  // 1 para busca por nome
    }

    if (resultado != NULL) {
        printf("Dados encontrados:\n");
        printf("CNPJ: %s\n", resultado->data.cnpj);
        printf("Nome: %s\n", resultado->data.nome);
        printf("Natureza Juridica: %s\n", resultado->data.natureza_juridica);
        printf("Qualificacao do Responsavel: %s\n", resultado->data.qualificacao_responsavel);
        printf("Capital Social: %s\n", resultado->data.capital_social);
        printf("Porte da Empresa: %s\n", resultado->data.porte_empresa);
        printf("Ente Federativo: %s\n", resultado->data.ente_federativo);
    } else {
        printf("Nenhum resultado encontrado.\n");
    }
    getchar();
}

int main() {
    inicializarECarregarDados("Empresas1.EMPRECSV");

    while (1) {
        exibirMenu();
        int opcao = capturarOpcao();

        if (opcao == 3) {
            break;
        }

        processarBusca(opcao);
    }

    liberarTabela(hashTableCNPJ);
    liberarTabela(hashTableNome);

    return 0;
}
