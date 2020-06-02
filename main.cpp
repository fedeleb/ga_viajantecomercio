#include <stdlib.h>
#include "ga/ga.h"
#include <iostream.h>
#include "distancia.h"

//Parametros del problema
const int N=100; //Total de ciudades
//Cabecera de funciones
void Iniciar(GAGenome &);
float FObjetivo(GAGenome&);
float Distancia(GAGenome&);
int Mutador(GAGenome& , float);

int main(void)
{
  int i=0;
  int mx=0; 
  //Parametros del AG
  int poblacion  = 100;//Nro Individuos
  int generaciones = 5000;//Cant Generaciones
  float probabilidad_mutacion = 0.05;//P.Mutacion
  float probabilidad_crossover = 0.7;//P.Crossover

  GA1DArrayGenome<int> genome(N, FObjetivo);//Genoma reprecentado por un arreglo

  cargar();//Carga las ciudades
  genome.initializer(Iniciar);//Func Inicializacion
  genome.mutator(Mutador);//Func Mutacion
  cout<<"Problema del Viajante de Comercio";
  cout<<"\n";
  cout<<"Aplicando algoritmo genetico...";

  GASimpleGA ag(genome);//Tipo de algoritmo genetico simple
  GALinearScaling scale;//Se utiliza para hacer escala en una población cuando se evalúa un genoma
  GATournamentSelector select;//Operacion de seleccion por torneo. 
  ag.crossover(GA1DArrayGenome<int>::OrderCrossover);//Operador de crossover Order Crossover
  //Seteos
  ag.scaling(scale);
  ag.selector(select);
  ag.populationSize(poblacion);
  ag.pCrossover(probabilidad_crossover);
  ag.pMutation(probabilidad_mutacion);
  ag.nGenerations(generaciones);
  ag.minimize();
  ag.evolve();
  //Mejor Individuo
  genome=ag.statistics().bestIndividual();
  //Compruebo que el camino no tiene ciudades repetidas
  int a=0;
  for(i=0;i<N;i++)
  	a+=i;
  int b=0;
  for(i=0;i<N;i++)
  	b+=genome.gene(i);
  if(b!=a)
  	cout<<"\n"<<"Se encontraron ciudades repetidas";
  cout<<"\n";
  cout<<"\nCiudad Inicial:  "<<genome.gene(0);
  cout<<"\n";
  cout<<"\nCamino Recorrido:  "<< genome<<genome.gene(0);
  //cout<<"\nCamino Recorrido:  "<< best;
  cout<<"\n";
  cout<<"\nDistancia Total: "<<Distancia(genome);
  //cout<<"\nDistancia Total: "<<best.score();
  cout<<"\n";

  getchar();
  return(mx);
}
int Mutador(GAGenome& g, float pmut) {
  GA1DArrayGenome<int> &Individuo = (GA1DArrayGenome<int> &)g;
  int i,P=0,aux1=0,aux2=0,aux3=0,aux4=0,aux5=0,aux6=0;
  float DistanciaA=0,DistanciaB=0,TotalD1=0,TotalD2=0;
  if(GARandomFloat()>=pmut)
  	return 0;
  if(Distancia(g)>30000)
  {
  	 	if(GARandomFloat()<=pmut)
		 {
		 	for(i=0; i < N;i++)
				Individuo.swap(i,GARandomInt(0,N-1));
  					return 1;  
		 }
		for(i=1;i<=N-2;i++)
		{
			P=GARandomInt(1,N-2);
		 	aux1=Individuo.gene(i-1);
			aux2=Individuo.gene(i);
			aux3=Individuo.gene(i+1);
			DistanciaA=dista_de[aux1][aux2]+dista_de[aux2][aux3];//Distancia actual sin mutar	
			aux4=Individuo.gene(P-1);
			aux5=Individuo.gene(P);
			aux6=Individuo.gene(P+1);
			DistanciaB=dista_de[aux4][aux5]+dista_de[aux5][aux6];//Distancia actual sin mutar
			//Distancia Total Actual
			TotalD1=DistanciaA+DistanciaB;
			//Distancia Total Si cambio
			DistanciaA=dista_de[aux1][aux5]+dista_de[aux5][aux3];//Distancia actual con mutacion
			DistanciaB=dista_de[aux4][aux2]+dista_de[aux2][aux6];//Distancia actual con mutacion
			TotalD2=DistanciaA+DistanciaB;     
			//Si la menor distancia es menor usando P la cambio, de lo contrario no.
			if(TotalD1>TotalD2)
				  	Individuo.swap(i,P);
	  	}
	  }	 
  return (1);
}
float Distancia(GAGenome & g ){
 int i;
 GA1DArrayGenome<int> & I = (GA1DArrayGenome<int> &)g;
 float score=0.0;
 for(i=0;i< N-1;i++) //Hasta el Penultimo N-2
    score+=dista_de[I.gene(i)][I.gene(i+1)];//N-2 -> N-1
 score+=dista_de[I.gene(N-1)][I.gene(0)]; //Se agrega la distancia entre la última ciudad y la inicial N-1 -> 0
 return score;
}
float FObjetivo(GAGenome & g ){
 return Distancia(g);
}
void Iniciar(GAGenome & g){
    int i,P1 = 0;
    GA1DArrayGenome<int> & Individuo = (GA1DArrayGenome<int> &)g;
    for(i=0; i < N;i++)
        Individuo.gene(i,i);//Genera el primer genoma al desde 0 hasta N-1

    for(i=0;i<N;i++)
    {
      P1 = GARandomInt(0,N-1);//Posicion aleatoria para intercambio 
      Individuo.swap(i,P1);
    }             
}
