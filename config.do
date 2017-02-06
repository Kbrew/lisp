cat <<-EOF > "$3"
CC=gcc
CXX=g++
CFLAGS="$(llvm-config --cflags)"
LDFLAGS="$(llvm-config --ldflags --libs --system-libs | tr '\r\n' '  ')"
CXXFLAGS="$(llvm-config --cxxflags)"

BUILD_SCRIPTS="config compile link-exe"
EOF
