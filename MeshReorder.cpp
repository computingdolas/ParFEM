//
//  main.cpp
//  Mesh_Reordering
//
//  Created by Sagar Dolas on 18/01/17.
//  Copyright © 2017 Sagar Dolas. All rights reserved.
//


#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>


// Global Constant 
#define zero 0 

// This Program takes .GOM file as a input , orders mesh elements according to Z-order space filling curve and
// generates new file .GOM file with elements and vertices renumbered. 
 
// Data Structure of vertices and Elements 

struct vertices {
    
	uint64_t index; 
	uint64_t sfc_index; 
	double x; 
	double y; 
	double z;
    bool flag ;
};


struct element {
    
	std::vector<int64_t> nodes;
	double centroidx ;
    double centroidy ;
    double centroidz ;
    double damping ;
	long int index;
    int numSolidMatPoints ;
    int numLiquidMatPoints ;
	uint64_t sfc_index; 
	uint64_t materialId; 
};

// Nodal wise data
struct fixity{
    
    uint64_t eleindex ;  // This is nodal index , reffered here as ele index just for the sake of convenience . 
    int xfixity ;
    int yfixity ;
    int zfixity ;
};

// Force Data Structure
struct force {
    double forceX ;
    double forceY ;
    double forceZ ;
};

// ELMMAT
struct elmmaterial  {
    
    uint64_t eleindex ;
    uint64_t materialId  ;
    
};

// Data Structure of Bounding Box
struct BoundingBox {
    
    double centre_x  ;
    double centre_y ;
    double centre_z ;
    double box_max_x ;
    double box_max_y ;
    double box_max_z ;
    double box_min_x ;
    double box_min_y ;
    double box_min_z ;
};

// Compare Functions Delcarations and Definitions

bool compareVerticesX(const vertices &v1,const vertices &v2) {
    
    if (v1.x < v2.x) {
        return true ;
    }
    else
        return false ;
}

bool compareVerticesY(const vertices&v1, const vertices&v2){
    
    if (v1.x < v2.x) {
        return true ;
    }
    else
        return false ;
    
}

bool compareVerticesZ(const vertices&v1, const vertices&v2){
    
    if (v1.x < v2.x) {
        return true ;
    }
    else
        return false ;
    
}

bool compareFixity(const fixity&f1, const fixity&f2) {

	if (f1.eleindex < f2.eleindex){
		return true; 
	}
	else{
		return false; 
	}
}

// Function Declarations
uint64_t Morton_Curve(element E,BoundingBox box,uint64_t N_level) ;
bool compareElement(const element &e1, const element &e2) ;
bool compareVertices(const vertices &v1, const vertices &v2) ;

// Algorithm for Reading the Data 

int main(int argc, char **argv)
{		
    
    // File pointers
    std::string _filepointers;
    std::string _filename = argv[1];
    
    // Bounding Box Variables
    BoundingBox box ;
    std::vector<double> maxCords(3,0.0) ;
    double _maxBoxDim ;
    
    // Global Variable to keep hold of Vertices Number
    int64_t _globalVertiNumber = 0 ;
    
	// Number of Vertices and Elements
	uint64_t _numVertices; 
	uint64_t _numElements;
    
    // Fixities
    uint64_t _numFixitySurfaceSolid ;
    uint64_t _numFixityLineSolid ;
    uint64_t _numFixityPointSolid ;
    uint64_t _numFixitySurfaceLiquid ;
    uint64_t _numFixityLineLiquid ;
    uint64_t _numFixityPointLiquid ;
    uint64_t _numFixitySurfaceGas ;
    uint64_t _numFixityLineGas ;
    uint64_t _numFixityPointGas ;
    uint64_t _numRemoveFixitySurfaceSolid ;
    uint64_t _numRemoveFixityLineSolid ;
    uint64_t _numRemoveFixityPointSolid ;
    uint64_t _numRemoveFixitySurfaceLiquid ;
    uint64_t _numRemoveFixityLineliquid ;
    uint64_t _numRemoveFixityPointLiquid ;
    uint64_t _numRemoveFixitySurfaceGas ;
    uint64_t _numRemoveFixityLineGas ;
    uint64_t _numRemoveFixityPointGas ;
    
    // Load Steps
    int _numLoadSolid ;
    int _numLoadLiquid ;
    int _numLoadGas ;
    
    // Material Properties
    int _numMaterials ;
    int _materialNum ;
    std::string _materialName ;
    std::string _materialType ;
    double _porositySolid ;
    double _densitySolid ;
    double _k0ValueSolid ;
    double _intrinsicPermeabilityLiquid ;
    double _densityLiquid ;
    double _bulkModulusLiquid ;
    double _dynamicViscosityLiquid ;
    std::string _materialModelSolid ;
    double _youngsModulus ;
    double _poissonRatio ;
    
    
    // Vector of Elements and Vertices
    std::vector<element> mesh ;
    std::vector<vertices> verti_array ;
    
    // Vector of Fixities
    std::vector<fixity> nodalFixitySurfaceSolid ;
    std::vector<fixity> nodalFixityLineSolid ;
    std::vector<fixity> nodalFixityPointSolid ;
    std::vector<fixity> nodalFixitySurfaceLiquid ;
    std::vector<fixity> nodalFixityLineLiquid ;
    std::vector<fixity> nodalFixityPointLiquid ;
    std::vector<fixity> nodalFixitySurfaceGas ;
    std::vector<fixity> nodalFixityLineGas ;
    std::vector<fixity> nodalFixityPointGas ;
    std::vector<fixity> nodalRemoveFixitySurfaceSolid ;
    std::vector<fixity> nodalRemoveFixityLineSolid ;
    std::vector<fixity> nodalRemoveFixityPointSolid ;
    std::vector<fixity> nodalRemoveFixitySurfaceLiquid ;
    std::vector<fixity> nodalRemoveFixityLineLiquid ;
    std::vector<fixity> nodalRemoveFixityPointLiquid ;
    std::vector<fixity> nodalRemoveFixitySurfaceGas ;
    std::vector<fixity> nodalRemoveFixityLineGas ;
    std::vector<fixity> nodalRemoveFixityPointGas ;
    
    // Vector of Material iD
    std::vector<elmmaterial> elementMat ;
    
    // Load Data Structure
    std::vector<std::map<int, force>> loadSolid ;
	std::vector<uint64_t> loadVertices;
	std::vector<double> loads; 

    
	//Opening the file 
	std::fstream gomfile; 
	gomfile.open(_filename, std::ios::out | std::ios::in); 
	std::getline(gomfile, _filepointers); 

    gomfile >> _numElements;
	gomfile >> _numVertices; 

    // Resizing the vector
    mesh.resize(_numElements) ;
    verti_array.resize(_numVertices) ;
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    // Reading in the vertices data
    double x, y, z ;
    for (size_t i = 0 ; i < _numVertices; ++i)
    {
        
        gomfile>>x ;
        gomfile>>y ;
        gomfile>>z ;
        verti_array[i].index = i + 1 ;
        verti_array[i].sfc_index = 0 ;
        verti_array[i].x = x ;
        verti_array[i].y = y ;
        verti_array[i].z = z ;
        verti_array[i].flag = false ;
    
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
	// Reading the element data
    for (size_t i =0 ; i < _numElements; ++i)
    {
        mesh[i].index = i+1 ;   // i + 1  since element starts from 1
        mesh[i].nodes.resize(10) ;
        for (size_t j =0 ; j < 10; ++j) {
            gomfile>>mesh[i].nodes[j] ;
        }
        
        // Initialising the centroid
        mesh[i].centroidx = 0.0 ;
        mesh[i].centroidy = 0.0 ;
        mesh[i].centroidz = 0.0 ;
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    // Reading the nodal fixity surface solid
    gomfile >> _numFixitySurfaceSolid ;
    nodalFixitySurfaceSolid.resize(_numFixitySurfaceSolid) ;
    
    for (size_t i =0 ; i < _numFixitySurfaceSolid; ++i){
        
        gomfile >> nodalFixitySurfaceSolid[i].eleindex ;
        gomfile >> nodalFixitySurfaceSolid[i].xfixity ;
        gomfile >> nodalFixitySurfaceSolid[i].yfixity ;
        gomfile >> nodalFixitySurfaceSolid[i].zfixity ;
        
    }
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    // Reading the nodal fixity line solid
    gomfile >> _numFixityLineSolid ;
    
    if (_numFixityLineSolid){
        nodalFixityLineSolid.resize(_numFixityLineSolid) ;
            for (size_t i = 0  ; i < _numFixityLineSolid; ++i)
            {
                
                gomfile >> nodalFixityLineSolid[i].eleindex ;
                gomfile >> nodalFixityLineSolid[i].xfixity ;
                gomfile >> nodalFixityLineSolid[i].yfixity ;
                gomfile >> nodalFixityLineSolid[i].zfixity ;
                
            }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    // Reading Fixity Point Solid
    gomfile >> _numFixityPointSolid ;
    
    if (_numFixityPointSolid) {
        nodalFixityPointSolid.resize(_numFixityPointSolid) ;
        
        for (size_t i =0 ; i < _numFixityPointSolid; ++i)
        {
            
            gomfile >> nodalFixityPointSolid[i].eleindex ;
            gomfile >> nodalFixityPointSolid[i].xfixity ;
            gomfile >> nodalFixityPointSolid[i].yfixity ;
            gomfile >> nodalFixityPointSolid[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);

    // Reading Fixity Surface Licquid
    std::getline(gomfile,_filepointers);
    gomfile >> _numFixitySurfaceLiquid ;
    
    if (_numFixitySurfaceLiquid) {
        
        nodalFixitySurfaceLiquid.resize(_numFixitySurfaceLiquid) ;
        for (size_t i = 0 ; i < _numFixitySurfaceLiquid; ++i)
        {
            
            gomfile >> nodalFixitySurfaceLiquid[i].eleindex ;
            gomfile >> nodalFixitySurfaceLiquid[i].xfixity ;
            gomfile >> nodalFixitySurfaceLiquid[i].yfixity ;
            gomfile >> nodalFixitySurfaceLiquid[i].zfixity ;
            
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    // Reading Fixity Line liquid
    std::getline(gomfile,_filepointers);
    gomfile >> _numFixityLineLiquid ;
    
    if (_numFixityLineLiquid) {
        nodalFixityLineLiquid.resize(_numFixityLineLiquid) ;
        for (size_t i = 0 ; i < _numFixitySurfaceLiquid; ++i)
        {
            gomfile >> nodalFixityLineLiquid[i].eleindex ;
            gomfile >> nodalFixityLineLiquid[i].xfixity ;
            gomfile >> nodalFixityLineLiquid[i].yfixity ;
            gomfile >> nodalFixityLineLiquid[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    // Reading Fixity Point Liquid
    std::getline(gomfile,_filepointers);
    gomfile >> _numFixityPointLiquid ;
    
    if (_numFixityPointLiquid) {
        
        nodalFixityPointLiquid.resize(_numFixityPointLiquid) ;
        for (size_t i =0 ; i < _numFixityPointLiquid; ++i) {
            
            gomfile >> nodalFixityPointLiquid[i].eleindex ;
            gomfile >> nodalFixityPointLiquid[i].xfixity ;
            gomfile >> nodalFixityPointLiquid[i].yfixity ;
            gomfile >> nodalFixityPointLiquid[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading the fixity Surface Gas
    std::getline(gomfile,_filepointers);
    gomfile >> _numFixitySurfaceGas ;
    
    if (_numFixitySurfaceGas) {
        
        nodalFixitySurfaceGas.resize(_numFixitySurfaceGas) ;
        for (size_t i =0 ; i < _numFixitySurfaceGas; ++i) {
            
            gomfile >> nodalFixityPointLiquid[i].eleindex ;
            gomfile >> nodalFixityPointLiquid[i].xfixity ;
            gomfile >> nodalFixityPointLiquid[i].yfixity ;
            gomfile >> nodalFixityPointLiquid[i].zfixity ;
            
        }
    }

    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;

    // Reading the fixity Line Gas
    std::getline(gomfile,_filepointers);
    gomfile >> _numFixityLineGas ;
    
    if (_numFixityLineGas) {
        
        nodalFixityLineGas.resize(_numFixityLineGas) ;
        if (_numFixityLineGas) {
            
            for (size_t i =0 ; i < _numFixityLineGas; ++i) {
                gomfile >> nodalFixityLineGas[i].eleindex ;
                gomfile >> nodalFixityLineGas[i].xfixity ;
                gomfile >> nodalFixityLineGas[i].yfixity ;
                gomfile >> nodalFixityLineGas[i].zfixity ;
            }
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;
    
    //Reading the Fixity Point Gas
    std::getline(gomfile,_filepointers);
    gomfile >> _numFixityPointGas ;
    
    if (_numFixityPointGas) {
        
        nodalFixityPointGas.resize(_numFixityPointGas) ;
        for (size_t i =0 ; i < _numFixityLineGas; ++i) {
            
            gomfile >> nodalFixityPointGas[i].eleindex ;
            gomfile >> nodalFixityPointGas[i].xfixity ;
            gomfile >> nodalFixityPointGas[i].yfixity ;
            gomfile >> nodalFixityPointGas[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Remove Fixity Surface Solid
    std::getline(gomfile,_filepointers);
    gomfile >> _numRemoveFixitySurfaceSolid ;
    
    if (_numRemoveFixitySurfaceSolid) {
        
        nodalRemoveFixitySurfaceSolid.reserve(_numRemoveFixitySurfaceSolid) ;
        for (size_t i =0 ; i < _numRemoveFixitySurfaceSolid; ++i) {
            
            gomfile >> nodalRemoveFixitySurfaceSolid[i].eleindex ;
            gomfile >> nodalRemoveFixitySurfaceSolid[i].xfixity ;
            gomfile >> nodalRemoveFixitySurfaceSolid[i].yfixity ;
            gomfile >> nodalRemoveFixitySurfaceSolid[i].zfixity ;
            
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Remove Fixity Line Solid
    std::getline(gomfile,_filepointers);
    gomfile >> _numRemoveFixityLineSolid ;
    
    if (_numRemoveFixityLineSolid) {
        
        nodalRemoveFixityLineSolid.resize(_numRemoveFixityLineSolid) ;
        for (size_t i = 0 ; i < _numRemoveFixityLineSolid; ++i) {
            
            gomfile >> nodalRemoveFixityLineSolid[i].eleindex ;
            gomfile >> nodalRemoveFixityLineSolid[i].xfixity ;
            gomfile >> nodalRemoveFixityLineSolid[i].yfixity ;
            gomfile >> nodalRemoveFixityLineSolid[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Remove Fixity Point Solid
    std::getline(gomfile,_filepointers);
    gomfile >> _numRemoveFixityPointSolid ;
    
    if (_numRemoveFixityPointSolid) {
        
        nodalRemoveFixityPointSolid.resize(_numRemoveFixityPointSolid) ;
        for (size_t i =0 ; i < _numRemoveFixityPointSolid; ++i) {
            
            gomfile >> nodalRemoveFixityPointSolid[i].eleindex ;
            gomfile >> nodalRemoveFixityPointSolid[i].xfixity ;
            gomfile >> nodalRemoveFixityPointSolid[i].yfixity ;
            gomfile >> nodalRemoveFixityPointSolid[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Remove Fixity Surface Liquid
    std::getline(gomfile,_filepointers);
    gomfile >> _numRemoveFixitySurfaceLiquid ;
    
    if (_numRemoveFixitySurfaceLiquid) {
        
        nodalRemoveFixitySurfaceLiquid.resize(_numRemoveFixitySurfaceLiquid) ;
        for (size_t i =0 ; i < _numRemoveFixitySurfaceLiquid; ++i) {
            
            gomfile >> nodalRemoveFixitySurfaceLiquid[i].eleindex ;
            gomfile >> nodalRemoveFixitySurfaceLiquid[i].xfixity ;
            gomfile >> nodalRemoveFixitySurfaceLiquid[i].yfixity ;
            gomfile >> nodalRemoveFixitySurfaceLiquid[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading the Remove Fixity Line Liquid
    std::getline(gomfile,_filepointers);
    gomfile >> _numRemoveFixityLineliquid ;
    
    if (_numRemoveFixityLineliquid) {
        
        nodalRemoveFixityLineLiquid.resize(_numRemoveFixityLineliquid) ;
        for (size_t i =0 ; i < _numRemoveFixityLineliquid; ++i) {
            
            gomfile >> nodalRemoveFixityLineLiquid[i].eleindex ;
            gomfile >> nodalRemoveFixityLineLiquid[i].xfixity ;
            gomfile >> nodalRemoveFixityLineLiquid[i].yfixity ;
            gomfile >> nodalRemoveFixityLineLiquid[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading the Remove Fixity Point Liquid
    std::getline(gomfile,_filepointers);

    gomfile >> _numRemoveFixityPointLiquid ;
    
    if (_numRemoveFixityPointLiquid) {
        
        nodalRemoveFixityPointLiquid.resize(_numRemoveFixityPointLiquid) ;
        for (size_t i =0 ; i < _numRemoveFixityPointLiquid; ++i) {
            
            gomfile >> nodalRemoveFixityPointLiquid[i].eleindex ;
            gomfile >> nodalRemoveFixityPointLiquid[i].xfixity ;
            gomfile >> nodalRemoveFixityPointLiquid[i].yfixity ;
            gomfile >> nodalRemoveFixityPointLiquid[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading the Remove Fixity Surface Gas
    std::getline(gomfile,_filepointers);
    gomfile >> _numRemoveFixitySurfaceGas ;
    
    if (_numRemoveFixitySurfaceGas) {
        
        nodalRemoveFixitySurfaceGas.resize(_numRemoveFixitySurfaceGas) ;
        for (size_t i = 0 ; i < _numRemoveFixitySurfaceGas; ++i) {
            
            gomfile >> nodalRemoveFixitySurfaceGas[i].eleindex ;
            gomfile >> nodalRemoveFixitySurfaceGas[i].xfixity ;
            gomfile >> nodalRemoveFixitySurfaceGas[i].yfixity ;
            gomfile >> nodalRemoveFixitySurfaceGas[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Remove Fixity Line gas
    std::getline(gomfile,_filepointers);
    gomfile >> _numRemoveFixityLineGas ;
    
    if (_numRemoveFixityLineGas) {
        
        nodalRemoveFixityLineGas.resize(_numRemoveFixityLineGas) ;
        for (size_t i =0 ; i < _numRemoveFixityLineGas; ++i) {
            
            gomfile >> nodalRemoveFixityLineGas[i].eleindex ;
            gomfile >> nodalRemoveFixityLineGas[i].xfixity ;
            gomfile >> nodalRemoveFixityLineGas[i].yfixity ;
            gomfile >> nodalRemoveFixityLineGas[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Remove Fixity Point Gas
    std::getline(gomfile,_filepointers);
    gomfile >> _numRemoveFixityPointGas ;
    
    if (_numRemoveFixityPointGas) {
        
        nodalRemoveFixityPointGas.resize(_numRemoveFixityPointGas) ;
        for (size_t i =0 ; i < _numRemoveFixityPointGas; ++i) {
            
            gomfile >> nodalRemoveFixityPointGas[i].eleindex ;
            gomfile >> nodalRemoveFixityPointGas[i].xfixity ;
            gomfile >> nodalRemoveFixityPointGas[i].yfixity ;
            gomfile >> nodalRemoveFixityPointGas[i].zfixity ;
        }
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading the load steps Solids
    
    std::getline(gomfile,_filepointers);
    gomfile >> _numLoadSolid ;
	
	uint64_t node; 
	double load; 
	if (_numLoadSolid) {
		for (int i = 0; i < _numLoadSolid; ++i) {
			for (int j = 0; j < 6; ++j) {
				gomfile >> node; 
				loadVertices.push_back(node); 
			}

			for (int k = 0; k < 18; ++k) {
				gomfile >> load; 
				loads.push_back(load); 
			}
		}
	}


    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Load Liquid
    
    std::getline(gomfile,_filepointers);
    gomfile >> _numLoadLiquid ;
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;

    // Reading Load Gas
    
    std::getline(gomfile,_filepointers);
    gomfile >> _numLoadGas ;
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Absorbing Boundary Surface Solid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Absorbing Boundary line Solid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Absorbing Boundary Point Solid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Boundary Surface Liquid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Boundary Line Liquid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Boundary Point Liquid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Absorbing Boundary Surface Gas
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Absorbing Boundary Line Gas
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading Absorbing Boundary Point Gas
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    //std::cout<<_filepointers<<std::endl ;
    
    // Reading the Number of Materials
    std::getline(gomfile,_filepointers);
    gomfile >> _numMaterials ;
    
    // Reading the material index
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _materialNum ;
    
    // Reading Material Name
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _materialName ;
    
    // Reading Material Type
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _materialType ;
    
    // Reading Porosity Solid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _porositySolid ;
    
    // Reading Density Solid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _densitySolid ;
    
    // Reading K0 value Solid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _k0ValueSolid ;
    
    // Reading Intrinsic Permeability Liquid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _intrinsicPermeabilityLiquid;
    
    // Reading Density Liquid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _densityLiquid ;
    
    // Reading Bulk Modulus Liquid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _bulkModulusLiquid ;
    
    // Reading Dynamic Viscosity Liquid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _dynamicViscosityLiquid ;
    
    // Reading Material Model Solid
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _materialModelSolid ;
    
    // Reading Youngs Modulus
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _youngsModulus ;
    
    // Reading Poisson's Ratio
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    gomfile >> _poissonRatio ;
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);

    // Reading Elemat
    std::getline(gomfile,_filepointers);
	for (size_t i = 0; i < _numElements; ++i) {
		gomfile >> mesh[i].materialId;
	}
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    // Reading Damping
    std::getline(gomfile,_filepointers);
    for (size_t i =0 ; i < _numElements; ++i) {
        gomfile >> mesh[i].damping ;
    }
    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
    // Reading Number of Material Points
    std::getline(gomfile,_filepointers);
    for (size_t i =0 ; i < _numElements; ++i) {
        gomfile >> mesh[i].numSolidMatPoints >> mesh[i].numLiquidMatPoints ;
    }

    
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    std::getline(gomfile,_filepointers);
    
	
	std::cout<<"The File Reading has been Finished" << std::endl ;
    
    // Closing the file
    gomfile.close();

    /*********************************************************/
    /*********************************************************/
    // Reading the file done
    /*********************************************************/
    /*********************************************************/
    
	// We have read all the data, we will now run the mesh reordering algorithm 
	// First step would be figure out bounding box, which can be figures out finding out
	// maximum point of all cordinate system and then find out the max of all three point
	// to find out the cubic domain associated with it
    // We need to figure out the centroid of the elements  .. for centroid of the elements
    // we need to take four first nodes of the elements and sort it according to that
    
	std::cout << "Reordering Mesh Elements using Morton order space filling curve  " << std::endl;


    // Finding out centroid of each element
    for (auto elem = mesh.begin(); elem < mesh.end(); ++elem ) {
        
        elem->centroidx = (verti_array[elem->nodes.at(0)-1].x +
                           verti_array[elem->nodes.at(1)-1].x +
                           verti_array[elem->nodes.at(2)-1].x +
                           verti_array[elem->nodes.at(3)-1].x ) * 0.25  ;
        
        elem->centroidy = (verti_array[elem->nodes.at(0)-1].y +
                           verti_array[elem->nodes.at(1)-1].y +
                           verti_array[elem->nodes.at(2)-1].y +
                           verti_array[elem->nodes.at(3)-1].y ) * 0.25  ;
        
        elem->centroidz = (verti_array[elem->nodes.at(0)-1].z +
                           verti_array[elem->nodes.at(1)-1].z +
                           verti_array[elem->nodes.at(2)-1].z +
                           verti_array[elem->nodes.at(3)-1].z ) * 0.25  ;
        
    }
    
    // Visualization of the elements in the mesh.
    std::ofstream gnuwrite ;
    
/*    gnuwrite.open("Vertices_without_order.dat") ;
    
    for (auto it = mesh.begin(); it != mesh.end(); ++it) {
        gnuwrite << it->centroidx<< "  " << it->centroidy << "  " << it->centroidz << std::endl ;
    }
    
    gnuwrite.close() ;*/ 
    
    
    // Finding out the maximum of cordinate system

    // Finding out the maximum in Each dimension
    auto max_vertex = std::max_element(verti_array.begin(), verti_array.end(), compareVerticesX) ;
    maxCords[0] = max_vertex->x ;
    max_vertex = std::max_element(verti_array.begin(), verti_array.end(), compareVerticesY) ;
    maxCords[1] = max_vertex->y ;
    max_vertex = std::max_element(verti_array.begin(), verti_array.end(), compareVerticesX) ;
    maxCords[2] = max_vertex->z ;
    
    // Finding out the global maximum, so that we can form bounding box
    auto maxiter = std::max_element(maxCords.begin(), maxCords.end()) ;
    _maxBoxDim = *maxiter ;
    
    // Rewriting it again
    //std::cout<<"The max box dimension is :="<<_maxBoxDim<<std::endl ;
    
    // Initialising the Bounding Box
    box.box_max_x = 1.2 ;   // Hard coded for this mesh file.
    box.box_max_y = 1.2 ;
    box.box_max_z = 1.2 ;
    box.box_min_x = 0.0 ;
    box.box_min_y = 0.0 ;
    box.box_min_z = 0.0 ;
    box.centre_x = ( box.box_max_x + box.box_min_x ) * 0.5 ;
    box.centre_y = ( box.box_max_y + box.box_min_y ) * 0.5 ;
    box.centre_z = ( box.box_max_z + box.box_min_z ) * 0.5 ;
    
    // We need to choose the level of N (number of octants)
    //std::cout<<"The centre of the box is :="<<box.centre_x<<std::endl ;
    
    uint64_t N_level = 6 ;
    // Traversing the morton order space filling curve
    for (size_t i = 0 ; i < mesh.size(); ++i) {
        mesh[i].sfc_index = Morton_Curve(mesh[i], box, N_level) ;
    }
    
    // Sorting of Elements
    std::sort(mesh.begin(), mesh.end(), compareElement) ; // Quicksort of O(NlogN)
    
    
    //gnuwrite.open("Vertices_with_order.dat") ;
    //for (auto it = mesh.begin(); it != mesh.end(); ++it) {
    //    gnuwrite << it->centroidx<< "  " << it->centroidy << "  " << it->centroidz << std::endl ;
    //}
    //
    //gnuwrite.close() ;
    
    // Renumbering the vertices according to space filling curve
    for (size_t e = 0 ; e < mesh.size() ; ++e){
        for (size_t i = 0 ; i < mesh[e].nodes.size() ; ++i) {
            if (!verti_array[mesh[e].nodes[i]-1].flag) {
                ++_globalVertiNumber ;
                verti_array[mesh[e].nodes[i]-1].flag = true ;
                verti_array[mesh[e].nodes[i]-1].sfc_index = _globalVertiNumber;
            }
        }
    }
    
    // Renumbering the nodal array in the elements array
    for (size_t i = 0 ;i < mesh.size() ; ++i) {
        for (size_t n =0 ; n < mesh[i].nodes.size(); ++n) {
            int64_t tempindex = verti_array[mesh[i].nodes[n]-1].sfc_index ;
            mesh[i].nodes[n] = tempindex ;
        }
    }
    
    // We need to make another temporary array of new index of vertices and SFC index
    std::map<uint64_t, uint64_t> _newOldVertices ;
    
    // Creating mapping of new and old vertices
    for (size_t i = 0 ; i < verti_array.size() ; ++i) {
        _newOldVertices.insert(std::pair<uint64_t, uint64_t>(verti_array[i].index,verti_array[i].sfc_index)) ;
    }
    
	// Renumbering the fixity Surface Solid 
	for (size_t i = 0; i < nodalFixitySurfaceSolid.size(); ++i) {
		uint64_t temp = nodalFixitySurfaceSolid[i].eleindex; 
		nodalFixitySurfaceSolid[i].eleindex = _newOldVertices[temp]; 
	}
	// Sort the nodal fixitysurface solid array 
	std::sort(nodalFixitySurfaceSolid.begin(), nodalFixitySurfaceSolid.end(), compareFixity); 

	// Renumber the Fixity Surface Licquid 
	for (size_t i = 0; i < nodalFixitySurfaceLiquid.size();  ++i) {
		uint64_t temp = nodalFixitySurfaceLiquid[i].eleindex; 
		nodalFixitySurfaceLiquid[i].eleindex = _newOldVertices[temp]; 
	}

	// Sort the nodal FixtitySurface Liquid 
	std::sort(nodalFixitySurfaceLiquid.begin(), nodalFixitySurfaceLiquid.end(), compareFixity); 

    // Sorting the vertices according to sfc order
    std::sort(verti_array.begin(), verti_array.end(), compareVertices) ; // QuickSort
    
	// Renumbering the load step 
	uint64_t temp; 
	for (size_t i = 0; i < loadVertices.size(); ++i) {
		temp = loadVertices[i]; 
		loadVertices[i] = _newOldVertices[temp]; 
	}


    /*********************************************************/
    /*********************************************************/
    // Now we write into the file
    /*********************************************************/
    /*********************************************************/
    
    std::cout << "Rewriting the .GOM file " << std::endl;

    std::ofstream outfile ;
    outfile.open("Example.GOM") ;
    outfile<<"$$STARTCOUNTERS"<<std::endl ;
    outfile<<_numElements<<" "<<_numVertices<<std::endl ;
    outfile<<"$$ENDCOUNTERS"<<std::endl ;
    outfile<<"$$STARTNODES"<<std::endl ;
    

	/**********************************************************/
    // Wrting vertices in to the file
    for (size_t i = 0 ; i < _numVertices; ++i) {
        outfile<<"       "<<verti_array[i].x<<"       "<<verti_array[i].y<<"       "<<verti_array[i].z<<std::endl ;
    }
    
    outfile<<"$$ENDNODES"<<std::endl ;
    
	/*********************************************************/
    // Writing elements into the file
	outfile << "$$STARTELEMCON" << std::endl;

    for (size_t i = 0 ; i < mesh.size(); ++i) {
        for (size_t n =0; n < mesh[i].nodes.size(); ++n) {
            outfile<<verti_array[mesh[i].nodes[n]-1].sfc_index<<" " ;
        }
        outfile<<std::endl ;
    }
    
    outfile<<"$$ENDELEMCON"<<std::endl;
	/*********************************************************/
	// Writing nodal fixities 
	outfile << "$$START_FIXITY_SURFACE_SOLID" << std::endl; 
	outfile << _numFixitySurfaceSolid << std::endl; 
	for (size_t i = 0; i < nodalFixitySurfaceSolid.size(); ++i) {
		outfile << nodalFixitySurfaceSolid[i].eleindex << "  " << nodalFixitySurfaceSolid[i].xfixity << "  " << nodalFixitySurfaceSolid[i].yfixity << "  "<<nodalFixitySurfaceSolid[i].zfixity << std::endl; 
	}
	outfile << "$$END_FIXITY_SURFACE_SOLID" << std::endl; 
	
	/*********************************************************/
	// Writing the line solid 
	outfile << "$$START_FIXITY_LINE_SOLID" << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_FIXITY_LINE_SOLID" << std::endl; 

	/*********************************************************/
	// Writing the Point Solid 
	outfile << "$$START_FIXITY_POINT_SOLID" << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_FIXITY_POINT_SOLID" << std::endl ;

	/*********************************************************/
	// Writing the Surface Liquid 
	outfile << "$$START_FIXITY_SURFACE_LIQUID" << std::endl; 
	outfile << _numFixitySurfaceLiquid << std::endl; 
	for (size_t i = 0; i < nodalFixitySurfaceLiquid.size(); ++i) {
		outfile << nodalFixitySurfaceLiquid[i].eleindex << "  " << nodalFixitySurfaceLiquid[i].xfixity << "  " << nodalFixitySurfaceLiquid[i].yfixity << "  " << nodalFixitySurfaceLiquid[i].zfixity << std::endl; 
	}
	outfile << "$$END_FIXITY_SURFACE_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing the Line liquid 
	outfile << "$$START_FIXITY_LINE_LIQUID" << std::endl; 
	outfile << _numFixityLineLiquid << std::endl; 
	outfile << "$$END_FIXITY_LINE_LIQUID" << std::endl;

	/*********************************************************/
	// Writing the Point Liquid
	outfile << "$$START_FIXITY_POINT_LIQUID" << std::endl; 
	outfile << _numFixityPointLiquid << std::endl; 
	outfile << "$$END_FIXITY_POINT_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing the Surface Gas 
	outfile << "$$START_FIXITY_SURFACE_GAS" << std::endl; 
	outfile << _numFixitySurfaceGas << std::endl; 
	outfile << "$$END_FIXITY_SURFACE_GAS" << std::endl; 

	/*********************************************************/
	// Writing the line Gas 
	outfile << "$$START_FIXITY_LINE_GAS" << std::endl; 
	outfile << _numFixityLineGas << std::endl; 
	outfile << "$$END_FIXITY_LINE_GAS" << std::endl; 

	/*********************************************************/
	// Writing the Point Gas 
	outfile << "$$START_FIXITY_POINT_GAS" << std::endl; 
	outfile << _numFixityPointGas << std::endl; 
	outfile << "$$END_FIXITY_POINT_GAS" << std::endl; 

	/*********************************************************/
	// Writing the Remove Fixity Surface Solid 
	outfile << "$$START_REMOVE_FIXITY_SURFACE_SOLID" << std::endl; 
	outfile << _numRemoveFixitySurfaceSolid << std::endl; 
	outfile << "$$END_REMOVE_FIXITY_SURFACE_SOLID" << std::endl; 

	/*********************************************************/
	// Writing the remove Fixity Line Solid 
	outfile << "$$START_REMOVE_FIXITY_LINE_SOLID" << std::endl; 
	outfile << _numRemoveFixityLineSolid << std::endl; 
	outfile << "$$END_REMOVE_FIXITY_LINE_SOLID" << std::endl; 

	/*********************************************************/
	// Writing the remove fixity point solid 
	outfile << "$$START_REMOVE_FIXITY_POINT_SOLID" << std::endl; 
	outfile << _numRemoveFixityPointSolid << std::endl; 
	outfile << "$$END_REMOVE_FIXITY_POINT_SOLID" << std::endl; 

	/*********************************************************/
	// Writing the remove Fixity Surface Liquid 
	outfile << "$$START_REMOVE_FIXITY_SURFACE_LIQUID" << std::endl; 
	outfile << _numRemoveFixitySurfaceLiquid << std::endl; 
	outfile << "$$END_REMOVE_FIXITY_SURFACE_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing the remove Fixity Line Liquid 
	outfile << "$$START_REMOVE_FIXITY_LINE_LIQUID" << std::endl; 
	outfile << _numRemoveFixityLineliquid << std::endl; 
	outfile << "$$END_REMOVE_FIXITY_LINE_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing the Remove fixity point liquid 
	outfile << "$$START_REMOVE_FIXITY_POINT_LIQUID" << std::endl; 
	outfile << _numRemoveFixityPointLiquid << std::endl;
	outfile << "$$END_REMOVE_FIXITY_POINT_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing the Remove fixity surface gas 
	outfile << "$$START_REMOVE_FIXITY_SURFACE_GAS" << std::endl; 
	outfile << _numRemoveFixitySurfaceGas << std::endl; 
	outfile << "$$END_REMOVE_FIXITY_SURFACE_GAS" << std::endl; 

	/*********************************************************/
	// Writing the Remove Fixity line gas 
	outfile << "$$START_REMOVE_FIXITY_LINE_GAS" << std::endl; 
	outfile << _numRemoveFixityLineGas << std::endl; 
	outfile << "$$END_REMOVE_FIXITY_LINE_GAS" << std::endl; 

	/*********************************************************/
	// Writing the Remove Fixity Point gas 
	outfile << "$$START_REMOVE_FIXITY_POINT_GAS" << std::endl; 
	outfile << _numRemoveFixityPointGas << std::endl; 
	outfile << "$$END_REMOVE_FIXITY_POINT_GAS" << std::endl; 

	/*********************************************************/
	// Writing the load steps     

	outfile << "$$START_LOAD_SOLID" << std::endl;
    outfile << _numLoadSolid << std::endl ;
	for (size_t i = 0; i < _numLoadSolid; ++i) {
		for (size_t j = i*6 ; j < (i*6 +6); ++j) {
			outfile << loadVertices[j]; 
			outfile << "  " ;
		}
		//outfile << std::endl; 
		for (size_t j = i*18 ; j < i*18 + 18 ; ++j) {
			outfile << loads[j]; 
			outfile << "      " ; 
		}
		outfile << std::endl;
	}

	outfile << "$$END_LOAD_SOLID" << std::endl; 

	/*********************************************************/
	// Writing the load liquid steps 
	outfile << "$$START_LOAD_LIQUID" << std::endl; 
	outfile << _numLoadLiquid << std::endl; 
	outfile << "$$END_LOAD_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing the load Gas 
	outfile << "$$START_LOAD_GAS" << std::endl; 
	outfile << _numLoadGas << std::endl; 
	outfile << "$$END_LOAD_GAS" << std::endl; 

	/*********************************************************/
	// Writing Start Absorbing Boundary Surface Solid 
	outfile << "$$START_ABSORBING_BOUNDARY_SURFACE_SOLID" << std::endl; 
	outfile << zero << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_ABSORBING_BOUNDARY_SURFACE_SOLID" << std::endl; 

	/*********************************************************/
	// Writing Absorbing Boundary Line Solid 
	outfile << "$$START_ABSORBING_BOUNDARY_LINE_SOLID" << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_ABSORBING_BOUNDARY_LINE_SOLID" << std::endl; 

	/*********************************************************/
	// Writing Absorbing Boundary Point Solid 
	outfile << "$$START_ABSORBING_BOUNDARY_POINT_SOLID" << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_ABSORBING_BOUNDARY_POINT_SOLID" << std::endl; 

	/*********************************************************/
	// Writing Absorbing Boundary Surface Liquid 
	outfile << "$$START_ABSORBING_BOUNDARY_SURFACE_LIQUID" << std::endl; 
	outfile << zero << std::endl; 
	outfile << zero << std::endl;
	outfile << "$$END_ABSORBING_BOUNDARY_SURFACE_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing Absorbing Boundary Line Liquid 
	outfile << "$$START_ABSORBING_BOUNDARY_LINE_LIQUID" << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_ABSORBING_BOUNDARY_LINE_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing Absorbing Boundary Point Liquid 
	outfile << "$$START_ABSORBING_BOUNDARY_POINT_LIQUID" << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_ABSORBING_BOUNDARY_POINT_LIQUID" << std::endl; 

	/*********************************************************/
	// Writing Absorbing Boundary Surface Gas 
	outfile << "$$START_ABSORBING_BOUNDARY_SURFACE_GAS" << std::endl; 
	outfile << zero << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_ABSORBING_BOUNDARY_SURFACE_GAS" << std::endl; 

	/*********************************************************/
	// Writing Absorbing Boundary Line Gas 
	outfile << "$$START_ABSORBING_BOUNDARY_LINE_GAS" << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_ABSORBING_BOUNDARY_LINE_GAS" << std::endl; 

	/*********************************************************/
	// Writing Absorbing Boundary Point Gas 
	outfile << "$$START_ABSORBING_BOUNDARY_POINT_GAS" << std::endl; 
	outfile << zero << std::endl; 
	outfile << "$$END_ABSORBING_BOUNDARY_POINT_GAS" << std::endl; 

	/*********************************************************/
	// Writing Material Properties and Physical Constants
	
	outfile << "$$NUMBER_OF_MATERIALS" << std::endl; 
	outfile << _numMaterials << std::endl; 
	
	outfile << "$$MATERIAL_INDEX" << std::endl; 
	outfile << _materialNum << std::endl; 

	outfile << "$$MATERIAL_NAME" << std::endl; 
	outfile << _materialName << std::endl; 

	outfile << "$$MATERIAL_TYPE" << std::endl; 
	outfile << _materialType << std::endl; 

	outfile << "$$POROSITY_SOLID" << std::endl; 
	outfile << _porositySolid << std::endl; 

	outfile << "$$DENSITY_SOLID" << std::endl; 
	outfile << _densitySolid << std::endl; 

	outfile << "$$K0_VALUE_SOLID" << std::endl; 
	outfile << _k0ValueSolid << std::endl; 

	outfile << "$$INTRINSIC_PERMEABILITY_LIQUID" << std::endl; 
	outfile << _intrinsicPermeabilityLiquid << std::endl; 

	outfile << "$$DENSITY_LIQUID" << std::endl; 
	outfile << _densityLiquid << std::endl; 

	outfile << "$$BULK_MODULUS_LIQUID" << std::endl; 
	outfile << _bulkModulusLiquid << std::endl; 

	outfile << "$$DYNAMIC_VISCOSITY_LIQUID" << std::endl; 
	outfile << _dynamicViscosityLiquid << std::endl; 

	outfile << "$$MATERIAL_MODEL_SOLID" << std::endl; 
	outfile << _materialModelSolid << std::endl; 

	outfile << "$$YOUNG_MODULUS" << std::endl; 
	outfile << _youngsModulus << std::endl; 

	outfile << "$$POISSON_RATIO" << std::endl; 
	outfile << _poissonRatio << std::endl; 

	outfile << "$$ENDMAT" << std::endl; 
	
	/*********************************************************/
	// Writing the Element Material Point 
	outfile << "$$STARTELMMAT" << std::endl; 
	for (size_t i = 0; i < mesh.size(); ++i) {
		outfile << mesh[i].materialId << std::endl; 
	}
	outfile << "$$ENDELMMAT" << std::endl; 

	/*********************************************************/
	// Writing the Element Damping 
	outfile << "$$STARTDAMPING" << std::endl; 
	for (size_t i = 0; i < mesh.size(); ++i) {

		outfile << mesh[i].damping << std::endl; 
	}
	outfile << "$$ENDDAMPING" << std::endl; 

	/*********************************************************/
	// Writing the Number of Material Points 
	outfile << "$$START_NUMBER_OF_MATERIAL_POINTS" << std::endl; 
	for (size_t i = 0; i < mesh.size(); ++i) {

		outfile << mesh[i].numSolidMatPoints << " "<< mesh[i].numLiquidMatPoints << std::endl;
	}
	outfile << "$$END_NUMBER_OF_MATERIAL_POINTS" << std::endl; 
	outfile << "$$FINISH" << std::endl; 
	/*********************************************************/

	return 0; 
}

bool compareElement(const element &e1, const element &e2) {
    
    if (e1.sfc_index < e2.sfc_index) {
        return true ;
    }
    else
        return false;
}

bool compareVertices(const vertices &v1, const vertices &v2) {
    
    if (v1.sfc_index < v2.sfc_index) {
        return  true ;
    }
    else return false ;
}

void updateBox(BoundingBox &b) {
    
    b.centre_x = (b.box_max_x + b.box_min_x ) * 0.5 ;
    b.centre_y = (b.box_max_y + b.box_min_y ) * 0.5 ;
    b.centre_z = (b.box_max_z + b.box_max_z ) * 0.5 ;

}

uint64_t Morton_Curve(element E,BoundingBox box,uint64_t N_level) {
    
    int64_t index = 0 ;
    //std::cout<<E.centroidx<<std::endl ;
    for (size_t i =0 ; i < N_level ; ++i) {
        
        index = index << 3;
        
        // Set the octant using Morton order curve
        if (E.centroidx > box.centre_x) {
            index = index + 1 ;
        }
        if (E.centroidy > box.centre_y) {
            index = index + 2 ;
        }
        if (E.centroidz > box.centre_z) {
            index = index + 4 ;
        }
        
        // Update the bounding box
        if (E.centroidx > box.centre_x) {
            box.box_min_x = box.centre_x ;
        }
        else box.box_max_x = box.centre_x ;
        
        if (E.centroidy > box.centre_y) {
            box.box_min_y = box.centre_y ;
        }
        else box.box_max_y = box.centre_y ;
        
        if (E.centroidz > box.centre_z) {
            box.box_min_z = box.centre_z ;
        }
        else box.box_max_z = box.centre_z ;
     
        // Update the bounding Box
        updateBox(box) ;
    }
    return  index ;
}



