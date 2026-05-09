# Lab 3 - Plano de Sprints

Entrega: 23/05/2026.

Objetivo do lab: adaptar a base do `lab03-atual` para construir uma representacao em memoria de uma funcao da linguagem, executar analise semantica de tipos e simular a execucao da funcao com parametros, imprimindo o ultimo valor retornado/atribuido conforme o tipo.

## Contexto Inicial

- Base principal: `lab3/lab03-atual/lab03-atual`.
- A base do professor vem configurada com `gramatica-9/gramatica-9.site` e `gramatica-9/tabela_lr1.conf`, mas a entrega final deve usar uma gramatica/tabela coerente com a linguagem sorteada, Nim. Se a gramatica 9 for reaproveitada como fatia intermediaria, isso precisa ser tratado apenas como etapa de desenvolvimento, nao como solucao final sem adaptacao.
- A base atual ja extrai uma `Funcao` com tipo de retorno, nome, parametros e comandos.
- A gramatica 9 atual cobre uma sintaxe parecida com C para funcao, parametros, declaracao, atribuicao, bloco, `return`, literais, variaveis e expressoes com `+`, `-`, `*`, `/`, `%`, `<`, `==`, `!`.
- O enunciado tambem pede operadores relacionais/logicos mais amplos, `if-then-else` e `while`. Isso ainda nao aparece na gramatica 9 atual nem nas classes de comando, e deve ser resolvido na fatia Nim final ou justificado com o professor se houver restricao aceita.
- O diretorio final `ins/` deve conter, para cada exemplo, arquivos `.ling`, `.tokens`, `.params` e `.saida`.

## Sprint 0 - Reconhecimento e Baseline

Meta: garantir que sabemos compilar, executar e reproduzir o comportamento inicial.

Status atual:

- Feito: base compilada com `g++` encontrado em `C:\Program Files\nim\nim-2.2.8\dist\mingw64\bin\g++.exe`.
- Feito: exemplo `exemplos/0-retorna_soma_int.tokens` parseia e extrai a funcao `soma`.
- Observado: `make` e `g++` nao estao no PATH do terminal atual.
- Observado: a saida ainda esta muito verbosa por causa dos debugs do parser/tabela/arvore.
- Observado: `Analisador::calcula_retorno` ainda e stub e retorna sempre `0`.
- Atualizado depois: `main` agora silencia os debugs internos e imprime somente o resultado final ou `ERRO SEMANTICO: ...`.

Tarefas:

- Compilar `lab03-atual` com `make`.
- Rodar exemplos validos existentes em `exemplos/*.tokens`.
- Separar exemplos que devem passar de exemplos `e*.tokens` que devem gerar erro semantico.
- Registrar o formato atual da entrada de tokens e decidir o formato dos arquivos `.params`.
- Identificar mensagens/debug excessivos que precisarao ser limpos antes da entrega.

Aceite:

- Build funcionando.
- Lista curta de comandos de execucao documentada.
- Pelo menos 1 exemplo valido parseado com sucesso.
- Pelo menos 1 exemplo de erro parseado, mesmo que ainda sem erro semantico implementado.

## Sprint 1 - Modelo de Valores, Tipos e Escopos

Meta: criar a infraestrutura semantica antes de implementar operadores.

Status atual:

- Feito: `Funcao::extrai_funcao` agora aceita a arvore Nim `Program/DeclList/FuncDecl` da gramatica do lab2.
- Feito: parametros Nim `ParamList/Param`, tipos `TYPE_INT`, `TYPE_FLOAT`, `TYPE_BOOL` e blocos `NEWLINE INDENT ... DEDENT` sao extraidos.
- Feito: ambiente de execucao com pilha de escopos, declaracao, busca e shadowing.
- Feito: `Analisador::calcula_retorno` agora retorna `ValorLiteral`, nao apenas `int`.
- Parcial: `main` ainda usa parametros fixos de teste; leitura de `.params` fica para Sprint 5.
- Feito: `main` le arquivo `.params` opcional, com um valor por linha na ordem dos parametros da funcao.

Tarefas:

- Ajustar `Analisador` para trabalhar com `ValorLiteral` em vez de retornar apenas `int`.
- Criar ambiente de execucao com pilha de escopos para parametros, variaveis locais e blocos aninhados.
- Registrar tipo declarado de cada variavel.
- Inicializar parametros a partir de `.params`.
- Definir regras de conversao entre `int`, `float` e `boolean`.
- Definir formato padrao de erro semantico.

Aceite:

- Parametros sao associados por ordem aos parametros da funcao.
- Declaracao duplicada no mesmo escopo gera erro.
- Uso de variavel inexistente gera erro.
- Shadowing em bloco interno funciona, pois ha exemplos para isso.

## Sprint 2 - Avaliacao de Expressoes

Meta: avaliar expressoes com tipos corretos e detectar incompatibilidades.

Status atual:

- Feito: avaliacao de literais, variaveis, operadores aritmeticos, relacionais, igualdade, `and`, `or`, `not` e menos unario.
- Feito: exemplo Nim `soma(a: int, b: int): int` retornou `50` com parametros de teste `10` e `20`.
- Feito: exemplo Nim com `if a < b: return b` retornou `20` com parametros de teste `10` e `20`.
- Feito: `float` e impresso com 2 casas.
- Pendente: revisar regras exatas de conversao Nim; hoje ha promocao de `int` para `float`.

Tarefas:

- Implementar avaliacao de `ExpressaoValor` e `ExpressaoVariavel`.
- Implementar operadores aritmeticos `+`, `-`, `*`, `/`, `%`.
- Implementar operadores relacionais existentes na gramatica: `<` e `==`.
- Implementar negacao logica `!`.
- Aplicar promocoes coerentes, principalmente `int + float -> float`.
- Bloquear combinacoes invalidas, como `!float`, `boolean < int`, `boolean + int`, `%` com float ou boolean.

Aceite:

- Exemplos validos de soma, produto, divisao, modulo, comparacao e parenteses retornam o valor esperado.
- Exemplos `e*.tokens` relacionados a tipos invalidos falham com mensagem clara.
- Impressao final respeita tipo: inteiro decimal, float com 2 casas, boolean como `true`/`false`.

## Sprint 3 - Execucao de Comandos

Meta: executar o corpo da funcao em ordem e respeitar blocos.

Status atual:

- Feito: execucao de declaracao, atribuicao, bloco, `if`, `while` e `return`.
- Parcial: testado erro semantico de `bool + int`, que gera mensagem clara.
- Pendente: testar mais casos de `while`, booleanos e erros semanticos.
- Pendente: suporte real a arrays e chamada de funcao ainda nao foi implementado.

Tarefas:

- Implementar execucao de declaracao.
- Implementar execucao de atribuicao com checagem do tipo da variavel.
- Implementar execucao de bloco com novo escopo.
- Implementar execucao de `return`.
- Decidir se "ultimo valor atribuido no ultimo comando" sera tratado como retorno explicito, ultima atribuicao ou ambos, conforme exemplos e enunciado.

Aceite:

- Funcoes com declaracoes e atribuicoes simples retornam corretamente.
- Blocos aninhados nao vazam variaveis internas para fora.
- Shadowing dos exemplos 21 a 25 funciona.
- Retorno com tipo diferente do tipo declarado da funcao gera erro.

## Sprint 4 - Cobertura Obrigatoria Ainda Ausente

Meta: decidir e implementar o que falta entre a gramatica atual e o enunciado.

Tarefas:

- Decidir se vamos adaptar a gramatica do lab2/Nim para o lab3 ou usar a gramatica 9 apenas como guia para reconstruir uma fatia Nim equivalente.
- Adicionar tokens/regras/classes Nim para `if-else` e `while`.
- Adicionar operadores Nim equivalentes a `&&`, `||`, `<=`, `>`, `>=`, `!=` conforme a sintaxe da linguagem.
- Gerar ou ajustar `tabela_lr1.conf` apos mudancas de gramatica.
- Criar classes de AST e execucao para novos comandos/operadores.

Aceite:

- A decisao fica documentada para apresentacao ao docente.
- Se implementado, ha exemplos `.tokens` validos para `if-else` e `while`.
- A tabela LR e a extracao de AST continuam funcionando com tokens gerados pelo analisador lexico do lab1 para Nim.

## Sprint 5 - Integracao com Lab 1 e Artefatos `ins/`

Meta: produzir os arquivos esperados na entrega.

Tarefas:

- Escolher conjunto final de exemplos `.c` a converter para a linguagem sorteada.
- Criar `ins/`.
- Para cada exemplo `X`: criar `X.ling`, gerar `X.tokens`, criar `X.params` e salvar `X.saida`.
- Rodar a fase lexica do lab1 alterado para gerar tokens quando aplicavel.
- Conferir se os tokens gerados batem com os terminais da gramatica 9.

Aceite:

- `ins/` contem pares completos `.ling`, `.tokens`, `.params`, `.saida`.
- As saidas foram geradas pelo executavel final.
- Ha exemplos validos e exemplos de erro semantico.

Status atual:

- Feito: criado primeiro exemplo completo `ins/0-soma.*`.
- Feito: `ins/0-soma.tokens` com `ins/0-soma.params` retorna `30`.
- Feito: lab1 alterado foi testado sobre `ins/0-soma.ling`; ele gera os tokens esperados e so precisa do marcador final `$ $`.
- Feito: criado `gera-tokens.ps1` para gerar `.tokens` a partir de `.ling` usando o lab1 alterado.
- Feito: adicionados exemplos completos `1-retorna-float`, `2-retorna-bool`, `3-if-else`, `4-while`, `e1-erro-retorno`, `e2-erro-operador`, `e3-erro-atribuicao`.
- Feito: os `.tokens` desses exemplos foram gerados pelo lab1 alterado.
- Feito: as `.saida` foram geradas pelo analisador atual.
- Feito: adicionados testes `5-shadowing`, `6-logicos-relacionais`, `e4-variavel-nao-declarada`, `e5-duplicada-mesmo-escopo`, `e6-condicao-nao-bool`.
- Feito: todas as `.saida` de `ins/` foram regeneradas apos a melhoria das mensagens de erro.
- Feito: suporte a `elif` implementado como cadeia de `if` aninhado; exemplo `7-elif` retorna `2`.
- Feito: convertidos todos os 36 exemplos originais de `exemplos/*.c` para `ins/*.ling`, com respectivos `.tokens`, `.params` e `.saida`.
- Feito: os 36 exemplos originais foram executados com sucesso: 26 retornos validos e 10 erros semanticos esperados.
- Feito: chamada de funcao implementada em expressoes, incluindo multiplas funcoes no mesmo programa e recursao.
- Feito: adicionados exemplos `26-chamada_funcao`, `27-chamada_recursiva`, `e11-funcao_nao_declarada`, `e12-parametros_chamada_incorretos`.
- Feito: arrays implementados para declaracao `array[tamanho, tipo]`, escrita `a[i] = valor` e leitura `a[i]`.
- Feito: adicionados exemplos `28-array_basico`, `29-array_while`, `e13-indice_array_float`, `e14-array_fora_limite`, `e15-array_atribuicao_tipo`.

## Sprint 6 - Limpeza, Teste Final e Empacotamento

Meta: deixar o projeto pronto para submissao.

Tarefas:

- Remover `.o`, executaveis e arquivos temporarios.
- Conferir se o `Makefile` compila em ambiente limpo.
- Remover ou controlar debug verboso (`parser.tabela.debug`, `parser.gram.debug`, `arv.debug`) se atrapalhar a saida exigida.
- Criar um README curto com como compilar, executar e formato de `.params`.
- Gerar o `.zip` final com o diretorio da solucao.

Aceite:

- `make clean && make` funciona.
- `make run < ins/X.tokens` funciona com os parametros definidos.
- Zip final nao contem binarios/objetos.
- A estrutura bate com o enunciado.

## Ordem Recomendada

1. Sprint 0 para validar o ponto de partida.
2. Sprint 1 e Sprint 2 para fechar a base semantica.
3. Sprint 3 para executar programas completos da gramatica atual.
4. Sprint 4 somente depois que a fatia atual estiver estavel.
5. Sprint 5 e Sprint 6 para entrega.

## Riscos

- A gramatica atual nao cobre tudo que o enunciado lista. Isso pode exigir mexer em gramatica, tabela LR, AST e analisador.
- O `Analisador::calcula_retorno` atualmente retorna `int`, mas o enunciado exige `int`, `float` e `boolean`.
- O `main` ainda usa parametros fixos de exemplo; sera necessario ler parametros reais.
- O debug atual pode poluir a saida `.saida`.
- A integracao com o lab1 precisa garantir que os nomes de tokens de Nim coincidam exatamente com a gramatica/tabela usadas no lab3.
