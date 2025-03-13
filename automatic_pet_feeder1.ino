#include <LiquidCrystal_I2C.h>
#include <Servo.h>


// Pin for the button
int buttonLeft = 10;
int buttonRight = 11;
int buttonUp = 12;
int buttonDown = 13;

int feedingHour = 12; // Default feeding hour (in 24-hour format)
int feedingMinute = 0; // Default feeding minute
int foodAmount = 2;  // Default food amount (in quarter cups)

int currentHour = 12;
int currentMinute = 0;

unsigned long previousMillis = 0; // To track time
const long interval = 60000; // Check every minute (60000 milliseconds)

int menuIndex = 0; // Index to navigate menu items
int menuItems = 3; // Total menu items

//servo setup
int servoPin = 9;   // Declare the Servo pin 
Servo Servo1;        // Create a servo object 

// LCD setup
LiquidCrystal_I2C lcd(0x20,16,2);

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.clear();
  lcd.backlight();
  // Set up button
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  //attach servo to used pin number
  Servo1.attach(servoPin);
  Servo1.write(180);
  //start screen
  lcd.setCursor(0, 0);
  lcd.print("Automatic Feeder");
  lcd.setCursor(0, 1);
  lcd.print("Press any button");
}

void loop() {
  long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    currentMinute += 1;
    if(currentMinute == 60){
      currentHour = (currentHour + 1) % 24;
    }
    currentMinute = currentMinute % 60;
    checkFeedingTime(); // Check feeding time when the interval elapses
  }
  if (digitalRead(buttonUp) == HIGH) {
    menuIndex = (menuIndex + menuItems - 1) % menuItems;
    displayMenu();
    delay(100);
  }

  if (digitalRead(buttonDown) == HIGH) {
    menuIndex = (menuIndex + 1) % menuItems;
    displayMenu();
    delay(100);
  }


  if (digitalRead(buttonRight) == HIGH) {
    modifyMenu(menuIndex);
    displayMenu();
    delay(200);
  }
}

void displayMenu() {
  lcd.clear();
  switch (menuIndex) {
    case 0:
    case 1:
      lcd.setCursor(1, 0);
  	  lcd.print("Feed Time ");
  	  lcd.setCursor(11, 0);
  	  if (feedingHour < 10) {
    	  lcd.print("0");
  	  }
  	  lcd.print(feedingHour);
  	  lcd.print(":");
  	  if (feedingMinute < 10) {
    	  lcd.print("0");
  	  }
  	  lcd.print(feedingMinute);
  
  	  lcd.setCursor(1, 1);
  	  lcd.print("Food Amount ");
  	  lcd.setCursor(14, 1);
  	  if (foodAmount < 10) {
  	  lcd.print("0");
  	  }
  	  lcd.print(foodAmount);
      lcd.setCursor(0, menuIndex);
  	  lcd.print(">");
      break;
    
    case 2:
      lcd.setCursor(1, 0);
  	  lcd.print("Food Amount ");
  	  lcd.setCursor(14, 0);
  	  if (foodAmount < 10) {
  	  lcd.print("0");
  	  }
  	  lcd.print(foodAmount);
    
      lcd.setCursor(1, 1);
  	  lcd.print("Curr Time ");
  	  lcd.setCursor(11, 1);
  	  if (currentHour < 10) {
    	  lcd.print("0");
  	  }
  	  lcd.print(currentHour);
  	  lcd.print(":");
  	  if (currentMinute < 10) {
    	  lcd.print("0");
  	  }
  	  lcd.print(currentMinute);
    
      lcd.setCursor(0, 1);
  	  lcd.print(">");
      break;
    default:
      break;
  }
  
  
  

}

void modifyMenu(int index) {
  switch (index) {
    case 0:
      modifyFeedingTime();
      break;
    case 1:
      modifyFoodAmount();
      break;
    case 2:
      modifyWorldTime();
      break;
    default:
      break;
  }
}

void modifyFeedingTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Feeding Time");
  lcd.setCursor(0, 1);
  lcd.print("Curr Time: ");
  lcd.print(feedingHour);
  lcd.print(":");
  if (feedingMinute < 10) {
    lcd.print("0");
  }
  lcd.print(feedingMinute);
  delay(4000);
  lcd.setCursor(0, 1);
  lcd.print("New Time: ");
  
  
  int newHour = feedingHour;
  int newMinute = feedingMinute;
    
  int cursorPos = 11;
  bool editingHour = true;
  
  
 while (digitalRead(buttonLeft) == LOW) {
    if (digitalRead(buttonUp) == HIGH) {
      if (editingHour) {
        newHour = (newHour + 1) % 24; // Hours from 0 to 23
      } else {
        newMinute = (newMinute + 1) % 60; // Minutes from 0 to 59
      }
      lcd.setCursor(cursorPos, 1);
      if (newHour < 10) {
        lcd.print("0");
      }
      lcd.print(newHour);
      lcd.print(":");
      if (newMinute < 10) {
        lcd.print("0");
      }
      lcd.print(newMinute);
      delay(200);
    }

    if (digitalRead(buttonDown) == HIGH) {
      if (editingHour) {
        newHour = (newHour + 23) % 24; // Hours from 0 to 23
      } else {
        newMinute = (newMinute + 59) % 60; // Minutes from 0 to 59
      }
      lcd.setCursor(cursorPos, 1);
      if (newHour < 10) {
        lcd.print("0");
      }
      lcd.print(newHour);
      lcd.print(":");
      if (newMinute < 10) {
        lcd.print("0");
      }
      lcd.print(newMinute);
      delay(200);
    }

    if (digitalRead(buttonRight) == HIGH) {
      editingHour = !editingHour; // Switch between editing hour and minute
      if (editingHour) {
        cursorPos = 11;
      } else {
        cursorPos = 11;
      }
      delay(200);
    }
 } 
  feedingHour = newHour;
  feedingMinute = newMinute;
  lcd.setCursor(0, 1);
  lcd.print("Set Time: ");
  lcd.setCursor(cursorPos, 1);
  lcd.print(feedingHour);
  lcd.print(":");
  if (feedingMinute < 10) {
    lcd.print("0");
  }
  lcd.print(feedingMinute);
  delay(3000);
   return;
}
  

void modifyFoodAmount() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Food Amount");
  lcd.setCursor(0, 1);
  lcd.print("Curr Amount: ");
  lcd.setCursor(14, 1);
  if (foodAmount < 10) {
        lcd.print("0");
      }
  lcd.print(foodAmount);
  delay(4000);
  lcd.setCursor(0, 1);
  lcd.print("New amount: ");


  int newAmount = foodAmount;
  while (digitalRead(buttonLeft) == LOW) {
    if (digitalRead(buttonUp) == HIGH) {
      newAmount = (newAmount + 1) % 21; // Range from 0 to 20
      lcd.setCursor(14, 1);
      lcd.print("   "); // Clear previous input
      lcd.setCursor(14, 1);
      if (newAmount < 10) {
        lcd.print("0");
      }
      lcd.print(newAmount);
      delay(200);
    }
    if (digitalRead(buttonDown) == HIGH) {
      newAmount = (newAmount + 20) % 21; // Range from 0 to 20
      lcd.setCursor(14, 1);
      lcd.print("   "); // Clear previous input
      lcd.setCursor(14, 1);
      if (newAmount < 10) {
        lcd.print("0");
      }
      lcd.print(newAmount);
      delay(200);
    }
  }
  foodAmount = newAmount;
  delay(500);
  return;
}  

void modifyWorldTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set World Time");
  lcd.setCursor(0, 1);
  lcd.print("Curr Time: ");
  lcd.print(currentHour);
  lcd.print(":");
  if (currentMinute < 10) {
    lcd.print("0");
  }
  lcd.print(currentMinute);
  delay(4000);
  lcd.setCursor(0, 1);
  lcd.print("New Time: ");
  
  
  int currentNewHour = currentHour;
  int currentNewMinute = currentMinute;
    
  int cursorPos = 11;
  bool editingHour = true;
  
  
 while (digitalRead(buttonLeft) == LOW) {
    if (digitalRead(buttonUp) == HIGH) {
      if (editingHour) {
        currentNewHour = (currentNewHour + 1) % 24; // Hours from 0 to 23
      } else {
        currentNewMinute = (currentNewMinute + 1) % 60; // Minutes from 0 to 59
      }
      lcd.setCursor(cursorPos, 1);
      if (currentNewHour < 10) {
        lcd.print("0");
      }
      lcd.print(currentNewHour);
      lcd.print(":");
      if (currentNewMinute < 10) {
        lcd.print("0");
      }
      lcd.print(currentNewMinute);
      delay(200);
    }

    if (digitalRead(buttonDown) == HIGH) {
      if (editingHour) {
        currentNewHour = (currentNewHour + 23) % 24; // Hours from 0 to 23
      } else {
        currentNewMinute = (currentNewMinute + 59) % 60; // Minutes from 0 to 59
      }
      lcd.setCursor(cursorPos, 1);
      if (currentNewHour < 10) {
        lcd.print("0");
      }
      lcd.print(currentNewHour);
      lcd.print(":");
      if (currentNewMinute < 10) {
        lcd.print("0");
      }
      lcd.print(currentNewMinute);
      delay(200);
    }

    if (digitalRead(buttonRight) == HIGH) {
      editingHour = !editingHour; // Switch between editing hour and minute
      if (editingHour) {
        cursorPos = 11;
      } else {
        cursorPos = 11;
      }
      delay(200);
    }
 } 
  currentHour = currentNewHour;
  currentMinute = currentNewMinute;
  lcd.setCursor(0, 1);
  lcd.print("Set Time: ");
  lcd.setCursor(cursorPos, 1);
  lcd.print(currentHour);
  lcd.print(":");
  if (currentMinute < 10) {
    lcd.print("0");
  }
  lcd.print(currentMinute);
  delay(3000);
   return;
}
  

void checkFeedingTime() {

  // Check if feeding time matches the simulated clock time
  if (currentHour == feedingHour && currentMinute == feedingMinute) {
    dispenseFood();
  }
}

void dispenseFood() {
  // Dispense food (using servo attached to a collecter part)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Feeding pet!");
  for(int i = 0; i < foodAmount; i++) {
   Servo1.write(90); // Make servo collect food
   delay(3000); 
  
   Servo1.write(0);  // Make servo put out food
   delay(3000); 
  
  

  }
  Servo1.write(90);  // Make servo go to collect food
  
  delay(2000); // Simulate feeding time duration
  lcd.clear();
  displayMenu(); // Display menu after feeding
}