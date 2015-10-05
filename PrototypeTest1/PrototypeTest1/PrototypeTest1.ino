/*
 * Prototype keyboard emulation control
 * Bunn - Stanton 7-7-2015
 */

/* Include the "Pluggable" HID and Keyboard libraries, rather than the
 *  old core Keyboard/Mouse libraries.  Possibly able to implement
 *  NKRO with the newer pluggable libs.
 */
#if (ARDUINO >= 10606)
#include "Keyboard.h"
#include "HID.h"
#endif

/* Define DEBUG if you want to enable the Arduino's hardware Serial port
 *  and block any code from being processed until a computer attaches to
 *  the serial port.  Once a user connects to the serial port, the program
 *  will continue, and debug messages will be output to it.
 */
//#define DEBUG

/* Define DEBUG2 to enable extra (possibly extremely cluttery and annoying)
 *  debug messages to be output over the Arduino's serial port.  If you want
 *  this to work properly, you'll need to define DEBUG also.
 */
//#define DEBUG2

/* Define this if you are using a Sanwa JLF-TP-8YT joystick oriented vertically
 *  with the 5-pin header pointing to the right as viewed with the joystick shaft
 *  pointing towards you.
 */
//#define SANWA_JLF

/* Define this if you are using a Sanwa JLF-TP-8YT joystick oriented horizontally
 *  with the 5-pin header pointing down as viewed with the joystick shaft pointing
 *  towards you.
 */
//#define SANWA_JLF_90DEG_CW

/* Define this if you are using a Sanwa JLF-TP-8YT joystick with the microswitch PCB
 *  replaced with the GamerFinger optical sensor PCB, oriented vertically with the
 *  6-pin header pointing to the right as viewed with the joystick shaft pointing
 *  towards you.
 */
//#define SANWA_OPTICAL

/* Define this if you are using a Sanwa JLF-TP-8YT joystick with the microswitch PCB
 *  replaced with the GamerFinger optical sensor PCB, oriented horizontally with the
 *  6-pin header pointing down as viewed with the joystick shaft pointing towards you.
 */
#define SANWA_OPTICAL_90DEG_CW

// Set pin numbers for outputs
const int led_kb_on = 1;

// Set pin numbers for buttons -- Arduino Leonardo
// - Joystick buttons
const int joystick_up = 5;
const int joystick_left = 2;
const int joystick_right = 3;
const int joystick_down = 4;
// - Action buttons
const int button_1 = 6;
const int button_2 = 7;
const int button_3 = 8;
const int button_4 = 9;
const int button_5 = 10;
const int button_6 = 11;
// Control buttons
const int button_kb_onoff = 0;

// Character constants
// - These are set to the default keybindings for Team Fortress 2
const char FORWARD = 'w';
const char STRAFE_LEFT = 'a';
const char BACKWARD = 's';
const char STRAFE_RIGHT = 'd';
const char JUMP = ' ';
const char DUCK = KEY_LEFT_CTRL;
const char RELOAD = 'r';
const char VOICE = 'v';
const char USE_ITEM = 'h';
const char DROP_INT = 'l';
const char LAST_WEAPON = 'q';

// Total number of buttons
const int numButtons = 11;

// Array that stores the current "pressed" state of all buttons
bool buttonStates[numButtons];

// Variable that determines whether keyboard functionality is enabled
bool kbOn = false;

/* Array that stores the current debounce state of the buttons.
 *   0 - not tracking a debounce action
 *   1 - tracking a button press debounce action
 *   2 - tracking a button release debounce action
 */
uint8_t debounceStates[numButtons];

// Times denoting the beginning of a debounce sequence for each button
long lastDebounceTimes[numButtons];

// Time in milliseconds to wait to debounce a key press/release
uint8_t debounceDelay = 5;

// Array of key codes we wish to assign to each button
char buttonValues[numButtons] = {
#ifdef SANWA_JLF
  FORWARD,
  STRAFE_LEFT,
  BACKWARD,
  STRAFE_RIGHT,
#endif
#ifdef SANWA_JLF_90DEG_CW
  STRAFE_RIGHT,
  FORWARD,
  STRAFE_LEFT,
  BACKWARD,
#endif
#ifdef SANWA_OPTICAL
  BACKWARD,
  STRAFE_LEFT,
  STRAFE_RIGHT,
  FORWARD,
#endif
#ifdef SANWA_OPTICAL_90DEG_CW
  STRAFE_LEFT,
  FORWARD,
  BACKWARD,
  STRAFE_RIGHT,
#endif
  JUMP,
  DUCK,
  RELOAD,
  USE_ITEM,
  LAST_WEAPON,
  VOICE
};

void setup()
{
  // Initialize button and debounce tracking arrays to '0'
  for ( int i = 0; i < numButtons; i++ )
  {
    buttonStates[i] = false;
    debounceStates[i] = 0;
    lastDebounceTimes[i] = 0;
  }

  // Set up pin states
  pinMode(joystick_left, INPUT_PULLUP);
  pinMode(joystick_down, INPUT_PULLUP);
  pinMode(joystick_right, INPUT_PULLUP);
  pinMode(joystick_up, INPUT_PULLUP);
  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP);
  pinMode(button_3, INPUT_PULLUP);
  pinMode(button_4, INPUT_PULLUP);
  pinMode(button_5, INPUT_PULLUP);
  pinMode(button_6, INPUT_PULLUP);
  pinMode(button_kb_onoff, INPUT_PULLUP);
  pinMode(led_kb_on, OUTPUT);
  digitalWrite(led_kb_on, LOW);

#ifdef DEBUG
  // In debug mode, we need to use the Serial port to see what is going on in the program
  Serial.begin(9600);
  while (!Serial) {}
#endif
  // Register Arduino as a USB HID Keyboard and begin tracking keypresses
  Keyboard.begin();
}

/*
 * button_press() -
 *   Use the USB HID Keyboard functionality to signal to the computer
 * that a keyboard key has been pressed.  The integer value "btnNumber"
 * is and array index that corresponds to the key code in the buttonValues[]
 * char array, the button's pressed state in buttonStates[], whether we
 * are tracking a debounce event in debounceStates[], and if so, when thatwad rh
 * debounce event began in the lastDebounceTimes[] array.
 *   This function allows easy enabling of debug code and also handles
 * the initiation of debounce tracking for the button being pressed.
 */
void button_press(uint8_t btnNumber)
{
  // Check that the button isn't already being debounced
  if (debounceStates[btnNumber] == 0)
  {
    // Set button's debounce state to "1", for "pressed"
    debounceStates[btnNumber] = 1;
    // Set time that the debounce action began
    lastDebounceTimes[btnNumber] = millis();
#ifdef DEBUG
    Serial.print("Starting debounce tracking on button press: ");
    Serial.println(btnNumber, DEC);
    Serial.print("  Press time: ");
    Serial.println(lastDebounceTimes[btnNumber]);
#endif
  }
#ifdef DEBUG2
  Serial.print(micros());
  Serial.print(" - Timestamp, button press: ");
  Serial.println(btnNumber);
#endif
}

/*
 * button_release() -
 *   Use the USB HID Keyboard functionality to signal to the computer
 * that a keyboard key has been released.  The integer value "btnNumber"
 * corresponds to the key code in the buttonValues[] char array.
 *   This function allows easy enabling of debug code and also handles
 * the initiation of debounce tracking for the button being released.
 */
void button_release(uint8_t btnNumber)
{
  // Check that the button isn't already being debounced
  if (debounceStates[btnNumber] == 0)
  {
    // Set button's debounce state to "2", for "released"
    debounceStates[btnNumber] = 2;
    // Set time that the debounce action began
    lastDebounceTimes[btnNumber] = millis();
#ifdef DEBUG
    Serial.print("Starting debounce tracking on button release: ");
    Serial.println(btnNumber, DEC);
    Serial.print("  Press time: ");
    Serial.println(lastDebounceTimes[btnNumber]); // */
#endif
  }
#ifdef DEBUG2
  Serial.print(micros());
  Serial.print(" - Timestamp, button release: ");
  Serial.println(btnNumber);
#endif
}

/*
 * check_debounce() -
 *   This function loops from 0 through numButtons-1 to check the current
 * debounce status of all buttons.  Any buttons that are successfully debounced
 * have their functionality carried out, assuming that the button's
 * functionality is currently enabled.
 */
void check_debounce()
{
  // Test every button for a debounce action
  for ( int i = 0; i < numButtons; i++ )
  {
    // If debounceStates[i] is 1 or 2, we are tracking a debounce.  If it's 0, skip this button.
    if (debounceStates[i] > 0)
    {
      /* If millis() is less than lastDebounceTimes for this button, that means
       *  that the timer has overrun the "long" data type, causing it to reset
       *  to 0 and begin counting again from there.  That also means that
       *  lastDebounceTimes[i] was likely within 10ms of the overrun point, which
       *  means we will never be able to complete the debounce action without
       *  setting it to something else.  Since we don't want to set it to less
       *  than zero, we pick 0 as the value.  That means we could be debouncing
       *  for up to 20ms, but it keeps us from having to check for the size of
       *  the data type and how close we are to it every time we set lastDebounceTimes.
       */
      if (millis() < lastDebounceTimes[i])
        lastDebounceTimes[i] = 0;

      // Check to see that debounceDelay ms have passed since we began tracking debounce for this button
      if (millis() - lastDebounceTimes[i] >= debounceDelay)
      {
        if (debounceStates[i] == 1)
        {
          // Change the button state to true
          buttonStates[i] = true;
#ifdef DEBUG
          Serial.print("Button press complete: ");
          Serial.println(i);
          Serial.print("Completion time: ");
          Serial.println(millis());
#endif
          if (i == numButtons - 1)
          {
            // Since this is the keyboard enabler button, enable the keyboard
            keyboard_enable();
          }
          else
          {
            // Because we are tracking a button "press" (1), press the button
            Keyboard.press(buttonValues[i]);
          }
        }
        if (debounceStates[i] == 2)
        {
          // Change the button state to false
          lastDebounceTimes[i] = 0;
          buttonStates[i] = false;
#ifdef DEBUG
          Serial.print("Button release complete: ");
          Serial.println(i);
          Serial.print("Completion time: ");
          Serial.println(millis());
#endif
          if (i == numButtons - 1)
          {
            // Since this is the keyboard enabler button, enable the keyboard
            keyboard_disable();
          }
          else
          {
            // Because we are tracking a button "release" (2), release the button
            Keyboard.release(buttonValues[i]);
          }
        }
        // Reset the lastDebounceTimes value to 0 and reset the debounceState value to 0
        lastDebounceTimes[i] = 0;
        debounceStates[i] = 0;
      }
    }
  }
}

/*
 * keyboard_enable() -
 *   This function is called when the button_kb_onoff pin is pulled low.
 * Here, we need to enable the HID keyboard functionality and light the
 * LED that signals that the controller is in operational mode.
 */
void keyboard_enable()
{
  kbOn = true;
  digitalWrite(led_kb_on, HIGH);
}

/*
 * keyboard_disable() -
 *   This function is called when the keyboard functionality of the game
 * controller is disabled.  This requires us to set all button and debounce
 * tracking variables to their original initialized states (0 or false),
 * release any keyboard keys we have been pressing, and turn the LED indicating
 * that the game controller is operational off.
 */
void keyboard_disable()
{
  for (int i = 0; i < numButtons; i++)
  {
    lastDebounceTimes[i] = 0;
    debounceStates[i] = 0;
    buttonStates[i] = false;
    kbOn = false;
  }
  Keyboard.releaseAll();
  digitalWrite(led_kb_on, LOW);
}

void loop()
{
  /******************************************************************
  * Check if any buttons have gone from not pressed to pressed.     *
  * If so, we will pass this info to the button_press() function.   *
  ******************************************************************/

  // Only check these buttons if keyboard functionality is on.
  if (kbOn)
  {
    if (digitalRead(joystick_up) == LOW && buttonStates[0] == false)
      button_press(0);
    if (digitalRead(joystick_left) == LOW && buttonStates[1] == false)
      button_press(1);
    if (digitalRead(joystick_down) == LOW && buttonStates[2] == false)
      button_press(2);
    if (digitalRead(joystick_right) == LOW && buttonStates[3] == false)
      button_press(3);
    if (digitalRead(button_1) == LOW && buttonStates[4] == false)
      button_press(4);
    if (digitalRead(button_2) == LOW && buttonStates[5] == false)
      button_press(5);
    if (digitalRead(button_3) == LOW && buttonStates[6] == false)
      button_press(6);
    if (digitalRead(button_4) == LOW && buttonStates[7] == false)
      button_press(7);
    if (digitalRead(button_5) == LOW && buttonStates[8] == false)
      button_press(8);
    if (digitalRead(button_6) == LOW && buttonStates[9] == false)
      button_press(9);
  }

  // This buttons enables/disables keyboard functionality, so we have to check it
  if (digitalRead(button_kb_onoff) == LOW && buttonStates[10] == false)
    button_press(10);


  /******************************************************************
  * Check if any buttons have gone from pressed to released.        *
  * If so, we will pass this info to the button_release() function. *
  ******************************************************************/

  // Only check these buttons if keyboard functionality is on.
  if (kbOn)
  {
    if (digitalRead(joystick_up) == HIGH && buttonStates[0] == true)
      button_release(0);
    if (digitalRead(joystick_left) == HIGH && buttonStates[1] == true)
      button_release(1);
    if (digitalRead(joystick_down) == HIGH && buttonStates[2] == true)
      button_release(2);
    if (digitalRead(joystick_right) == HIGH && buttonStates[3] == true)
      button_release(3);
    if (digitalRead(button_1) == HIGH && buttonStates[4] == true)
      button_release(4);
    if (digitalRead(button_2) == HIGH && buttonStates[5] == true)
      button_release(5);
    if (digitalRead(button_3) == HIGH && buttonStates[6] == true)
      button_release(6);
    if (digitalRead(button_4) == HIGH && buttonStates[7] == true)
      button_release(7);
    if (digitalRead(button_5) == HIGH && buttonStates[8] == true)
      button_release(8);
    if (digitalRead(button_6) == HIGH && buttonStates[9] == true)
      button_release(9);
  }

  // This buttons enables/disables keyboard functionality, so we have to check it
  if (digitalRead(button_kb_onoff) == HIGH && buttonStates[10] == true)
    button_release(10);

  // Every time through the loop, we check the debounce state of every button
  check_debounce();
}
