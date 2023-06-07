#include <Servo.h>
#include <Arduino.h>
#include <BasicLinearAlgebra.h>
#include <ElementStorage.h>
#include "fkine.h"

using namespace BLA;
Servo servo[6];
int ErelA,ErelB,ArelB;
int d_b,d_a,d_e,d_f,d_w,d_g,p_b,p_a,p_e,p_f,p_w,p_g;
int d_base,d_arm,d_elbow,d_forearm,d_wrist;


void move_servos(int s)
{
  int ib,ia,ie,iF,iw,ig;

  pinMode(2,INPUT_PULLUP);
   
  ib=p_b;

  ia=p_a;

  ie=p_e;

  iF=p_f;

  iw=p_w;

  ig=p_g;
  
  for(int i=0;i<=180;i++)
  {
    
    //**************base******************
    if((d_b>p_b)&&(ib<=d_b))
    {
      servo[0].write(ib);
      ib++;
    }
    else if((d_b<p_b)&&(ib>=d_b))
    {
      servo[0].write(ib);
      ib--;
    }

    //**************arm*******************
    if((d_a>p_a)&&(ia<=d_a))
    {
      servo[1].write(ia);
      ia++;
    }
    else if((d_a<p_a)&&(ia>=d_a))
    {
      servo[1].write(ia);
      ia--;
    }
    
    //*************elbow******************
    if((d_e>p_e)&&(ie<=d_e))
    {
      servo[2].write(map(ie,0,180,180,0));
      ie++;
    }
    else if((d_e<p_e)&&(ie>=d_e))
    {
      servo[2].write(map(ie,0,180,180,0));
      ie--;
    }
    
    //***********forearm*****************
    if((d_f>p_f)&&(iF<=d_f))
    {
      servo[3].write(iF);
      iF++;
    }
    else if((d_f<p_f)&&(iF>=d_f))
    {
      servo[3].write(iF);
      iF--;
    }
    
    //************wrist*****************
    if((d_w>p_w)&&(iw<=d_w))
    {
      servo[4].write(iw);
      iw++;
    }
    else if((d_w<p_w)&&(iw>=d_w))
    {
      servo[4].write(iw);
      iw--;
    }

    //************gripper***************
    
    if((d_g>p_g)&&(ig<=d_g))
    {
      servo[5].write(map(ig,0,180,180,0));
      ig++;
    }
    else if((d_g<p_g)&&(ig>=d_g))
    {
      if(digitalRead(2)==LOW)
      {
        servo[5].write(map(ig,0,180,180,0));
        d_g=ig;
        break;
      }
      else
      {
        servo[5].write(map(ig,0,180,180,0));
        ig--;
      }      
    }

    //**********stop*******************
    if((ib==d_b)&&(ia==d_a)&&(ie==d_e)&&(iF==d_f)&&(iw==d_w)&&(ig==d_g))
    {
      break;
    }
    delay(s);
  }
  p_b=d_b;
  p_a=d_a;
  p_e=d_e;
  p_f=d_f;
  p_w=d_w;
  p_g=d_g;
}

int base(int d)
{ 
//  if(d<30)
//  {
//    d=30;
//  }
//  else if(d>150)
//  {
//    d=150;
//  }
  d_base=d;
  d_b=d;
  return d_base;
}
int arm(int d)
{
  if(d<30)
  {
    d=30;
  }
  else if(d>160)
  {
    d=160;
  }
  if(d+d_elbow>230)
  {
    d=230-d_elbow;
    d_elbow=230-d;
  }
  d_a=d-20;
  ArelB=d_a;
  ErelB=ArelB+ErelA;
  d_e=ErelB;
  d_arm=d;
  return d_arm;
}
int elbow(int d)
{
  if(d<30)
  {
    d=30;
  }
  else if(d>140)
  {
    d=140;
  }
  if(d+d_arm>230)
  {
    d=230-d_arm;
    d_arm=230-d;
  }  
  d_elbow=d;
  ErelA=d-73;
  ErelB=ArelB+ErelA;
  d_e=ErelB;
  return d_elbow;
}
int forearm(int d)
{
  d_f=d-5;
  d_forearm=d;
  return d_forearm;
}
int wrist(int d)
{
  d_w=d+5;
  d_wrist=d;
  return d_wrist;
}
int gripper(int d)
{
  if(d<65)
  {
    d=65;
  }
  else if(d>170)
  {
    d=170;
  }
  d_g=d;
  return d_g;
}
BLA::Matrix<5,1> get_angles()
{
  BLA::Matrix<5,1> angles={d_base,d_arm,d_elbow,d_forearm,d_wrist};
  return angles;
}
void set_angles(BLA::Matrix<5,1> angles)
{
  base(angles(0));
  arm(angles(1));
  elbow(angles(2));
  forearm(angles(3));
  wrist(angles(4));
}
void setup_servos(int b,int a, int e,int f,int w,int g)
{
  d_base=b;
  d_arm=a;
  d_elbow=e;
  d_forearm=f;
  d_wrist=w;
  base(b);
  arm(a);
  elbow(e);
  forearm(f);
  wrist(w);
  gripper(g);
  move_servos(1);
  servo[0].attach(3);
  servo[1].attach(5);
  servo[2].attach(6);
  servo[3].attach(9);
  servo[4].attach(10);
  servo[5].attach(11);
  
}
