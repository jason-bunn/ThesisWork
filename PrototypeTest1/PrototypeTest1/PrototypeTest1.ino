/*
 * Prototype keyboard emulation control
 * Bunn - Stanton 7-7-2015
 * 
 */

//set pin numbers for buttons
//joystick buttons
const int joystick_left = 2;   //Joystick left
const int joystick_down = 3;   //Joystick down
const int joystick_right = 4;  //Joystick right
const int joystick_up = 5;     //Joystick up

//action buttons
const int button_1 = 6;     //left shift
const int button_2 = 7;     //spacebar
const int button_3 = 8;     //M key
const int button_4 = 9;     //L Key
const int button_5 = 10;    //V Key
const int button_6 = 11;    //H Key

//character constants
const char FORWARD = 'w';
const char STRAFE_LEFT = 'a';
const char STRAFE_RIGHT = 'd';
const char BACKWARD = 's';
const char JUMP = ' ';
const char CROUCH = KEY_LEFT_CTRL;
const char RELOAD = 'r';
const char VOICE = 'v';
const char USE_ITEM = 'h';
const char DROP_INT = 'l';


//total number of buttons
const int numButtons = 10;

bool buttonStates[numButtons];

void setup() {
  // put your setup code here, to run once:
  pinMode(joystick_left, INPUT);
  pinMode(joystick_down, INPUT);
  pinMode(joystick_right, INPUT);
  pinMode(joystick_up, INPUT);
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(button_3, INPUT);
  pinMode(button_4, INPUT);
  pinMode(button_5, INPUT);
  pinMode(button_6, INPUT);

  for (int i = 0; i < numButtons; i++)
  {
    buttonStates[i] = false;
  }
  
  Keyboard.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  //check if button is being pressed
  if(digitalRead(joystick_left) == HIGH && buttonStates[0] == false)
  {
    Keyboard.press(STRAFE_LEFT);
    buttonStates[0] = true;
  }
  
  if(digitalRead(joystick_down) == HIGH && buttonStates[1] == false)
  {
    Keyboard.press(BACKWARD);
    buttonStates[1] = true;
  }

  if(digitalRead(joystick_right) == HIGH && buttonStates[2] == false)
  {
    Keyboard.press(STRAFE_RIGHT);
    buttonStates[2] = true;
  }
  if(digitalRead(joystick_up) == HIGH && buttonStates[3] == false)
  {
    Keyboard.press(FORWARD);
    buttonStates[3] = true;
  }

  if(digitalRead(button_1) == HIGH && buttonStates[4] == false)
  {
    Keyboard.press(CROUCH);
    buttonStates[4] = true;
  }

  if(digitalRead(button_2) == HIGH && buttonStates[5] == false)
  {
    Keyboard.press(JUMP);
    buttonStates[5] = true;
  }

  if(digitalRead(button_3) == HIGH && buttonStates[6] == false)
  {
    Keyboard.press(RELOAD);
    buttonStates[6] = true;
  }

  if(digitalRead(button_4) == HIGH && buttonStates[7] == false)
  {
    Keyboard.press(USE_ITEM);
    buttonStates[7] = true;
  }

  if(digitalRead(button_5) == HIGH && buttonStates[8] == false)
  {
    Keyboard.press(DROP_INT);
    buttonStates[8] = true;
  }

  if(digitalRead(button_6) == HIGH && buttonStates[9] == false)
  {
    Keyboard.press(VOICE);
    buttonStates[9] == true;
  }

  //
  //
  //
  //check if buttons have been released
  if (digitalRead(joystick_left) == LOW)
  {
    Keyboard.release(STRAFE_LEFT);
    buttonStates[0] = false;
  }
  if (digitalRead(joystick_down) == LOW)
  {
    Keyboard.release(BACKWARD);
    buttonStates[1] = false;
  }
  if (digitalRead(joystick_right) == LOW)
  {
    Keyboard.release(STRAFE_RIGHT);
    buttonStates[2] = false;
  }
  if (digitalRead(joystick_up) == LOW)
  {
    Keyboard.release(FORWARD);
    buttonStates[3] = false;
  }
  if(digitalRead(button_1) == LOW)
  {
    Keyboard.release(CROUCH);
    buttonStates[4] = false;
 }
  if(digitalRead(button_2) == LOW)
  {
    Keyboard.release(JUMP);
    buttonStates[5] = false;
  }

  if(digitalRead(button_3) == LOW)
  {
    Keyboard.release(RELOAD);
    buttonStates[6] = false;
  }

  if(digitalRead(button_4) == LOW)
  {
    Keyboard.release(USE_ITEM);
    buttonStates[7] = false;
  }

  if(digitalRead(button_5) == LOW)
  {
    Keyboard.release(DROP_INT);
    buttonStates[8] = false;
  }

  if(digitalRead(button_6) == LOW)
  {
    Keyboard.release(VOICE);
    buttonStates[9] = false;
  }

  
  
}
