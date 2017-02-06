redo-ifchange ./config ./link-exe
source ./config

MAIN_OBJ="$(basename $1 ${EXE_EXT}).o"
OBJS="$MAIN_OBJ types.o read.o make.o print.o"
redo-ifchange $OBJS

./link-exe ${OBJS} -o $3
