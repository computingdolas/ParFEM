//
//  Compresed_Row_Storage.cpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#include "Compresed_Row_Storage.hpp"

template<typename T,class MatrixStorage>
SIWIR2::FEM::Compressed_Row_Storage<T,MatrixStorage>::Compressed_Row_Storage(const MatrixStorage &M) : M(&M) {
    
}

template<typename T ,class MatrixStorage>
SIWIR2::FEM::Compressed_Row_Storage<T,MatrixStorage>::~Compressed_Row_Storage<T,MatrixStorage>(){
    
    
}

template<typename T ,typename MatrixStorage>
void SIWIR2::FEM::Compressed_Row_Storage<T,MatrixStorage>::StoreCRSFormat(){
    
    // temp variable
    const real_l  N= (*M).size() ;
    real_l rowcount = 0 ;
    // Sweep across all the rows in the map
    
    for (real_l row = 0; row < N; ++row) {
        
        //Pushing the row
        rowpointers.push_back(rowcount) ;
        
        // Sweep across the columns
        for (auto col = (*M).begin(row); col != (*M).end(row); ++col) {
            // Pushing the columns in CRS columns
            coldata.push_back( col->first ) ;
            value.push_back(col->second) ;
            ++rowcount ;
        }
    }
    rowpointers.push_back(rowcount) ;
    
}

template<typename T,typename MatrixStorage>
const SIWIR2::FEM::DomainData<T> SIWIR2::FEM::Compressed_Row_Storage<T, MatrixStorage>::operator*(const SIWIR2::FEM::DomainData<T> &v) const{
    
    //Here Matrix Vector multiplication takes place
    
    SIWIR2::FEM::DomainData<real_d> c(v.size(),SIWIR2::FEM::dataType::normal) ;
    real_l iter = 0 ;
    
    for (real_l row =0; row < c.size() ; ++row) {
        for (real_l j = rowpointers[iter] ; j < rowpointers[iter+1] ; ++j) {
            c[row] += value[j] * v[coldata[j]] ;
        }
        ++iter ;
    }
    
    return c ; 
    
}



