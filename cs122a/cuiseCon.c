#include "rims.h"

volatile char TimerFlag = 0;

/*
https://docs.google.com/document/d/1K8yCnOUe-fGAMrTA2PDLsZhLsv4IrYGsWc0boLBwiLU/edit?usp=sharing
https://youtu.be/5HYFO-ivb-g

*/
void TimerISR(){
   TimerFlag = 1;
}


//PID variables
int Desired = 0;
int Actual =0;
int Integ =0;
int Error = 0;
int ActualPrev =0;
int Actuator =0;
int ActuatorPrev =0;
int actuatorMax = 100;
int actuatorMin = -100;
int integMax = 500;
int integMin = -500;
int Deriv;
int p = 50;
int i = 1;
int d = 2500;

void PID(){
    
    if(A5 == 0){
      Desired = ((A& 01)*5);
      if(A1 == 1){Desired += 10;}
      if(A2 == 1){Desired += 20;}
      if(A3 == 1){Desired += 40;}
      if(A4 == 1){Desired += 80;}
    }
    else{
      if(A6 == 1){Desired += 1;}
      if(A7 == 1){Desired -= 1;}
    }
    if(Desired > 100){Desired = 100;}
    if(Desired < 0){Desired = 0;}
    
    Error = (Desired) - (Actual);
    Deriv = (Actual) - (ActualPrev);
    Integ += Error;
    if(Integ > integMax){
        Integ=integMax;
    }
    if(Integ < integMin){
        Integ=integMin;
    }
    Actuator = p*Error + i*Integ - d*Deriv;
    if((Actuator < 0 || Actuator -ActuatorPrev <0) && Actual > Desired){
        if(Actuator > -75){
            Actuator = -75;
        }
    }
    //printf("Actuator = %d\n",Actuator);
    ActuatorPrev = Actuator; 
    Actuator= Actuator/75;
    
    if(Actuator < actuatorMin){
        Actuator = actuatorMin;
    }
    if(Actuator > actuatorMax){
        Actuator = actuatorMax;
    }
   
   //printf("Actuator = %d\n",Actuator);
   //printf("Actual = %d\n",Actual);
   printf("Desired = %d\n",Desired);
   
   if(Actuator > 4){
       if(Actual + 4 > Desired){
           Actual = Desired;
       }
       Actuator = 4;
   }
   else if(Actuator < -4){
       if(Actual - 4 < Desired){
           Actual = Desired;
       }
       Actuator = -4;
   }
   
   if(Actual != Desired){
       if(Actuator > 0 && Actual < Desired){
         Actual += Actuator;
       }else if(Actuator < 0 && Actual > Desired){
           Actual += Actuator;
       }
   }
   else{Actuator == 0;}
   
   if(Actual > 100){
       Actual=100;
   }else if(Actual <= 0){
       Actual = 0;
   }
    B=Actual;
    ActualPrev = Actual;
    
}

void main()
{
int timer = 10000;
   int period = 1000;
   
   TimerSet(period);
   TimerOn();
   while (A != 0xFF) {
      if(timer >= 1000){
      PID();
      timer =0;
      }
      timer += 5;
   }

}
