#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define Tam_Max 20

typedef struct {
    char palavra[Tam_Max];
    int length;
} String;

typedef struct {
    union {
        char palavras[20][Tam_Max];
        char letras[26];
    };
    int length;
} Chutes;

void configurar_exibir(String *copy_string, String *string);
int configurar_chances(String palavra);
bool busca(char elemento, char espaco[], int length);
bool buscar_letra(String palavra, char letra, String *exibir);
bool letra_repetida(Chutes chute, char letra);
bool palavra_repetida(Chutes chute, char palavra[]);
void definir_palavra(char *retorno);
void adicionar_palavra();


int main() {
    String palavra_secreta, exibir;
    Chutes letra = {.length = 0}, palavra = {.length = 0};
    
    definir_palavra(palavra_secreta.palavra);
    palavra_secreta.length = strlen(palavra_secreta.palavra);

    configurar_exibir(&exibir, &palavra_secreta);
    int chances = configurar_chances(palavra_secreta);

    for (int i = 0; i < chances; i++)
    {
        printf("%s\n", exibir.palavra);
        char tipo;

        printf("'l' para inserir uma letra ou 'p' para chutar uma palavra\n");
        scanf(" %c", &tipo);

        if (tipo == 'l') {
            char letra_test;
            printf("digite a letra\n");
            scanf(" %c", &letra_test);

            if (letra_repetida(letra, letra_test)) {
                printf("letra já testada\n");
                continue;
            } else {
                letra.letras[letra.length] = letra_test;
                letra.length++;
            }
            
            bool encontrou = buscar_letra(palavra_secreta, letra_test, &exibir);
            if (encontrou) {
                printf("letra encontrada\n");
            } else {
                printf("não tem essa letra\n");
            }
        }

        if (tipo == 'p') {
            char palavra_test[20];
            printf("escreva a palavra\n");
            scanf(" %s", palavra_test);

            if (palavra_repetida(palavra, palavra_test)) {
                printf("palavra já testada\n");
                continue;
            } else {
                strcpy(palavra.palavras[palavra.length], palavra_test);
                palavra.length++;
            }

            if (strcmp(palavra_test, palavra_secreta.palavra) == 0) {
                printf("acertou\n");
                strcpy(exibir.palavra, palavra_secreta.palavra);
                break;
            } else {
                printf("nao e essa a palvra secreta\n");
                continue;
            }
        }

        if (strcmp(exibir.palavra, palavra_secreta.palavra) == 0) {
            printf("jogo acabou\n");
            break;
        }
    }
    
    printf("\n%s\n\n", exibir.palavra);

    printf("quer adicionar uma palvra ao jogo? (S/N)\n");
    char resposta;
    scanf(" %c", &resposta);
    if(resposta == 'S') {
        adicionar_palavra();
    }
}

void definir_palavra(char *retorno) {
    FILE *arquivo;

    arquivo = fopen("palavras.txt", "r");
    if (arquivo == 0) {
        printf("ERRO");
        return;
    }

    int qtd_de_palavras;
    fscanf(arquivo, "%d", &qtd_de_palavras);

    srand(time(NULL));
    int linha = rand() % qtd_de_palavras;


    for (int i = 0; i <= linha; ++i)
    {
        fscanf(arquivo, "%s", retorno);
    }

    fclose(arquivo);
}

void adicionar_palavra() {
    FILE *arq;
    char nova_palavra[Tam_Max];
    int qtd_de_palavras;

    arq = fopen("palavras.txt", "r+");
    if (arq == 0) {
        printf("ERRO");
        return;
    }


    printf("digite uma nova palavra para o jogo (minuscula)\n");
    scanf(" %s", nova_palavra);

    fscanf(arq, "%d", &qtd_de_palavras);
    fseek(arq, 0, SEEK_SET);
    fprintf(arq, "%d", ++qtd_de_palavras);

    fseek(arq, 0, SEEK_END);
    fprintf(arq, "\n%s", nova_palavra);

    fclose(arq);
}

void configurar_exibir(String *copy_string, String *string) {
    copy_string->length = string->length;
    for (int i = 0; i <= copy_string->length; i++)
    {
        copy_string->palavra[i] = string->palavra[i] == ' ' ? ' ' : '-';
        if (i == copy_string->length)
            copy_string->palavra[i] = '\0';
    }
}

int configurar_chances(String palavra) {
    char letras[26];
    int letras_len = 0;
    for (int i = 0; i < palavra.length; i++)
    {
        if (!busca(palavra.palavra[i], letras, letras_len)) {
            letras[letras_len] = palavra.palavra[i];
            letras_len++;
        }
    }
    return letras_len + 5;
}

bool busca(char elemento, char espaco[], int length) {
    for (int i = 0; i < length; i++)
    {
        if (elemento == espaco[i])
        return true;
    }
    return false;
}

bool buscar_letra(String palavra, char letra, String *exibir) {
    bool achou = false;
    for (int i = 0; i < palavra.length; i++)
    {
        if (palavra.palavra[i] == letra) {
            exibir->palavra[i] = letra;
            achou = true;
        }
    }
    return achou;
}

bool letra_repetida(Chutes chute, char letra) {
    for (int i = 0; i < chute.length; i++)
    {
        if (chute.letras[i] == letra) {
            return true;
        }
    }
    return false;
}

bool palavra_repetida(Chutes chute, char palavra[]) {
    for (int i = 0; i < chute.length; i++)
    {
        if (strcmp(chute.palavras[i], palavra) == 0) {
            return true;
        }
    }
    return false;
}