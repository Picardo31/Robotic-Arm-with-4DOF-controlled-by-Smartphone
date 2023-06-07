#include <BasicLinearAlgebra.h>
#include <ElementStorage.h>
#include <Arduino.h>4


using namespace BLA;

float L0=9.5;
float L1=3;
float L2=15;
float L3=1;
float L4=12;
float L5=9;


BLA::Matrix<4,4> B;
BLA::Matrix<4,4> A;
BLA::Matrix<4,4> E;
BLA::Matrix<4,4> F;
BLA::Matrix<4,4> W;
BLA::Matrix<4,4> G;
BLA::Matrix<4,4> GrelB;

BLA::Matrix<6,1> get_Links()
{
  BLA::Matrix<6,1> Links={L0,L1,L2,L3,L4,L5};
  return Links;
}
float deg2rad(int deg)
{
  return deg*PI/180.0;
}
float rad2deg(int rad)
{
  return rad*180.0/PI;
}
BLA::Matrix<4,4> fkine(BLA::Matrix<5,1>Q)//(int Q1,int Q2, int Q3, int Q4, int Q5)
{
  
  float q1=deg2rad(Q(0));
  float q2=deg2rad(Q(1));
  float q3=deg2rad(Q(2));
  float q4=deg2rad(Q(3));
  float q5=deg2rad(Q(4));

  
  B={cos(q1),-sin(q1),0,0,sin(q1),cos(q1),0,0,0,0,1,L0,0,0,0,1};
  A={cos(q2),-sin(q2),0,-L1,0,0,-1,0,sin(q2),cos(q2),0,0,0,0,0,1};
  E={cos(q3-PI/2),-sin(q3-PI/2),0,L2,sin(q3-PI/2),cos(q3-PI/2),0,0,0,0,1,0,0,0,0,1};
  F={cos(q4-PI/2),-sin(q4-PI/2),0,L3,0,0,-1,-L4,sin(q4-PI/2),cos(q4-PI/2),0,0,0,0,0,1};
  W={cos(q5),-sin(q5),0,0,0,0,-1,0,sin(q5),cos(q5),0,0,0,0,0,1};
  G={1,0,0,L5,0,1,0,0,0,0,1,0,0,0,0,1};
  GrelB=B*A*E*F*W*G;
  
  return GrelB;
}
