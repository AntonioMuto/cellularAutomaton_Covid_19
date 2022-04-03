#ifndef PERSONA_H
#define PERSONA_H
#include <iostream>
using namespace std;

class Persona{
 private: 
   int stato = 0;
   int gInfezione = 0;

 public:

   Persona(){
    stato = 0;
    gInfezione = 0;
   }

   Persona(int s){	
    stato = s;
   }
 
  ~Persona(){
 	
   }

   void setStato(int s){
     stato = s;	
   }	
	
   int getStato(){
     return stato;
   }

   void setGinfezione(int g){
    gInfezione = g;
   }

   int getGinfezione(){
    return gInfezione;
   }
   
};

#endif
