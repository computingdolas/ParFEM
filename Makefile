#Declaring the variables
CC=icpc 

#Declaring the flags
CFLAGS = -c -std=c++11 -qopenmp -O3 -Wall -pedantic
FLAGS  = -std=c++11 -qopenmp -O3 -Wall -pedantic

#Declaring the ColSamm Folder 
lib    = Source/ 		

all 				:FEM 

FEM 				:Grid_Refinement.o writeMatrix.o mapStorage.o functions.o SolverFunction.o DomainData.o Initial.o Compresed_Row_Storage.o
				$(CC) $(FLAGS) Grid_Refinement.o writeMatrix.o mapStorage.o functions.o SolverFunction.o DomainData.o Initial.o Compresed_Row_Storage.o Simulation.cpp -o waveguide

Grid_Refinement.o		:Grid_Refinement.cpp
				$(CC) $(CFLAGS) Grid_Refinement.cpp

writeMatrix.o			:writeMatrix.cpp
				$(CC) $(CFLAGS) writeMatrix.cpp

mapStorage.o			:mapStorage.cpp
				$(CC) $(CFLAGS) mapStorage.cpp

functions.o			:functions.cpp
				$(CC) $(CFLAGS) functions.cpp

SolverFunction.o		:SolverFunction.cpp
				$(CC) $(CFLAGS) SolverFunction.cpp

DomainData.o			:DomainData.cpp
				$(CC) $(CFLAGS) DomainData.cpp

Initial.o			:Initial.cpp
				$(CC) $(CFLAGS) Initial.cpp

Compresed_Row_Storage.o		:Compresed_Row_Storage.o
				$(CC) $(CFLAGS) Compresed_Row_Storage.cpp

clean				:
				rm -rf *.o ksq.txt A.txt M.txt eigenmode.txt Lambda.txt waveguide

		
