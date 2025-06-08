# Analysis of Algorithms I (MC458) - Project 2

- [Requirements](./Enunc.pdf)
- [Instructions](./Instructions.pdf)
- [Report](./relatorio/main.pdf)

---

## `escalada` - Minimum-Risk Climb

CLI program in C/C++ that reads an $n \times m$ grid of positive costs and outputs **one integer**:
the minimum total cost of a path that starts on **any cell of the first row** and reaches **any cell of the last row**, moving only to the three blocks directly above (up-left, up, up-right).

The implemented algorithm is a bottom-up dynamic-programming sweep that fills one DP row per lattice row, running in $\Theta(nm)$ time and $\Theta(nm)$ space. See the report for proof of optimality and complexity details.

---

## Building

```sh
make
```

### Running Tests

```sh
make test
```
