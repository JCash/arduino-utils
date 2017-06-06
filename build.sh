
BUILD=build

mkdir -p $BUILD

clang++ -O0 src/image2header.cpp -o $BUILD/image2header