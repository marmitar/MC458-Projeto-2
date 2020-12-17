MAX = None

try:
    linha = input().strip().split()
    MAX = max(int(s) for s in linha)

    while True:
        linha = input().strip().split()
        lmax = max(int(s) for s in linha)
        MAX = max(MAX, lmax)
except EOFError:
    pass

print(MAX)
