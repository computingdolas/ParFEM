//
//  main.cpp
//  space filling curve
//
//  Created by Sagar Dolas on 08/01/17.
//  Copyright © 2017 Sagar Dolas. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <fstream>

struct vertices {
    
    int64_t index ;
    int64_t sfcindex ;
    bool sfc_index ;
    double x ;
    double y ;
};


struct element{
    
    int v1 ;
    int v2 ;
    int v3 ;
    int eleindex ;
    int64_t location_code ;
    double centroid_x ;
    double centroid_y ;
};

struct bounding_box {
    
    double centre_x ;
    double centre_y ;
    double box_max_x ;
    double box_max_y ;
    double box_min_x ;
    double box_min_y ;
    
};

int64_t Morton_curve(element ele, bounding_box box,int N_level) ;
void updatebox(bounding_box &box) ;
bool compareEle(const element &ele1, const element &ele2) ;


int main(int argc, const char * argv[]) {
    
    
    // Number of octant level
    int N_level = 5 ;
    
    // Number of grid points
    int N = 4 ;
    long int total_num_gridpoints = pow(2,N) * pow(2,N) ;
    
    // Mesh details
    double h = 1.0 / 16 ;
    
    // Allocate space for mesh
    std::vector<element> ele(total_num_gridpoints) ;

    // Initialise the mesh
    int linear_index ;
    for (size_t y =0 ; y < pow(2, N); ++y) {
        for (size_t x =0 ; x < pow(2, N); ++x) {
            linear_index = y * pow(2, N) + x ;
            ele[linear_index].eleindex = linear_index ;
            ele[linear_index].centroid_x = (x + 0.5) * h ;
            ele[linear_index].centroid_y = (y + 0.5) * h ;
        }
    }
    
    // Bounding box intialisation
    bounding_box box ;
    box.centre_x = 8 * h ;
    box.centre_y = 8 * h ;
    box.box_max_x = 16 * h ;
    box.box_min_x = 0 ;
    box.box_max_y = 16 * h ;
    box.box_min_y = 0 ;
    
    std::ofstream gnufile_ ;
    
    gnufile_.open("without_order.dat") ;
    
    for (auto it = ele.begin(); it!= ele.end(); ++it) {
        gnufile_ << it->centroid_x<<"  "<<it->centroid_y << std::endl ;
    }
    
    gnufile_.close() ;
    
    // Morton order space filling curve
    for (size_t i = 0 ; i < ele.size() ; ++i) {
        
        ele[i].location_code = Morton_curve(ele[i], box, N_level) ;
        //std::cout<<ele[i].location_code<<std::endl ;
        
    }
    
    // Sorting with respect to Location code
    std::sort(ele.begin(), ele.end(), compareEle) ; // O(nlogn) where n is distance from begin to end of element vector

    gnufile_.open("with_order.dat") ;
    
    for (auto it = ele.begin(); it!= ele.end(); ++it) {
        gnufile_ << it->centroid_x<<"  "<<it->centroid_y << std::endl ;
    }
    
    gnufile_.close() ;
        
    return 0;
}

bool compareEle(const element &ele1, const element &ele2) {
    
    if (ele1.location_code < ele2.location_code) {
        return true ;
    }
    else
        return false ;
    
}

void updatebox(bounding_box &box) {
    
    box.centre_x = ( box.box_max_x + box.box_min_x ) / 2 ;
    box.centre_y = (box.box_max_y + box.box_min_y ) / 2 ;
    
}

int64_t Morton_curve( element ele, bounding_box box, int N_level ){
    
    int64_t index = 0 ;
    
    for (int i =0 ; i < N_level ; ++i) {
        index = index << 3 ;

        // Set the octant using morton order
        if (ele.centroid_x > box.centre_x) {
            index = index + 1 ;
        }
        if (ele.centroid_y > box.centre_y) {
            index = index + 2 ;
        }
        // Update the bounding box
        if (ele.centroid_x > box.centre_x) {
            box.box_min_x = box.centre_x ;
        }
        else box.box_max_x = box.centre_x ;
            
        if (ele.centroid_y > box.centre_y) {
            box.box_min_y = box.centre_y ;
        }
        else box.box_max_y = box.centre_y ;
        updatebox(box) ;
        
    }
    return index ;
}
