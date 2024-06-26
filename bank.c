#include "bank.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int contem_apenas_letras(const char *str) {
  if (*str == '\0') {
      return 0; 
  }
  while (*str) {
    if (!isalpha(*str) && *str != ' ') { // Verifica se o caractere não é uma letra ou espaço
      return 0;
    }
    str++;
  }
  return 1;
}


int criar_cliente(Cliente clientes[], int *posicao) {
  // Verificando se a posição não excedeu o total.
  if (*posicao >= total) {
    printf("Não é possível adicionar mais clientes, a lista de clientes está cheia.\n");
    return 0;
  }

  // Pedindo para o usuário os dados e salvando-os na struct
  clearBuffer();
  printf("Nome: ");
  fgets(clientes[*posicao].nome, max_nome, stdin);
  // Removendo o caractere de nova linha, se presente
  clientes[*posicao].nome[strcspn(clientes[*posicao].nome, "\n")] = '\0';
  // Verificando se o nome não está vazio e contém apenas letras
  if (!contem_apenas_letras(clientes[*posicao].nome)) {
    printf("O nome não pode estar vazio e só pode conter apenas letras.\n");
    return 0;
  }

  printf("CPF (Ex: 111111111111): ");
  scanf("%ld", &clientes[*posicao].cpf);

int tipo_con;
  printf("Tipo de conta (1 - Comum, 2 - Plus): ");
  scanf("%d", &tipo_con);
  // Verificando se o tipo de conta é válido
  if (tipo_con != 1 && tipo_con != 2) {
    printf("Tipo de conta inválido.\n");
    return 0;
  } else if (tipo_con == 1) {
    strcpy(clientes[*posicao].tipo_conta, "Comum");
  } else if(tipo_con == 2) {
    strcpy(clientes[*posicao].tipo_conta, "Plus");
  }


  printf("Saldo inicial da conta: ");
  scanf("%f", &clientes[*posicao].saldo);

  printf("Senha do usuário: ");
  clearBuffer();
  fgets(clientes[*posicao].senha, max_senha, stdin);
  // Removendo o caractere de nova linha, se presente
  clientes[*posicao].senha[strcspn(clientes[*posicao].senha, "\n")] = '\0';

  (*posicao)++; // Incrementa a posição

  printf("Cliente adicionado com sucesso!\n");

  return 1; // Retorna 1 indicando que o cliente foi criado com sucesso
}

int apagar_cliente(Cliente clientes[], int *posicao) {
    long cpfCliente = 0;

    if (*posicao == 0) {
        printf("Não há clientes a serem removidos.\n");
        return 0;
    }

    printf("Digite o CPF do cliente que deseja remover: ");
    scanf("%ld", &cpfCliente);

    int encontrado = 0; 

    for (int i = 0; i < *posicao; i++) {
        if (clientes[i].cpf == cpfCliente) {
            encontrado = 1; 
            for (int j = i; j < *posicao - 1; j++) {
                clientes[j].cpf = clientes[j + 1].cpf;
                clientes[j].saldo = clientes[j + 1].saldo;
                clientes[j].num_transacoes = clientes[j + 1].num_transacoes;
                strcpy(clientes[j].nome, clientes[j + 1].nome);
                strcpy(clientes[j].tipo_conta, clientes[j + 1].tipo_conta);
                strcpy(clientes[j].senha, clientes[j + 1].senha);
            }
            printf("Cliente removido com sucesso!\n");
            break; // Parar o loop após encontrar o cliente
        }
    }

    if (!encontrado) {
        printf("Cliente com CPF %ld não encontrado.\n", cpfCliente);
        return 0;
    }

    *posicao = *posicao - 1;

    return 1;
}



int listar_clientes(Cliente clientes[], int *posicao) {
  // Verificação de há contatos
  if (*posicao == 0) {
    printf("Não há contatos, impossível listá-los.");
    return 0;
  }

  // Looping para printar os contatos
  for (int i = 0; i < *posicao; i++) {
    printf("Posição: %d\t", i + 1);
    printf("Nome: %s\t", clientes[i].nome);
    printf("CPF: %ld\t", clientes[i].cpf);
    printf("Tipo de conta: %s\t", clientes[i].tipo_conta);
    printf("Saldo atual: %.2f\n", clientes[i].saldo);
  }
  return 1;
}

// Função para debitar valor da conta de um cliente
int debitar(Cliente clientes[], int *posicao) {
    long cpf;
    char senha[max_senha];
    float valor;

    // Solicitando dados do usuário
    printf("CPF: ");
    scanf("%ld", &cpf);
    printf("Senha: ");
    clearBuffer();
    fgets(senha, max_senha, stdin);
    // Removendo o caractere de nova linha, se presente
    senha[strcspn(senha, "\n")] = '\0';
    printf("Valor: ");
    scanf("%f", &valor);

    // Verificando os dados do cliente
    for (int i = 0; i < *posicao; i++) {
        if (cpf == clientes[i].cpf && strcmp(senha, clientes[i].senha) == 0) {
            // Verificando limite de saldo negativo
            float verificando_limite_saldo = clientes[i].saldo - valor;
            float tarifa_saque;

            if (verificando_limite_saldo < -1000 && strcmp(clientes[i].tipo_conta, "Comum") == 0) {
                printf("\nSeu limite de saldo negativo é -1000\n");
                printf("Impossível realizar o débito, seu saldo ficaria %.2f\n", verificando_limite_saldo);
                return 0;
            } else if (verificando_limite_saldo < -5000 && strcmp(clientes[i].tipo_conta, "Plus") == 0) {
                printf("\nSeu limite de saldo negativo é -5000\n");
                printf("Impossível realizar o débito, seu saldo ficaria: R$%.2f\n", verificando_limite_saldo);
                return 0;
            }

            // Calculando tarifa de saque
            if (strcmp(clientes[i].tipo_conta, "Comum") == 0) {
                tarifa_saque = 0.05 * valor;
            } else if (strcmp(clientes[i].tipo_conta, "Plus") == 0) {
                tarifa_saque = 0.03 * valor;
            }

            // Descontando tarifa do saldo
            clientes[i].saldo -= tarifa_saque;
            printf("\nFoi descontado do valor de seu saque: R$%.2f\n", tarifa_saque);

            // Realizando o saque
            clientes[i].saldo -= valor;
            printf("Valor sacado: R$%.2f\n", valor);

            strcpy(clientes[i].extrato[clientes[i].num_transacoes].tipo, "Débito");
            clientes[i].extrato[clientes[i].num_transacoes].valor = valor;
            clientes[i].extrato[clientes[i].num_transacoes].tarifa = tarifa_saque;
            clientes[i].num_transacoes++;
            // Registrando transação
            printf("Transação registrada com sucesso!\n");

            return 1;
        }
    }

    // Caso não encontre o cliente
    printf("\nCPF ou senha não encontrados.\n");
    return 0;
}

int depositar(Cliente clientes[], int *posicao) {
    long cpf;
    char senha[max_senha];
    float valor_deposito;

    // Solicitar CPF e senha do usuário para autenticação
    printf("CPF: ");
    scanf("%ld", &cpf);
    clearBuffer(); // Limpar o buffer após a leitura do inteiro
    printf("Senha: ");
    fgets(senha, max_senha, stdin);
    senha[strcspn(senha, "\n")] = '\0'; // Remover o caractere de nova linha

    // Verifica se o CPF e a senha correspondem a algum cliente
    int i;
    for (i = 0; i < *posicao; i++) {
        if (clientes[i].cpf == cpf && strcmp(clientes[i].senha, senha) == 0) {
            // CPF e senha correspondem, solicitar valor do depósito
            printf("Valor do depósito: ");
            scanf("%f", &valor_deposito);
            // Verificar se o valor do depósito é válido
            if (valor_deposito <= 0) {
                printf("Valor de depósito inválido.\n");
                return 0;
            }
            // Realizar o depósito adicionando o valor ao saldo do cliente
            clientes[i].saldo += valor_deposito;

            // Registrando a operação no histórico
strcpy(clientes[i].extrato[clientes[i].num_transacoes].tipo, "Depósito");
clientes[i].extrato[clientes[i].num_transacoes].valor = valor_deposito;
clientes[i].extrato[clientes[i].num_transacoes].tarifa = 0;
            clientes[i].num_transacoes++;
            printf("Depósito realizado com sucesso. Novo saldo: %.2f\n", clientes[i].saldo);
            return 1;
        }
    }

    // Se o loop terminar, significa que o cliente não foi encontrado
    printf("CPF ou senha incorretos. Não foi possível realizar o depósito.\n");
    return 0;
}

int extrato(Cliente clientes[], int *posicao) {
    long cpf;
    char senha[max_senha];

    // Solicitar CPF e senha do usuário para autenticação
    printf("CPF: ");
    scanf("%ld", &cpf);
    clearBuffer(); // Limpar o buffer após a leitura do inteiro
    printf("Senha: ");
    fgets(senha, max_senha, stdin);
    senha[strcspn(senha, "\n")] = '\0'; // Remover o caractere de nova linha

    // Verifica se o CPF e a senha correspondem a algum cliente
    int i;
    for (i = 0; i < *posicao; i++) {
        if (clientes[i].cpf == cpf && strcmp(clientes[i].senha, senha) == 0) {
            // CPF e senha correspondem, criar e escrever no arquivo de extrato
            FILE *extrato_file;
            char filename[50]; // Nome do arquivo será o CPF do cliente
            sprintf(filename, "%ld.txt", cpf);
            extrato_file = fopen(filename, "w");
            if (extrato_file == NULL) {
                printf("Erro ao abrir o arquivo para escrita.\n");
                return 0;
            }

            // Escrever no arquivo
            fprintf(extrato_file, "Extrato de operações para o cliente: %s\n", clientes[i].nome);
            fprintf(extrato_file, "CPF: %ld\n", clientes[i].cpf);
            fprintf(extrato_file, "Tipo de conta: %s\n", clientes[i].tipo_conta);
            fprintf(extrato_file, "Saldo atual: %.2f\n\n", clientes[i].saldo);
            fprintf(extrato_file, "Histórico de operações:\n");

            // Iterar sobre as transações do extrato e escrevê-las no arquivo
            for (int j = 0; j < clientes[i].num_transacoes; j++) {
                fprintf(extrato_file, "Operação %d:\n", j + 1);
                fprintf(extrato_file, "Tipo: %s\n", clientes[i].extrato[j].tipo); 
                fprintf(extrato_file, "tarifa: %.2f\n", clientes[i].extrato[j].tarifa);
                fprintf(extrato_file, "Valor: %.2f\n", clientes[i].extrato[j].valor);
                
                // Aqui você pode adicionar mais detalhes sobre as operações, como tarifas, datas, etc.
                fprintf(extrato_file, "\n");
            }

            fclose(extrato_file); // Fechar o arquivo

            printf("Extrato gerado com sucesso! Consulte o arquivo \"%s\".\n", filename);
            return 1;
        }
    }

    // Se o loop terminar, significa que o cliente não foi encontrado
    printf("CPF ou senha incorretos. Não foi possível gerar o extrato.\n");
    return 0;
}



int tranferencia(Cliente clientes[], int *posicao) {
    long cpf_remetente;
    char senha_remetente[max_senha];
    long cpf_destinatario;
    int posicao_remetente = -1; // Inicialize com um valor que não é uma posição válida no vetor de clientes
    float valor;
    float taxa;

    // Solicitar CPF e senha do remetente para autenticação
    printf("CPF: ");
    scanf("%ld", &cpf_remetente);
    clearBuffer(); 
    printf("Senha: ");
    fgets(senha_remetente, max_senha, stdin);
    senha_remetente[strcspn(senha_remetente, "\n")] = '\0'; // Remover o caractere de nova linha

    // Verifica se o CPF e a senha correspondem a algum cliente
    int i;
    for (i = 0; i < *posicao; i++) {
        if (clientes[i].cpf == cpf_remetente && strcmp(clientes[i].senha, senha_remetente) == 0) {
            posicao_remetente = i; // Atualize a posição do remetente
            break; // Pare o loop assim que o remetente for encontrado
        }
    }

    

    // Verificar se o remetente foi encontrado
    if (posicao_remetente == -1) {
        printf("CPF ou senha incorretos. Não foi possível realizar a transferência.\n");
        return 0;
    }

    

    // Solicitar CPF do destinatário
    printf("CPF do destinatário: ");
    scanf("%ld", &cpf_destinatario);

    // Verificar se o destinatário existe
    int encontrado = 0;
    for (int j = 0; j < *posicao; j++) {
        if (clientes[j].cpf == cpf_destinatario) {
            encontrado = 1;
            // Solicitar o valor a ser transferido
            printf("Valor da transferência: ");
            scanf("%f", &valor);

            // Verificar se o valor da transferência é válido
            if (valor <= 0) {
                printf("Valor de transferência inválido.\n");
                return 0;
            }

            float tarifa_saque;

            //Verificando se a conta do remetente é comum ou plus e adicionando
            // a tarifa.
            if (strcmp(clientes[posicao_remetente].tipo_conta, "Comum") == 0) {
                tarifa_saque = 0.05 * valor;
            } else if (strcmp(clientes[posicao_remetente].tipo_conta, "Plus") == 0) {
                tarifa_saque = 0.03 * valor;
            }

            // Verificar se o saldo do remetente é suficiente
            float verificando_limite_saldo = clientes[posicao_remetente].saldo - 
            (valor + tarifa_saque);
            

            if (verificando_limite_saldo < -1000 && strcmp(clientes[posicao_remetente].tipo_conta, "Comum") == 0) {
                printf("\nSeu limite de saldo negativo é -1000\n");
                printf("Impossível realizar a transferência, seu saldo ficaria %.2f\n", verificando_limite_saldo);
                return 0;
            } else if (verificando_limite_saldo < -5000 && strcmp(clientes[posicao_remetente].tipo_conta, "Plus") == 0) {
                printf("\nSeu limite de saldo negativo é -5000\n");
                printf("Impossível realizar A transferência, seu saldo ficaria: R$%.2f\n", verificando_limite_saldo);
                return 0;
            }



            
            
            
            // Realizar a transferência
            clientes[posicao_remetente].saldo -= (valor + tarifa_saque); // Descontar o valor transferido do saldo do remetente + a taxa
            clientes[j].saldo += valor; // Adicionar o valor transferido ao saldo do destinatário

// Registrando a operação no histórico do remetente
            strcpy(clientes[posicao_remetente].extrato[clientes[posicao_remetente].num_transacoes].tipo, "transferência -");       clientes[posicao_remetente].extrato[clientes[posicao_remetente].num_transacoes].valor = valor;
clientes[posicao_remetente].extrato[clientes[posicao_remetente].num_transacoes].tarifa = tarifa_saque;
clientes[posicao_remetente].num_transacoes++;

// Registrando no histórico do destinatário
            strcpy(clientes[j].extrato[clientes[j].num_transacoes].tipo, "transferência +");
clientes[j].extrato[clientes[j].num_transacoes].valor = valor;
clientes[j].extrato[clientes[j].num_transacoes].tarifa = 0;
clientes[j].num_transacoes++;

            printf("Taxa de transferência: R$%.2f\n", tarifa_saque);
            printf("Transferência realizada com sucesso!\n");
            return 1;
        }
    }

    // Se o loop terminar, significa que o destinatário não foi encontrado
    if (!encontrado) {
        printf("Destinatário não encontrado.\n");
        return 0;
    }

    return 0; // Isso só será alcançado se algo der errado
}

int salvar_em_binario(Cliente clientes[], int *posicao) {
  FILE *f = fopen("Clientes.bin", "wb");
  if (f == NULL) {
    printf("Erro ao abrir o arquivo para leitura\n");
    return 0;
  }
    
  int qtd = fwrite(clientes, total, sizeof(Cliente), f);
  if (qtd == 0) {
    printf("Erro ao ler os clientes do arquivo\n");
    return 0;
  }
  qtd = fwrite(posicao, 1, sizeof(int), f);
  if (qtd == 0) {
    printf("Erro ao ler a posição do arquivo\n");
    return 0;
  }
  if (fclose(f)) {
    printf("Erro ao fechar o arquivo após a leitura\n");
    return 0;
  }

  printf(
      "\nOs dados foram salvos em arquivo binário com o nome: 'Clientes.bin'. \n");
  return 1;
}

int carregar_de_binario(Cliente clientes[], int *posicao) {

  FILE *f = fopen("Clientes.bin", "rb");
  if (f == NULL) {
    printf("Atenção! nenhum cliente foi salvo ainda\n");
    return 0;
  }

  int qtd = fread(clientes, total, sizeof(Cliente), f);
  if (qtd == 0) {
    printf("Erro ao ler os clientes do arquivo\n");
    return 0;
  }

  qtd = fread(posicao, 1, sizeof(int), f);
  if (qtd == 0) {
    printf("Erro ao ler a posição do arquivo\n");
    return 0;
  }

  if (fclose(f)) {
    printf("Erro ao fechar o arquivo\n");
    return 0;
  }

  printf("\nClientes carregados com sucesso!\n");
  return 1;
}


void clearBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
