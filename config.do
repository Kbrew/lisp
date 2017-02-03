cat <<-EOF > "$3"
CC=clang
CXX=clang
CFLAGS="$(llvm-config --cflags)"
LDFLAGS="$(llvm-config --ldflags --libs --system-libs | tr '\n' ' ')"
CXXFLAGS="$(llvm-config --cxxflags)"

BUILD_SCRIPTS="config compile link-exe"
EOF
