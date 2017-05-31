//
//  Grid_Refinement.cpp
//  FEM
//
//  Created by Sagar Dolas on 04/06/16.
//  Copyright �� 2016 Sagar Dolas. All rights reserved.
//

#include "Grid_Refinement.hpp"

using namespace SIWIR2::FEM ;

template<class VertexSet,class FaceSet>
SIWIR2::FEM::Refinement<VertexSet,FaceSet>::Refinement(VertexSet & vertex,const real_l level) : vertex(&vertex),level(level) {
    
}

template<class VertexSet,class FaceSet>
SIWIR2::FEM::Refinement<VertexSet,FaceSet>::~Refinement<VertexSet, FaceSet>(){
    
}

template<class VertexSet,class FaceSet>
const FaceSet SIWIR2::FEM::Refinement<VertexSet, FaceSet>::refinedGrid(const FaceSet coarseFace){
    
    
    logonScreen("//////////////////////////") ;
    std::cout<<std::endl ;
    logonScreen("Refinement Started") ;
    
    DomainData<real_l> temp(coarseFace) ;
    
    for (real_l i =0 ; i < level; ++i) {
        
        DomainData<real_l> newface(dataType::face) ;
        refine(temp, newface) ;
        temp.resize(newface.size(), 0) ;
        temp = newface ;
        newface.clear() ;
        
        std::cout<<"Refinement "<<i+1<<" "<<"Completed"<<std::endl ;
    }
    
    logonScreen("Total Refinement Completed") ;
    std::cout<<std::endl ; 
    logonScreen("///////////////////////////") ;
    return temp  ;
}

template<class VertexSet,class FaceSet>
void SIWIR2::FEM::Refinement<VertexSet,FaceSet>::refine(const FaceSet &oldface,FaceSet &newface){
    
    
    //Temporary vector of vertices
    std::vector<real_l> vertices(6,0) ;
    
    //Existing number of faces
    const real_l oldnumface = (oldface).size() / SIWIR2::FEM::geometricMap::faceindex ;
    
    
    //Sweep across all the faces
    for (real_l f = 0; f < oldnumface; ++f) {
        
        // Find out the index of face in face array
        real_l findex  = f * SIWIR2::FEM::geometricMap::faceindex ;
        
        // Find out the vertex of the current face
        real_l v1 = (oldface)[findex] ;
        real_l v2 = (oldface)[findex+1] ;
        real_l v3 = (oldface)[findex+2] ;
        
        // The index of the new vertices to be found ;
        real_l v4 = 0 ;
        real_l v5 = 0 ;
        real_l v6 = 0 ;
        
        // form an edge using two vertex and check if it is already there in map
        std::pair<real_l, real_l> e1 = std::make_pair(v1, v2) ; 
        v4 = checkAndInsertedge(e1) ;
        
        std::pair<real_l, real_l> e2 = std::make_pair(v2, v3) ;
        v5 = checkAndInsertedge(e2) ;
        
        std::pair<real_l, real_l> e3 = std::make_pair(v3, v1) ;
        v6 = checkAndInsertedge(e3) ;
        
        // Now insert six vertices into new face
        std::vector<real_l> v{v1,v2,v3,v4,v5,v6} ;
        insertface(newface,v) ;
        
    }
    
}

template<class VertexSet,class FaceSet>
void SIWIR2::FEM::Refinement<VertexSet,FaceSet>::insertface(FaceSet &newface, const std::vector<real_l> vertices) {
    
    newface.pushback(vertices[0]) ;
    newface.pushback(vertices[3]) ;
    newface.pushback(vertices[5]) ;
    newface.pushback(vertices[3]) ;
    newface.pushback(vertices[1]) ;
    newface.pushback(vertices[4]) ;
    newface.pushback(vertices[5]) ;
    newface.pushback(vertices[4]) ;
    newface.pushback(vertices[2]) ;
    newface.pushback(vertices[5]) ;
    newface.pushback(vertices[3]) ;
    newface.pushback(vertices[4]) ;
}

template<class VertexSet,class FaceSet>
const real_l SIWIR2::FEM::Refinement<VertexSet,FaceSet>::checkAndInsertedge(const std::pair<real_l, real_l> edge) {
    
    
    //local cordinates
    std::vector<real_d> cordinates(4,0.0) ;
    
    // midpoints - new vertices
    std::vector<real_d> newcord(2,0.0) ;
    
    if (isedge(edge)) {
        //logonScreen("here") ;
        return midpoint(edge) ;
    }
    else {
        
        // Find out the midpoint of the edge , insert it into vertex set
        real_l vright = edge.first * geometricMap::vertexindex ;
        real_l vleft = edge.second  * geometricMap::vertexindex ;
        
        cordinates[0] = (*vertex)[vright] ;
        cordinates[1] = (*vertex)[vright+1] ;
        cordinates[2] = (*vertex)[vleft] ;
        cordinates[3] = (*vertex)[vleft+1] ;
        
        // Find out the midpoint of this edge
        newcord[0] = ( cordinates[0] + cordinates[2] ) / 2.0 ;
        newcord[1] = ( cordinates[1] + cordinates[3] ) / 2.0 ;
        
        // Insert it into vertex
        (*vertex).pushback(newcord[0]) ;
        (*vertex).pushback(newcord[1]) ;
        
        // Find out the position of new vertex
        real_l nvindex = ((*vertex).size() / 2) -1 ;
        
        // make pair of this edge and index of midpoint
        std::map<std::pair<real_l, real_l>, real_l> ewmidpoint ;
        ewmidpoint.insert(std::make_pair(edge, nvindex)) ;
        
        // push it into vector<std::maps>
        edgeWithMidpoint[edge] = nvindex ;
        return nvindex ;
    }

}

template<class VertexSet,class FaceSet>
bool SIWIR2::FEM::Refinement< VertexSet, FaceSet>::isedge(const std::pair<real_l, real_l> edge) {
    
    auto iter = edgeWithMidpoint.find(edge) ;
    
    if (iter != edgeWithMidpoint.end()){
        
        return true ;
    }
    else {
        std::pair<real_l, real_l> reverse = std::make_pair(edge.second, edge.first) ;
        auto iter2 = edgeWithMidpoint.find(reverse) ;
        
        if(iter2!= edgeWithMidpoint.end())
        {
            return true ;
        }
        else {
            return false ;
        }
    }
}

template<class VertexSet,class FaceSet>
const real_l SIWIR2::FEM::Refinement< VertexSet, FaceSet>::midpoint(const std::pair<real_l, real_l> edge) const {
    
    auto iter = edgeWithMidpoint.find(edge) ;
    if (iter != edgeWithMidpoint.end()){
        
        return iter->second ;
    }
    else {
        std::pair<real_l, real_l> reverse = std::make_pair(edge.second, edge.first) ;
        auto iter2 = edgeWithMidpoint.find(reverse) ;
        
        return iter2->second ;
        
    }
    
}

