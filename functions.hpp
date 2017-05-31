//
//  functions.hpp
//  FEM
//
//  Created by Sagar Dolas on 24/05/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//


#ifndef functions_hpp
#define functions_hpp
#pragma once

#include <stdio.h>


#include <stdio.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <functional>
#include "Initial.hpp"
#include "Enums.h"
#include "Type.h"

#include <iomanip>
#include <iostream>
#include <fstream>

// Variable delta

real_d ksqaure(const real_d x, const real_d y) ;
void delta(const real_d value) ;

const real_l index(const real_l row,const real_l col, const real_l numrow) ;

//void delta(const SIWIR2::Initial & i ) ;

template <typename T>
void logerror(const T error) {
    std::cout<<error<<std::endl ;
}

template <typename T>
void logonScreen(const T value) {
    std::cout<<std::setprecision(10)<<value<<std::endl ;
}

template <typename Print,typename Linked>
void printdata(const Print &p,const Linked &l,std::ofstream &out ) {
    
    const real_l printsize = p.size() ;
    
    for (real_l i =0 ; i < printsize; ++i) {
        
        out<<std::setprecision(10)<<l[2*i]<<" "<<l[2*i +1]<<" "<<p[i]<<std::endl;
    }
    
}

template<typename T>
void printdata(const T value,std::ofstream &out){
    
    out<<std::setprecision(10)<<value<<std::endl ;
    
}

#endif /* functions_hpp */
