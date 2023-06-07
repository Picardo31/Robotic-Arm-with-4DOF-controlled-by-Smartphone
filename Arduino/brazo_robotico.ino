#include <BasicLinearAlgebra.h>
#include <ElementStorage.h>
#include "motores_servo.h"
#include "fkine.h"
#include "ikine.h"

using namespace BLA;

String s;
char c[360];
int mode;
int vel=10;
long t1,t2;
float x,y,z;

BLA::Matrix<5,1> q={90,90,90,90,90};
BLA::Matrix<4,4> T;

void setup() {
  Serial.begin(9600);
  setup_servos(q(0),q(1),q(2),q(3),q(4),180); 
  T=fkine(q);
  x=T(0,3);
  y=T(1,3);
  z=T(2,3);
  delay(1000);
}

void loop() {

}

void fkine_mode()
{
  String pos;
  for(int i=s.length();i>=0;i--)
  {
    if(s[i]=='B')
    {
      pos=s.substring(i+1,s.length());
      base(pos.toInt());
      move_servos(vel);
      break;
    }
    else if(s[i]=='A')
    {
      pos=s.substring(i+1,s.length());
      arm(pos.toInt());
      move_servos(vel);
      break;
    }
    else if(s[i]=='E')
    {
      pos=s.substring(i+1,s.length());
      elbow(pos.toInt());
      move_servos(vel);
      break;
    }
    else if(s[i]=='F')
    {
      pos=s.substring(i+1,s.length());
      forearm(pos.toInt());
      move_servos(vel);
      break;
    }
    else if(s[i]=='W')
    {
      pos=s.substring(i+1,s.length());
      wrist(pos.toInt());
      move_servos(vel);
      break;
    }
  }
}
void ikine_mode()
{
  String pos;
  bool enable=false;
  for(int i=0;i<s.length();i++)
  {
    if(s[i]=='X')
    {
      enable=true;
      pos=s.substring(i+1,s.length());
      x=pos.toFloat();
    }
    if(s[i]=='Y')
    {
      enable=true;
      pos=s.substring(i+1,s.length());
      y=pos.toFloat();
    }
    if(s[i]=='Z')
    {
      enable=true;
      pos=s.substring(i+1,s.length());
      z=pos.toFloat();
    }
  }  
  if(enable)
  {
    set_angles(ikine(x,y,z));
    move_servos(vel);
    enable=false;
  }
}

bool charIsEqual(char c_in[],String s_in)
{
  bool comp=true;

  for(int i=0;i<s_in.length();i++)
  {  
    if((c_in[i]!=s_in[i])||(c[s_in.length()]!=' '))
    {
      comp=false;
      break;
    }
  }
  return comp;
}
void serialEvent()
{
  String sp,pos;
  if(Serial.available() > 0)
  { 
    s=Serial.readString();
    for(int i=0;i<s.length()-1;i++)
    {
      c[i+2]=' ';
      c[i+1]=' ';
      c[i]=s[i];
    }
  }
  if(charIsEqual(c,"fkine"))
  {
    mode=1;
  }
  else if(charIsEqual(c,"ikine"))
  {
    mode=2;
  }

  for(int i=s.length();i>=0;i--)
  {
    if(s[i]=='S')
    {
      sp=s.substring(i+1,s.length());
      vel=sp.toInt();
      break;
    }
    if(s[i]=='G')
    {
      pos=s.substring(i+1,s.length());
      gripper(pos.toInt());
      move_servos(vel);
      break;
    }
  }
  
  switch(mode)
  {
    case 1:
    fkine_mode();
    break;
    case 2:
    ikine_mode();
    break;
    default:
    break;
  }
  q=get_angles();
  T=fkine(q);
  x=T(0,3);
  y=T(1,3);
  z=T(2,3);
  Serial<<x<<"I"<<y<<"I"<<z<<"I"<<q(4)<<"I"<<q(3)<<"I"<<q(2)<<"I"<<q(1)<<"I"<<q(0)<<'\n';

   
}
