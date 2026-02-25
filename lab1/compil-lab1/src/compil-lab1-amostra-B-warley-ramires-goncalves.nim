import strutils

type
  Ponto = object
    x: int
    y: int

type
  Triangulo = object
    a: Ponto
    b: Ponto
    c: Ponto
    cor: int

proc calcFunc(v: seq[Ponto], n: int, t: Triangulo): float =
  if n <= 0:
    return 1.0
  elif n == 1:
    return 1.01 + float(v[0].x) / 1.0e2 + float(v[0].y) / 0.1e-2 - float(t.a.x * t.a.x) + float(t.b.y * t.c.x)
  var res: float = 0.25e-13
  for i in countdown(n - 1, 0):
    if v[i].x <= 0:
      break
    var temp = float(v[i].y * v[i].x mod 123)
    if temp < 0.0:
      res -= res * 2.0e-2 + calcFunc(v, n - 1, t) * temp - float(t.a.y * t.cor)
    else:
      res += res * 0.3e3 + calcFunc(v, n - 2, t) * temp + float(t.c.x * t.cor)
      echo "Estranho, ne?"
  return res

proc f2(t: Triangulo): int =
  var a = 0
  var soma: array[10, float]
  if (t.a.x >= 10 or t.b.y > 20 or t.a.y < 30 or t.b.x <= 50) and not (t.c.x != 90 or t.c.y == 0):
    return 10 mod 3
  else:
    a = 1
  while a < 10:
    var total = 0
    total += t.c.x * t.c.y
    total += t.b.x * t.a.y
    total += t.a.x * t.b.y
    soma[a] = float(total mod 100)
    a = a + 1
