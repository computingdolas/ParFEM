//
//  writeMatrix.cpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright �� 2016 Sagar Dolas. All rights reserved.
//

#include "writeMatrix.hpp"

template<typename T>
SIWIR2::FEM::writeMatrix<T>::writeMatrix(const SIWIR2::FEM::MatrixMap<T> &A) : data_(&A) {
        
}
template<typename T>
SIWIR2::FEM::writeMatrix<T>::~writeMatrix<T>(){
    
}
template<typename T>
void SIWIR2::FEM::writeMatrix<T>::write_to_file(std::ofstream &out){
    
    // Temporary Variables
    const real_l N  = (*data_).size() ;

    //Sweep across all the rows
    for (real_l row = 0 ; row < N; ++row) {
        
        // Go to each rows and print to file untill it ends
        for (auto col = (*data_).begin(row); col!=(*data_).end(row); ++col) {
            out<<row<<" "<<col->first<<" "<<col->second<<std::endl ;
        }
    }
}





