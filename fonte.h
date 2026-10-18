#ifndef ALUNO_H
#define ALUNO_H

typedef struct
{
    int dia;
    int mes;
    int ano;
} data;

typedef struct
{
    float preco;
    char descriçao[100];
    data data_compra;
} compra;

typedef struct CompraNode
{
    compra despesa;
    struct CompraNode *next;
} CompraNode;

typedef struct
{
    char nome[100];
    data nascimento;
    char curso[100];
    int ano;
    int numero;
    float dinheiro;
    CompraNode *compras;
} aluno;

typedef struct Node
{
    aluno aluno;
    struct Node *next;
} Node;

typedef struct
{
    Node *front;
    Node *rear;
} Queue;

#endif