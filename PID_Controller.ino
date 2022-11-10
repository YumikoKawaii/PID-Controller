#define PWM 5
#define MOTOR_N 7
#define MOTOR_P 6

#define ENCA 2
#define ENCB 3

volatile int currP = 0; // Current Position
long prevT = 0; //Previous Time 
float prevE = 0; // Previous Error
float inteE = 0; // Error Integral

// PID constant
float kP = 1;
float kI = 0;
float kD = 0.025;

void setup()
{
  Serial.begin(9600);
  
  pinMode(PWM, OUTPUT);
  pinMode(MOTOR_N, OUTPUT);
  pinMode(MOTOR_P, OUTPUT);
  
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
}

void loop()
{
  // Desired Position
  int target = 250*sin(prevT/1e6);
  
  long currT = micros(); //Current Time in microseconds
  float deltaT = (float) (currT - prevT)/1e6; // Time difference in seconds
  prevT = currT;
  
  int pos = 0;
  noInterrupts(); // Disable interrupts temporarily while reading encoder
  pos = currP;
  interrupts();
  
  int e = target - pos;
  
  // Derivative Error
  float deriE = (e - prevE)/deltaT;
  
  // Integral Error
  float inteE = inteE + e*deltaT;
  
  // Speed 
  float u = kP*e + kI*inteE + kD*deriE;
  
  // PWM width
  float w = fabs(u) > 255?255:fabs(u);
  
  // Rotation Direction
  int d = u > 0?1:-1;
  
  setMotor(d, w);
  
  prevE = e;
}

void setMotor(int dir, int width)
{
  analogWrite(PWM, width);
  if (dir == 1){
    digitalWrite(MOTOR_N, LOW);
    digitalWrite(MOTOR_P, HIGH);
  }
  else {
    digitalWrite(MOTOR_N, HIGH);
    digitalWrite(MOTOR_P, LOW);
  }
}

void readEncoder()
{
  if (digitalRead(ENCB) > 0){
    currP++;
  }
  else currP--;
}
