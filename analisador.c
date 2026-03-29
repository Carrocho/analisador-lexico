#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define QTD_PADROES 12

typedef enum
{
    TOKEN_KEYWORD,
    TOKEN_FLOAT,
    TOKEN_INT,
    TOKEN_ID,
    TOKEN_OP,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_ERROR,
    TOKEN_COMMENT,
    TOKEN_DELIM
} TokenType;

typedef struct
{
    const char *padrao;
    TokenType tipo;
    const char *nome;
} TokenDef;

TokenDef definicoes[] = {
    // Comentário
    {"^/\\*([^*]|\\*+[^/*])*\\*+/", TOKEN_COMMENT, "COMENTARIO"},

    // String
    {"^\"([^\"\\\\\n\r]|\\\\[^\n\r])*\"", TOKEN_STRING, "STRING"},
    {"^\"([^\"\\\\\n\r]|\\\\[^\n\r])*", TOKEN_ERROR, "ERRO_STR"},

    // Palavras-chave
    {"^(if|else|return|while|wesley|for|do|int|float|char|void)", TOKEN_KEYWORD, "P_CHAVE"},

    // Erros
    {"^[0-9]+\\.[0-9.]*\\.[0-9.]*", TOKEN_ERROR, "ERRO_NUM"},
    {"^[0-9]+[a-zA-Z_][a-zA-Z0-9_]*", TOKEN_ERROR, "ERRO_ID"},
    {"^[@#$\\?~`]+[a-zA-Z0-9_]*", TOKEN_ERROR, "ERRO_ID"},

    // Números
    {"^[0-9]+\\.[0-9]+", TOKEN_FLOAT, "FLOAT"},
    {"^[0-9]+", TOKEN_INT, "INTEIRO"},

    // Símbolos
    {"^(\\(|\\)|\\{|\\}|\\[|\\]|;|,|\\.)", TOKEN_DELIM, "DELIMITADOR"},
    {"^(==|!=|>=|<=|[+*/=<>-])", TOKEN_OP, "OPERADOR"},

    // Identificadores
    {"^[a-zA-Z_][a-zA-Z0-9_]*", TOKEN_ID, "ID"}};

int identificarVazio(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

void analisar(const char *input)
{
    if (!input)
        return;
    const char *batedor = input;
    const char *ponteiroInicio = batedor;

    regex_t regexes[QTD_PADROES];
    regmatch_t matches[1];

    for (int i = 0; i < QTD_PADROES; i++)
    {
        if (regcomp(&regexes[i], definicoes[i].padrao, REG_EXTENDED) != 0)
        {
            printf("Erro no regex: %s\n", definicoes[i].padrao);
            return;
        }
    }

    printf("%-25s | %-12s | %-6s | %-6s\n", "LEXEMA", "TIPO", "LINHA", "COLUNA");
    printf("--------------------------|--------------|--------|--------\n");

    int linha = 1, coluna = 1;

    while (*batedor != '\0')
    {
        if (identificarVazio(*batedor))
        {
            if (*batedor == '\n')
            {
                linha++;
                coluna = 1;
            }
            else
            {
                coluna++;
            }
            batedor++;
            continue;
        }

        ponteiroInicio = batedor;
        int t_lin = linha;
        int t_col = coluna;
        int achou = 0;

        // Tenta encontrar um token válido
        for (int i = 0; i < QTD_PADROES; i++)
        {
            if (regexec(&regexes[i], batedor, 1, matches, 0) == 0 && matches[0].rm_so == 0)
            {
                int tam = matches[0].rm_eo;

                for (int j = 0; j < tam; j++)
                {
                    if (batedor[j] == '\n')
                    {
                        linha++;
                        coluna = 1;
                    }
                    else
                    {
                        coluna++;
                    }
                }
                batedor += tam;

                printf("%-25.*s | %-12s | %-6d | %-6d\n",
                       (int)(batedor - ponteiroInicio), ponteiroInicio, definicoes[i].nome, t_lin, t_col);

                achou = 1;
                break;
            }
        }

        // Se não achar um token válido, verifica se tem um conjunto de erros lexicos
        if (!achou)
        {
            while (*batedor != '\0')
            {
                // Verifica se o próximo caractere pode iniciar um token válido, se sim, para o erro aqui
                int proximo_eh_valido = 0;
                for (int i = 0; i < QTD_PADROES; i++)
                {
                    if (regexec(&regexes[i], batedor, 1, matches, 0) == 0 && matches[0].rm_so == 0)
                    {
                        proximo_eh_valido = 1;
                        break;
                    }
                }
                if (proximo_eh_valido)
                    break;

                batedor++;
                coluna++;
            }

            printf("%-20.*s | %-12s | %-6d | %-6d\n", (int)(batedor - ponteiroInicio), ponteiroInicio, "ERRO LÉXICO", t_lin, t_col);
        }
    }
    for (int i = 0; i < QTD_PADROES; i++)
    {
        regfree(&regexes[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s <nome_do_arquivo.txt>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'\n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(tam + 1);
    if (buf)
    {
        size_t n = fread(buf, 1, tam, f);
        buf[n] = '\0';
        analisar(buf);
        free(buf);
    }

    fclose(f);
    return 0;
}