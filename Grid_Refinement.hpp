//
//  Grid_Refinement.hpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#ifndef Grid_Refinement_hpp
#define Grid_Refinement_hpp

//Standard Library
#include <stdio.h>
#include <vector>
#include <map>
#include <utility>

//User Defined Library
#include "Type.h"
#include "functions.hpp"
#include "Enums.h"


namespace SIWIR2 {
    namespace FEM {
        
        template<class VertexSet,class FaceSet>
        class Refinement {
            
        private:
            
            VertexSet * const vertex ;
            
            const real_l level ;
            
            std::map<std::pair<real_l,real_l>, real_l> edgeWithMidpoint ;
            
            
        public:
            
            explicit Refinement(VertexSet &vertex,const real_l level) ;
            ~Refinement() ;
            
            void refine(const FaceSet &oldface,FaceSet &newface) ;
            const FaceSet refinedGrid(const FaceSet coarseFace) ; // to be included in assignment in domain data
            
            void insertface(FaceSet &newface,const std::vector<real_l> vertices) ;
            bool isedge(const std::pair<real_l, real_l> edge) ;
            const real_l midpoint(const std::pair<real_l,real_l> edge) const ;
            
            const real_l checkAndInsertedge(const std::pair<real_l,real_l> edge)  ;
            
        };
        
    }
}

//#include "Grid_Refinement.impl.hpp"

#endif /* Grid_Refinement_hpp */
