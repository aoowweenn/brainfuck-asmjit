# HOW TO BUILD
```sh
$ git clone git@github.com:aoowweenn/brainfuck-asmjit.git
$ cd brainfuck-asmjit
$ git submodule init
$ git submodule update
$ mkdir build
$ cd build
$ cmake ..
```
**If you encounter CMAKE newer version problem, change the minimal version of your cmake in `asmjit/CMakeLists.txt`.**

# Usage
```sh
$ cd build
$ make hello
$ make simple
$ make rot13
```
