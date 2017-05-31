//
//  main.cpp
//  FEM
//
//  Created by Sagar Dolas on 24/05/17.
//  Copyright �� 2016 Sagar Dolas. All rights reserved.
//

// Standard Template Library 
#include <iostream>
#include <fstream>
#include <chrono>

// Local Library 
#include "Initial.hpp"
#include "functions.hpp"
#include "SolverFunction.hpp"
#include "SolverFunction.cpp"
#include "Assemble_Matrix.hpp"
#include "Assemble_Matrix.cpp"
#include "writeMatrix.hpp"
#include "writeMatrix.cpp"
#include "Compresed_Row_Storage.hpp"
#include "Compresed_Row_Storage.cpp"
#include "Grid_Refinement.hpp"
#include "Grid_Refinement.cpp"

using namespace SIWIR2 ;

int main(int argc, const char * argv[]) {
    
    // Declaring the buffer
    FEM::DomainData<real_d> vertex(FEM::dataType::vertex) ;
    FEM::DomainData<real_l> face(FEM::dataType::face) ;
    
    Initial input(argc,argv) ;
    input.filldata(vertex, face) ;
    input.outOnScreen() ;

//     //Allocating memory for 
//     SIWIR2::FEM::MatrixMap<real_d> A((vertex.size()/2)) ;
//     SIWIR2::FEM::MatrixMap<real_d> M((vertex.size()/2)) ;
//     
//     // Assemble the matrix into global matrix map 
//     // Parallelise the assemble operation
//     SIWIR2::FEM::Assemble(A,M,vertex,face,(vertex.size()/2),(face.size()/3),input.delta) ;

    return 0;
}
