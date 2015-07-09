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
    Keyboard.press('a');
    buttonStates[0] = true;
  }
  
  if(digitalRead(joystick_down) == HIGH && buttonStates[1] == false)
  {
    Keyboard.press('s');
    buttonStates[1] = true;
  }

  if(digitalRead(joystick_right) == HIGH && buttonStates[2] == false)
  {
    Keyboard.press('d');
    buttonStates[2] = true;
  }
  if(digitalRead(joystick_up) == HIGH && buttonStates[3] == false)
  {
    Keyboard.press('w');
    buttonStates[3] = true;
  }

  if(digitalRead(button_1) == HIGH && buttonStates[4] == false)
  {
    Keyboard.press(KEY_LEFT_SHIFT);
    buttonStates[4] = true;
  }

  if(digitalRead(button_2) == HIGH && buttonStates[5] == false)
  {
    Keyboard.press(' ');
    buttonStates[5] = true;
  }

  if(digitalRead(button_3) == HIGH && buttonStates[6] == false)
  {
    Keyboard.press('m');
    buttonStates[6] = true;
  }

  if(digitalRead(button_4) == HIGH && buttonStates[7] == false)
  {
    Keyboard.press('l');
    buttonStates[7] = true;
  }

  if(digitalRead(button_5) == HIGH && buttonStates[8] == false)
  {
    Keyboard.press('v');
    buttonStates[8] = true;
  }

  if(digitalRead(button_6) == HIGH && buttonStates[9] == false)
  {
    Keyboard.press('h');
    buttonStates[9] == true;
  }

  //
  //
  //
  //check if buttons have been released
  if (digitalRead(joystick_left) == LOW)
  {
    Keyboard.release('a');
    buttonStates[0] = false;
  }
  if (digitalRead(joystick_down) == LOW)
  {
    Keyboard.release('s');
    buttonStates[1] = false;
  }
  if (digitalRead(joystick_right) == LOW)
  {
    Keyboard.release('d');
    buttonStates[2] = false;
  }
  if (digitalRead(joystick_up) == LOW)
  {
    Keyboard.release('w');
    buttonStates[3] = false;
  }
  if(digitalRead(button_1) == LOW)
  {
    Keyboard.release(KEY_LEFT_SHIFT);
    buttonStates[4] = false;
 }
  if(digitalRead(button_2) == LOW)
  {
    Keyboard.release(' ');
    buttonStates[5] = false;
  }

  if(digitalRead(button_3) == LOW)
  {
    Keyboard.release('m');
    buttonStates[6] = false;
  }

  if(digitalRead(button_4) == LOW)
  {
    Keyboard.release('l');
    buttonStates[7] = false;
  }

  if(digitalRead(button_5) == LOW)
  {
    Keyboard.release('v');
    buttonStates[8] = false;
  }

  if(digitalRead(button_6) == LOW)
  {
    Keyboard.release('h');
    buttonStates[9] = false;
  }

  
  
}
