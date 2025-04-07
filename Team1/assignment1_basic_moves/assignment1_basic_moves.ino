//left motor 
#define LEFT_FWD 6
#define LEFT_BCKWD 10

//right motor
#define RIGHT_FWD 5
#define RIGHT_BCKWD 9

//store the next time

unsigned long _timer = 0;

//set the interval at 0.3 seconds
int _interval = 300; 

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_FWD , OUTPUT);
  pinMode(LEFT_BCKWD , OUTPUT);
  pinMode(RIGHT_FWD , OUTPUT);
  pinMode(RIGHT_BCKWD , OUTPUT);
  forword();
}

void loop() {
  if(millis() < 4000)
  {
    forword();
  }
  else if(millis() < 6000)
 {
  backwords();
 } 
 else if(millis() < 6500)
 {
  left();
  
 }
 else if(millis() < 10000)
 {
  forword();
 }
 else if(millis() < 10500)
 {
  right();
 }
 else if(millis() < 12500)
 {
  forword();
 }
else
stop();

}

void forword()
{
  analogWrite(LEFT_FWD , 225 );
  analogWrite(LEFT_BCKWD , 0 );
  analogWrite(RIGHT_FWD , 250 );
  analogWrite(RIGHT_BCKWD , 0 );
}

void backwords()
{
  analogWrite(LEFT_BCKWD, 225 );
  analogWrite(LEFT_FWD , 0 );
  analogWrite(RIGHT_BCKWD , 250 );
  analogWrite(RIGHT_FWD , 0 );
}

void right()
{
    analogWrite(LEFT_FWD , 240 );
    analogWrite(LEFT_BCKWD , 0 );
    analogWrite(RIGHT_FWD , 0 );
    analogWrite(RIGHT_BCKWD , 250 );
}

void left()
{
    analogWrite(LEFT_FWD , 0 );
    analogWrite(LEFT_BCKWD , 250 );
    analogWrite(RIGHT_FWD , 250 );
    analogWrite(RIGHT_BCKWD , 0 ); 
}

void stop()
{
  analogWrite(LEFT_FWD , 0 );
  analogWrite(LEFT_BCKWD , 0 );
  analogWrite(RIGHT_FWD , 0 );
  analogWrite(RIGHT_BCKWD , 0 );
}
