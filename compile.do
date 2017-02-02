redo-ifchange ./config
source ./config

echo "${CC} ${CFLAGS} \"\$@\"" > $3
