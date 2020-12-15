#!/bin/sh

if [ ! -d saidas ]; then
    mkdir saidas
fi

for f in testes/entradas/*.in;
do
    bname=`basename $f`
    ofile=`echo $bname | sed "s/\.[^\.]*$//"`
    ./escalada < $f > saidas/$ofile.out

    if diff testes/saidas/$ofile.out saidas/$ofile.out > /dev/null; then
	echo "$bname: Correta."
    else
  	echo "$bname: Incorreta."
    fi
done
