# Image Processor Implemented in C++ with OpenMP 
Distributed computing and parallel programming concepts implemented with a gritty image processor

![Alt text](https://github.com/falmatam/image-processor-openMP/blob/master/monalisa.PNG "Mona Lisa")

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites

G++ compiler, OpenMP binary (see Testing Locally section below)


### Installing

Simply download ZIP of project or clone the repository

```
cd proj_directory_on_your_computer
git init
git clone https://github.com/falmatam/image-processor-openMP.git
```

Done.



### Testing Locally

Be sure to have local copy of OpenMP installed
> Find download link here --> http://www.openmp.org/resources/openmp-compilers/

```
cd proj_directory_on_your_computer
g++   -fopenmp   processor.c
./a.out <Input image filename> <Output image filename> <Chunk size> <a1/a2>
```



## Acknowledgments

* OpenMP
* Distributed and Parallel Programming founders Luigi Federico Menabrea, Charles Babbage and S. Gill (Ferranti)