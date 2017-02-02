redo-ifchange ./config
source ./config

echo "${CXX} $\"\$@\" ${CXXFLAGS} ${LDFLAGS}"  > link-exe
