# Lab 3 - Analisador Semantico Nim

Este diretorio contem a adaptacao do lab3 para a linguagem sorteada Nim.

## Compilar

Em ambiente com `make` e `g++` no PATH:

```sh
make
```

No Windows deste workspace, foi usado o `g++` instalado junto com Nim:

```powershell
& 'C:\Program Files\nim\nim-2.2.8\dist\mingw64\bin\g++.exe' -std=c++11 -Wall -Wextra -Wno-sign-compare -Isrc -Isrc/src-gram9 src\sol.cpp src\Arvore.cpp src\Gramatica.cpp src\Parser.cpp src\TabelaLR1.cpp src\debug-util.cpp src\matriz-util.cpp src\src-gram9\*.cpp -o compilador.exe
```

## Executar

Formato:

```sh
./compilador gramatica-nim/gramatica-nim.site gramatica-nim/tabela_lr1.conf ins/0-soma.params < ins/0-soma.tokens
```

Tambem funciona sem passar gramatica/tabela, pois o padrao ja aponta para `gramatica-nim`:

```sh
./compilador ins/0-soma.params < ins/0-soma.tokens
```

No PowerShell, use:

```powershell
Get-Content ins\0-soma.tokens | .\compilador.exe gramatica-nim\gramatica-nim.site gramatica-nim\tabela_lr1.conf ins\0-soma.params
```

## Parametros

O arquivo `.params` usa um valor por linha, na ordem dos parametros da funcao.

Exemplo para:

```nim
proc soma(a: int, b: int): int =
  return a + b
```

`ins/0-soma.params`:

```text
10
20
```

## Gerar Tokens Pelo Lab1

O script `gera-tokens.ps1` roda o lab1 alterado e acrescenta o marcador final `$ $`.

```powershell
powershell -ExecutionPolicy Bypass -File .\gera-tokens.ps1 -EntradaLing ins\0-soma.ling -SaidaTokens ins\0-soma.tokens
```

## Exemplos

Os arquivos em `ins/` seguem o formato pedido:

- `X.ling`: programa Nim de entrada.
- `X.tokens`: tokens gerados pela fase lexica.
- `X.params`: parametros usados na simulacao.
- `X.saida`: saida produzida pelo analisador.

Foram convertidos todos os 36 exemplos originais do diretorio `exemplos/`:

- `0-retorna_soma_int` ate `25-shadowing_duplo_aninhado`: exemplos validos.
- `e1-retorno_float_em_int` ate `e10-retorno_float_em_bool`: exemplos de erro semantico.

Tambem foram adicionados exemplos de chamada de funcao:

- `26-chamada_funcao`
- `27-chamada_recursiva`
- `e11-funcao_nao_declarada`
- `e12-parametros_chamada_incorretos`

Tambem foram adicionados exemplos de array:

- `28-array_basico`
- `29-array_while`
- `e13-indice_array_float`
- `e14-array_fora_limite`
- `e15-array_atribuicao_tipo`

Tambem existem alguns exemplos auxiliares menores (`0-soma`, `1-retorna-float`, etc.) usados durante o desenvolvimento.
