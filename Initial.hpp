//
//  Initial.hpp
//  FEM
//
//  Created by Sagar Dolas on 24/05/16.
//  Copyright �� 2016 Sagar Dolas. All rights reserved.
//

#ifndef Initial_hpp
#define Initial_hpp

#include <stdio.h>
#include "Type.h"
#include <string>
#include <iostream>
#include "DomainData.hpp"
#include "DomainData.cpp"
#include <fstream>
#include "Enums.h"


namespace SIWIR2 {
    
    class Initial {
        
    private:
        
    public:
        
        std::string filename_ ;
        real_d delta ;
        real_d epsilon ;
        real_l refinement ;
        real_l numVertex ;
        real_l numFaces ;
        
        // Constuctor
        explicit Initial(int argc, const char * argv[]) ;
        ~Initial() ;
        
        // fill the data into the efficient data structure
        void filldata(SIWIR2::FEM::DomainData<real_d> &vertex,SIWIR2::FEM::DomainData<real_l> &faces ) ;
        
        void outOnScreen() ;

    };
}

#endif /* Initial_hpp */
