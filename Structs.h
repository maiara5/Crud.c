#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_CLIENTES 20
#define MAX_PRODUTOS 20
#define MAX_ITENS_VENDA 10
#define MAX_VENDAS 50

typedef struct {
    int id;
    char nome[50];
    char cpf[15];
} Cliente;

typedef struct {
    int id;
    char nome[50];
    float preco;
    int estoque;
} Produto;

typedef struct {
    int produto_id;
    int quantidade;
    float valor_unitario;
} ItemVenda;

typedef struct {
    int id;
    int cliente_id;
    char data[11];
    ItemVenda itens[MAX_ITENS_VENDA];
    int total_itens;
    float total;
} Venda;

#endif
