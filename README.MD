# Sdk of opp language

## Build

```shell
$ conan create ./conan/recipes/llvm/all --build missing -s compiler.cppstd=17 -s compiler.runtime=static -o conan_center_index_limits=False --version=17.0.2 -s build_type=Debug
$ conan install . --build missing -s compiler.cppstd=17 -s compiler.runtime=static -s build_type=Debug
$ conan build . -s compiler.cppstd=17 -s compiler.runtime=static -s build_type=Debug
```
