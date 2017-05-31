//
//  SolverFunction.hpp
//  FEM
//
//  Created by Sagar Dolas on 27/05/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#ifndef SolverFunction_hpp
#define SolverFunction_hpp

#include <memory>
#include <stdio.h>
#include "Initial.hpp"
#include "functions.hpp"
#include <iomanip>
#include "mapStorage.hpp"

namespace SIWIR2 {
    namespace FEM {
        
        template<class CompressedRowStorage>
        class Solver {
            
        private:
        
            const CompressedRowStorage * A ;
            const CompressedRowStorage * M ;
            DomainData<real_d> * u ;
            real_d eps ;
            
            DomainData<real_d> f ; 
            
        public:
            
            real_d lambda ; 
            
            Solver(const CompressedRowStorage &A,
                   const CompressedRowStorage &M,
                   SIWIR2::FEM::DomainData<real_d> &u,
                   const real_d _eps) ;
            
            ~Solver() ;
            
            
            // Solver Capabilities
            bool ConjugateGradient() ;
            void InversePowerIteration() ;
        };
    }
}

#endif /* SolverFunction_hpp */
