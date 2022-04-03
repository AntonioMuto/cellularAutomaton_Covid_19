#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <iostream>

void analyzeMatrix(int i, int j, int** mainMatrix,int day,int rank,bool endF);
void controlInfect(int i,int j,int** mainMatrix,bool endF);
void controlHealed(int i,int j,int** mainMatrix,int day, bool endF);
void move(int i, int j, int** mainMatrix, bool endF);

int main(int argc, char** argv) {
  srand(time(0));
  double start, end;
  bool keep = true, endF = true;
  int rank, size,day = 0,incr = 0,border = 0, max_inf = 0, cont_inf = 0,day_max = 0 , tot = 0;
  int strip_size, A_row, A_col;
  int **A, *Adata;
  int **strip_A,*stripdata; 
  MPI_Datatype strip;

  MPI_Init(&argc,&argv) ;
  start = MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD,&rank) ;
  MPI_Comm_size(MPI_COMM_WORLD,&size) ;

  if(size == 2){
     border = 11;
  }
  if(size == 4){
     border = 5;
  }
       A_row = 20;
       A_col = 20;
       strip_size = (A_row / size);
       int Gsize = (400 / size);
       Adata = (int *)malloc(sizeof(int)* A_row*A_col);
       A = (int **)malloc(sizeof(int*) * A_row);
       for(int i = 0; i < A_row; i++) {
              A[i] = (int *)malloc(sizeof(int) * A_col);
       }

      if(rank == 0) {
		    for(int i = 0; i < A_row; i++){
                            for(int j = 0; j < A_col; j++){
				A[i][j] = 0;
			    }
		    }
                    for(int i = 0; i < A_row; i++){
                            for(int j = 0; j < A_col; j++){
			       int perc = rand()%100;
           			    if(perc >= 0 && perc <= 47)
        			      A[i][j] = 2;
	 			    if(perc > 47 && perc < 50){
	  			      A[i][j] = 1;
				    }
	  			    if(perc >= 50 && perc <= 100){
            			      A[i][j] = 0;	
 	  			    }
				std::cout<< A[i][j] << " ";
                            }
			std::cout<<std::endl;
                    }

// trasforma la matrice dinamica in vettore
                     int k = 0;
                    for(int i = 0; i < A_row; i++) {
                            for(int j = 0; j < A_col; j++) {
                               Adata[k] = A[i][j];
                               k++;	
                            }
                    }
 	}
            /* defining a datatype for sub-matrix */
            MPI_Type_vector(strip_size, A_col, A_col, MPI_INT, &strip);
            MPI_Type_commit(&strip);


// in questo vettore ricevo, tramite lo scatter gli elementi del vettore della matrice
// di origine
       stripdata = (int *)malloc(sizeof(int)* (strip_size*A_col + (2 * A_row)));
       for(int i = 0; i < strip_size*A_col + (2 * A_row); ++i)
              stripdata[i] = -1;

// questa è la matrice dove, una volta ricevuti i dati tramite lo scatter, li converte in matrice
       strip_A = (int **)malloc(sizeof(int*)*(12));
       for(int i= 0; i<12; i++)
              strip_A[i] = (int *)malloc(sizeof(int) *A_row);

       for(int i= 0; i< A_row/2 + 2; i++)
              for(int j = 0; j < A_col; ++j)
                     strip_A[i][j] = -1; 

       int cont = 0;
       while(keep && day < 50) {
       MPI_Scatter(&Adata[0], 1, strip, &(stripdata[A_row]), 1, strip, 0, MPI_COMM_WORLD);

       MPI_Status status;   

       if(rank == 0) { 
              MPI_Recv(&stripdata[strip_size*A_col + A_row], A_row, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
	      MPI_Send(&stripdata[strip_size*A_col], A_row, MPI_INT, 1, 1, MPI_COMM_WORLD);
       }
       if(rank == 1) {
              MPI_Send(&stripdata[A_row], A_row, MPI_INT, 0, 0, MPI_COMM_WORLD);
              MPI_Recv(&stripdata[0], A_row, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
       }

// trasformazione in matrici nei vari processi
       for(int k = 0, i = 0, j = 0; k < strip_size*A_col + (2 * A_row); ++k, ++j) {
              if(j >= A_row) {
                     j = 0;
                     i++;
              }
              strip_A[i][j] = stripdata[k];
       }

// analizzo la matrice

       if(rank == 0) { 
	      for(int i=0; i < strip_size; i++) {
                     for(int j = 0; j < A_col; ++j) {
			 analyzeMatrix(i,j,strip_A,day,rank,endF);
		         stripdata[incr] = strip_A[i][j];
			 incr++;
                     }
              }
       } 
	
       if(rank == 1) {
              for(int i=0; i < strip_size; i++) {
                     for(int j = 0; j < A_col; ++j) {
			  analyzeMatrix(i,j,strip_A,day,rank,endF);
		           stripdata[incr] = strip_A[i][j];
			    incr++;
                     }
              }
       }
	
       if(rank == 2) {
              for(int i=0; i < strip_size; i++) {
                     for(int j = 0; j < A_col; ++j) {
			  analyzeMatrix(i,j,strip_A,day,rank,endF);
		           stripdata[incr] = strip_A[i][j];
			    incr++;
                     }
              }
       }
	
	if(rank == 3) {
              for(int i=0; i < strip_size; i++) {
                     for(int j = 0; j < A_col; ++j) {
			  analyzeMatrix(i,j,strip_A,day,rank,endF);
		           stripdata[incr] = strip_A[i][j];
			    incr++;
                     }
              }
       }
       MPI_Barrier(MPI_COMM_WORLD);
	

              // gather per portare tutto nella amtrice inizilaie
              MPI_Gather(&stripdata[A_row], Gsize, MPI_INT, Adata, Gsize, MPI_INT, 0, MPI_COMM_WORLD);
              // stampa matrice completea
              if(rank == 0) {
                     for(int k = 0, i = 0, j = 0; k < 400; ++k, ++j) {
                            if(j >= A_col) {
                                   j = 0;
                                   i++;
                            }
                     A[i][j] = Adata[k];
                     }
			    std::cout << std::endl;
			    std::cout <<"DAY: "<< day;
	  	            std::cout << std::endl;
		    keep = false;
		    cont_inf = 0;
                     for(int i = 1; i < 19; ++i) {
                            for(int j = 0; j < 20; ++j) {
			      if(i != 4 && i != 9 && i != 14){
                                 std::cout << A[i][j] << " "; 
				  if(A[i][j] == 1){
				    cont_inf++;
				    keep = true;
                            	  }
				if(cont_inf > max_inf){
				   max_inf = cont_inf;
				   day_max = day;
				}
			      }
			    }
			    if(i != 4 && i != 9 && i != 14){
	                        std::cout << std::endl;				      
			    }
                     }
              }
              day++;
	      incr = 0;
	    if(!keep){
		int death = 0, healed = 0, not_infected = 0;
		for(int i = 1; i < 19; ++i) {
                  for(int j = 0; j < 20; ++j) {
		    if(i != 4 && i != 9 && i != 14){
			if(A[i][j] == 4){
			  death++;
			  tot++;
			}
			if(A[i][j] == 3){
			  healed++;
			  tot++;
			}
			if(A[i][j] == 2){
			  not_infected++;
			  tot++;
			}
		    }  
		  }
		}		
		 end = MPI_Wtime();
		std::cout<<"PERSONE TOTALI: "<<tot<<"\n";
  		std::cout<<"PERSONE NON INFETTE: "<<not_infected<<"\n";
  		std::cout<<"PERSONE GUARITE: "<<healed<<"\n";
  		std::cout<<"PICCO MASSIMO INFETTI: "<<max_inf<<" NEL "<<day_max+1<<"° GIORNO"<<"\n";
  		std::cout<<"GIORNI PASSATI DALL'IMMUNIZZAZIONE: "<<day<<"\n";
  		std::cout<<"MORTI: "<<death<<"\n";
 		 double perc_death = (float) death/tot;
 		 std::cout<<"PERCENTUALE DECESSI: "<<perc_death<<"%"<<"\n";
		return 0;
	    }
       }     
 
    MPI_Type_free(&strip);

    for(int i = 0; i < A_row/2 + 2; ++i)
    free(strip_A[i]);
    free(strip_A);

    free(stripdata);
    free(Adata);

    for(int i = 0; i < A_row; ++i)
       free(A[i]);
    free(A);
    
    MPI_Finalize();
    return 0;
}

void analyzeMatrix(int x, int y, int** mainMatrix,int day, int rank, bool endF) {
    if(mainMatrix[x][y] == 1 ){
        move(x,y, mainMatrix,endF);
    	controlHealed(x,y,mainMatrix,day,endF); 
    }
    if(mainMatrix[x][y] == 2){
	move(x,y, mainMatrix,endF);
    	controlInfect(x,y,mainMatrix,endF); 
    }
}



void move(int i, int j, int** mainMatrix,bool endF){
 int perc_2 = rand()%100;
 if(endF){
   return;
 }
    if (i == 1 && j == 0) {
       if(perc_2 >= 0 && perc_2 <= 33 && mainMatrix[i][j + 1]  == 0){
        mainMatrix[i][j + 1] = mainMatrix[i][j];
	mainMatrix[i][j] = 0;	
       }
       if(perc_2 >33 && perc_2 <= 66 && mainMatrix[i + 1][j]  == 0){
        mainMatrix[i + 1][j] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j + 1]  == 0){
        mainMatrix[i + 1][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
      return;
    }
    if (i == 11 &&  j == 20 -1 ) {
       if(perc_2 >= 0 && perc_2 <= 33 && mainMatrix[i][j - 1]  == 0){
        mainMatrix[i][j - 1] = mainMatrix[i][j];
	mainMatrix[i][j] = 0;	
       }
       if(perc_2 >33 && perc_2 <= 66 && mainMatrix[i - 1][j]  == 0){
	mainMatrix[i - 1][j] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i - 1][j -1 ]  == 0){
	mainMatrix[i - 1][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
      return;
    }
    if (i == 1 && j == 20 - 1) {
       if(perc_2 >= 0 && perc_2 <= 33 && mainMatrix[i][j -1]  == 0){
	mainMatrix[i][j - 1] = mainMatrix[i][j];
	mainMatrix[i][j] = 0;	
       }
       if(perc_2 >33 && perc_2 <= 66 && mainMatrix[i + 1][j]  == 0){
	mainMatrix[i + 1][j] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j - 1]  == 0){
	mainMatrix[i + 1][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
      return;       
    }
    if (i == 11 && j == 0) {
       if(perc_2 >= 0 && perc_2 <= 33  && mainMatrix[i][j + 1]  == 0){
	mainMatrix[i][j + 1] = mainMatrix[i][j];
	mainMatrix[i][j] = 0;	
       }
       if(perc_2 >33 && perc_2 <= 66 && mainMatrix[i - 1][j]  == 0){
	mainMatrix[i - 1][j] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i - 1][j + 1]  == 0){
	mainMatrix[i - 1][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
      return;        
    }
    if (i == 1  && ( j > 0 && j < 20-1)) {
       if(perc_2 >= 0 && perc_2 <= 20 && mainMatrix[i][j - 1]  == 0){
	mainMatrix[i][j - 1] = mainMatrix[i][j];
	mainMatrix[i][j] = 0;	
       }
       if(perc_2 >20 && perc_2 <= 40 && mainMatrix[i + 1][j]  == 0){
	mainMatrix[i][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j - 1]  == 0){
	mainMatrix[i + 1][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >20 && perc_2 <= 40  && mainMatrix[i][j + 1]  == 0){
	mainMatrix[i][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >66 && perc_2 <= 100 && mainMatrix[i + 1][j + 1]  == 0){
	mainMatrix[i + 1][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
      return;
    }
    if (i == 11 && (j > 0 && j < 20 - 1)) {
      if(perc_2 >= 0 && perc_2 <= 20 && mainMatrix[i - 1][j]  == 0){
	mainMatrix[i - 1][j] = mainMatrix[i][j];
	mainMatrix[i][j] = 0;	
       }
       if(perc_2 > 20 && perc_2 <= 40 && mainMatrix[i][j + 1]  == 0){
	mainMatrix[i][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 40 && perc_2 <= 60 && mainMatrix[i][j - 1]  == 0){
	mainMatrix[i][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 60 && perc_2 <= 80 && mainMatrix[i - 1][j - 1]  == 0){
	mainMatrix[i - 1][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 80 && perc_2 <= 100 && mainMatrix[i - 1][j - 1]  == 0){
	mainMatrix[i - 1][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
      return;
    }
    if (j == 0 && (i > 1 && i < 11)) {
       if(perc_2 >= 0 && perc_2 <= 20 && mainMatrix[i - 1][j]  == 0){
	mainMatrix[i][j - 1] = mainMatrix[i][j];
	mainMatrix[i][j] = 0;	
       }
       if(perc_2 >20 && perc_2 <= 40 && mainMatrix[i + 1][j]  == 0){
	mainMatrix[i + 1][j] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >40 && perc_2 <= 60 && mainMatrix[i][j + 1]  == 0){
	mainMatrix[i][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >60 && perc_2 <= 80 && mainMatrix[i + 1][j - 1]  == 0){
	mainMatrix[i + 1][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 >80 && perc_2 <= 100 && mainMatrix[i + 1][j + 1]  == 0){
	mainMatrix[i + 1][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
      return;        
    }
    else {
       if(perc_2 >= 0 && perc_2 <= 25 && mainMatrix[i - 1][j - 1]  == 0){
	mainMatrix[i - 1][j - 1] = mainMatrix[i][j];
	mainMatrix[i][j] = 0;	
       }
       if(perc_2 > 8 && perc_2 <= 15 && mainMatrix[i - 1][j]  == 0){
	mainMatrix[i - 1][j] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 25 && perc_2 <= 50 && mainMatrix[i - 1][j + 1]  == 0){
	mainMatrix[i - 1][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 22 && perc_2 <= 34 && mainMatrix[i][j - 1]  == 0){
	mainMatrix[i][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 34 && perc_2 <= 40 && mainMatrix[i][j + 1]  == 0){
	mainMatrix[i][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 50 && perc_2 <= 75 && mainMatrix[i + 1][j - 1]  == 0){
	mainMatrix[i + 1][j - 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 47 && perc_2 <= 58 && mainMatrix[i + 1][j]  == 0){
	mainMatrix[i + 1][j] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
       if(perc_2 > 75 && perc_2 <= 100 && mainMatrix[i + 1][j + 1]  == 0){
	mainMatrix[i + 1][j + 1] = mainMatrix[i][j];
        mainMatrix[i][j] = 0; 
       }
      return;
  }
}

void controlInfect(int i,int j,int** mainMatrix,bool endF){
 int perc = rand()%300;
  if(perc <= 1){
    mainMatrix[i][j] = 4;
    return;
  }
   if (i == 0 && j == 0) {
    if(mainMatrix[i][j+1] == 1 || mainMatrix[i + 1][j] == 1){
       mainMatrix[i][j] = 1;
    }
     return;
   }
   if (i == 20 - 1 &&  j == 20 - 1) {
    if(mainMatrix[i][j - 1] == 1 || mainMatrix[i - 1][j] == 1){
       mainMatrix[i][j] = 1;
    }
       return;
   }
   if (i == 0 && j == 20 - 1) {
     if(mainMatrix[i][j - 1] == 1 || mainMatrix[i + 1][j] == 1){
       mainMatrix[i][j] = 1;
    }
       return;
   }
   if (i == 20 - 1 && j == 0) {  
     if(mainMatrix[i][j+1] == 1 || mainMatrix[i - 1][j] == 1){
       mainMatrix[i][j] = 1;
    }
       return;
   }
   if (i == 0  && ( j > 0 && j < 20-1)) {
     if(mainMatrix[i][j+1] == 1 || mainMatrix[i + 1][j] == 1 || mainMatrix[i][j - 1] == 1) {
       mainMatrix[i][j] = 1;
    }
       return;
   }
   if (i == 20 - 1 && (j > 0 && j < 20 - 1)) {
     if(mainMatrix[i][j+1] == 1 || mainMatrix[i - 1][j] == 1 || mainMatrix[i][j - 1] == 1) {
       mainMatrix[i][j] = 1;
    }
       return;
   }
   if (j == 0 && (i > 0 && i < 20 - 1)) {
      if(mainMatrix[i][j+1] == 1 || mainMatrix[i + 1][j] == 1 || mainMatrix[i - 1][j] == 1){
       mainMatrix[i][j] = 1;
    }
       return;   
   }
   else{ 
  	if(mainMatrix[i-1][j] == 1 || mainMatrix[i][j-1] == 1 || mainMatrix[i+1][j] == 1 || mainMatrix[i][j+1] == 1){
		mainMatrix[i][j] = 1;
  	}
   }
 if(perc >= 0 &&  perc <= 10)
	   mainMatrix[i][j] = 1;
 return;  
}

void controlHealed(int i,int j,int** mainMatrix, int day,bool endF){
  int perc = rand()%100;
  int dayR = rand()%15;
	       if(mainMatrix[i][j] == 1 && day >= 10+dayR && perc > 70){
		   mainMatrix[i][j] = 3;
	       }
	       if(perc > 98 && perc <= 100){
		 mainMatrix[i][j] = 4;
 	       }
}


