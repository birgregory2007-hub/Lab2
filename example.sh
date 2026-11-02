#!/bin/bash
BIN=${1:-./dbms}
FILE=file.data
rm -f "$FILE"

run() {
    echo "\$ $BIN --file $FILE --query '$1'"
    $BIN --file "$FILE" --query "$1" | sed 's/^/  /'
}

echo "=== Задание 1. Стек: значение арифметического выражения ==="
run 'EVAL 2 + 3 * (4 - 1)'
run 'EVAL (1 + 2) * (3 - 5) * -4'
run 'EVAL 2000000000 - 1999999999 * 1'
run 'EVAL 1 2'
run 'EVAL (1 + 2'
run 'EVAL 4 * (5 + )'

echo "=== Задание 2. АТД Множество ==="
for v in 4 10 5 1 3 7; do run "SETADD myset $v"; done
run 'SETADD myset 5'
run 'SET_AT myset 7'
run 'SET_AT myset 8'
run 'SETDEL myset 7'
run 'SETDEL myset 7'
run 'SETADD myset 7'
run 'SETSIZE myset'
run 'PRINT myset'

echo "=== Задание 3. Множество: разбиение на подмножества с равной суммой ==="
run 'SETSPLIT myset 15'
run 'SETSPLIT myset 10'
run 'SETSPLIT myset 8'
run 'SETSPLIT myset'

echo "=== Задание 4. Массив: все различные подмассивы ==="
run 'MCREATE arr'
run 'MPUSH arr x'
run 'MPUSH arr y'
run 'MPUSH arr z'
run 'MSUBS arr'
run 'MPUSH dup 1'
run 'MPUSH dup 1'
run 'MPUSH dup 2'
run 'MSUBS dup'
run 'MGET arr 1'
run 'MGET arr 10'

echo "=== Задание 5. Двоичное дерево: правый край ==="
run 'BBUILD tree 8 5 10 3 9 15 12'
run 'PRINT tree'
run 'BRIGHT tree'
run 'BINSERT tree 20'
run 'BRIGHT tree'
run 'BINORDER tree'
run 'BINSERT tree 20'

echo "=== Задание 6. Хеш-таблица: линейное апробирование и двойное хеширование ==="
run 'HFOLD 523456795'
run 'HFOLD 12345'
run 'HFOLD ab'
run 'HCREATE hlin linear 11'
run 'HCREATE hdbl double 11'
run 'HCREATE hbad double 10'
for k in 523456795 123456789 321654987 111222333 999000111 456; do
    run "HSET hlin $k v$k"
    run "HSET hdbl $k v$k"
done
run 'HHASH hlin 321654987'
run 'HHASH hdbl 321654987'
run 'PRINT hlin'
run 'PRINT hdbl'
run 'HGET hlin 321654987'
run 'HDEL hlin 123456789'
run 'HGET hlin 321654987'
run 'HGET hlin 123456789'
run 'PRINT hlin'
run 'HSTAT hlin'
run 'HBENCH'

echo "=== Задание 7. LRU-кэш ==="
run 'CCREATE cache 2'
run 'CSET cache 1 2'
run 'CSET cache 2 3'
run 'CSET cache 1 5'
run 'PRINT cache'
run 'CSET cache 4 5'
run 'CSET cache 6 7'
run 'PRINT cache'
run 'CGET cache 4'
run 'PRINT cache'
run 'CSET cache 1 2'
run 'CGET cache 3'
run 'PRINT cache'
run 'CRUN 2 SET 1 2 GET 1'
run 'CRUN 2 SET 1 2 SET 2 3 SET 1 5 SET 4 5 SET 6 7 GET 4 SET 1 2 GET 3'
