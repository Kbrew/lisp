redo-ifchange ./compile $2.c
./compile -MD -MF $2.d -c $2.c -o $3

#remove carriage returns when building on windows
cat $2.d | tr -d '\r' | read DEPS
redo-ifchange ${DEPS#*:}
