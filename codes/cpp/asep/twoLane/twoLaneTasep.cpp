// Implementation of totally asymmetric exclusion process( TASEP) in 1-dimension.
//
//
//
// Rajesh Singh
// 
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "../mt19937ar.h"
#include "../mt19937ar.c"
#define Ns 1000               // lattice sized
#define beta 1              // interaction parameter between lattices.
using namespace std;


int main()
{
    //// Declarations
    //define functions for modularity
    void initialise(    int [], int [], double [], double []);
    void addParticle(   int [], double);
    void removeParticle(int [], double);
    void moveParticle(  int [], int   );
    void moveParticle2D(int [], int [], int   );
    
    int iter=1e8;       // lattice and iterations to be done on them.
    int i, j, ii, rn;
    int intrvl = 100;             // interval after how many iterations at which observation is made.
    double lbc=0.3, rbc=0.6;      // boundary conditions (BCs) on the left and right boundaries.
    double lbc2=0.1, rbc2=0.8;      // boundary conditions (BCs) on the left and right boundaries.
    int A[Ns], B[Ns];             // occupancy of the site on the lattice
    double d[Ns], d2[Ns];                  


	init_genrand(time(NULL));                                               // seed the random number generator with the NULL of the time!

    initialise(A, B, d, d2);                                                 // initialise the array!  
    
    // now simulate
    for (j=1; j<iter; j++){                  
        rn = genrand_real2()*(Ns + 1);                                      // NOTE that we have generated N+1 RNs for N sized lattice.    

        if (genrand_real2() <0.5){   // choose the lattice randomly!

            // case corresponding to the addition of particle
            if (( rn == Ns) && (A[0]==0) )           addParticle(A, lbc);      


            // case corresponding to the particle removal
            else if ( (rn == Ns-1) && (A[Ns-1]==1) ) removeParticle(A, rbc);    
                          

            // move the  particles on the lattice 
            else if ( (A[rn]==1) && (A[rn+1]==0) )   moveParticle2D(A, B, rn);       
            }
       

        else{       // lattice 2
        
            // case corresponding to the addition of particle
            if (( rn == Ns) && (B[0]==0) )           addParticle(B, lbc2);      


            // case corresponding to the particle removal
            else if ( (rn == Ns-1) && (B[Ns-1]==1) ) removeParticle(B, rbc2);    
                          

            // move the  particles on the lattice 
            else if ( (B[rn]==1) && (B[rn+1]==0) )   moveParticle2D(B, A, rn);       
            }


        // take the observation with this interval!
        if (j%intrvl==0){                                           
            for (ii=0; ii<Ns; ii++) {
                d[ii] +=A[ii];
                d2[ii]+=B[ii];
            }
        } 
    }


//simulation completed! Now plot, save, etc. with the data.       
        ofstream myfile;
        myfile.open ("testData.txt");
        for (i=0; i<Ns; i++){
             cout   <<i<<"  "<<intrvl*d[i]/iter<<"  "<<intrvl*d2[i]/iter<<endl;
             myfile <<i<<"  "<<intrvl*d[i]/iter<<"  "<<intrvl*d2[i]/iter<<endl;
        }
        myfile.close();

// that is all!
return 0;
}


//--------------------------------------------------
// functions block
//--------------------------------------------------

void initialise(int A[], int B[], double d[], double d2[]){
    int i;
    // initialise the lattice randomly and zero the density to start with.
    for (i = 0; i < Ns; i++){
        if ( genrand_real2() <= 0.5) A[i] = 1;
        else A[i] = 0;     
        
        if ( genrand_real2() <= 0.5) B[i] = 1;
        else B[i] = 0;     

        d[i]  =  0.0;          //initialise the density to be zero on the site.
        d2[i]  = 0.0;          //initialise the density to be zero on the site.
        }
}


// add particle
void addParticle(int A[], double lbc){
    double rn1; 
    if (A[0]==0){
        rn1 = genrand_real2();
        if (rn1 <=lbc) A[0]=1;
    }         
}


// remove particle
void removeParticle(int A[], double rbc){
    double rn1;
    if (A[Ns-1]==1){
        rn1 = genrand_real2();
        if (rn1 <=rbc) A[Ns-1]=0;
        }
}


// move the particle
// Movement of particles on the lattice depending on the BCs.
void moveParticle(int A[], int rn){
    int rn1;
    rn1 = A[rn]; 
    A[rn] = A[rn]*A[rn+1];
    A[rn+1] = A[rn+1] + (1- A[rn+1])*rn1;
}


// move the particle in 2D
// Movement of particles on the lattice depending on the BCs.
void moveParticle2D(int C[], int D[], int rn){
    int r1 = D[rn] + D[rn+1];
    double rate = (1 + (beta-1)*r1/2.0);
    if (genrand_real2() <= rate){
        C[rn] = 0;   
        C[rn+1]=1;
    }
}