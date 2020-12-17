set n (for arq in (ls testes/entradas/*.in); head -n 1 $arq | awk '{print$1}'; end | python max.py)
set m (for arq in (ls testes/entradas/*.in); head -n 1 $arq | awk '{print$2}'; end | python max.py)
set c (for arq in (ls testes/entradas/*.in); tail -n +2 $arq; end | python max.py)

echo n = $n
echo m = $m
echo c = $c
