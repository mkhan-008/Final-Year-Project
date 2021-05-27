#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
// Above are the libraries used in this code. They are used for the different components of the hardware
// Create instances
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(A2,A0); // MFRC522 mfrc522(SDA_PIN, RST_PIN)
Servo sg90;
// Initialize Pins for led's, servo and buzzer
// Blue LED is connected to 5V
 int trig = 4;  // Arduino pin connected to Ultrasonic Sensor's TRIG pin
 int echo = 3;  // Arduino pin connected to Ultrasonic Sensor's ECHO pin
const int Distance_threshold = 20; // in centimeter ( Sensitivty of ultrasonic sensor 
float duration_us, distance_cm;// veriables
constexpr uint8_t greenLed = 1;


constexpr uint8_t servoPin = A1;
constexpr uint8_t buzzerPin = 2;
char initial_password[4] = {'1', '2', '3', '4'};  // Variable to store initial password. This is where you set the keypad password 
String tagUID = "03 D9 1E 02";  // String to store UID of tag. This is used to store the master RFID tag. 
char password[4];   // Variable to store users password
boolean RFIDMode = true; // This programme will run on the basis that the boolean is true. So as long as the rfid is active every other component will run.
char key_pressed = 0; // Variable to store incoming keys
uint8_t i = 0;  // Variable used for counter
// defining how many rows and columns our keypad have
const byte rows = 4;
const byte columns = 4;
// Keypad pin map
char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Initializing pins for keypad
byte row_pins[rows] = {8,9,10,A3};
byte column_pins[columns] = {5,6,7};
// Create instance for keypad
Keypad key_keypad = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup() {
  Serial.begin (9600); // initialising serial port
  // Arduino Pin configuration
  pinMode(buzzerPin, OUTPUT);
  pinMode(trig , OUTPUT);
  pinMode (echo , INPUT);
  pinMode(greenLed, OUTPUT);
  sg90.attach(servoPin);  //Declare pin A0 for servo
  sg90.write(0); // Set initial position for servo vertical
  lcd.init();
  lcd.begin(16,2);   // LCD screen
  lcd.backlight();
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  lcd.clear(); // Clear LCD screen. The next lines to follow are for the lcd to display the inital message.
  lcd.setCursor(0, 0); //This sets the cursor to the first row of the lcd
  lcd.print("  Door Locked ");
  lcd.setCursor(0, 1); //This sets the cursor to the second row of the lcd
  lcd.print(" Scan Your ID ");
}

//Function for the actions to take when the keypad or RFID is matched and accepted
void accepted(){
  lcd.clear();
  Serial.print("AccessGranted.");
  Serial.print(",");
      lcd.setCursor(0, 0);
      tone (2, 1500, 500); //once accepted and correct a tone would play
      lcd.print("Door Unlocked"); //lcd will show message
      lcd.setCursor(0, 1);
      lcd.print("Welcome Home");
      digitalWrite(greenLed, HIGH); //LED will turn on to indicate successfull access
      sg90.write(90); // Door Unlocked - Motor will rotate 90 degrees sliding the lock of the door
      delay(3000); // 3 second wait
      digitalWrite(greenLed, LOW);
      sg90.write(0); // Door closed //The motor will now change back into the intial position which is locked
      lcd.clear();
      lcd.print("System is Armed"); // Another message on lcd after the wait to show it is armed again
      lcd.setCursor(0, 1);
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Door Locked ");
      lcd.setCursor(0, 1);
      lcd.print(" Scan Your ID ");
  
}
//Function for when the incorrect passcode is entered on keypad or incorrect RFID tag is used.
void notaccepted(){
  lcd.clear();
  Serial.print("AccessDenied.");
  Serial.print(",");
        tone( 2, 200, 300); //Different tone for incorrect password
        delay(100);
        tone( 2, 500, 200);// tone plays twice in shorter burst
        lcd.setCursor(0, 0);
        lcd.print("Wrong Password"); //Changes message on LCD
        lcd.setCursor(0, 1);        
        lcd.print("Enter Valid Password");
        delay (2000); // Shows the message for 2 second on LCD
        lcd.clear(); // Clears message on LCD
        lcd.setCursor(0, 0);
        lcd.print("  Door Locked "); // Shows initial message on the LCD
        lcd.setCursor(0, 1);
        lcd.print(" Scan Your ID ");
}


//Main loop of the programme
void loop() {

 // generates 10-microsecond pulse to TRIG pin
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
  // measure duration of pulse from ECHO pin
    duration_us = pulseIn(echo, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;
 if(distance_cm <  Distance_threshold) // This if loop will check the ultrasonic sensor to see if there is a difference in distance which will trigger it.
 {
     //The tone command takes into account 3 values. The first is the PIN position of the buzzer, second is the pitch of the sound and lastly is the duration of the sound.
     Serial.print("Intrusion..");
     tone(2, 2000, 5000);
     lcd.clear(); //Clears the lcd of the previous message displayed
     lcd.setCursor(0, 0);
     lcd.print("     ALERT! "); // New message on lcd
     lcd.setCursor(0, 1);
     lcd.print("   INTRUDER!   ");
     delay(5000); //5 second wait before next instance of loop
     lcd.clear(); //clears intruder message and sets back to normal
     lcd.setCursor(0, 0);
     lcd.print("  Door Locked ");
     lcd.setCursor(0, 1);
     lcd.print(" Scan Your ID ");
     
}

//Checks boolean state to proceed with if statement
 if (RFIDMode == true ) {
    key_pressed = key_keypad.getKey(); // Storing keys
    if (key_pressed)
    {
      lcd.clear();
      password[i] = key_pressed; // Storing in password variable
      lcd.setCursor(i,1);
      lcd.print("*");
      i++;
      //The code above is for the asterisk sign to show on the lcd when typing on keypad. It uses a counter to count the pressed keys and more the cursor for each count.
      
    }
    if (i == 4) // If 4 keys are completed
    {
      delay(200);
      char initial_password[4] = {'1', '2', '3', '4'}; 
      
      if ((strncmp(password, initial_password, 4) ==0)) //This uses the command strncomp to check if the password are equal. It has 3 parameters the 3rd being the length of string
      {                                                 //If password is matched = 0. Can also use '!((strncmp))' as normally the code will give a 0 output, by inverting it you can get a 1.
      accepted(); // Will call the function 'Accepted'
      i = 0; //reset counter for password 
      }
      else    // If password is not matched
      {
        notaccepted();
        i = 0; //reset counter for password
      }
    }
  }

  // System will first look for the state of the RFID reader mode
  if (RFIDMode == true) {
      
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    //Reading from the card
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();
    //Checking the card
    if (tag.substring(1) == tagUID)
    {
      // If UID of tag is matched. Will call the function for accepted
      accepted();
    }
    else
    {
      // If UID of tag is not matched.
      notaccepted();
    }
  }
  
 
}
