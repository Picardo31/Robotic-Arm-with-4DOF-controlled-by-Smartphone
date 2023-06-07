#include <Arduino.h>
#include <BasicLinearAlgebra.h>
#include <ElementStorage.h>
#include "fkine.h"

using namespace BLA;

BLA::Matrix<5,1> ikine(float x, float y, float z)
{
  
  int d_forearm=90; 
  int j;
  int cont=0;

  float er=1;
  float Min[4];
  float spx[4];
  float spy[4];
  float spz[4];
  
  BLA::Matrix<4,4> T;
  BLA::Matrix<5,1> q{0,0,0,d_forearm,0};
  BLA::Matrix<4,1> sQ1={0,0,0,0};
  BLA::Matrix<4,1> sQ2={0,0,0,0};
  BLA::Matrix<4,1> sQ3={0,0,0,0};
  BLA::Matrix<4,1> sQ5={0,0,0,0};
  BLA::Matrix<6,1> Links=get_Links();
  
  while(1)
  {
    for(int i=1;i<4;i++)
    { 
      sQ1(i)=random(0,181);
      sQ5(i)=random(0,181);
      while(1)
      {      
        sQ2(i)=random(30,161);
        sQ3(i)=random(30,141); 
        if(sQ2(i)+sQ3(i)<=230)
        {
          break;
        }
      }    
    }      
    for(int i=0;i<4;i++)
    {
      q(0)=sQ1(i);
      q(1)=sQ2(i);
      q(2)=sQ3(i);
      q(4)=sQ5(i);
      T=fkine(q);
      spx[i]=T(0,3);
      spy[i]=T(1,3);
      spz[i]=T(2,3);
    }
    for(int i=0;i<4;i++)
    {
      Min[i]=abs(x-spx[i])+abs(y-spy[i])+abs(z-spz[i]);
    }
    j=0;
    for(int i=1;i<4;i++)
    {
      if(Min[i]<Min[j])
      {
        j=i;
      }
    }
  
    sQ1(0)=sQ1(j);
    sQ2(0)=sQ2(j);
    sQ3(0)=sQ3(j);
    sQ5(0)=sQ5(j);
    
    if((abs(x-spx[0])<=er) && (abs(y-spy[0])<=er) && (abs(z-spz[0])<=er))
    {
      break;
    }
    else if(cont>1000)
    {
      break;
    }
    cont++;
  }

  q(0)=sQ1(0);
  q(1)=sQ2(0);
  q(2)=sQ3(0);
  q(4)=sQ5(0);
  return q;
}
