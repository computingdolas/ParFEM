//
//  mapStorage.cpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#include "mapStorage.hpp"
template<typename T>
SIWIR2::FEM::MatrixMap<T>::MatrixMap(const real_l _numvertices): numvertices_(_numvertices) ,Matrix_(_numvertices) {
    
}

template<typename T>
SIWIR2::FEM::MatrixMap<T>::~MatrixMap<T>(){
    
    
}

template<typename T>
const T & SIWIR2::FEM::MatrixMap<T>::operator()(const real_l row, const real_l col) const {

    return Matrix_[row][col];

}

template<typename T>
T& SIWIR2::FEM::MatrixMap<T>::operator()(const real_l row, const real_l col) {
    
    return Matrix_[row][col];
    
}

template<typename T>
const real_l SIWIR2::FEM::MatrixMap<T>::size() const {
    
    return this->Matrix_.size()  ;
    
}

template<typename T>
typename std::map<real_l, T>::const_iterator SIWIR2::FEM::MatrixMap<T>::begin(const real_l position) const {
    
    return Matrix_[position].begin() ; 
    
}

template<typename T>
typename std::map<real_l, T>::const_iterator SIWIR2::FEM::MatrixMap<T>::end(const real_l position) const {
    
    return Matrix_[position].end() ;
}







