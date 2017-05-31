//
//  writeMatrix.hpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#ifndef writeMatrix_hpp
#define writeMatrix_hpp

#include <stdio.h>
#include <fstream>

#include "mapStorage.hpp"

namespace SIWIR2 {
    
    namespace FEM {

        template<typename T>
        class writeMatrix {
            
        private:
            const MatrixMap<T> * const data_ ;
            
        public:
            
            explicit writeMatrix(const MatrixMap<T> &A) ;
            ~writeMatrix() ; 
            
            void write_to_file(std::ofstream &out) ;
            
        };
        
    }
}




#endif /* writeMatrix_hpp */
