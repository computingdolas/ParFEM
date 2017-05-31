//
//  Assemble_Matrix.hpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#ifndef Assemble_Matrix_hpp
#define Assemble_Matrix_hpp

#include <stdio.h>
#include <omp.h>
#include <chrono>

#include "mapStorage.hpp"
#include "mapStorage.cpp"
#include "DomainData.hpp"
#include "functions.hpp"
#include "Initial.hpp"
#include "Source/Colsamm.h"
#include <vector>


namespace SIWIR2 {
    
    namespace FEM {
        
        class Assemble {
            
        private:
            
            MatrixMap<real_d> * const A_ ;
            MatrixMap<real_d> * const M_ ;
            const DomainData<real_d> * const vertex_ ;
            const DomainData<real_l> * const face_ ;
                        
            const real_l numvertex ;
            const real_l numface ;
            const real_d d ;
            
        public:
        
            // Constructor and Destructor
            explicit Assemble(MatrixMap<real_d> &A,MatrixMap<real_d> &M,const DomainData<real_d> &vertex, const DomainData<real_l> &face,const real_l _numvertex, const real_l _numface,const real_d d ) ;
            
            ~Assemble() ;
            
        };

    }
}

#endif /* Assemble_Matrix_hpp */
