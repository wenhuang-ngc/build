Instructions for installing LLVM from sources can be found at:
[https://llvm.org/docs/GettingStarted.html](https://llvm.org/docs/GettingStarted.html)


```
# Install dependencies on Ubuntu Linux system
sudo apt update
# sudo apt upgrade
sudo apt install cmake gcc g++ build-essential checkinstall 
sudo apt install python python-pip python-dev python-pygments python-yaml 
sudo apt install python3 python3-pip python3-dev python3-pygments python3-yaml 
sudo apt install llvm ocaml opam 
opam init
eval `opam config env`
opam install ctypes ctypes-foreign 
opam depext conf-llvm.6.0.0
opam depext conf-m4.1
opam depext conf-pkg-config.1.1
sudo apt install libxml2-dev libplist-dev libedit-dev llvm-7-tools
sudo apt install curl swig doxygen gccgo

# Create top level directory  
mkdir -p ~/closure

# the z3 package is outdated, need to build from source
# may not be an issue in Ubuntu 19.10
cd ~/closure
git clone https://github.com/Z3Prover/z3
cd z3
python scripts/mk_make.py
cd build
make -j24
sudo checkinstall 

# Fetch latest LLVM sources from our fork
# XXX: LLVM master & older versions at https://github.com/llvm/llvm-project.git
cd ~/closure
git clone https://github.com/gaps-closure/llvm-project.git

# Build LLVM projects
cd llvm-project

# Optionally switch to qualatypes branch which has type annotation mods
# master branch mostly tracks the upstream repo
git checkout qualatypes

mkdir build
cd build
# For a full-featured build (fails on workhorse)
# Note: Can use 'all' instead of list of projects
# XXX: Likely symptoms we need a newer GNU toolchain
# XXX: -- build fails on some lldb plugins for gdb-remote
# XXX: -- build also fails on llgo, particularly llgoi on cmath
# XXX: -- numerous CMake errors related to std-cxx-17
# cmake -G 'Unix Makefiles' -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra;compiler-rt;debuginfo-tests;libclc;libcxx;libcxxabi;libunwind;lld;lldb;llgo;openmp;parallel-libs;polly;pstl' ../llvm
#
# Following works on workhorse server
# cmake -G 'Unix Makefiles' -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra;compiler-rt;debuginfo-tests;libclc;libcxx;libcxxabi;libunwind;lld;openmp;parallel-libs;polly;pstl' -DCLANG_PYTHON_BINDINGS_VERSIONS='2.7;3.5' -DLLVM_TARGETS_TO_BUILD='X86' -DCMAKE_BUILD_TYPE=Release ../llvm
# Alternatively, build a smaller subset (quicker)
cmake -G 'Unix Makefiles' -DLLVM_ENABLE_PROJECTS='clang;libclc;libcxx;libcxxabi;lld' -DCLANG_PYTHON_BINDINGS_VERSIONS='2.7;3.5' -DLLVM_TARGETS_TO_BUILD='X86' -DCMAKE_BUILD_TYPE=Release ../llvm
make -j24
# For some reason ocaml_doc does not get built
make ocaml_doc 

# Optionally install under default prefix /usr/local
# XXX: you can always go to build/bin and invoke tools from there
sudo make install

# Create and test sample program
cd ..
cat << EOF > helloworldmars.cpp
#include <iostream>

using namespace std;

string prt() 
{
  string __attribute__((annotate("HIGH_1"))) str = "Hello World, Mars!";
  return str;
}

int main()
{
  cout << prt() << endl;
  return 0;
}
EOF

# Compile and run program
clang++ helloworldmars.cpp -stdlib=libc++
./a.out

# Compile and generate LLVM IR
clang -S -emit-llvm helloworldmars.cpp -stdlib=libc++

# Clean up the build 
rm -rf build
```
