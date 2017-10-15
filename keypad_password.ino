  #include <Keypad.h>
  #include <EEPROM.h>

  char* secretCode = "1234";

  int position = 0;
  int position2 = 0;
  boolean locked = true;

  const byte rows = 4;
  const byte cols = 4;
  char keys[rows][cols] = {
     {'1','2','3','A'},
     {'4','5','6','B'},
     {'7','8','9','C'},
     {'*','0','#','D'}
  };
  byte rowPins[rows] = {12, 11, 10, 9};
  byte colPins[cols] = {8, 7, 6, 5};

  Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

  int redPin = 2;
  int greenPin = 4;
  int bluePin = 3;
  int solenoidPin = 13;
  int i = 0;

  void setup() {
     pinMode(redPin, OUTPUT);
     pinMode(greenPin, OUTPUT);
     pinMode(bluePin, OUTPUT);
     loadCode();
     flash();
     updateOutputs();
  }

  void loop() {
     char key = keypad.getKey();

     if (key) {
        position2 ++;
        digitalWrite(bluePin, HIGH);
        delay(30);
        digitalWrite(bluePin, LOW);
     }

     if (key == '*' && ! locked) {
        position = 0;
        position2 = 0;
        getNewCode();
        updateOutputs();
     }

     if (key == '#') {
        locked = true;
        position = 0;
        position2= 0;
        updateOutputs();
        digitalWrite(bluePin, HIGH);
        delay(300);
        digitalWrite(bluePin, LOW);
     }

     if (key == secretCode[position]) {
        position ++;
     }

     if (position == 4 & position2 == 4) {
        locked = false;
        digitalWrite(bluePin, HIGH);
        delay(300);
        digitalWrite(bluePin, LOW);
        updateOutputs();
     }
     delay(100);
  }

  void updateOutputs() {
     if (locked) {
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
        digitalWrite(solenoidPin, HIGH);
     } else {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);
        digitalWrite(solenoidPin, LOW);
        delay (50);
        for (int i=0; i <= 100; i++) {
           char key;
           key = keypad.getKey();
           if (key == '*') {
              position = 0;
              position2 = 0;
              getNewCode();
              updateOutputs();
           }
           delay (50);
        }
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
        digitalWrite(solenoidPin, HIGH);
        locked = true;
        position = 0;
        position2= 0;
     }
  }

  void getNewCode() {
     flash();
     for (int i = 0; i < 4; i++ ) {
        char key;
        key = keypad.getKey();
        while (key == 0) {
           key = keypad.getKey();
        }
        flash();
        secretCode[i] = key;
     }
     saveCode();
     flash();flash();
  }

  void loadCode() {
     if (EEPROM.read(0) == 7) {
        secretCode[0] = EEPROM.read(1);
        secretCode[1] = EEPROM.read(2);
        secretCode[2] = EEPROM.read(3);
        secretCode[3] = EEPROM.read(4);
     }
  }

  void saveCode() {
     EEPROM.write(1, secretCode[0]);
     EEPROM.write(2, secretCode[1]);
     EEPROM.write(3, secretCode[2]);
     EEPROM.write(4, secretCode[3]);
     EEPROM.write(0, 7);
  }

  void flash() {
     digitalWrite(redPin, HIGH);
     digitalWrite(greenPin, LOW);
     delay(100);
     digitalWrite(redPin, LOW);
     digitalWrite(greenPin, HIGH);
     delay(100);
  }
