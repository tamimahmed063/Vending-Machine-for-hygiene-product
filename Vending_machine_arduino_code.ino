//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//Arduino code



#include <Adafruit_Fingerprint.h>

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)

SoftwareSerial mySerial(2, 3);

#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display



//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(10, 11);

//Initialisation of DHT11 Sensor

int id;
String product;
int value;



//const int ledPin = 13;// We will use the internal LED
const int buttonPin = 12;// the pin our push button is on
const int buttonPin1 = 13;// the pin our push button is on

int digitalVal;
int digitalVal1;

int motorA_IN1 = 6;// motor a
int motorA_IN2 = 7;//motor a


int motorB_IN3 = 8;// motor b
int motorB_IN4 = 9; //motor b
int rot1 = 15;
int rot2 = 10;


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  Serial.println("Program started");

  Serial.begin(9600);
  nodemcu.begin(9600);
  delay(1000);

  Serial.println("node mcu");


  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);

  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorB_IN3, OUTPUT);
  pinMode(motorB_IN4, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP); // Set the Tilt Switch as an input
  pinMode(buttonPin1, INPUT_PULLUP); // Set the Tilt Switch as an input

  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vending Machine");

  Serial.println("lcd on");


  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprintln sensor!");
  } else {
    Serial.println("Did not find fingerprintln sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.println("Sensor contains "); Serial.println(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");



  Serial.println("Program started");

}

void loop()                     // run over and over again
{
  Serial.println("loop on");
  int x;
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();


  Serial.println("Please scan the fingerprintln to verify yourself");
  x = getFingerprintIDez();

  Serial.println(x);
  delay(50);            //don't ned to run this at full speed.

  digitalWrite(11, HIGH);
  digitalWrite(buttonPin, HIGH);
  digitalVal = digitalRead(buttonPin); // Take a reading

  digitalWrite(12, HIGH);
  digitalWrite(buttonPin1, HIGH);
  digitalVal1 = digitalRead(buttonPin1); // Take a reading

  if (x == 8)
  {
    id = 63;
    lcd.clear();
    lcd.println("Welcome 1706063");
  }
  if (x == 9)
  {
    id = 49;
    lcd.clear();
    lcd.println("Welcome 1706049");
  }
  if (x == 7)
  {
    id = 49;
    lcd.clear();
    lcd.println("Welcome 1706049");
  }





  while (x > 0)
  {

    digitalVal = digitalRead(buttonPin);
    digitalVal1 = digitalRead(buttonPin1);
    if (digitalVal == LOW)
    {
      //digitalWrite(ledPin, HIGH); //Turn the LED on
      Serial.println(digitalVal);
      //digitalWrite(ledPin, HIGH); //Turn the LED on

      lcd.clear();
      lcd.println("Product A is selected");
      Serial.println("Product A is selected  ");
      product = "A";
      value = 100;
      delay(100);

      delay(5000);
      int k = 0;
      while (k < rot1) {
        //analogWrite(motorA_PWM, spd);
        digitalWrite(motorA_IN1, LOW);
        digitalWrite(motorA_IN2, HIGH);

        delay(50);
        digitalWrite(motorA_IN1, LOW);
        digitalWrite(motorA_IN2, LOW);
        delay(50);
        k = k + 1;
      }
      Serial.println(" Motor turned on");

      lcd.clear();
      lcd.println("Product deliverd");
      lcd.clear();
      delay(50);
      lcd.println("Vending Machine");
      delay(1000);
      Serial.println(" Verified. Product will be down in a short time");
      break;
    }

    if (digitalVal1 == LOW)
    {
      //digitalWrite(ledPin, HIGH); //Turn the LED on
      Serial.println(digitalVal1);
      //digitalWrite(ledPin, HIGH); //Turn the LED on
      lcd.clear();
      lcd.println("Product B is selected");
      Serial.println("Product B is selected  ");
      delay(100);
      Serial.println("Product B is selected  ");

      product = "B";
      value = 150;

      delay(5000);
      int k = 0;
      while (k < rot2) {
        //analogWrite(motorA_PWM, spd);
        digitalWrite(motorB_IN3, LOW);
        digitalWrite(motorB_IN4, HIGH);

        delay(50);
        digitalWrite(motorB_IN3, LOW);
        digitalWrite(motorB_IN4, LOW);
        delay(50);
        k = k + 1;
      }
      lcd.clear();
      lcd.println("Product deliverd");
      lcd.clear();
      delay(50);
      lcd.println("Vending Machine");
      Serial.println(" Verified. Product will be down in a short time");

      break;
    }



  }
  if (x > 0) {

    //Obtain Temp and Hum data
    //Assign collected data to JSON Object
    data["ID"] = id;
    data["Product"] = product;
    data["Value"] = value;

    Serial.println(id);
    Serial.println(value);
    Serial.println(product);


    //Send data to NodeMCU
    data.printTo(nodemcu);
    jsonBuffer.clear();

    delay(2000);
    finger.begin(57600);



  }

}
uint8_t getFingerprintID() {


  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);
  delay(1000);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);

  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
