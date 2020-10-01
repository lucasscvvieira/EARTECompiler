# Escopo

## Proposta do Projeto

A ideia inicial é compreender o funcionamento do compilador e contemplar o processo de desenvolvimento do mesmo, lidando com as dificuldades e os desafios enfrentados.

## Estruturas Contempladas

Como a proposta do projeto não é criar um compilador completo, algumas estruturas não serão adicionadas, assim tornando um compilador simplificado.
As estruturas contempladas serão:
- Variáveis
- Vetores
- Funções
- Operações Matemáticas
- Operações com Strings
- IO Simplificado
- Estruturas de Repetição

### Variáveis

Os tipo de variáveis aceitos também será reduzido, mantendo:
- Void
- Int
- Float
- Char

### Strings

O tipo de strings aceito será um vetor de char (`char[]`)

### IO

Será utilizado funções simples para entrada e saída, não sendo criado todo o libc. As funções serão:
- readInt
- readFloat
- readChar
- readString
- printInt
- printFloat
- printChar
- printString

## Estruturas em Segundo Plano

O foco inicial do desenvolvimento é criar um compilador que contemple a estrutura básica apresentada acima e caso tudo seja finalizado antes do tempo previsto, algumas estruturas podem ser adicionadas, sendo elas:
- Structs
- Ponteiros
- Operadores Unários
