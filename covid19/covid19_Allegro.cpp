#include <allegro5/allegro.h>
#include "persona.h"
#include "jallegro/Allegro.h"
#include "jallegro/Frame.h"
#include "Covid_19.h"
#include <iostream>
using namespace std;
using namespace jallegro;
int dim=150;
int day = 0;
int infected = 0, not_infected = 0, healed = 0, tot = 0, max_inf = 0,day_max= 0, death = 0, tag = 1, source = 0;
int numtasks, rank_p,rowtype;
bool e = false;
Panel* p;

bool thereAreInfect(Persona**);
void controlHealed(int,int,Persona**);
void controlInfect(int,int,Persona**);
void move(int,int,Persona**);
void analyzeMatrix(int, int, Persona**);
void game(Persona**,int**);
void print();

int main(int argc, char **argv) {
    Allegro::init();
    srand(time(0));
    Persona **mat1;
    int **mat2;


    if(rank_p == 0){
      mat1 = new Persona *[dim];
      mat2 = new int *[dim];
       for (int i=0;i<dim;++i) {
            mat1[i] = new Persona [dim];
      	    mat2[i] = new int [dim];
       }
    }

    for (int i=0;i<dim;++i) {
        for (int j=0;j<dim;++j){
           int perc = rand()%100;
            if(perc > 0 && perc <= 21){
             mat1[i][j].setStato(2);
             mat2[i][j] = 2;
	    }
	    if(perc > 21 && perc < 24){
	     mat1[i][j].setStato(1);
	     mat2[i][j] = 1;
	     mat1[i][j].setGinfezione(day);
	    }
	    if(perc >= 24 && perc <= 100){
	     mat1[i][j].setStato(0);
             mat2[i][j] = 0;	
 	    }
	}    
    }

    Frame f(dim * 5 + 20, dim * 5 + 20, "Covid-19");
    p = new Covid_19(mat1, dim, dim);
    f.add(p);
    f.show();
    game(mat1,mat2);
    Allegro::sleep(100000);
     return 0;
    
    for (int i=0;i<dim;++i) {
        delete mat1[i];
    }
    delete[] mat1;
    delete p;
    return 0;
}

void game(Persona** mainMatrix,int** matr2) {
    int time = 0;
    int time_limit = 1000;

    while(time < time_limit && !e) {
        time++;
        Allegro::sleep(0.3);
	day++;
       for (int i = 0; i < dim; ++i) {
       	 for (int j = 0; j < dim; ++j) {
           analyzeMatrix(i,j,mainMatrix);
           if(mainMatrix[i][j].getStato() == 1)
            infected++;
           if(infected > max_inf){
            max_inf = infected;
            day_max = day; 
           }
         }
      }
	if(!thereAreInfect(mainMatrix)){
	     e = true;
	}
	print();
      infected = 0;
 }
   for (int i = 0; i < dim; ++i) {
       for (int j = 0; j < dim; ++j) {
          if(mainMatrix[i][j].getStato() == 2){
	    not_infected++;
	    tot++;
	  }
	  if(mainMatrix[i][j].getStato() == 3){
	    healed++;
	    tot++;
	  }
       }
   }
  cout<<"PERSONE TOTALI: "<<tot<<endl;
  cout<<"PERSONE NON INFETTE: "<<not_infected<<endl;
  cout<<"PERSONE GUARITE: "<<healed<<endl;
  cout<<"PICCO MASSIMO INFETTI: "<<max_inf<<" NEL "<<day_max<<"° GIORNO"<<endl;
  cout<<"GIORNI PASSATI DALL'IMMUNIZZAZIONE: "<<day<<endl;
  cout<<"MORTI: "<<death<<endl;
  double perc_death = (float) death/tot;
  cout<<"PERCENTUALE DECESSI: "<<perc_death<<"%"<<endl;
}

void analyzeMatrix(int x, int y, Persona** mainMatrix) {
    if(mainMatrix[x][y].getStato() == 1){
        move(x,y, mainMatrix);
    	controlHealed(x,y,mainMatrix); 
	return;
    }
    if(mainMatrix[x][y].getStato() == 2){
        move(x,y, mainMatrix);
    	controlInfect(x,y,mainMatrix); 
	return;
    }
    if(mainMatrix[x][y].getStato() == 3){
        move(x,y, mainMatrix);
	return;
    }
}

bool thereAreInfect(Persona** mainMatrix){
  	for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
	      if(mainMatrix[i][j].getStato() == 1){
		return true;
       	      }
            }
	}
   return false;
}


void controlHealed(int i,int j,Persona** mainMatrix){
  int perc = rand()%100;
  int dayR = rand()%15;
  	for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
	       if(mainMatrix[i][j].getStato() == 1 && day >= (mainMatrix[i][j].getGinfezione()+15)+dayR && perc > 70){
		   mainMatrix[i][j].setStato(3);
	       }
            }
        }
}


void controlInfect(int i,int j,Persona** mainMatrix){
 int perc = rand()%300;

  if(perc <= 1){
    mainMatrix[i][j].setStato(4);
    death++;
    return;
  }
   if (i == 0 && j == 0) {
    if(mainMatrix[i][j+1].getStato() == 1 || mainMatrix[i + 1][j].getStato() == 1){
       mainMatrix[i][j].setStato(1);
       mainMatrix[i][j].setGinfezione(day);
    }
     return;
   }
   if (i == dim - 1 &&  j == dim - 1) {
    if(mainMatrix[i][j - 1].getStato() == 1 || mainMatrix[i - 1][j].getStato() == 1){
       mainMatrix[i][j].setStato(1);
       mainMatrix[i][j].setGinfezione(day);
    }
       return;
   }
   if (i == 0 && j == dim - 1) {
     if(mainMatrix[i][j - 1].getStato() == 1 || mainMatrix[i + 1][j].getStato() == 1){
       mainMatrix[i][j].setStato(1);
       mainMatrix[i][j].setGinfezione(day);
    }
       return;
   }
   if (i == dim - 1 && j == 0) {  
     if(mainMatrix[i][j+1].getStato() == 1 || mainMatrix[i - 1][j].getStato() == 1){
       mainMatrix[i][j].setStato(1);
       mainMatrix[i][j].setGinfezione(day);
    }
       return;
   }
   if (i == 0  && ( j > 0 && j < dim-1)) {
     if(mainMatrix[i][j+1].getStato() == 1 || mainMatrix[i + 1][j].getStato() == 1 || mainMatrix[i][j - 1].getStato() == 1) {
       mainMatrix[i][j].setStato(1);
       mainMatrix[i][j].setGinfezione(day);
    }
       return;
   }
   if (i == dim - 1 && (j > 0 && j < dim - 1)) {
     if(mainMatrix[i][j+1].getStato() == 1 || mainMatrix[i - 1][j].getStato() == 1 || mainMatrix[i][j - 1].getStato() == 1) {
       mainMatrix[i][j].setStato(1);
       mainMatrix[i][j].setGinfezione(day);
    }
       return;
   }
   if (j == 0 && (i > 0 && i < dim - 1)) {
      if(mainMatrix[i][j+1].getStato() == 1 || mainMatrix[i + 1][j].getStato() == 1 || mainMatrix[i - 1][j].getStato() == 1){
       mainMatrix[i][j].setStato(1);
       mainMatrix[i][j].setGinfezione(day);
    }
       return;   
   }
   else{ 
  	if(mainMatrix[i-1][j].getStato() == 1 || mainMatrix[i][j-1].getStato() == 1 || mainMatrix[i+1][j].getStato() == 1 || mainMatrix[i][j+1].getStato() == 1){
		mainMatrix[i][j].setStato(1);
		mainMatrix[i][j].setGinfezione(day);
  	}
   }
}



void move(int i, int j, Persona** mainMatrix){
 int perc_2 = rand()%100;
    if (i == 0 && j == 0) {
       if(perc_2 >= 0 && perc_2 <= 33 && mainMatrix[i][j + 1].getStato()  == 0){
        mainMatrix[i][j + 1].setStato(mainMatrix[i][j].getStato());
	mainMatrix[i][j].setStato(0);	
       }
       if(perc_2 >33 && perc_2 <= 66 && mainMatrix[i + 1][j].getStato()  == 0){
        mainMatrix[i + 1][j].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j + 1].getStato()  == 0){
        mainMatrix[i + 1][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
      return;
    }
    if (i == dim - 1 &&  j == dim - 1) {
       if(perc_2 >= 0 && perc_2 <= 33 && mainMatrix[i][j - 1].getStato()  == 0){
        mainMatrix[i][j - 1].setStato(mainMatrix[i][j].getStato());
	mainMatrix[i][j].setStato(0);	
       }
       if(perc_2 >33 && perc_2 <= 66 && mainMatrix[i - 1][j].getStato()  == 0){
	mainMatrix[i - 1][j].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i - 1][j -1 ].getStato()  == 0){
	mainMatrix[i - 1][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
      return;
    }
    if (i == 0 && j == dim - 1) {
       if(perc_2 >= 0 && perc_2 <= 33 && mainMatrix[i][j -1].getStato()  == 0){
	mainMatrix[i][j - 1].setStato(mainMatrix[i][j].getStato());
	mainMatrix[i][j].setStato(0);	
       }
       if(perc_2 >33 && perc_2 <= 66 && mainMatrix[i + 1][j].getStato()  == 0){
	mainMatrix[i + 1][j].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j - 1].getStato()  == 0){
	mainMatrix[i + 1][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
      return;       
    }
    if (i == dim - 1 && j == 0) {
       if(perc_2 >= 0 && perc_2 <= 33  && mainMatrix[i][j + 1].getStato()  == 0){
	mainMatrix[i][j + 1].setStato(mainMatrix[i][j].getStato());
	mainMatrix[i][j].setStato(0);	
       }
       if(perc_2 >33 && perc_2 <= 66 && mainMatrix[i - 1][j].getStato()  == 0){
	mainMatrix[i - 1][j].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i - 1][j + 1].getStato()  == 0){
	mainMatrix[i - 1][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
      return;        
    }
    if (i == 0  && ( j > 0 && j < dim-1)) {
       if(perc_2 >= 0 && perc_2 <= 20 && mainMatrix[i][j - 1].getStato()  == 0){
	mainMatrix[i][j - 1].setStato(mainMatrix[i][j].getStato());
	mainMatrix[i][j].setStato(0);	
       }
       if(perc_2 >20 && perc_2 <= 40 && mainMatrix[i + 1][j].getStato()  == 0){
	mainMatrix[i][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j - 1].getStato()  == 0){
	mainMatrix[i + 1][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >20 && perc_2 <= 40  && mainMatrix[i][j + 1].getStato()  == 0){
	mainMatrix[i][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j + 1].getStato()  == 0){
	mainMatrix[i + 1][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
      return;
    }
    if (i == dim - 1 && (j > 0 && j < dim - 1)) {
      if(perc_2 >= 0 && perc_2 <= 20 && mainMatrix[i - 1][j].getStato()  == 0){
	mainMatrix[i - 1][j].setStato(mainMatrix[i][j].getStato());
	mainMatrix[i][j].setStato(0);	
       }
       if(perc_2 > 20 && perc_2 <= 40 && mainMatrix[i][j + 1].getStato()  == 0){
	mainMatrix[i][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 40 && perc_2 <= 60 && mainMatrix[i][j - 1].getStato()  == 0){
	mainMatrix[i][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 60 && perc_2 <= 80 && mainMatrix[i - 1][j - 1].getStato()  == 0){
	mainMatrix[i - 1][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 80 && perc_2 <= 100 && mainMatrix[i - 1][j - 1].getStato()  == 0){
	mainMatrix[i - 1][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
      return;
    }
    if (j == 0 && (i > 0 && i < dim - 1)) {
       if(perc_2 >= 0 && perc_2 <= 20 && mainMatrix[i - 1][j].getStato()  == 0){
	mainMatrix[i][j - 1].setStato(mainMatrix[i][j].getStato());
	mainMatrix[i][j].setStato(0);	
       }
       if(perc_2 >20 && perc_2 <= 40 && mainMatrix[i + 1][j].getStato()  == 0){
	mainMatrix[i + 1][j].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i][j + 1].getStato()  == 0){
	mainMatrix[i][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >20 && perc_2 <= 40 && mainMatrix[i + 1][j - 1].getStato()  == 0){
	mainMatrix[i + 1][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j + 1].getStato()  == 0){
	mainMatrix[i + 1][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
      return;        
    }
    else {
       if(perc_2 >= 0 && perc_2 <= 8 && mainMatrix[i - 1][j - 1].getStato()  == 0){
	mainMatrix[i - 1][j - 1].setStato(mainMatrix[i][j].getStato());
	mainMatrix[i][j].setStato(0);	
       }
       if(perc_2 > 8 && perc_2 <= 15 && mainMatrix[i - 1][j].getStato()  == 0){
	mainMatrix[i - 1][j].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 15 && perc_2 <= 22 && mainMatrix[i - 1][j + 1].getStato()  == 0){
	mainMatrix[i - 1][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 22 && perc_2 <= 34 && mainMatrix[i][j - 1].getStato()  == 0){
	mainMatrix[i][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 34 && perc_2 <= 40 && mainMatrix[i][j + 1].getStato()  == 0){
	mainMatrix[i][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 40 && perc_2 <= 47 && mainMatrix[i + 1][j - 1].getStato()  == 0){
	mainMatrix[i + 1][j - 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 47 && perc_2 <= 58 && mainMatrix[i + 1][j].getStato()  == 0){
	mainMatrix[i + 1][j].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
       if(perc_2 > 58 && perc_2 <= 63 && mainMatrix[i + 1][j + 1].getStato()  == 0){
	mainMatrix[i + 1][j + 1].setStato(mainMatrix[i][j].getStato());
        mainMatrix[i][j].setStato(0); 
       }
      return;
    }
}


void print() {
    p->repaint();
}
