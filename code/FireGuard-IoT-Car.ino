/*
 FireGuard IoT Car
 ESP32 Fire Fighting Robot

 Description:
 This program controls a fire-fighting robot using an ESP32 microcontroller.
 The robot detects flames using sensors and extinguishes fire using a water pump.

 Features:
 - Flame detection
 - Autonomous movement
 - Servo-controlled water spraying
 - Motor control using L298N
 - Automatic fire extinguishing

 Hardware:
 - ESP32
 - Flame Sensors
 - L298N Motor Driver
 - DC Motors
 - Servo Motor
 - Water Pump
*/

// =========================
// Pin Mapping
// =========================

#define min_val 25

// Motor Driver L298N
#define enA 14
#define in1 26
#define in2 27
#define in3 33
#define in4 25
#define enB 15

// Flame Sensors
#define ir_R 34
#define ir_F 35
#define ir_L 32

// Servo and Pump
#define servo 18
#define pump 19


// =========================
// PWM Settings
// =========================

const int Speed = 160;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int pwmFreq = 1000;
const int pwmResBits = 8;

int s1, s2, s3;


// =========================
// Pump Functions
// =========================

void pump_on() {
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);
}

void pump_off() {
  pinMode(pump, INPUT_PULLUP);
  digitalWrite(pump, HIGH);
}


// =========================
// Setup
// =========================

void setup() {

  Serial.begin(9600);

  pinMode(ir_R, INPUT);
  pinMode(ir_F, INPUT);
  pinMode(ir_L, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(servo, OUTPUT);

  pump_off();

  ledcSetup(pwmChannelA, pwmFreq, pwmResBits);
  ledcSetup(pwmChannelB, pwmFreq, pwmResBits);

  ledcAttachPin(enA, pwmChannelA);
  ledcAttachPin(enB, pwmChannelB);

  ledcWrite(pwmChannelA, Speed);
  ledcWrite(pwmChannelB, Speed);

  delay(500);
}


// =========================
// Main Loop
// =========================

void loop() {

  s1 = analogRead(ir_R);
  s2 = analogRead(ir_F);
  s3 = analogRead(ir_L);

  Serial.print(s1);
  Serial.print("\t");
  Serial.print(s2);
  Serial.print("\t");
  Serial.println(s3);

  delay(50);


  if (s1 < min_val) {

    Stop();
    pump_on();

    for (int angle = 90; angle >= 40; angle -= 3)
      servoPulse(servo, angle);

    for (int angle = 40; angle <= 90; angle += 3)
      servoPulse(servo, angle);
  }


  else if (s2 < min_val) {

    Stop();
    pump_on();

    for (int angle = 90; angle <= 140; angle += 3)
      servoPulse(servo, angle);

    for (int angle = 140; angle >= 40; angle -= 3)
      servoPulse(servo, angle);

    for (int angle = 40; angle <= 90; angle += 3)
      servoPulse(servo, angle);
  }


  else if (s3 < min_val) {

    Stop();
    pump_on();

    for (int angle = 90; angle <= 140; angle += 3)
      servoPulse(servo, angle);

    for (int angle = 140; angle >= 90; angle -= 3)
      servoPulse(servo, angle);
  }


  else if (s1 >= min_val && s1 <= 4000) {

    pump_off();
    backword();
    delay(100);
    turnRight();
    delay(200);
  }


  else if (s2 >= min_val && s2 <= 4000) {

    pump_off();
    forword();
  }


  else if (s3 >= min_val && s3 <= 4000) {

    pump_off();
    backword();
    delay(100);
    turnLeft();
    delay(200);
  }


  else {

    pump_off();
    Stop();
  }

  delay(10);
}


// =========================
// Servo Function
// =========================

void servoPulse(int pin, int angle) {

  int pwm = (angle * 11) + 500;

  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);

  delay(50);
}


// =========================
// Motor Functions
// =========================

void forword() {

  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}


void backword() {

  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}


void turnLeft() {

  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}


void turnRight() {

  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}


void Stop() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
