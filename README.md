# LibGNME 
A C++ library for evaluating non-orthogonal matrix elements in electronic structure.
<ul>
<li>Hugh G. A. Burton (2022&ndash;)</li>
</ul>

If you incorporate this code in your work, please consider citing the following work:
<ol reversed>
<li>"Generalised nonorthogonal matrix elements for arbitrary excitations"</li>

<li>"Generalized nonorthogonal matrix elements: Unifying Wick’s theorem and the Slater–Condon rules"; <a href="https://doi.org/10.1063/5.0045442"><i>J. Chem. Phys.</i> <b>154</b>, 144109 (2021)</a>
</ol>

## Installation
### Prerequisites
The libGNME package requires a set of standard libraries:
1. LAPACK, BLAS or Intel MKL
2. OpenMP
3. CMake (version 3.12 or higher)

### Compilation

```bash
mkdir build && cd build
cmake ..
make -j2
```

### Running the example

```bash
cd build/example
cp overlap ../../example
cd ../../example
./overlap
```

### Changing the example

In build

```bash
make
```

In the `example` folder

```bash
cp ../build/example/overlap .
./overlap
```

