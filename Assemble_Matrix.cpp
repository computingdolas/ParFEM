//
//  Assemble_Matrix.cpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#include "Assemble_Matrix.hpp"

using namespace ::_COLSAMM_ ;
using namespace SIWIR2::FEM ;

SIWIR2::FEM::Assemble::Assemble(SIWIR2::FEM::MatrixMap<real_d> &A,
                                SIWIR2::FEM::MatrixMap<real_d> &M,
                                const SIWIR2::FEM::DomainData<real_d> &vertex,
                                const SIWIR2::FEM::DomainData<real_l> &face , 
                                const real_l _numvertex,
                                const real_l _numface,const real_d d ) : A_(&A),
                                                                        M_(&M),
                                                                        vertex_(&vertex),
                                                                        face_(&face),
                                                                        numvertex(_numvertex),
                                                                        numface(_numface),d(d) {
                
    std::vector<real_d> corners ;
    std::vector<std::vector<real_d>> mylocalstiffmat_1 ;
    std::vector<std::vector<real_d>> mylocalstiffmat_2 ;
    std::vector<real_l> pos ;
    ELEMENTS::Triangle my_element ;
    //int td ; 
    
    // We need to parallise the assemble operation to see whether how it responds for the on ccnUMA systems 
    //omp_set_num_threads(2);

    real_l iter, findex, v1, v2, v3 , vindex1, vindex2, vindex3, i,j,row,col ;  

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    #pragma omp parallel for schedule(static) private(iter,findex,v1,v2,v3,vindex1,vindex2,vindex3,corners,mylocalstiffmat_1,mylocalstiffmat_2,my_element,pos,i,j,row,col) shared(A,M,vertex,face) num_threads(8)
    for (iter = 0 ; iter < numface; ++iter) {

        //td  = omp_get_thread_num() ; 
        //if(td == 0 )   
        //    std::cout << "I am thread Id := "<< td << "I am doing parallel assembly matrix operation" <<std::endl ; 

        //Calculate the face index
        findex = iter * geometricMap::faceindex ;
    
        //Access the vertices
        v1 = (*face_)[findex] ;
        v2 = (*face_)[findex+1] ;
        v3 = (*face_)[findex+2] ;
        
        // Access the index of vertex in vertex dataset
        vindex1 = v1 * geometricMap::vertexindex ;
        vindex2 = v2 * geometricMap::vertexindex ;
        vindex3 = v3 * geometricMap::vertexindex ;

        // Resizing the corners 
        corners.resize(6, 0.0) ;

        //Access the corner points
        corners[0] = (*vertex_)[vindex1] ;
        corners[1] = (*vertex_)[vindex1 + 1] ;
        corners[2] = (*vertex_)[vindex2] ;
        corners[3] = (*vertex_)[vindex2 + 1] ;
        corners[4] = (*vertex_)[vindex3] ;
        corners[5] = (*vertex_)[vindex3 + 1] ;

        //form local elemet
        my_element(corners) ;
        
        //Initialise delta
        delta(d) ;

        // Initialize the locals stiffness matrix 
        mylocalstiffmat_1.resize(3, {0.0,0.0,0.0}) ;
        mylocalstiffmat_2.resize(3, {0.0,0.0,0.0}) ;

        //Build local stiffness and local mass matrices
        mylocalstiffmat_1 = my_element.integrate(grad(v_()) * grad(w_()) ) ;
        mylocalstiffmat_2 = my_element.integrate(func<real_d>(ksqaure) * v_() * w_()) ;

        // Update the local stiffness matrices A
        for (row = 0; row < 3; ++row) {
            for (col = 0 ; col < 3 ; ++col){
                mylocalstiffmat_1[row][col] -= mylocalstiffmat_2[row][col] ;
            }
        }
        

        //Find out the actual position in global_stiffness_matrix
        pos.resize(3) ; 
        pos[0] = v1 ;
        pos[1] = v2 ;
        pos[2] = v3 ;

        //Update their position in global stiffness Matrix
        for (i = 0; i < 3; ++i) {
            for ( j = 0 ;j < 3; ++j ){
                #pragma omp atomic 
                (*A_)(pos[i],pos[j]) += mylocalstiffmat_1[i][j] ;
            }
        }

        //std::cout << "Problem here " << std::endl ; 
        
        // update directly in Compressed row storage or map
        mylocalstiffmat_1.resize(3, {0.0,0.0,0.0}) ;
        // Doing it for the mass matrix
        
        //Build local mass matrix
        mylocalstiffmat_1 = my_element.integrate(v_() * w_()) ;
        
        //Update their position in global mass Matrix
        for ( i = 0; i < 3; ++i) {
            for ( j = 0 ;j < 3; ++j ){
                #pragma omp atomic 
                (*M_)(pos[i],pos[j]) += mylocalstiffmat_1[i][j] ;
            }
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Assembly took =; " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() <<" microseconds"<< std::endl ; 

    
}




SIWIR2::FEM::Assemble::~Assemble(){
    
    
}
