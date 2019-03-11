
# Simple Matrix Library
 A simple matrix library that use expression templates and modern concepts of C++ to solve algebric expressions. This project is built for Programming Competency Test under GSoC2O19/Boost/uBLAS.
## Prerequisites
```
Clang (minimum -v7.0.1)
```
## Installation 
1.  Clone the repository.
```
git clone https://github.com/NavneetSurana/uBLAS-Programming-Competency-Test.git
```  
2.  Include lazy_matrix.h in your source file.
```
#include"path-to-the-cloned-repo/include/lazy_matrix.h"
```  
3. Use clang compiler for compilation.
```
clang++ -std=c++17 [your src file name].cpp -o build
```
```
./build
```
## Example Source File
Please view [example.cpp](example.cpp) for reference.
## Features
The table below shows various operations that can be performed using this library along with short description.

| Operators | Expression Templates Used|Description|
|:---:|:--------------------------:|-----------|
| `%`  |   `Yes` | Performs standard Matrix-Matrix Multiplication|
| `%=` |   `No`  | Performs assignment after standard Matrix-Matrix Multiplication|
| `+`  |   `Yes` | Performs element-wise Matrix-Matrix Addition|
| `+=` |   `No`  | Performs assignment after element-wise Matrix-Matrix Addition|
| `-`  |   `Yes` | Performs element-wise Matrix-Matrix Subtraction|
| `-=` |   `No`  | Performs assignment after element-wise Matrix-Matrix Subtraction|
| `/`  |   `Yes` | Performs element-wise Matrix-Matrix Division|
| `/=` |   `No`  | Performs assignment after element-wise Matrix-Matrix Division|
| `*`  |   `Yes` | Performs element-wise Matrix-Matrix Multiplication|
| `*=` |   `No`  | Performs assignment after element-wise Matrix-Matrix Multiplication|



