#include <Bounce2.h>

/* Pro Micro Test Code
   by: Florian Maurer
   GameThing by Cuddleburrito
   date: March 6, 2015
   license: Public Domain - please use this code however you'd like.
   It's provided as a learning tool.

   This code is provided to how to turn an arduino into an HID keyboard controller for arcade games 
*/

//---------
// USE THESE FOR THE buttonPresets ARRAY BELOW
#define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT   0x82
#define KEY_LEFT_GUI   0x83  
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT    0x85
#define KEY_RIGHT_ALT  0x86
#define KEY_RIGHT_GUI  0x87

#define KEY_UP_ARROW   0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW    0xD7
#define KEY_BACKSPACE  0xB2
#define KEY_TAB        0xB3
#define KEY_RETURN 0xB0
#define KEY_ESC        0xB1
#define KEY_INSERT 0xD1
#define KEY_DELETE 0xD4
#define KEY_PAGE_UP    0xD3
#define KEY_PAGE_DOWN  0xD6
#define KEY_HOME   0xD2
#define KEY_END        0xD5
#define KEY_CAPS_LOCK  0xC1
#define KEY_F1     0xC2
#define KEY_F2     0xC3
#define KEY_F3     0xC4
#define KEY_F4     0xC5
#define KEY_F5     0xC6
#define KEY_F6     0xC7
#define KEY_F7     0xC8
#define KEY_F8     0xC9
#define KEY_F9     0xCA
#define KEY_F10        0xCB
#define KEY_F11        0xCC
#define KEY_F12        0xCD

//---------

#define LED_PIN 13
//#define BOUNCE_LOCK-OUT //activate the alternative debouncing method. This method is a lot more responsive, but does not cancel noise.

//========== CONFIGURATION SETTINGS ==========
#define BOUNCE_WAIT 10
#define BOUNCE_COUNT 1

  //pins 2-9, 10, 14,15,16, 18-21
  //for buttons 1-12, up, down, left and right
int buttonPins[] = {
  9, //button 1
  8, //button 2
  7, //button 3
  3, //button 4
  2, //button 5
  4, //button 6
  6, //button 7
  5, //button 8
  18, //button 9
  19, //button 1 back panel
  20, //button 2 back panel
  21, //button 3 back panel
  10, //joystick up
  16, //joystick down
  14, //joystick left
  15 // joystick right
};

char buttonPresets[] = { 
  KEY_LEFT_CTRL, //button 1
  KEY_LEFT_ALT, //button 2
  ' ', //button 3
  KEY_LEFT_SHIFT, //button 4
  'z', //button 5
  'x', //button 6
  '7', //button 7
  '8', //button 8
  '9', //button 9
  '1', //button 1 back panel
  '5', //button 2 back panel
  '2', //button 3 back panel
  KEY_UP_ARROW, //joystick up
  KEY_DOWN_ARROW, //joystick down
  KEY_LEFT_ARROW, //joystick left
  KEY_RIGHT_ARROW // joystick right
};

//========== END CONFIGURATION SETTINGS ==========

int RXLED = 17;  // The RX LED has a defined Arduino pin
// Instantiate button state array
boolean buttonPressed[16];
// Instantiate a Bounce object array to store each debouncer in
Bounce debouncers[16];
//Instantiate a counter array for each button to debounce count timer
int debounceCount[16];

void setup() {
  // put your setup code here, to run once:

  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
 // TX LED is set as an output behind the scenes 
 
   //Setup the LED :
 pinMode(LED_PIN,OUTPUT);
 digitalWrite(RXLED, LOW);
  
 
 Keyboard.begin();
 
  // Create debounce instances :
   for (int i = 0; i < 16; i++) {
     debouncers[i] = Bounce();
     debounceCount[i] = 0;
     pinMode(buttonPins[i],INPUT_PULLUP);
     (debouncers[i]).attach(buttonPins[i]);
     (debouncers[i]).interval(BOUNCE_WAIT);
        delay(100);
     buttonPressed[i] = false;, 
   }
     
}

void loop() {
  
  for (int j = 0; j < 16; j++) { //iterate over each button (pin)
    
     (debouncers[j]).update(); //check current value
     int value = (debouncers[j]).read();
     
     if ( value == LOW ) { //if button pressed
       
       if(debounceCount[j] == BOUNCE_COUNT && buttonPressed[j] == false) { //the button has been held down long enough and it hasn't been previously registered as pressed
          Keyboard.press(char(buttonPresets[j])); //Keyboard.write('1');
          buttonPressed[j] = true;
        } else {
            if(debounceCount[j] < BOUNCE_COUNT) { 
              debounceCount[j] = debounceCount[j] + 1; //else increment the count
            }
        }
        
      } else { //button is not pressed
        
        if(debounceCount[j] > 0) {
          debounceCount[j] = debounceCount[j] - 1; //keep decreasing count unless 0
        } else {
           Keyboard.release(char(buttonPresets[j])); //if 0 then release button
           buttonPressed[j] = false;
        }
        
      }

  }

}
