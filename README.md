# CS3102-proj

## Requirements

### Ubuntu 18.04 LTS

Install required packages.

> On Ubuntu 16.04 LTS, omit the `clang-tools` package, which is included in the `clang` package on Ubuntu 16.04 LTS.

```bash
$ sudo apt install gcc g++ clang clang-format cmake
```

## Build

```bash
$ cd build
$ cmake ..
$ make
```

## Run

First, perform a build as described in the **Build** section, then run the following
commands in the `build` directory.

### Application

```bash
$ ./cs3102-proj < ../data/green_tripdata_2015-01.csv
```

### Unit tests

Unit tests are written using the [Catch2](https://github.com/catchorg/Catch2/) unit testing
framework.

```bash
$ ./unit_test
```

## Clang Tools

### Clang-Format

For formatting run `clang-format` on the project.

```bash
$ cd build
$ cmake ..
$ make clang-format
```
