//ARDUINO CODE FOR INVERTED PENDULUM
 #define encpin1a 2 //PIN A OF ENCODER MEASURING X POSITION(enc1)
 #define encpin1b 5 //PIN B OF ENCODER MEASURING X POSITION(enc1)
 #define encpin2a 3 //PIN A OF ENCODER MEASURING PENDULUM ANGLE(enc2)
 #define encpin2b 12 //PIN B OF ENCODER MEASURING PENDULUM ANGLE(enc2)
long counter1; //effective pulses of enc1
long counter2; //effective pulses of enc2
int direction1; //direction of motor +1/-1
unsigned long lasttime=0,lasttime1=0;
double Output=0, Setpoint=0,Output1=0, Setpoint1=0;
double errsum, lasterr, errsum1, lasterr1;
double kp1, ki1, kd1,kp2,ki2,kd2;
void updateEncoder();
void updateEncoder1();
void SetTunings0(double,double,double);
void setup() {
 Serial.begin (9600);
pinMode(encpin1a, INPUT_PULLUP); 
pinMode(encpin1b, INPUT_PULLUP);
pinMode(encpin2a, INPUT_PULLUP);
pinMode(encpin2b, INPUT_PULLUP);
attachInterrupt(0, updateEncoder, CHANGE); //whenever any change occurs in pulse of encoder updateEncoder func get called
attachInterrupt(1, updateEncoder1, CHANGE);//whenever any change occurs in pulse of encoder updateEncoder func get called
//Setpoint = 0;
// myPID.SetMode(AUTOMATIC);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
}

void loop(){ 
 
SetTunings0(38,0.01,39);

   Compute();
        
  
   
   


 Serial.println(constrain(Output, -255, 255));
  if(Output>0){
    direction1=1;
  digitalWrite(7,LOW);       
  digitalWrite(8,HIGH);      
  if(Output>255){
    Output=255; 
  }
  
  
  }
  if(Output<0){
    direction1=-1;
   Output=-Output;
digitalWrite(8,LOW);
 digitalWrite(7,HIGH);}
 if(Output>255){
  Output=255;
}
  analogWrite(9,Output);
 Serial.print(counter2);
 Serial.print("\t");
 Serial.print(counter1);
 Serial.print("\t");
 //Serial.println(direction1);
}



void updateEncoder(){ // counting no of pulses for enc1
 if(digitalRead(encpin1a)==HIGH){
    if(digitalRead(encpin1b)==LOW)  counter1++;
    else counter1--;
  }
  else{
    if(digitalRead(encpin1b)==LOW)  counter1--;
    else counter1++;
  }
  
}
void updateEncoder1(){ // counting no of pulses for enc2
if(digitalRead(encpin2a)==HIGH){
    if(digitalRead(encpin2b)==LOW)  counter2++;
    else counter2--;
  }
  else{
    if(digitalRead(encpin2b)==LOW)  counter2--;
   else counter2++;
  }
  //Seri0al.print(" c2 ");
  //Serial.println(counter2);
  }
   
void Compute()
{
   
  // unsigned long now = millis();
  // double timeChange = (double)(now - lasttime);
if(counter1>2900){ // for avoiding the pendulum move at end point
  Setpoint=-2;  
}
else if(counter1<-2900){
  Setpoint=2;
}
else{Setpoint=0;}
  
  /*if(counter1>=3052) Setpoint=counter2;
  if(counter1<=-3052) Setpoint=-counter2;*/
  //PID CODE
   double error = Setpoint - counter2;
   errsum += error;
   double dErr = (error - lasterr);
  
//   Serial.println(error);
/*if(abs(counter2)<=4) {
  errsum=0;
  if(error>0){ f=0;}
  if(error<0) {f=2;}
  }*/ 

   Output = kp1 * error + ki1 * errsum + kd1 * dErr;
  Serial.print(kp1*error);
  Serial.print("\t");
  Serial.print(kd1*dErr);
  Serial.print("\t");
  Serial.println(ki1*errsum);
   
   lasterr = error;
  // lasttime = now;
}
//THIS FUNCTION void compute1() is for applying PID to the X coordinate of cart position.I HAVENOT USED THIS BUT IT CAN BE USED FOR EFFECTIVE CONTROL 
//  void compute1(){
//     unsigned long now1 = millis();
//   double timeChange1 = (double)(now1 - lasttime1);
//  
//  
//   double error1 = Setpoint1 - counter1;
//   errsum1 += (error1 * timeChange1);
//    double dErr1 = (error1 - lasterr1) / timeChange1;
//  
////   Serial.println(error);
//   Output1 = kp2 * error1 + ki2 * errsum1 + kd2 * dErr1;
//  //Serial.println(kp1);
//  //Serial.println(kd1);
//  //Serial.println(Output1);
//   
//   lasterr1 = error1;
//   lasttime1 = now1;
   
 // }
  
void SetTunings0(double Kp, double Ki, double Kd) //setting kp ki and kd PID ON PENDULUM ANGLE
{
   kp1 = Kp;
   ki1 = Ki;
   kd1 = Kd;
}

void SetTunings1(double Kp, double Ki, double Kd) //setting kp ki and kd PID on X COORDINATE
{
   kp2 = Kp;
   ki2 = Ki;
   kd2 = Kd;
}
