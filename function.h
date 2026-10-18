#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "fonte.h"

#define POR_NOME 1
#define POR_NUMERO 2

void ler_ficheiro(Queue *q);
void atualizar_ficheiro(Queue *q);

int comparar_alunos(const void *a, const void *b);

aluno *procurar_aluno_numero(Queue *q, int numero);
aluno *procurar_aluno_nome(Queue *q, char *nome);

void introduzir_aluno(Queue *q, aluno novo_aluno);
void eliminar_aluno(Queue *q, int criterio, char *nome, int numero);
void listar_alfabeticamente(Queue *q);

void despesa(aluno *a, float preco, char *descricao, int dia, int mes, int ano);
void carregar(aluno *aluno, float dinheiro);
void listar_despesas_aluno(aluno *a);
void libertar(Queue *q);

#endif