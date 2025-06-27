#include <stdio.h> // biblioteca padrão 
#include <string.h> // biblioteca usada para manipulação de strings
#include "Structs.h" // arquivo com definições das structs Cliente, Produto, Venda

Cliente clientes[MAX_CLIENTES]; 
Produto produtos[MAX_PRODUTOS];
Venda vendas[MAX_VENDAS];

// Variáveis para controlar quantos clientes, produtos e vendas foram cadastrados
int total_clientes = 0, total_produtos = 0, total_vendas = 0;

//função para cadastrar um novo cliente no sistema
void cadastrar_cliente() {
    if (total_clientes < MAX_CLIENTES) { //verifica se ainda é possível cadastrar mais clientes
        Cliente c;
        c.id = total_clientes + 1; //atribui um ID automático ao cliente
        printf("Nome do cliente: ");
        scanf(" %[^\n]", c.nome); //lê o nome completo com espaços
        printf("CPF do cliente: ");
        scanf(" %14s", c.cpf); //lê o CPF
        clientes[total_clientes++] = c; //salva o cliente no vetor e incrementa o contador
        printf("Cliente cadastrado com ID %d\n", c.id);
    } else {
        printf("Limite de clientes atingido.\n"); //mensagem se atingir o limite máximo de clientes
    }
}

//função para exibir todos os clientes cadastrados
void listar_clientes() {
    if (total_clientes == 0) { //verifica se há clientes cadastrados
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    for (int i = 0; i < total_clientes; i++) {
        //imprime os dados de cada cliente
        printf("ID: %d | Nome: %s | CPF: %s\n", clientes[i].id, clientes[i].nome, clientes[i].cpf);
    }
}

//função para cadastrar um novo produto no sistema
void cadastrar_produto() {
    if (total_produtos < MAX_PRODUTOS) { //verifica se ainda é possível cadastrar mais produtos
        Produto p;
        p.id = total_produtos + 1; //atribui um ID automático
        printf("Nome do produto: ");
        scanf(" %[^\n]", p.nome); //lê o nome do produto
        printf("Preço unitário: ");
        scanf("%f", &p.preco); //lê o valor do produto
        printf("Estoque disponível: ");
        scanf("%d", &p.estoque); //lê a quantidade disponível em estoque
        produtos[total_produtos++] = p; //salva o produto e incrementa o contador
        printf("Produto cadastrado com ID %d\n", p.id);
    } else {
        printf("Limite de produtos atingido.\n"); //alerta se atingir o limite de produtos
    }
}

//função para exibir todos os produtos cadastrados
void listar_produtos() {
    if (total_produtos == 0) {
        printf("Nenhum produto cadastrado.\n"); //verifica se há produtos cadastrados
        return;
    }
    for (int i = 0; i < total_produtos; i++) {
        //exibe os dados de cada produto
        printf("ID: %d | Nome: %s | Preço: R$ %.2f | Estoque: %d\n",
               produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].estoque);
    }
}

//função para cadastrar uma venda no sistema
void cadastrar_venda() {
    if (total_vendas >= MAX_VENDAS) { //verifica se é possível registrar mais vendas
        printf("Limite de vendas atingido.\n");
        return;
    }

    Venda v;
    v.id = total_vendas + 1; //atribui ID à venda

    printf("ID do cliente: ");
    if (scanf("%d", &v.cliente_id) != 1) { //lê e valida o ID do cliente
        printf("Entrada inválida! Retornando ao menu...\n");
        while (getchar() != '\n');
        return;
    }

    //verifica se o cliente existe
    int cliente_existe = 0;
    for (int i = 0; i < total_clientes; i++) {
        if (clientes[i].id == v.cliente_id) {
            cliente_existe = 1;
            break;
        }
    }
    if (!cliente_existe) {
        printf("Cliente não encontrado.\n");
        return;
    }

    printf("Data da venda (DD/MM/AAAA): ");
    scanf(" %10s", v.data); //lê a data da venda

    printf("Quantos itens na venda? ");
    if (scanf("%d", &v.total_itens) != 1 || v.total_itens <= 0 || v.total_itens > MAX_ITENS_VENDA) {
        printf("Quantidade inválida. Retornando ao menu...\n");
        while (getchar() != '\n');
        return;
    }

    v.total = 0; //iinicializa o total da venda

    //loop para registrar os itens da venda
    for (int i = 0; i < v.total_itens; i++) {
        int pid, qtd;
        printf("ID do produto %d: ", i + 1);
        if (scanf("%d", &pid) != 1) {
            printf("ID inválido. Retornando ao menu...\n");
            while (getchar() != '\n');
            return;
        }

        //verifica se o produto existe
        Produto *p = NULL;
        for (int j = 0; j < total_produtos; j++) {
            if (produtos[j].id == pid) {
                p = &produtos[j];
                break;
            }
        }

        if (!p) {
            printf("Produto não encontrado.\n");
            i--; //permite tentar novamente
            continue;
        }

        printf("Quantidade: ");
        if (scanf("%d", &qtd) != 1 || qtd <= 0) {
            printf("Quantidade inválida. Retornando ao menu...\n");
            while (getchar() != '\n');
            return;
        }

        if (qtd > p->estoque) {
            printf("Estoque insuficiente!\n");
            i--; //tenta novamente se o estoque não for suficiente
            continue;
        }

        //registra os dados do item na venda
        v.itens[i].produto_id = pid;
        v.itens[i].quantidade = qtd;
        v.itens[i].valor_unitario = p->preco;
        v.total += qtd * p->preco; //soma ao total da venda

        p->estoque -= qtd; //atualiza o estoque
    }

    vendas[total_vendas++] = v; //armazena a venda
    printf("Venda registrada! Total da venda: R$ %.2f\n", v.total);
}

//função para listar todas as vendas registradas
void listar_vendas() {
    if (total_vendas == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }
    for (int i = 0; i < total_vendas; i++) {
        Venda *v = &vendas[i];
        printf("\nVenda ID: %d | Cliente ID: %d | Data: %s | Total: R$ %.2f\n", v->id, v->cliente_id, v->data, v->total);
        for (int j = 0; j < v->total_itens; j++) {
            printf("  Produto ID: %d | Quantidade: %d | Valor unitário: R$ %.2f\n",
                   v->itens[j].produto_id, v->itens[j].quantidade, v->itens[j].valor_unitario);
        }
    }
}

//função para atualizar os dados de um cliente
void atualizar_cliente() {
    int id;
    printf("Informe o ID do cliente a ser atualizado: ");
    
    if (scanf("%d", &id) != 1) {
        printf("Entrada inválida! Retornando ao menu...\n");
        while (getchar() != '\n');
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < total_clientes; i++) {
        if (clientes[i].id == id) {
            //mostra os dados atuais
            printf("Cliente encontrado: %s (CPF: %s)\n", clientes[i].nome, clientes[i].cpf);
            printf("Novo nome: ");
            scanf(" %[^\n]", clientes[i].nome);
            printf("Novo CPF: ");
            scanf(" %14s", clientes[i].cpf);
            printf("Cliente atualizado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Cliente com ID %d não encontrado.\n", id);
    }
}

//função para excluir um cliente do sistema
void excluir_cliente() {
    int id;
    printf("Informe o ID do cliente a ser excluído: ");

    if (scanf("%d", &id) != 1) {
        printf("Entrada inválida! Retornando ao menu...\n");
        while (getchar() != '\n');
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < total_clientes; i++) {
        if (clientes[i].id == id) {
            //remove o cliente e reorganiza o vetor
            for (int j = i; j < total_clientes - 1; j++) {
                clientes[j] = clientes[j + 1];
            }
            total_clientes--;
            printf("Cliente excluído com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Cliente com ID %d não encontrado.\n", id);
    }
}

//função para atualizar os dados de um produto
void atualizar_produto() {
    int id;
    printf("Informe o ID do produto a ser atualizado: ");
    if (scanf("%d", &id) != 1) {
        printf("Entrada inválida! Retornando ao menu...\n");
        while (getchar() != '\n');
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < total_produtos; i++) {
        if (produtos[i].id == id) {
            //exibe dados atuais e permite alteração
            printf("Produto encontrado: %s | Preço: R$ %.2f | Estoque: %d\n",
                   produtos[i].nome, produtos[i].preco, produtos[i].estoque);
            printf("Novo nome: ");
            scanf(" %[^\n]", produtos[i].nome);
            printf("Novo preço: ");
            scanf("%f", &produtos[i].preco);
            printf("Novo estoque: ");
            scanf("%d", &produtos[i].estoque);
            printf("Produto atualizado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Produto com ID %d não encontrado.\n", id);
    }
}

//função para excluir um produto do sistema
void excluir_produto() {
    int id;
    printf("Informe o ID do produto a ser excluído: ");
    if (scanf("%d", &id) != 1) {
        printf("Entrada inválida! Retornando ao menu...\n");
        while (getchar() != '\n');
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < total_produtos; i++) {
        if (produtos[i].id == id) {
            for (int j = i; j < total_produtos - 1; j++) {
                produtos[j] = produtos[j + 1]; //reorganiza o vetor
            }
            total_produtos--;
            printf("Produto excluído com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Produto com ID %d não encontrado.\n", id);
    }
}

//função principal - exibe o menu e executa as ações escolhidas pelo usuário
int main() {
    int opcao;

    do { //menu principal
        printf("\nBem-vindo ao MENU, o que deseja fazer?:\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Cadastrar Produto\n");
        printf("3 - Cadastrar Venda\n");
        printf("4 - Listar Clientes\n");
        printf("5 - Listar Produtos\n");
        printf("6 - Listar Vendas\n");
        printf("7 - Atualizar Cliente\n");
        printf("8 - Excluir Cliente\n");
        printf("9 - Atualizar Produto\n");
        printf("10 - Excluir Produto\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao); //lê a opção do usuário

        //verifica a opção escolhida e chama a função correspondente
        switch (opcao) {
            case 1: cadastrar_cliente(); break;
            case 2: cadastrar_produto(); break;
            case 3: cadastrar_venda(); break;
            case 4: listar_clientes(); break;
            case 5: listar_produtos(); break;
            case 6: listar_vendas(); break;
            case 7: atualizar_cliente(); break;
            case 8: excluir_cliente(); break;
            case 9: atualizar_produto(); break;
            case 10: excluir_produto(); break;
            case 0: printf("Encerrando programa.\n"); break;
            default: printf("Opção inválida.\n");
        }

    } while (opcao != 0); //repete o menu até o usuário digitar 0

    return 0;
}
