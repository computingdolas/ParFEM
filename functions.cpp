//
//  functions.cpp
//  FEM
//
//  Created by Sagar Dolas on 24/05/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#include "functions.hpp"

namespace global {

	real_d delta  ;
}


real_d ksqaure(const real_d x, const real_d y) {
    
    return ((100.0 + global::delta) * std::exp(-50.0 * ((x*x) + (y*y)) )) - 100.0 ;
}

const real_l index(const real_l row,const real_l col, const real_l numrow) {
    
    return (row * numrow + col) ;
}


void delta(const real_d value) {

	global::delta = value ;

}

