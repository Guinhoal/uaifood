
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <locale.h>
#include <unistd.h>
// Invocação das biblioteca.

typedef struct restaurante
{
    int rest;
    char nome[500];
} restaurante;
// Struct que representa o restaurante

typedef struct prato
{
    int codigo_prato;
    char nome_prato[100];
    float preco_prato;
    int codigorestaurante;
} prato;

typedef struct cliente
{
    int codigo_cliente;
    char nome_cliente[100];
} cliente;
// Struct que representa os dados do cliente

typedef struct pedido
{
    int codigo_cliente;
    int codigo_restaurante;
    int codigo_prato;
    int quantidade;
    float preco;
} pedido;
// Struct com os dados dos pedidos
#define TAXA_ENTREGA 5.0 // Valor hipotético, substitua pelo valor correto
float taxa_entrega = TAXA_ENTREGA;
#define PEDIDOS_FILENAME "Pedidos.bin"
prato *pratos;
restaurante *restaurantes;
cliente *clientes;
int tamanhorestaurantes = 3;
int tamanhopratos = 12;
int tamanhoclientes = 2;
int qntd;
pedido *pedidos = NULL; // Ponteiro para o vetor de pedidos
int num_pedidos = 0;    // Variável para rastrear o número de pedidos
FILE *fp;
FILE *fp2;
FILE *fp3;
FILE *fp4;

// Area destinada aos prtótipos das funções.
int escolhadouaifood();
int iniciorestaurante();
void preencheRestaurantes(restaurante *r);
void salvaRestaurantes(restaurante *r, int tamanho);
void restaurantespreviamentesalvos();
void pratospreviamentesalvos();
void newrestaurante();
void salvaPratos(prato *p, int tamanho);
int menu(int x);
void trocadenome(restaurante *r);
void alterarpratos(restaurante *r, int num_prato);
int loginrestaurantes();
void cadastrarNovoPrato(restaurante *r, int *tamanhoPratos);
void clientespreviamentesalvos();
void salvaClientes(cliente *c, int tamanho);
int loginpedidos();
void newcliente();
int iniciopedidos();
int menucliente(int codecliente);
int fazerpedido(int codigodocliente);
int escolhadoprato(int coderestaurante, int codigocliente);
void adiciona_pedido(int cliente_id, int restaurante_id, int prato_id, int quantidade, float preco);
void pedido_fechado(int cliente_id);
void salvaPedidos(pedido *p, int tamanho);

int main()
{
    setlocale(LC_ALL, "Portuguese");
    fp = fopen("Restaurantes.bin", "a+b");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }
    fp2 = fopen("Pratos.bin", "a+b");
    if (fp2 == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }
    fp3 = fopen("Clientes.bin", "a+b");
    if (fp3 == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }
    fp4 = fopen(PEDIDOS_FILENAME, "a+b");
    if (fp4 == NULL)
    {
        printf("Erro ao abrir o arquivo de pedidos.\n");
        exit(EXIT_FAILURE);
    }
    restaurantes = (restaurante *)malloc(tamanhorestaurantes * sizeof(restaurante));
    if (restaurantes == NULL)
    {
        printf("Erro na alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    pratos = (prato *)malloc(tamanhopratos * sizeof(prato));
    if (pratos == NULL)
    {
        printf("Erro na alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    clientes = (cliente *)malloc(tamanhoclientes * sizeof(cliente));
    if (clientes == NULL)
    {
        printf("Erro na alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    pedidos = (pedido *)malloc(sizeof(pedido));
    if (pedidos == NULL)
    {
        printf("Erro na alocação de memória para pedidos.\n");
        exit(EXIT_FAILURE);
    }

    restaurantespreviamentesalvos();
    salvaRestaurantes(restaurantes, tamanhorestaurantes);
    pratospreviamentesalvos();
    salvaPratos(pratos, tamanhopratos);
    clientespreviamentesalvos();
    salvaClientes(clientes, tamanhoclientes);

    int option, option2, rest, verif, option3, option4, verif2, verif3;
    while (1)
    {
        option = escolhadouaifood();
        if (option == 1)
        {
            option2 = iniciorestaurante();
            switch (option2)
            {
            case 1:
                newrestaurante();
                break;
            case 2:
                rest = loginrestaurantes() - 1;
                if (rest == -4)
                {
                    break;
                }
                verif = menu(rest);
                if (verif == -3)
                {
                    break;
                }
                break;
            case 3:
                escolhadouaifood();
                break;
            }
        }

        else if (option == 2)
        {
            option3 = loginpedidos();
            switch (option3)
            {
            case 1:
                newcliente();
                break;
            case 2:
                option4 = iniciopedidos();
                if (option4 == -3)
                {
                    break;
                }
                verif2 = menucliente(option4);
                if (verif2 == -3)
                {
                    break;
                }
                else if (verif2 == 3)
                {
                    verif3 = fazerpedido(option4);
                    if (verif3 == 1)
                    {
                    }
                    if (verif3 == -1)
                    {
                        menucliente(option4);
                    }
                }

                break;
            case 3:
                printf("Selecione uma opção válida.\n");
                escolhadouaifood();
                break;
            }
        }
    }
    return 0;
}
// Função main (principal do programa)

void salvaRestaurantes(restaurante *r, int tamanho)
{
    fwrite(r, sizeof(restaurante), tamanho, fp);
}

void salvaPratos(prato *p, int tamanho)
{
    fwrite(p, sizeof(prato), tamanho, fp2);
}

void salvaClientes(cliente *c, int tamanho)
{
    fwrite(c, sizeof(cliente), tamanho, fp3);
}
void salvaPedidos(pedido *p, int tamanho)
{
    fseek(fp4, 0, SEEK_SET);
    fwrite(p, sizeof(pedido), num_pedidos, fp4);
}
int escolhadouaifood()
{
    int option;
    do
    {
        printf("uaiFood - Menu Principal\n");
        printf("1 - uaiFood Restaurante\n");
        printf("2 - uaiFood Pedidos\n");
        printf("-1 - Sair\n");
        scanf("%d", &option);
        fflush(stdin);
        switch (option)
        {
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        case -1:
            printf("Saindo...\n");
            sleep(5);
            exit(0);
            break;
        default:
            printf("Opção inválida!\n");
            break;
        }
    } while (option != -1);
}
// Seleciona qual dos aplicativos uaifood vai acessar.

int iniciorestaurante()
{
    while (1)
    {
        int option;
        printf("**************************************************\n");
        printf("************** uaiFood Restaurante **************\n");
        printf("**************************************************\n");
        printf("Selecione a opção desejada:\n");
        printf("1 - Criar Restaurante\n");
        printf("2 - Login\n");
        printf("3 - Voltar para a tela inicial\n");
        printf("-1 - Sair do programa\n");
        scanf("%d", &option);
        fflush(stdin);
        switch (option)
        {
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        case 3:
            return 3;
            break;
        case -1:
            printf("Saindo...\n");
            sleep(5);
            exit(0);
            break;
        default:
            printf("Opção inválida!\n");
            break;
        }
    }
}

void newrestaurante()
{
    tamanhorestaurantes++;
    restaurantes = realloc(restaurantes, tamanhorestaurantes * sizeof(restaurante));
    if (restaurantes == NULL)
    {
        printf("Erro ao realocar memória para o array de restaurantes!\n");
        exit(1);
    }
    printf("**************************************************\n");
    printf("************** uaiFood Restaurante **************\n");
    printf("**************************************************\n");
    printf("Criar novo restaurante - Digite o nome do restaurante\n");
    scanf(" %[^\n]", restaurantes[tamanhorestaurantes - 1].nome);
    restaurantes[tamanhorestaurantes - 1].rest = restaurantes[tamanhorestaurantes - 2].rest + 1;

    salvaRestaurantes(restaurantes, tamanhorestaurantes);
}

int loginrestaurantes()
{
    while (1)
    {
        int codigorestaurante;
        printf("Digite o código do restaurante: ");
        scanf("%d", &codigorestaurante);
        fflush(stdin);
        for (int i = 0; i < tamanhorestaurantes; i++)
        {
            if (codigorestaurante == restaurantes[i].rest)
            {
                return codigorestaurante;
            }
        }
        printf("Código inválido!\n");
        return -3;
    }
}

void cadastrarNovoPrato(restaurante *r, int *tamanhoPratos)
{
    if (*tamanhoPratos >= 100)
    {
        printf("Limite de pratos atingido. Não é possível adicionar mais pratos.\n");
        return;
    }

    (*tamanhoPratos)++;
    pratos = realloc(pratos, (*tamanhoPratos) * sizeof(prato));
    if (pratos == NULL)
    {
        printf("Erro ao realocar memória para o array de pratos!\n");
        exit(1);
    }

    printf("**************************************************\n");
    printf("************** uaiFood Restaurante **************\n");
    printf("**************************************************\n");
    printf("Cadastrar novo prato - Digite o nome do prato: ");
    scanf(" %[^\n]", pratos[*tamanhoPratos - 1].nome_prato);

    printf("Digite o preço do prato: ");
    scanf("%f", &pratos[*tamanhoPratos - 1].preco_prato);

    int proximoCodigo = 1;
    for (int i = 0; i < *tamanhoPratos; i++)
    {
        if (pratos[i].codigorestaurante == r->rest && pratos[i].codigo_prato >= proximoCodigo)
        {
            proximoCodigo = pratos[i].codigo_prato + 1;
        }
    }

    pratos[*tamanhoPratos - 1].codigo_prato = proximoCodigo;
    pratos[*tamanhoPratos - 1].codigorestaurante = r->rest;

    printf("Prato cadastrado com sucesso!\n");
    salvaPratos(pratos, *tamanhoPratos);
}

int menu(int x)
{
    int num_restaurantes = 3;
    int sel, prato;
    int pratoIndex = -1;
    while (1)
    {
        printf("%s – Menu de Opções\n", restaurantes[x].nome);
        printf("1 – Renomear\n");
        printf("2 – Deletar\n");
        printf("3 – Mostrar pratos\n");
        printf("0 – Cadastrar novo prato\n");
        printf("-1 – Voltar ao menu anterior\n");
        scanf("%i", &sel);
        fflush(stdin);
        switch (sel)
        {
        case 3:
            while (1)
            {
                printf("%s – Pratos - Menu de Opções\n", restaurantes[x].nome);
                for (int i = 0; i < 100; i++)
                {
                    if (pratos[i].codigorestaurante == x + 1)
                    {
                        printf("%d - %s - R$ %.2f\n", pratos[i].codigo_prato, pratos[i].nome_prato, pratos[i].preco_prato);
                    }
                }
                printf("0 - Cadastrar novo prato\n");
                printf("-1 – Voltar ao menu anterior\n");

                printf("Digite o código do prato a ser alterado (ou -1 para voltar): ");
                scanf("%d", &prato);

                if (prato == -1)
                {
                    break;
                }
                else
                {
                    for (int i = 0; i < 100; i++)
                    {
                        if (pratos[i].codigo_prato == prato)
                        {
                            pratoIndex = i;
                            break;
                        }
                    }

                    if (pratoIndex != -1 && pratoIndex != 0)
                    {
                        alterarpratos(&restaurantes[x], prato);
                    }
                    else
                    {
                        printf("Código de prato inválido. Tente novamente.\n");
                        continue;
                    }
                }
            }
            break;

        case 0:
            cadastrarNovoPrato(&restaurantes[x], &tamanhopratos);
            break;

        case 1:
            trocadenome(&restaurantes[x]);
            printf("Renomeado:\n%s\n", restaurantes[x].nome);
            break;

        case 2:
            restaurantes[x].rest = 0;
            printf("\nDeletado com sucesso!\n");
            return -3;
            break;
        case -1:
            return -3;
            break;
        default:
            printf("Opção não encontrada\n");
            continue;
            break;
        }
    }
}

void alterarpratos(restaurante *r, int num_prato)
{
    int opc;
    while (1)
    {
        printf("%s - Alterar Prato:\n", r->nome);
        for (int i = 0; i < tamanhopratos; i++)
        {
            if (pratos[i].codigorestaurante == r->rest && pratos[i].codigo_prato == num_prato)
            {
                printf("Prato %d - %s - Preço: R$ %.2f\n", pratos[i].codigo_prato, pratos[i].nome_prato, pratos[i].preco_prato);
                break;
            }
        }
        printf("1 - Renomear Prato\n");
        printf("2 - Alterar Preço\n");
        printf("3 - Deletar Prato\n");
        printf("4 - Voltar ao menu anterior\n");
        scanf("%d", &opc);
        fflush(stdin);
        switch (opc)
        {
        case 1:
            printf("Digite o novo nome para o prato: ");
            char novo_nome[50];
            fgets(novo_nome, sizeof(novo_nome), stdin);
            if (novo_nome[strlen(novo_nome) - 1] == '\n')
                novo_nome[strlen(novo_nome) - 1] = '\0';

            // Encontrar o prato no array e atualizar o nome
            for (int i = 0; i < tamanhopratos; i++)
            {
                if (pratos[i].codigorestaurante == r->rest && pratos[i].codigo_prato == num_prato)
                {
                    strcpy(pratos[i].nome_prato, novo_nome);
                    printf("Nome do prato alterado com sucesso!\n");
                    break;
                }
            }
            break;

        case 2:
            printf("Digite o novo preço para o prato: ");
            float novo_preco;
            scanf("%f", &novo_preco);

            // Encontrar o prato no array e atualizar o preço
            for (int i = 0; i < tamanhopratos; i++)
            {
                if (pratos[i].codigorestaurante == r->rest && pratos[i].codigo_prato == num_prato)
                {
                    pratos[i].preco_prato = novo_preco;
                    printf("Preço do prato alterado com sucesso!\n");
                    break;
                }
            }
            break;

        case 3:
            // Encontrar o prato no array e marcá-lo como excluído
            for (int i = 0; i < tamanhopratos; i++)
            {
                if (pratos[i].codigorestaurante == r->rest && pratos[i].codigo_prato == num_prato)
                {
                    pratos[i].codigo_prato = 0;
                    pratos[i].codigorestaurante = 0;
                    pratos[i].nome_prato[0] = '\0';
                    pratos[i].preco_prato = 0.0;
                    printf("Prato excluído com sucesso!\n");
                    break;
                }
            }
            break;

        case 4:
            return;

        default:
            printf("Opção não encontrada\n");
            break;
        }
    }
}

void trocadenome(restaurante *r)
{
    char novo_nome[100];
    printf("Digite o novo nome: ");
    scanf(" %[^\n]s", novo_nome);
    strcpy(r->nome, novo_nome);
}

int loginpedidos()
{
    while (1)
    {
        int option;
        printf("******************************************\n");
        printf("************** uaiFood Pedidos **************\n");
        printf("******************************************\n");
        printf("Selecione a opção desejada:\n");
        printf("1 - Criar Cliente\n");
        printf("2 - Fazer Login\n");
        printf("-1 - Voltar para a tela inicial\n");
        printf("0 - Sair do programa\n");
        scanf("%d", &option);
        fflush(stdin);
        switch (option)
        {
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        case -1:
            return 3;
            break;
        case 0:
            printf("Saindo...\n");
            sleep(5);
            exit(0);
            break;
        default:
            printf("Opção inválida!\n");
            break;
        }
    }
}

void newcliente()
{
    tamanhoclientes++;
    clientes = realloc(clientes, tamanhoclientes * sizeof(cliente));
    if (clientes == NULL)
    {
        printf("Erro ao realocar memória para o array de clientes!\n");
        exit(1);
    }
    printf("**************************************************\n");
    printf("************** uaiFood Pedidos **************\n");
    printf("**************************************************\n");
    printf("Criar novo cliente - Digite o nome do cliente\n");
    scanf(" %[^\n]", clientes[tamanhoclientes - 1].nome_cliente);
    clientes[tamanhoclientes - 1].codigo_cliente = tamanhoclientes;

    salvaClientes(clientes, tamanhoclientes);
}

int iniciopedidos()
{
    int codigodocliente;
    printf("Digite o código do cliente (ou 0 para sair):");
    scanf("%d", &codigodocliente);
    fflush(stdin);
    for (int i = 0; i < tamanhoclientes; i++)
    {
        if (codigodocliente == 0)
        {
            return -3;
        }
        if (codigodocliente == clientes[i].codigo_cliente)
        {
            return codigodocliente;
        }
    }
    printf("Código inválido!\n");
    return -3;
}

int menucliente(int codecliente)
{
    char novo_nome[50];
    int opc;
    printf("Bem Vindo %s - Selecione a opção desejada\n", clientes[codecliente - 1].nome_cliente);
    ~printf("1 - Renomear Cliente\n");
    printf("2 - Deletar Cliente\n");
    printf("3 - Fazer Pedido\n");
    printf("4 - Mostrar Histórico de pedidos\n");
    printf("-1 - fazer Logout\n");
    scanf("%d", &opc);
    fflush(stdin);
    switch (opc)
    {
    case 1:
        printf("Digite o novo nome: ");
        fgets(novo_nome, sizeof(novo_nome), stdin);
        if (novo_nome[strlen(novo_nome) - 1] == '\n')
            novo_nome[strlen(novo_nome) - 1] = '\0';

        for (int i = 0; i < tamanhoclientes; i++)
        {
            if (clientes[i].codigo_cliente == codecliente)
            {
                strcpy(clientes[i].nome_cliente, novo_nome);
                printf("Nome do cliente alterado com sucesso!\n");
                break;
            }
        }
        break;
    case 2:
        clientes[codecliente - 1].codigo_cliente = 0;
        printf("\nDeletado com sucesso!\n");
        return -3;
        break;
    case 3:
        return 3;
        break;
    case 4:
        // Neste case já irei chamara a função que mostra o histórico de forma direta
        break;
    case -1:
        return -3;
        break;
    }
}

int fazerpedido(int codigodocliente)
{
    int nc = codigodocliente;
    int op;
    int opcao;
    while (1)
    {
        printf("%s - BEM VINDO  - Selecione o restaurante\n", clientes[nc - 1].nome_cliente);
        for (int i = 0; i < tamanhorestaurantes; i++)
        {
            printf("%d - %s\n", restaurantes[i].rest, restaurantes[i].nome);
        }
        printf("0 - Fechar Pedido\n");
        printf("-1 Voltar ao menu anterior\n");
        scanf("%d", &op);
        fflush(stdin);
        switch (op)
        {
        case 0:
            printf("Fechando o pedido ...");
            sleep(5);
            pedido_fechado(nc);
            // Aqui chmarei a função que fecha o pedido.
            break;
        case -1:
            return -1;
            break;
        default:
            // Verificar se o código do restaurante escolhido é válido
            if (op > 0 && op <= tamanhorestaurantes)
            {
                // Agora você pode passar o código do restaurante selecionado para outra função, se necessário.
                printf("Restaurante escolhido: %s\n", restaurantes[op - 1].nome);
                escolhadoprato(op, nc);
                // Adicione aqui qualquer lógica adicional que você precisar
            }
            else
            {
                printf("Opção Inválida!\n");
            }
        }
    }
}

int escolhadoprato(int coderestaurante, int codigocliente)
{
    int op;
    int nc = codigocliente;

    while (1)
    {
        printf("%s - Selecione o prato\n", restaurantes[coderestaurante - 1].nome);
        for (int i = 0; i < tamanhopratos; i++)
        {
            if (pratos[i].codigorestaurante == coderestaurante)
            {
                printf("%d - %s - R$ %.2f\n", pratos[i].codigo_prato, pratos[i].nome_prato, pratos[i].preco_prato);
            }
        }
        printf("0 - Fechar Pedido\n");
        printf("-1 Voltar ao menu anterior\n");
        scanf("%d", &op);
        fflush(stdin);

        switch (op)
        {
        case 0:
            printf("Fechando o pedido ...\n");
            sleep(5);
            pedido_fechado(codigocliente);
            // Aqui chamará a função que fecha o pedido.
            return 0;
            break;

        case -1:
            return -1;
            break;

        default:
            // Verificar se o código do restaurante escolhido é válido
            if (op > 0 && op <= tamanhopratos)
            {
                // Agora você pode passar o código do prato selecionado para outra função, se necessário.
                printf("Prato escolhido: %s\n", pratos[op - 1].nome_prato);
                printf("Digite a quantidade:\n");
                scanf("%d", &qntd);
                fflush(stdin);
                adiciona_pedido(nc, coderestaurante, op, qntd, pratos[op - 1].preco_prato);
                printf("Pedido anotado com sucesso!\n");
                // Adicione aqui qualquer lógica adicional que você precisar
            }
            else
            {
                printf("Opção Inválida!\n");
            }
        }
    }
}

void adiciona_pedido(int cliente_id, int restaurante_id, int prato_id, int quantidade, float preco)
{
    // Aloca espaço para um novo pedido
    pedido *novo_pedido = (pedido *)malloc(sizeof(pedido));

    if (novo_pedido == NULL)
    {
        printf("Erro na alocação de memória para o pedido.\n");
        exit(1); // Ou trate o erro de alocação adequadamente
    }

    // Preenche os detalhes do pedido
    novo_pedido->codigo_cliente = cliente_id;
    novo_pedido->codigo_restaurante = restaurante_id;
    novo_pedido->codigo_prato = prato_id;
    novo_pedido->quantidade = quantidade;
    novo_pedido->preco = preco;

    // Realoca o vetor de pedidos para incluir o novo pedido
    num_pedidos++;
    pedidos = (pedido *)realloc(pedidos, num_pedidos * sizeof(pedido));

    if (pedidos == NULL)
    {
        printf("Erro na realocação do vetor de pedidos.\n");
        exit(1); // Ou trate o erro de alocação adequadamente
    }

    // Adiciona o novo pedido ao vetor
    pedidos[num_pedidos - 1] = *novo_pedido;

    free(novo_pedido); // Libera o pedido temporário
}
void pedido_fechado(int cliente_id)
{
    printf("\nResumo do Pedido - %s\n", clientes[cliente_id - 1].nome_cliente);
    printf("**************************************************\n");

    float total_pedido = 0.0;

    for (int i = 0; i < num_pedidos; i++)
    {
        int restaurante_id = pedidos[i].codigo_restaurante;
        int prato_id = pedidos[i].codigo_prato;
        int quantidade = pedidos[i].quantidade;
        float preco_unitario = 0.0; // Inicialize com 0, pois a busca deve ser feita no vetor de pratos

        // Buscar o preço do prato no vetor de pratos
        for (int j = 0; j < tamanhopratos; j++)
        {
            if (pratos[j].codigo_prato == prato_id && pratos[j].codigorestaurante == restaurante_id)
            {
                preco_unitario = pratos[j].preco_prato;
                break;
            }
        }

        printf("%s\n", restaurantes[restaurante_id - 1].nome);
        printf("%dx %s = R$ %.2f\n", quantidade, pratos[prato_id - 1].nome_prato, quantidade * preco_unitario);

        total_pedido += quantidade * preco_unitario;
    }

    // Adiciona a taxa de entrega para cada restaurante diferente
    float taxa_total = taxa_entrega * (float)num_pedidos;
    printf("Taxa de entrega x %d = R$ %.2f\n", num_pedidos, taxa_total);

    printf("---------------------------------------------------------------------\n");
    printf("TOTAL DO PEDIDO = %.2f\n", total_pedido + taxa_total);
}

void restaurantespreviamentesalvos()
{
    strcpy(restaurantes[0].nome, "Tempero Mineiro");
    restaurantes[0].rest = 1;

    strcpy(restaurantes[1].nome, "Fogo e Sabor");
    restaurantes[1].rest = 2;

    strcpy(restaurantes[2].nome, "Serve Bem");
    restaurantes[2].rest = 3;
}

void pratospreviamentesalvos()
{
    pratos[0].codigo_prato = 1;
    pratos[0].codigorestaurante = 1;
    strcpy(pratos[0].nome_prato, "Lombo Assado");
    pratos[0].preco_prato = 65.99;
    pratos[1].codigo_prato = 2;
    pratos[1].codigorestaurante = 1;
    strcpy(pratos[1].nome_prato, "Tropeiro");
    pratos[1].preco_prato = 25.00;
    pratos[2].codigo_prato = 3;
    pratos[2].codigorestaurante = 1;
    strcpy(pratos[2].nome_prato, "Tutu de Feijão");
    pratos[2].preco_prato = 19.90;
    pratos[3].codigo_prato = 4;
    pratos[3].codigorestaurante = 1;
    strcpy(pratos[3].nome_prato, "Arroz Carreteiro");
    pratos[3].preco_prato = 27.50;

    pratos[4].codigo_prato = 1;
    pratos[4].codigorestaurante = 2;
    strcpy(pratos[4].nome_prato, "Picanha");
    pratos[4].preco_prato = 69.90;
    pratos[5].codigo_prato = 2;
    pratos[5].codigorestaurante = 2;
    strcpy(pratos[5].nome_prato, "Maminha");
    pratos[5].preco_prato = 59.90;
    pratos[6].codigo_prato = 3;
    pratos[6].codigorestaurante = 2;
    strcpy(pratos[6].nome_prato, "Asinha de Frango");
    pratos[6].preco_prato = 25.70;
    pratos[7].codigo_prato = 4;
    pratos[7].codigorestaurante = 2;
    strcpy(pratos[7].nome_prato, "Contra Filé");
    pratos[7].preco_prato = 65.00;

    pratos[8].codigo_prato = 1;
    pratos[8].codigorestaurante = 3;
    strcpy(pratos[8].nome_prato, "Salada");
    pratos[8].preco_prato = 9.00;
    pratos[9].codigo_prato = 2;
    pratos[9].codigorestaurante = 3;
    strcpy(pratos[9].nome_prato, "Arroz");
    pratos[9].preco_prato = 5.00;
    pratos[10].codigo_prato = 3;
    pratos[10].codigorestaurante = 3;
    strcpy(pratos[10].nome_prato, "Feijão");
    pratos[10].preco_prato = 3.50;
    pratos[11].codigo_prato = 4;
    pratos[11].codigorestaurante = 3;
    strcpy(pratos[11].nome_prato, "Kibe");
    pratos[11].preco_prato = 14.90;
}

void clientespreviamentesalvos()
{
    clientes[0].codigo_cliente = 1;
    strcpy(clientes[0].nome_cliente, "João Silva");
    clientes[1].codigo_cliente = 2;
    strcpy(clientes[1].nome_cliente, "Jose Almeida");
}