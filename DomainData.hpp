//
//  DomainData.hpp
//  FEM
//
//  Created by Sagar Dolas on 24/05/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#pragma once 

#ifndef DomainData_hpp
#define DomainData_hpp

#include <stdio.h>
#include <vector>
#include <functional>
#include <cmath>
#include <assert.h>
#include <fstream>


#include "Enums.h"
#include "Type.h"

namespace SIWIR2 {
    
    namespace FEM {
        
        template <typename T>
        class DomainData{
        
        private:
            
            real_l numData_ ;
            std::vector<T> data_ ;
            
        public:
            
            SIWIR2::FEM::dataType datatype_ ;

            explicit DomainData(const real_l _numData,const SIWIR2::FEM::dataType _datatype) ;
            explicit DomainData(const SIWIR2::FEM::dataType _datatype) ;
            DomainData(const DomainData<real_d> &vertex, const real_d delta) ;
            DomainData(const DomainData<T> &obj) ;
            
            ~DomainData() ;
            
            // Assignment operator
            DomainData<T> & operator = (const DomainData<T> &obj) ;
            
            // Addition operator
            const DomainData<T> operator + (const DomainData<T> &obj) const ;
            
            // Substration operator
            const DomainData<T> operator - (const DomainData<T> &obj) const ;
            
            // Multiplying Operator
            const real_d operator * (const DomainData<T> &obj) const ;
            const DomainData<T>  operator * (const real_d c) const  ;
            
            //Access Operator
            const T& operator[] (const real_l index) const ;
            T& operator[] (const real_l index) ;
            
            // Resizing operation
            void resize(const real_l number,const T value) ;
            void resize(const T value) ;
            void resize() ;
            void resize_vector(const real_l number) ; 
            
            //Size 
            const real_l size() const ;
            
            // PushBack
            void pushback(const T value) ;
            
            // Returning Iterator
            typename std::vector<T>::iterator randomPosition(const real_l position) ;
            
            // Clear the vector
            void clear() ; 
            
            
        };
        
    }
}

#endif /* DomainData_hpp */
