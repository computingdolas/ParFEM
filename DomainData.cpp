//
//  DomainData.cpp
//  FEM
//
//  Created by Sagar Dolas on 24/05/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//


#include "DomainData.hpp"

using namespace SIWIR2::FEM ;

template<typename T>
SIWIR2::FEM::DomainData<T>::DomainData(const real_l _numData, const SIWIR2::FEM::dataType _datatype): numData_(_numData),datatype_(_datatype)  {
    
    
    real_l num = 0 ;
    
    if (datatype_ == FEM::dataType::vertex ) num = numData_ * 2 ;
    else if (datatype_ == dataType::face) num = numData_ * 3 ;
    else if (datatype_== dataType::matrix) num = numData_ * numData_;
    else num = numData_ ;
    
    resize(num, static_cast<T>(0.0)) ;
}

template<typename T>
SIWIR2::FEM::DomainData<T>::DomainData(const SIWIR2::FEM::dataType _datatype):datatype_(_datatype) {
    
}

template<typename T>
SIWIR2::FEM::DomainData<T>::DomainData(const DomainData<real_d>& vertex, const real_d delta) : numData_(vertex.size()/2), datatype_(SIWIR2::FEM::dataType::normal){
    
    resize() ;
    
    if (vertex.datatype_ == SIWIR2::FEM::dataType::vertex) {
        for (real_l i =0 ; i < numData_; ++i) {
            real_l vindex = i * SIWIR2::FEM::geometricMap::vertexindex ;
            real_d x = vertex[vindex] ;
            real_d y = vertex[vindex+1] ;
            data_[i] = ((100.0 + delta) * std::exp(-50.0 * ((x*x) + (y*y)) )) - 100.0 ;
        }
    }
}

template<typename T>
SIWIR2::FEM::DomainData<T>::DomainData(const SIWIR2::FEM::DomainData<T> & obj) {
    
    numData_ = obj.size() ;
    this->data_.resize(numData_,static_cast<T>(0.0)) ;
    
    for (real_l i =0; i < numData_; ++i) {
        data_[i] = obj[i] ;
    }
    
}

template<typename T>
SIWIR2::FEM::DomainData<T>& SIWIR2::FEM::DomainData<T>::operator=(const DomainData<T> &obj){
    
    
    assert(this->size() == obj.size()) ;
    for (real_l i =0 ; i < obj.size(); ++i) {
        data_[i] = obj[i] ;
    }
    
    return (*this) ;
}

template<typename T>
const SIWIR2::FEM::DomainData<T> SIWIR2::FEM::DomainData<T>::operator+(const DomainData<T> &obj) const {
    
    assert((*this).size() == obj.size()) ;

    
    SIWIR2::FEM::DomainData<T> temp(obj.size(), SIWIR2::FEM::dataType::normal) ;
    for (real_l i = 0 ; i < obj.size(); ++i) {
        temp[i] = (*this)[i] + obj[i] ;
    }
    
    return temp ;
    
}

template<typename T>
const SIWIR2::FEM::DomainData<T> SIWIR2::FEM::DomainData<T>::operator-(const DomainData<T> &obj) const {
    
    assert((*this).size() == obj.size()) ;
    
    SIWIR2::FEM::DomainData<T> temp(obj.size(), SIWIR2::FEM::dataType::normal) ;
    for (real_l i = 0 ; i < obj.size(); ++i) {
        temp[i] = (*this)[i] - obj[i] ;
    }
    
    return temp ;
}

template<typename T>
const real_d SIWIR2::FEM::DomainData<T>::operator*(const DomainData<T> &obj) const  {
    
    assert((*this).size() == obj.size()) ;
    real_d sum = 0.0 ;
    
    for (real_l i =0; i < numData_; ++i) {
        sum += (*this)[i] * obj[i] ;
    }
    
    return sum ;
}

template<typename T>
const SIWIR2::FEM::DomainData<T> SIWIR2::FEM::DomainData<T>::operator*(const real_d c) const {
    
    SIWIR2::FEM::DomainData<T> temp((*this).size(), SIWIR2::FEM::dataType::normal) ;

    for (real_l i =0; i < numData_; ++i) {
        temp[i] = (*this)[i] * c ;
    }
    
    return temp ;
}

template<typename T>
SIWIR2::FEM::DomainData<T>::~DomainData<T>(){
    
    
}
template<typename T>
const T & SIWIR2::FEM::DomainData<T>::operator[](const real_l index) const{
    
    return data_[index] ;
    
}

template<typename T>
T & SIWIR2::FEM::DomainData<T>::operator[](const real_l index) {
    
    return data_[index] ;
}

template<typename T>
void SIWIR2::FEM::DomainData<T>::resize(const real_l number, const T value){
    
    data_.resize(number,value) ;
    
}

template<typename T>
void SIWIR2::FEM::DomainData<T>::resize(const T value){
    
    data_.resize(numData_,value) ;
}

template<typename T>
void SIWIR2::FEM::DomainData<T>::resize(){
    
    data_.resize(numData_,static_cast<T>(0.0)) ; 
}

template<typename T>
const real_l SIWIR2::FEM::DomainData<T>::size() const{
    
    return data_.size() ;
}

template<typename T>
void SIWIR2::FEM::DomainData<T>::pushback(const T value){
    
    data_.push_back(value) ;
}

template<typename T>
typename std::vector<T>::iterator SIWIR2::FEM::DomainData<T>::randomPosition(const real_l position){
    
    return  data_.begin() + position ;
    
}

template<typename T>
void SIWIR2::FEM::DomainData<T>::clear(){
    
    data_.clear() ;
}
template<typename T>
void SIWIR2::FEM::DomainData<T>::resize_vector(const real_l number){
    data_.resize(number) ; 
}



