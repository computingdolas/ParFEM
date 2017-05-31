//
//  Compresed_Row_Storage.hpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#ifndef Compresed_Row_Storage_hpp
#define Compresed_Row_Storage_hpp

#include <stdio.h>
#include "mapStorage.hpp"
#include "DomainData.hpp"
namespace SIWIR2 {
    
    namespace FEM {
        
        template<typename T, class MatrixStorage>
        class Compressed_Row_Storage {
            
        private:
            
            const MatrixStorage * const M ;
            
        public:
            
            std::vector<real_d> value ;
            std::vector<real_l> rowpointers ;
            std::vector<real_l> coldata ;
            
            explicit Compressed_Row_Storage(const MatrixStorage &M) ;
            ~Compressed_Row_Storage() ;
            
            void StoreCRSFormat() ;
            
            const SIWIR2::FEM::DomainData<T> operator * (const SIWIR2::FEM::DomainData<T> &v) const ; 
            
        };
        
    }
}

#endif /* Compresed_Row_Storage_hpp */
