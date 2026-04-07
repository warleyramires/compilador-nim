proc fatorialA(n: int): int =
  if n <= 1:
    return 1
  return n * fatorialA(n - 1)

proc fatorialB(n: int): int =
  var a: int = n - 1
  if n <= 1:
    return 1
  return n * fatorialB(a)

proc fatorialC(n: int, P: int): int =
  if n <= 1:
    return 1
  return fatorialC(n - 1, P * n)

proc fatorialD(n: int, P: int): int =
  var a: int = n - 1
  var b: int = P * n
  if n <= 1:
    return 1
  return fatorialD(a, b)

proc fatorialE(n: int, P: int): int =
  if n <= 1:
    return 1
  else:
    var a: int = n - 1
    var b: int = P * n
    return fatorialD(a, b)

proc C1(a: int, b: int): int =
  if a < b:
    return 1
  return 2

proc C2(a: int, b: int): int =
  if a <= b:
    return 1
  return 2

proc C3(a: int, b: int): int =
  if a > b:
    return 1
  return 2

proc C4(a: int, b: int): int =
  if a >= b:
    return 1
  return 2

proc C5(a: int, b: int): int =
  if a == b:
    return 1
  return 2

proc C6(a: int, b: int): int =
  if a != b:
    return 1
  return 2

proc D1(a: int, b: int): int =
  if a != b and a < b:
    return 1
  return 2

proc D2(a: int, b: int): int =
  if not (a != b and a < b):
    return 1
  return 2

proc D3(a: int, b: int): int =
  if (not (a != b and a < b)) or (a + 2 == b):
    return 1
  return 2

proc E1(a: int, b: int): int =
  var X: bool = (not (a != b and a < b)) or (a + 2 == b)
  if a != b or X or a + 5 == b:
    a = a + 1
  return 2

proc E2(a: int, b: int): int =
  var X: bool = (not (a != b and a < b)) or (a + 2 == b)
  while a != b or X or a + 5 == b:
    a = a + 1
    while a < b:
      b = b - 1
  return 2

proc func1(n: int): int =
  if n <= 0:
    return 1
  elif n == 1:
    return 101
  var res: int = 0
  var i: int = n - 1
  while i >= 0:
    var temp: int = 0
    if temp < 0:
      res = res - res * 2 + func1(n - 1) * temp
    else:
      res = res + res * 3 + func1(n - 2) * temp
    i = i - 1
  return res

proc F2(n: int): int =
  var A: int = 0
  var soma: array[10, int]
  if n >= 10:
    return 10 mod 3
  else:
    A = 1
  while A < 10:
    var total: int = 0
    total = total + A * n
    soma[A] = total mod 100
    A = A + 1
  return A

proc F1(ax: float, ay: float, bx: float, by: float, px: float, py: float): float =
  var abx: float = bx - ax
  var aby: float = by - ay
  var apx: float = px - ax
  var apy: float = py - ay
  var ab2: float = abx * abx + aby * aby
  var t: float = (apx * abx + apy * aby) / ab2
  if t < 0.0:
    t = 0.0
  if t > 1.0:
    t = 1.0
  var qx: float = ax + t * abx
  var qy: float = ay + t * aby
  var dx: float = px - qx
  var dy: float = py - qy
  return dx * dx + dy * dy
