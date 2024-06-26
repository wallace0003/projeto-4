#include "bank.h"
#include <stdio.h>

int main() {
  Cliente clientes[total];
  int posicao = 0;
  carregar_de_binario(clientes, &posicao);
  while (1) {
    int opcao;
    printf("\n");
    printf("1 - Criar cliente\n");
    printf("2 - Apagar cliente\n");
    printf("3 - Listar clientes\n");
    printf("4 - Débito\n");
    printf("5 - Depósito\n");
    printf("6 - Extrato\n");
    printf("7 - Transferências\n");
    printf("0 - Sair\n");
    printf("\nDigite a opção desejada:");
    scanf("%d", &opcao);

    if (opcao == 1) {
      criar_cliente(clientes, &posicao);
    } else if (opcao == 2) {
      apagar_cliente(clientes, &posicao);
    } else if (opcao == 3) {
      listar_clientes(clientes, &posicao);
    } else if (opcao == 4) {
      debitar(clientes, &posicao);
    } else if (opcao == 5) {
      depositar(clientes, &posicao);
    } else if (opcao == 6) {
      extrato(clientes, &posicao);
    } else if (opcao == 7) {
      tranferencia(clientes, &posicao);
    } else if (opcao == 0) {
      salvar_em_binario(clientes, &posicao);
      printf("\nSaindo...\n");
      break;
    } else {
      printf("Opção inválida!\n");
    }
  }
  return 1;
}