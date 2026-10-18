#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "fonte.h"
#include "function.h"

// auxiliares
static void libertar_compras(CompraNode *compras)
{
    while (compras != NULL)
    {
        CompraNode *temp = compras;
        compras = compras->next;
        free(temp);
    }
}
void libertar(Queue *q)
{
    Node *curr = q->front;
    while (curr != NULL)
    {
        Node *temp = curr;
        curr = curr->next;
        libertar_compras(temp->aluno.compras);
        free(temp);
    }

    q->front = NULL;
    q->rear = NULL;
}

aluno *procurar_aluno_numero(Queue *q, int numero)
{
    Node *curr = q->front;

    while (curr != NULL)
    {
        if (curr->aluno.numero == numero)
        {
            return &curr->aluno;
        }
        curr = curr->next;
    }

    return NULL;
}

aluno *procurar_aluno_nome(Queue *q, char *nome)
{
    Node *curr = q->front;

    while (curr != NULL)
    {
        if (strcmp(curr->aluno.nome, nome) == 0)
        {
            return &curr->aluno;
        }
        curr = curr->next;
    }

    return NULL;
}
// Funcões principais
void ler_ficheiro(Queue *q)
{
    FILE *f = fopen("dados.txt", "r");
    if (f == NULL)
    {
        printf("Aviso: nao foi possivel abrir dados.txt. O programa vai iniciar sem dados.\n");
        return;
    }

    char linha[256];
    Node *ultimo_aluno = NULL;

    while (fgets(linha, sizeof(linha), f) != NULL)
    {
        linha[strcspn(linha, "\n")] = '\0';

        if (strlen(linha) == 0)
        {
            continue;
        }

        int separadores = 0;
        for (int i = 0; linha[i] != '\0'; i++)
        {
            if (linha[i] == '|')
            {
                separadores++;
            }
        }

        if (separadores == 5)
        {
            aluno novo_aluno;
            novo_aluno.compras = NULL;

            int dia, mes, ano_nasc;
            if (sscanf(linha, " %99[^|]|%d/%d/%d|%99[^|]|%d|%d|%f",
                       novo_aluno.nome,
                       &dia, &mes, &ano_nasc,
                       novo_aluno.curso,
                       &novo_aluno.ano,
                       &novo_aluno.numero,
                       &novo_aluno.dinheiro) == 8)
            {

                novo_aluno.nascimento.dia = dia;
                novo_aluno.nascimento.mes = mes;
                novo_aluno.nascimento.ano = ano_nasc;

                introduzir_aluno(q, novo_aluno);
                ultimo_aluno = q->rear;
            }
            else
            {
                printf("Linha de aluno invalida: %s\n", linha);
            }
        }
        else if (separadores == 2)
        {
            if (ultimo_aluno == NULL)
            {
                printf("Erro: compra encontrada antes de qualquer aluno: %s\n", linha);
                continue;
            }

            CompraNode *nova_compra = (CompraNode *)malloc(sizeof(CompraNode));
            if (nova_compra == NULL)
            {
                printf("Erro de alocacao de memoria para compra.\n");
                fclose(f);
                return;
            }

            int dia, mes, ano_compra;
            if (sscanf(linha, " %f|%99[^|]|%d/%d/%d",
                       &nova_compra->despesa.preco,
                       nova_compra->despesa.descriçao,
                       &dia, &mes, &ano_compra) == 5)
            {

                nova_compra->despesa.data_compra.dia = dia;
                nova_compra->despesa.data_compra.mes = mes;
                nova_compra->despesa.data_compra.ano = ano_compra;
                nova_compra->next = NULL;

                if (ultimo_aluno->aluno.compras == NULL)
                {
                    ultimo_aluno->aluno.compras = nova_compra;
                }
                else
                {
                    CompraNode *aux = ultimo_aluno->aluno.compras;
                    while (aux->next != NULL)
                    {
                        aux = aux->next;
                    }
                    aux->next = nova_compra;
                }
            }
            else
            {
                printf("Linha de compra invalida: %s\n", linha);
                free(nova_compra);
            }
        }
        else
        {
            printf("Linha com formato desconhecido: %s\n", linha);
        }
    }

    fclose(f);
}

void atualizar_ficheiro(Queue *q)
{
    FILE *f = fopen("dados.txt", "w");
    if (f == NULL)
    {
        printf("Erro: nao foi possivel abrir dados.txt para escrita.\n");
        return;
    }

    Node *curr = q->front;
    while (curr != NULL)
    {
        fprintf(f, "%s|%02d/%02d/%04d|%s|%d|%d|%.2f\n",
                curr->aluno.nome,
                curr->aluno.nascimento.dia,
                curr->aluno.nascimento.mes,
                curr->aluno.nascimento.ano,
                curr->aluno.curso,
                curr->aluno.ano,
                curr->aluno.numero,
                curr->aluno.dinheiro);

        CompraNode *compra_atual = curr->aluno.compras;
        while (compra_atual != NULL)
        {
            fprintf(f, "%.2f|%s|%02d/%02d/%04d\n",
                    compra_atual->despesa.preco,
                    compra_atual->despesa.descriçao,
                    compra_atual->despesa.data_compra.dia,
                    compra_atual->despesa.data_compra.mes,
                    compra_atual->despesa.data_compra.ano);

            compra_atual = compra_atual->next;
        }

        curr = curr->next;
    }

    fclose(f);
}

// Função auxiliar para o qsort comparar os nomes
int comparar_alunos(const void *a, const void *b)
{
    // Lançamos os ponteiros para (aluno **) porque o nosso array será um array de ponteiros
    aluno *aluno_A = *(aluno **)a;
    aluno *aluno_B = *(aluno **)b;
    return strcmp(aluno_A->nome, aluno_B->nome);
}

void introduzir_aluno(Queue *q, aluno novo_aluno)
{
    Node *novo_no = (Node *)malloc(sizeof(Node));
    if (novo_no == NULL)
        return;

    novo_no->aluno = novo_aluno;
    novo_no->aluno.compras = NULL; // começar sem compras
    novo_no->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = novo_no;
    }
    else
    {
        q->rear->next = novo_no;
        q->rear = novo_no;
    }
}

void eliminar_aluno(Queue *q, int criterio, char *nome, int numero)
{
    if (q->front == NULL)
    {
        printf("A fila de alunos esta vazia.\n");
        return;
    }

    Node *curr = q->front;
    Node *prev = NULL;

    while (curr != NULL)
    {
        int encontrado = 0;

        if (criterio == POR_NOME && strcmp(curr->aluno.nome, nome) == 0)
        {
            encontrado = 1;
        }
        else if (criterio == POR_NUMERO && curr->aluno.numero == numero)
        {
            encontrado = 1;
        }

        if (encontrado)
        {
            libertar_compras(curr->aluno.compras);

            if (prev == NULL)
            {
                q->front = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }

            if (curr == q->rear)
            {
                q->rear = prev;
            }

            free(curr);
            printf("Aluno eliminado com sucesso.\n");
            return;
        }

        prev = curr;
        curr = curr->next;
    }

    if (criterio == POR_NOME)
    {
        printf("Erro: Aluno com o nome '%s' nao encontrado.\n", nome);
    }
    else if (criterio == POR_NUMERO)
    {
        printf("Erro: Aluno com o numero '%d' nao encontrado.\n", numero);
    }
}

void listar_alfabeticamente(Queue *q)
{
    if (q->front == NULL)
    {
        printf("Não há alunos registados no sistema.\n");
        return;
    }

    // Contar o número de alunos na fila
    int num_alunos = 0;
    Node *curr = q->front;
    while (curr != NULL)
    {
        num_alunos++;
        curr = curr->next;
    }

    // Criar um array temporário de ponteiros para alunos
    aluno **array_alunos = malloc(num_alunos * sizeof(aluno *));
    if (array_alunos == NULL)
    {
        printf("Erro de alocação de memória.\n");
        return;
    }

    // Preencher o array com os ponteiros para os alunos na lista ligada
    curr = q->front;
    for (int i = 0; i < num_alunos; i++)
    {
        array_alunos[i] = &(curr->aluno);
        curr = curr->next;
    }

    // Ordenar o array alfabeticamente usando a função qsort
    qsort(array_alunos, num_alunos, sizeof(aluno *), comparar_alunos);

    // Imprimir a lista ordenada
    printf("\n--- Lista de Alunos (Ordem Alfabética) ---\n");
    for (int i = 0; i < num_alunos; i++)
    {
        printf("Nome: %-25s | Num: %-6d | Curso: %-15s | Saldo: %.2f\n",
               array_alunos[i]->nome,
               array_alunos[i]->numero,
               array_alunos[i]->curso,
               array_alunos[i]->dinheiro);
    }
    printf("------------------------------------------\n\n");

    // Libertar o array temporário
    free(array_alunos);
}

void despesa(aluno *a, float preco, char *descricao, int dia, int mes, int ano)
{
    if (a == NULL)
    {
        printf("Erro: aluno invalido.\n");
        return;
    }

    if (preco <= 0)
    {
        printf("Erro: valor da despesa invalido.\n");
        return;
    }

    if (a->dinheiro < preco)
    {
        printf("Erro: saldo insuficiente.\n");
        return;
    }

    CompraNode *novo = (CompraNode *)malloc(sizeof(CompraNode));
    if (novo == NULL)
    {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return;
    }

    novo->despesa.preco = preco;
    strcpy(novo->despesa.descriçao, descricao);
    novo->despesa.data_compra.dia = dia;
    novo->despesa.data_compra.mes = mes;
    novo->despesa.data_compra.ano = ano;
    novo->next = NULL;

    if (a->compras == NULL)
    {
        a->compras = novo;
    }
    else
    {
        CompraNode *aux = a->compras;
        while (aux->next != NULL)
        {
            aux = aux->next;
        }
        aux->next = novo;
    }

    a->dinheiro -= preco;
    printf("Despesa registada com sucesso.\n");
}

void carregar(aluno *a, float dinheiro)
{
    if (a == NULL)
    {
        printf("Erro: aluno invalido.\n");
        return;
    }

    if (dinheiro <= 0)
    {
        printf("Erro: valor invalido.\n");
        return;
    }

    a->dinheiro += dinheiro;
}

void listar_despesas_aluno(aluno *a)
{
    if (a == NULL)
    {
        printf("Erro: aluno invalido.\n");
        return;
    }

    printf("\n--- Despesas do aluno %s ---\n", a->nome);

    if (a->compras == NULL)
    {
        printf("Este aluno nao tem despesas registadas.\n");
        printf("-----------------------------\n\n");
        return;
    }

    CompraNode *curr = a->compras;
    float total = 0.0f;
    int contador = 1;

    while (curr != NULL)
    {
        printf("%d. %-20s | %.2f EUR | %02d/%02d/%04d\n",
               contador,
               curr->despesa.descriçao,
               curr->despesa.preco,
               curr->despesa.data_compra.dia,
               curr->despesa.data_compra.mes,
               curr->despesa.data_compra.ano);

        total += curr->despesa.preco;
        contador++;
        curr = curr->next;
    }

    printf("Total gasto: %.2f EUR\n", total);
    printf("-----------------------------\n\n");
}