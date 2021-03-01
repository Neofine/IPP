#!/bin/bash

# JAKUB PANASIUK 418362

for f in $2/*.in
do

valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --quiet ./$1 <$f >$2/test.out 2>$2/test.err;

# wypisuje cala sciezke pliku przetwarzanego w tym momencie

echo ${f%.in}; 

# wywoluje 2 funkcje diff, jedna dla pliku out a druga dla pliku err
# jezeli ktorys z nich sie nie zgadza to wypisuje WRONG, w przeciwnym wypadku OK
if diff ${f%.in}.out $2/test.out>/dev/null && diff ${f%.in}.err $2/test.err>/dev/null
then
	echo "OK";
else 
	echo "WRONG";
fi	

done

# usuwam pliki ktore stworzylem podczas testowania

rm $2/test.out;
rm $2/test.err;
