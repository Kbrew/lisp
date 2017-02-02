redo-ifchange ./config ./link-exe
source ./config

OBJS="$(basename $1 ${EXE_EXT}).o"
redo-ifchange $OBJS

./link-exe ${OBJS} -o $3
