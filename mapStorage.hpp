//
//  mapStorage.hpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright �� 2016 Sagar Dolas. All rights reserved.
//

#ifndef mapStorage_hpp
#define mapStorage_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <omp.h>

#include "Type.h"
#include "functions.hpp"

namespace SIWIR2 {
    
    namespace FEM {
        
        template<typename T>
        class MatrixMap {
            
        private:
            const real_l numvertices_ ;
            std::vector<std::map<real_l,T>> Matrix_ ;

            
        public:
            
            // Constructor and Destructor
            MatrixMap(const real_l numVertex) ;
            ~MatrixMap() ;
            
            // Acess operator
            const T & operator ()(const real_l row, const real_l col)  const ;
            T & operator() (const real_l row , const real_l col ) ;
            
            // Returning iterator
            typename std::map<real_l, T>::const_iterator begin(const real_l position) const;
            typename std::map<real_l, T>::const_iterator end(const real_l position) const ;
            
            // Size of the map
            const real_l size() const ; 
            
        };
        
    }
}

#endif /* mapStorage_hpp */
