# myTVM
a TVM replica to study AI compilers


# build requirements
- cmake
- conan


# Build commands
```bash
conan install . --output-folder=_conan --build=missing
cmake .. -DCMAKE_TOOLCHAIN_FILE=../_conan/conan_toolchain.cmake

