//
//  Enums.h
//  FEM
//
//  Created by Sagar Dolas on 24/05/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#ifndef Enums_h
#define Enums_h

namespace SIWIR2 {
    namespace FEM {
        
        enum dataType {
            
            vertex ,
            face ,
            normal ,
            matrix
            
        };
        
        enum geometricMap{
            
            normalindex = 1,
            vertexindex= 2 ,
            faceindex = 3,
            ebeindex = 9
        };
        
        enum matrixType {
            
            globalStiffnessMatrix ,
            massmatrix
        };
        
    }
}

#endif /* Enums_h */
