#include <stdio.h>
#include <stdlib.h>
#include "fonte.h"
#include "function.h"

int main()
{
    Queue bar;
    bar.front = NULL;
    bar.rear = NULL;

    ler_ficheiro(&bar);

    int opcao = -1;
    while (opcao != 0)
    {
        printf("---Menu---\n");
        printf("1. Adicionar aluno\n");
        printf("2. Eliminar aluno\n");
        printf("3. Listar alunos alfabeticamente\n");
        printf("4. Realizar compra\n");
        printf("5. Carregar dinheiro\n");
        printf("6. Listar despesas de um aluno\n");
        printf("0. Sair\n");
        printf("Escolha uma opção (0 para terminar): ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 0:
            printf("Saindo do programa...\n");
            break;
        case 1:
        {
            aluno novo_aluno;
            printf("Nome do aluno: ");
            scanf(" %99[^\n]", novo_aluno.nome);
            printf("Data de nascimento (dd mm aaaa): "); // fazer mais bonito
            scanf("%d %d %d", &novo_aluno.nascimento.dia, &novo_aluno.nascimento.mes, &novo_aluno.nascimento.ano);
            printf("Curso: ");
            scanf(" %99[^\n]", novo_aluno.curso);
            printf("Ano: ");
            scanf("%d", &novo_aluno.ano);
            printf("Número de estudante: ");
            scanf("%d", &novo_aluno.numero);
            printf("Crédito inicial: ");
            scanf("%f", &novo_aluno.dinheiro);
            introduzir_aluno(&bar, novo_aluno);
            printf("Aluno adicionado com sucesso.\n");
            break;
        }
        case 2:
        {
            printf("Como deseja procurar o aluno a eliminar?\n");
            printf("1. Procurar por nome\n");
            printf("2. Procurar por número de estudante\n");
            int opcao_apagar;
            int num = 0;
            char nome[100];
            scanf("%d", &opcao_apagar);
            switch (opcao_apagar)
            {
            case 1:
                printf("Nome do aluno a eliminar: ");
                scanf(" %99[^\n]", nome);
                eliminar_aluno(&bar, POR_NOME, nome, 0);
                break;
            case 2:
                printf("Número do aluno a eliminar: ");
                scanf("%d", &num);
                eliminar_aluno(&bar, POR_NUMERO, NULL, num);
                break;
            default:
                printf("Opção inválida.\n");
            }
            break;
        }
        case 3:
        {
            listar_alfabeticamente(&bar);
            break;
        }
        case 4:
        {
            int opcao_procura;
            int numero = 0;
            aluno *aluno_encontrado = NULL;
            char nome[100];
            char descricao[100];
            int dia, mes, ano;
            float valor;

            printf("Como deseja procurar o aluno?\n");
            printf("1. Procurar por nome\n");
            printf("2. Procurar por numero de estudante\n");
            scanf("%d", &opcao_procura);

            switch (opcao_procura)
            {
            case 1:
                printf("Nome do aluno: ");
                scanf(" %99[^\n]", nome);
                aluno_encontrado = procurar_aluno_nome(&bar, nome);
                break;

            case 2:
                printf("Numero do aluno: ");
                scanf("%d", &numero);
                aluno_encontrado = procurar_aluno_numero(&bar, numero);
                break;

            default:
                printf("Opcao invalida.\n");
                break;
            }

            if (aluno_encontrado == NULL)
            {
                printf("Aluno nao encontrado.\n");
                break;
            }

            printf("Valor da despesa: ");
            scanf("%f", &valor);

            printf("Descricao: ");
            scanf(" %99[^\n]", descricao);

            printf("Data (dd mm aaaa): ");
            scanf("%d %d %d", &dia, &mes, &ano);

            despesa(aluno_encontrado, valor, descricao, dia, mes, ano);
            break;
        }
        case 5:
        {
            int opcao_procura;
            int numero = 0;
            float valor;
            aluno *aluno_encontrado = NULL;
            char nome[100];

            printf("Como deseja procurar o aluno?\n");
            printf("1. Procurar por nome\n");
            printf("2. Procurar por numero de estudante\n");
            scanf("%d", &opcao_procura);

            switch (opcao_procura)
            {
            case 1:
                printf("Nome do aluno: ");
                scanf(" %99[^\n]", nome);
                aluno_encontrado = procurar_aluno_nome(&bar, nome);
                break;

            case 2:
                printf("Numero do aluno: ");
                scanf("%d", &numero);
                aluno_encontrado = procurar_aluno_numero(&bar, numero);
                break;

            default:
                printf("Opcao invalida.\n");
                break;
            }

            if (aluno_encontrado == NULL)
            {
                printf("Aluno nao encontrado.\n");
                break;
            }

            printf("Valor a carregar: ");
            scanf("%f", &valor);

            if (valor <= 0)
            {
                printf("Valor invalido.\n");
                break;
            }

            carregar(aluno_encontrado, valor);
            printf("Saldo carregado com sucesso.\n");
            break;
        }
        case 6:
        {
            int opcao_procura;
            int numero = 0;
            aluno *aluno_encontrado = NULL;
            char nome[100];

            printf("Como deseja procurar o aluno?\n");
            printf("1. Procurar por nome\n");
            printf("2. Procurar por numero de estudante\n");
            scanf("%d", &opcao_procura);

            switch (opcao_procura)
            {
            case 1:
                printf("Nome do aluno: ");
                scanf(" %99[^\n]", nome);
                aluno_encontrado = procurar_aluno_nome(&bar, nome);
                break;

            case 2:
                printf("Numero do aluno: ");
                scanf("%d", &numero);
                aluno_encontrado = procurar_aluno_numero(&bar, numero);
                break;

            default:
                printf("Opcao invalida.\n");
                break;
            }

            if (aluno_encontrado == NULL)
            {
                printf("Aluno nao encontrado.\n");
                break;
            }

            listar_despesas_aluno(aluno_encontrado);
            break;
        }
        default:
        {
            printf("Opção inválida.\n");
            break;
        } // close case
        } // close switch
    } // close while
    atualizar_ficheiro(&bar);
    libertar(&bar);
    return 0;
} // close main
