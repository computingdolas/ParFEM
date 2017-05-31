//
//  SolverFunction.cpp
//  FEM
//
//  Created by Sagar Dolas on 27/05/16.
//  Copyright © 2016 Sagar Dolas. All rights reserved.
//

#include "SolverFunction.hpp"

template<class CompressedRowStorage>
SIWIR2::FEM::Solver<CompressedRowStorage>::Solver(const CompressedRowStorage &A,
                            const CompressedRowStorage &M,
                            SIWIR2::FEM::DomainData<real_d> &u,
                            const real_d _eps) : A(&A),M(&M),u(&u),eps(_eps),f(u.size(),FEM::dataType::normal) {
    
}

template<class CompressedRowStorage>
SIWIR2::FEM::Solver<CompressedRowStorage>::~Solver(){
    
}

template<class CompressedRowStorage>
bool SIWIR2::FEM::Solver<CompressedRowStorage>::ConjugateGradient(){
    
    // Temporary arrays
    SIWIR2::FEM::DomainData<real_d> r((*u).size(), SIWIR2::FEM::dataType::normal) ;
    SIWIR2::FEM::DomainData<real_d> p((*u).size(), SIWIR2::FEM::dataType::normal) ;
    SIWIR2::FEM::DomainData<real_d> Ap((*u).size(), SIWIR2::FEM::dataType::normal) ;
    
    // Temporary values
    real_d alpha = 0.0 ;
    real_d rsold = 0.0 ;
    real_d rsnew = 0.0 ;
    
    r = f - (*A)*(*u) ;
    p = r ;
    rsold = r * r ;

    //Number of columns space
    const real_l N = (*u).size() ;
    
    for (real_l iter =0 ; iter <N ; ++iter) {
        
        Ap = (*A) * p ;
        alpha = rsold / (p * Ap ) ;
        (*u) = (*u) + p * alpha ;
        r = r - Ap * alpha ;
        rsnew = r * r ;
        if (std::sqrt(rsnew) < eps) {
            return true ;
        }
        p = r + p * (rsnew/rsold) ;
        rsold = rsnew ;
    }
    return true;
}

template<class CompressedRowStorage>
void SIWIR2::FEM::Solver<CompressedRowStorage>::InversePowerIteration(){
    
    logonScreen("/////////////////////////////////") ;
    logonScreen("Inverse Power Iteration Starting ") ;
    logonScreen("/////////////////////////////////") ;
    logonScreen("") ;
    // Local values
        
    real_d lamdaold = ((*u) * ((*A) * (*u)) ) / ((*u) * ((*M) * (*u)));
    real_d lambatemp = 0.0 ;
    real_d relative_error = 0.0 ;
    real_l iter = 1 ;
    
    do {
        
        lambatemp = lamdaold ;
        this->f = (*M) * (*u) ;
        ConjugateGradient() ;
        real_d normuinv  = 1.0 / std::sqrt( (*u) * (*u) ) ;
        (*u) = (*u) * normuinv ;
        lambda = ((*u) * ((*A) * (*u)) ) / ((*u) * ((*M) * (*u))) ;
        lamdaold = lambda ;
        relative_error = std::fabs(((lambda - lambatemp)/ lambatemp)) ;
        
        std::cout<<std::endl ;
        logonScreen("/*******************************************/") ;
        std::cout<<"Iteration := "<<" "<<iter<<" "<<std::endl ;
        std::cout<<"Relative Error := "<<" "<<relative_error<<std::endl ;
        std::cout<<std::setprecision(16)<<"Lambda := "<<" "<<lambda<<std::endl ;
        logonScreen("/*******************************************/") ;
        std::cout<<std::endl ;
        
        ++iter ;
        
    }while (relative_error > eps) ;
    
}

