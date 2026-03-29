# Analisador Léxico em C (Lexer)

Este projeto é um **Analisador Léxico (Scanner)** desenvolvido em linguagem C puro. Ele utiliza a biblioteca POSIX de expressões regulares (`regex.h`) para ler um arquivo de código-fonte (`.txt`) e convertê-lo em uma sequência de _tokens_ classificados.

O diferencial deste analisador é a sua capacidade de **identificar e isolar erros léxicos** sem interromper a análise do restante do código.

---

## Funcionalidades

O analisador é capaz de reconhecer os seguintes padrões:

- **Palavras-chave:** `if`, `else`, `while`, `for`, `int`, `float`, etc.
- **Identificadores:** Nomes de variáveis e funções válidas.
- **Números:** Inteiros (ex: `123`) e Ponto Flutuante (ex: `10.5`).
- **Textos:** Strings entre aspas duplas (`"texto"`).
- **Operadores:** Matemáticos, relacionais e lógicos (`+`, `-`, `>=`, `++`, etc).
- **Delimitadores:** Chaves `{}`, parênteses `()`, colchetes `[]`, ponto e vírgula `;`, etc.
- **Comentários:** Múltiplas linhas (`/* ... */`).

### Tratamento de Erros

O analisador possui regras robustas para evitar que erros quebrem a compilação:

- **`ERRO_STR`**: Captura strings que foram abertas, mas não fechadas na mesma linha.
- **`ERRO_NUM`**: Identifica números malformados com múltiplos pontos (ex: `10.0.0`).
- **`ERRO_ID`**: Bloqueia nomes de variáveis que começam com números (ex: `1var`) ou símbolos inválidos (ex: `@teste`).

---

## Estrutura do Projeto

Certifique-se de que seus arquivos estejam organizados da seguinte forma no seu diretório:

```text
/
├── analisador.c    # Código-fonte principal do analisador léxico
├── Makefile        # Script de automação para compilação e execução
├── teste.txt       # Arquivo contendo o código que será analisado
└── README.md       # Este arquivo de documentação
```

---

## Como Executar

O projeto possui um `Makefile` configurado para facilitar a compilação e os testes. Certifique-se de ter o arquivo `teste.txt` na mesma pasta que o código.

Abra o terminal no diretório do projeto e utilize um dos comandos abaixo:

### 1. Compilar e Executar (Tudo de uma vez)

Este é o comando principal. Ele compila o código `analisador.c`, gera o executável e já roda a análise lendo o arquivo `teste.txt`:

```bash
make
```

### 2. Apenas Executar

Se você já compilou o código antes e apenas alterou o texto dentro do `teste.txt`, não precisa compilar de novo. Basta rodar:

```bash
make run
```

### 3. Limpar Arquivos

Para apagar o executável gerado (`analisador`) e manter a pasta limpa:

```bash
make clean
```

---

## Exemplo de Saída

Dado um arquivo `teste.txt` contendo o seguinte código com alguns erros intencionais:

```c
int main() {
    float x = 10.5;
    string texto = "teste sem fechar
    1numero_invalido = 0;
}
```

Ao rodar o comando `make`, o analisador imprimirá no terminal a seguinte tabela formatada, classificando os tokens e isolando os erros perfeitamente:

```text
LEXEMA                    | TIPO         | LINHA  | COLUNA
--------------------------|--------------|--------|--------
int                       | P_CHAVE      | 1      | 1
main                      | ID           | 1      | 5
(                         | DELIMITADOR  | 1      | 9
)                         | DELIMITADOR  | 1      | 10
{                         | DELIMITADOR  | 1      | 12
float                     | P_CHAVE      | 2      | 5
x                         | ID           | 2      | 11
=                         | OPERADOR     | 2      | 13
10.5                      | FLOAT        | 2      | 15
;                         | DELIMITADOR  | 2      | 19
string                    | ID           | 3      | 5
texto                     | ID           | 3      | 12
=                         | OPERADOR     | 3      | 18
"teste sem fechar         | ERRO_STR     | 3      | 20
1numero_invalido          | ERRO_ID      | 4      | 5
=                         | OPERADOR     | 4      | 22
0                         | INTEIRO      | 4      | 24
;                         | DELIMITADOR  | 4      | 25
}                         | DELIMITADOR  | 5      | 1
```

---

_Desenvolvido para estudos de Compiladores e Análise Léxica._
