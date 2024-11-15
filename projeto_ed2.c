//NOME:Thales do PradO Menendez   RA:2510170
//NOME:Marcus Vinicius Santos de Magalhães   RA:2524651
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

// Definição de uma estrutura para representar um paciente em uma árvore binária de busca (BST)
typedef struct noPaciente
{
    int IDpaciente; // ID do paciente (utilizado como chave na árvore)
    int idadePaciente; // Idade do paciente
    char nomePaciente[30]; // Nome do paciente
    char condicaoMedica[40]; // Condição médica do paciente
    struct noPaciente *dir; // Ponteiro para o nó da direita
    struct noPaciente *esq; // Ponteiro para o nó da esquerda
}noPaciente;

// Prototipação das funções (declaração para uso posterior)
noPaciente *alocarNo(int IDpaciente, char *nomePaciente, int idadePaciente, char *condicaoMedica);
noPaciente *inserirID(noPaciente *raiz, int IDpaciente, char *nomePaciente, int idadePaciente, char *condicaoMedica);
noPaciente *inserirNome(noPaciente *raiz, int IDpaciente, char *nomePaciente, int idadePaciente, char *condicaoMedica);
noPaciente *buscarID(noPaciente *raiz, int IDpaciente);
noPaciente *buscarNome(noPaciente *raiz, char *nomePaciente); 
noPaciente **buscarPaiID(noPaciente **raiz, int IDpaciente); 
noPaciente **buscarPaiNome(noPaciente **raiz, char *nomePaciente); 
noPaciente **ppMenor(noPaciente **raiz);
int contarPacientes(noPaciente *raiz);
void exibeRaiz(noPaciente *raiz);
void normalizarString(char *str);
void lerArquivoEInserirDados(const char *nomeArquivo, noPaciente **raizID,noPaciente **raizNome,int i) ;
void imprimirIDEmOrdem(noPaciente *raiz);
void imprimirNomeEmOrdem(noPaciente *raiz);
void removerID(noPaciente **raiz, int IDpaciente);
void removerNome(noPaciente **raiz, char *nomePaciente);
void salvarArquivo(noPaciente *raiz, FILE *arq);
void desalocarArvore(noPaciente *raiz);


int main() 
{
    noPaciente *raizID = NULL, *raizNome = NULL;
    noPaciente **raizEscolhida = NULL;  // Ponteiro para a raiz da árvore escolhida
    int criterioOrdenacao = 0;          // Critério de ordenação (1 para ID, 2 para Nome)
    int totalPacientes = 0;             // Contador de pacientes
    int op = 1;                         // Variável para armazenar a opção do menu

    // Escolha do critério de ordenação (por ID ou Nome)
    printf("Escolha o critério de ordenação:\n1 - Ordenar por ID\n2 - Ordenar por Nome\n:");
    scanf("%d", &criterioOrdenacao);
    getchar(); // Limpa o buffer do teclado

    // Definindo a árvore a ser usada com base no critério escolhido
    if (criterioOrdenacao == 1) 
        raizEscolhida = &raizID;
    else if (criterioOrdenacao == 2) 
        raizEscolhida = &raizNome;
    else 
    {
        printf("Critério inválido. Encerrando programa.\n");
        return 1;
    }

    // Carregar dados de pacientes do arquivo
    lerArquivoEInserirDados("paciente1.txt", &raizID, &raizNome, 0);

    // Menu principal de operações
    while (op != 8) 
    {
        printf("\n--- Menu de Opções ---\n");
        printf("1 - Imprimir Pacientes\n");
        printf("2 - Cadastrar Paciente\n");
        printf("3 - Buscar Paciente\n");
        printf("4 - Remover Paciente\n");
        printf("5 - Salvar Alterações\n");
        printf("6 - Contagem Total de Pacientes\n");
        printf("7 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &op);
        getchar();

        switch (op) 
        {
            case 1:  // Imprimir Pacientes
                if (criterioOrdenacao == 1) 
                    imprimirIDEmOrdem(raizID);
                else 
                {
                    imprimirNomeEmOrdem(raizNome);
                }
                break;
            case 2: { // Cadastrar Paciente
                int ID, idade;
                char nomePaciente[30], condicaoMedica[40];

                printf("Digite o ID do paciente: ");
                scanf("%d", &ID);
                getchar(); // Limpar buffer

                printf("Digite o nome do paciente: ");
                fgets(nomePaciente, sizeof(nomePaciente), stdin);
                nomePaciente[strcspn(nomePaciente, "\n")] = 0; // Remover newline

                printf("Digite a idade do paciente: ");
                scanf("%d", &idade);
                getchar();

                printf("Digite a condição médica do paciente: ");
                fgets(condicaoMedica, sizeof(condicaoMedica), stdin);
                condicaoMedica[strcspn(condicaoMedica, "\n")] = 0;

                // Verificar duplicatas
                if (buscarID(raizID, ID) != NULL) {
                    printf("Paciente com o mesmo ID ou nome já existe.\n");
                } else {
                    // Inserir com base no critério de ordenação
                    if (criterioOrdenacao == 1) {
                        raizID = inserirID(raizID, ID, nomePaciente, idade, condicaoMedica);
                    } else {
                        raizNome = inserirNome(raizNome, ID, nomePaciente, idade, condicaoMedica);
                    }
                    printf("Paciente cadastrado com sucesso.\n");
                }
                break;
            }

            case 3: { // Busca paciente por ID ou Nome, dependendo do critério
                if (criterioOrdenacao == 1) {
                    int ID;
                    printf("Digite o ID do paciente: ");
                    scanf("%d", &ID);
                    noPaciente *paciente = buscarID(raizID, ID);
                    if (paciente) {
                        printf("Paciente encontrado: %d, %s, %d anos, Condição: %s\n",
                               paciente->IDpaciente, paciente->nomePaciente,
                               paciente->idadePaciente, paciente->condicaoMedica);
                    } else {
                        printf("Paciente não encontrado.\n");
                    }
                } else {
                    char nomePaciente[40];
                    printf("Digite o nome do paciente: ");
                    fgets(nomePaciente, sizeof(nomePaciente), stdin);
                    noPaciente *paciente = buscarNome(raizNome, nomePaciente);
                    if (paciente) {
                        printf("Paciente encontrado: %d, %s, %d anos, Condição: %s\n",
                        paciente->IDpaciente, paciente->nomePaciente,
                        paciente->idadePaciente, paciente->condicaoMedica);
                    } else {
                        printf("Paciente não encontrado.\n");
                    }
                }
                break;
            }

            case 4: { // Remove um paciente da árvore (por ID ou Nome)
                if (criterioOrdenacao == 1) {
                    int ID;
                    printf("Digite o ID do paciente a remover: ");
                    scanf("%d", &ID);
                    removerID(&raizID, ID);
                } else {
                    char nomePaciente[100];
                    printf("Digite o nome do paciente a remover: ");
                    fgets(nomePaciente, sizeof(nomePaciente), stdin);        
                    removerNome(&raizNome, nomePaciente);
                }
                break;
            }

            case 5: { // Salva os dados da árvore no arquivo "dados_atualizados.txt"
                FILE *arq = fopen("dados_atualizados.txt", "w");
                if (arq) {
                    if (criterioOrdenacao == 1) {
                        salvarArquivo(raizID, arq);
                    } else {
                        salvarArquivo(raizNome, arq);
                    }
                    fclose(arq);
                    printf("Alterações salvas no arquivo com sucesso.\n");
                } else {
                    perror("Erro ao abrir o arquivo para salvar");
                }
                break;
            }

            case 6: // Exibe a contagem total de pacientes
                totalPacientes = contarPacientes(*raizEscolhida);
                printf("Total de pacientes cadastrados: %d\n", totalPacientes);
                break;

            case 7: // Sair e liberar memória
                printf("Encerrando programa.\n");
                FILE *arq = fopen("dados_atualizados.txt", "w");
                if (arq) {
                    if (criterioOrdenacao == 1) {
                        salvarArquivo(raizID, arq);
                    } else {
                        salvarArquivo(raizNome, arq);
                    }
                    fclose(arq);
                    printf("Dados salvos antes de sair.\n");
                }
                desalocarArvore(raizID);
                desalocarArvore(raizNome);
                return 0;

            default:
                printf("Opção inválida.\n");
                break;
        }
    }
    return 0;
}

// Função para exibir o nó raiz, se existir
void exibeRaiz(noPaciente *raiz) 
{
    if (raiz != NULL) {
        printf("Raiz: %d\n", raiz->IDpaciente);
    } else {
        printf("A árvore está vazia.\n");
    }
}

// Função para ler e inserir os dados na arvore
void lerArquivoEInserirDados(const char *nomeArquivo, noPaciente **raizID,noPaciente **raizNome,int i) 
{
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) 
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int ID, idade;
    char nome[50], condicaoMedica[50];
    fscanf(arq,"%d", &i);
    while (fscanf(arq, "%d %s %d %s", &ID, nome, &idade, condicaoMedica) == 4) 
    {
        *raizID = inserirID(*raizID, ID, nome, idade, condicaoMedica);
        *raizNome = inserirNome(*raizNome, ID, nome, idade, condicaoMedica);
    }

    fclose(arq);
}

// Função para alocar os nós na arvore
noPaciente *alocarNo(int IDpaciente, char *nomePaciente, int idadePaciente, char *condicaoMedica)
{
    noPaciente *novo = NULL;
    novo = (noPaciente*)malloc(sizeof(noPaciente));
    if(novo)
    {
        novo->IDpaciente = IDpaciente;
        novo->idadePaciente = idadePaciente;
        strcpy(novo->nomePaciente, nomePaciente);
        strcpy(novo->condicaoMedica, condicaoMedica);
        novo->dir = NULL;
        novo->esq = NULL;
        return novo;
    }
    return novo;
}

// Função para função para inserir baseado no ID
noPaciente *inserirID(noPaciente *raiz, int IDpaciente, char *nomePaciente, int idadePaciente, char *condicaoMedica)
{
    if(raiz == NULL)
        return alocarNo(IDpaciente, nomePaciente, idadePaciente, condicaoMedica);
    else
    {
        if(IDpaciente < raiz->IDpaciente)
            raiz->esq = inserirID(raiz->esq,IDpaciente, nomePaciente, idadePaciente, condicaoMedica);
        else
            raiz->dir = inserirID(raiz->dir,IDpaciente, nomePaciente, idadePaciente, condicaoMedica);
        return raiz;
    }
}

// Função para inserir baseado no nome
noPaciente *inserirNome(noPaciente *raiz, int IDpaciente, char *nomePaciente, int idadePaciente, char *condicaoMedica)
{
    if(raiz == NULL)
        return alocarNo(IDpaciente, nomePaciente, idadePaciente, condicaoMedica);
    else if(strcmp(nomePaciente, raiz->nomePaciente) < 0)
        raiz->esq = inserirNome(raiz->esq,IDpaciente, nomePaciente, idadePaciente, condicaoMedica);
    else
        raiz->dir = inserirNome(raiz->dir,IDpaciente, nomePaciente, idadePaciente, condicaoMedica);
    return raiz;
}

// Função para imprimir em ordem de nome
void imprimirNomeEmOrdem(noPaciente *raiz)
{
    if(raiz)
    {
        imprimirNomeEmOrdem(raiz->esq);
		printf("%d %s", raiz->IDpaciente,raiz->nomePaciente);
		printf("\n");
		imprimirNomeEmOrdem(raiz->dir);	
    }
}
// Função para imprimir em ordem do ID
void imprimirIDEmOrdem(noPaciente *raiz)
{
    if(raiz)
    {
        imprimirIDEmOrdem(raiz->esq);
		printf("%d %s", raiz->IDpaciente, raiz->nomePaciente);
		printf("\n");
		imprimirIDEmOrdem(raiz->dir);	
    }
}
// Função para buscar um ID na arvore
noPaciente *buscarID(noPaciente *raiz, int IDpaciente)
{
    if(raiz == NULL)
        return NULL;
    if(IDpaciente == raiz->IDpaciente)
        return raiz;
    else
    {
        if(IDpaciente < raiz->IDpaciente)
            return buscarID(raiz->esq, IDpaciente);
        else
            return buscarID(raiz->dir, IDpaciente);
    }
}
// Função para buscar um nome na arvore
noPaciente *buscarNome(noPaciente *raiz, char *nomePaciente)
{ 
    if(raiz == NULL)
        return NULL;
    
    char nomeNormal[30];
    strcpy(nomeNormal, raiz->nomePaciente);  // Copia o conteúdo de raiz->nomePaciente para nomeNormal

    normalizarString(nomeNormal);
    normalizarString(nomePaciente);

    if(strcmp(nomePaciente, nomeNormal) == 0)
        return raiz;
    else
    {
        if(strcmp(nomePaciente, nomeNormal) < 0)
            return buscarNome(raiz->esq, nomePaciente);
        else
            return buscarNome(raiz->dir, nomePaciente);
    }
}

// Função para buscar um ID pai
noPaciente **buscarPaiID(noPaciente **raiz, int IDpaciente)
{
    if (*raiz == NULL)
        return NULL;
    while(*raiz!=NULL)
    {
        if((*raiz)->IDpaciente == IDpaciente)
            return raiz;
        if((*raiz)->IDpaciente < IDpaciente)
            raiz = &((*raiz)->dir);
        else
            raiz = &((*raiz)->esq);
    }
}

// Função para buscar um nome pai fazendo a normalização dos nomes e voltando a raiz
noPaciente **buscarPaiNome(noPaciente **raiz, char *nomePaciente) {
    if (*raiz == NULL)
        return NULL;

    // Normalizar nomePaciente de entrada uma vez
    char nomePacienteNormalizado[30];
    strcpy(nomePacienteNormalizado, nomePaciente);
    normalizarString(nomePacienteNormalizado);

    while (*raiz != NULL) {
        // Copia e normaliza o nome do nó atual para comparar com o nome normalizado
        char nomeNormalizadoNo[30];
        strcpy(nomeNormalizadoNo, (*raiz)->nomePaciente);
        normalizarString(nomeNormalizadoNo);

        int cmp = strcmp(nomePacienteNormalizado, nomeNormalizadoNo);
        if (cmp == 0) {
            return raiz;
        } else if (cmp < 0) {
            raiz = &((*raiz)->esq);
        } else {
            raiz = &((*raiz)->dir);
        }
    }
    return NULL;
}

// Função para remover um elemento usando como chave o IDpaciente
void removerID(noPaciente **raiz, int IDpaciente)
{
    noPaciente **pai=NULL;
    if((*raiz) == NULL)
        return;
    pai = buscarPaiID(raiz, IDpaciente);
    if((*pai)->esq == NULL && (*pai)->dir == NULL)
    {
        free(*pai);
        *pai = NULL;
        return;
    }
    else{
        if(((*pai)->esq != NULL) ^ (*pai)->dir !=NULL)
        {
            if((*pai)->esq != NULL)
            {
                noPaciente *paux=(*pai)->esq;
                free((*pai));
                *pai = paux;
            }
            if((*pai)->dir != NULL)
            {
                noPaciente *paux=(*pai)->dir;
                free((*pai));
                *pai = paux;
            }
            return;
        }
        else{
                noPaciente **paux = ppMenor(&(*pai)->dir);
                printf("\n %d", (*paux)->IDpaciente);
                printf("\nMenor %d %p", (*paux)->IDpaciente, paux);
                (*pai)->IDpaciente = (*paux)->IDpaciente;
                strcpy((*pai)->nomePaciente, (*paux)->nomePaciente);
                printf("\ntrocou chave...");
                removerID(paux, (*paux)->IDpaciente);
        }
    }
}

// Função para remover um elemento usando como chave o nomePaciente
void removerNome(noPaciente **raiz, char *nomePaciente) {
    noPaciente **pai = NULL;

    // Verifica se a árvore está vazia
    if ((*raiz) == NULL)
        return;

    // Busca o nó pai do paciente a ser removido
    pai = buscarPaiNome(raiz, nomePaciente);

    // Verifica se o paciente foi encontrado
    if (pai == NULL || *pai == NULL) {
        printf("Paciente não encontrado.\n");
        return;
    }

    // Caso o nó seja folha (sem filhos)
    if ((*pai)->esq == NULL && (*pai)->dir == NULL) {
        free(*pai);
        *pai = NULL;
        printf("Paciente Excluido com sucesso!\n");
        return;
    }

    // Caso o nó tenha apenas um filho
    else{
        if(((*pai)->esq != NULL) ^ (*pai)->dir !=NULL)
        {
            if((*pai)->esq != NULL)
            {
                noPaciente *paux=(*pai)->esq; //guardando end. filho
                free((*pai));
                *pai = paux;
                printf("Paciente Excluido com sucesso!\n");
            }
            if((*pai)->dir != NULL)
            {
                noPaciente *paux=(*pai)->dir; //guardando end. filho
                free((*pai));
                *pai = paux;
                printf("Paciente Excluido com sucesso!\n");
            }
            return;
        }

    // Caso o nó tenha dois filhos
    else{
            {
                noPaciente **paux = ppMenor(&(*pai)->dir); 
                strcpy((*pai)->nomePaciente, (*paux)->nomePaciente);
                removerNome(paux, (*paux)->nomePaciente);
            }
        }
    }
}
// Função para saber qual o menor elemento da arvore
noPaciente **ppMenor(noPaciente **raiz)
{
    noPaciente **pmenor=raiz;
    while((*pmenor)->esq)
    {
        printf("\n%d", (*pmenor)->IDpaciente);
        pmenor = &(*pmenor)->esq;
    }
    return pmenor;
}

// Função para salvar os dados num arquivo externo
void salvarArquivo(noPaciente *raiz, FILE *arq)
{
	if(raiz != NULL){
		//recursividade
		salvarArquivo(raiz->esq, arq);
		fprintf(arq, "%d %s %d %s\n", raiz->IDpaciente, raiz->nomePaciente, raiz->idadePaciente, raiz->condicaoMedica);
		salvarArquivo(raiz->dir, arq);
	}
}	
// Função para contar a quantidade de pacientes recursivamente
int contarPacientes(noPaciente *raiz) 
{
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarPacientes(raiz->esq) + contarPacientes(raiz->dir);
}
// Função para desalocar a arvore recursivamente
void desalocarArvore(noPaciente *raiz) 
{
    if (raiz != NULL) {
        desalocarArvore(raiz->esq);
        desalocarArvore(raiz->dir);
        free(raiz);
    }
}
// Função para normalizar uma string enviada, tirando seus caracteres em caixa alta e caracteres especiais
void normalizarString(char *str) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (isalpha(str[i])) {
            // Converter para minúscula e copiar para a nova posição
            str[j++] = tolower(str[i]);
        }
        i++;
    }
    // Adicionar o caractere nulo no final da string
    str[j] = '\0';
}