param(
  [Parameter(Mandatory=$true)]
  [string]$EntradaLing,

  [Parameter(Mandatory=$true)]
  [string]$SaidaTokens,

  [string]$Lab1Src = "..\..\..\warley-ramires-goncalves-lab2\lab1_alterado\src"
)

$lab1Resolvido = Resolve-Path -LiteralPath $Lab1Src
$entradaResolvida = Resolve-Path -LiteralPath $EntradaLing

$tokens = Get-Content -LiteralPath $entradaResolvida | java -cp $lab1Resolvido CompilLab1
$tokens + '$ $' | Set-Content -LiteralPath $SaidaTokens
