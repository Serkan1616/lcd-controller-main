#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize LCD with I2C address 0x27, 16 columns, and 2 rows

    uint8_t Turkish_ch1[8] = { //ç Letter
      0b00000,
      0b01110,
      0b10001,
      0b10000,
      0b10000,
      0b10001,
      0b01110,
      0b00100
    };
    uint8_t Turkish_ch2[8] = { //Ğ Letter
      0b01110,
      0b00000,
      0b01110,
      0b10001,
      0b10000,
      0b10111,
      0b10001,
      0b01111
    };
    uint8_t Turkish_ch3[8] = { //Ş Letter
      0b00000,
      0b00000,
      0b01110,
      0b10000,
      0b01110,
      0b00001,
      0b01110,
      0b00100
    };
    uint8_t Turkish_ch4[8] = { //ğ Letter
      0b01110,
      0b00000,
      0b01111,
      0b10001,
      0b10001,
      0b01111,
      0b00001,
      0b01110
    };
    uint8_t Turkish_ch5[8] = { //İ Letter
      0b00100,
      0b00000,
      0b01110,
      0b00100,
      0b00100,
      0b00100,
      0b00100,
      0b01110
    };
    uint8_t Turkish_ch6[8] = { //i Letter
      0b00000,
      0b00000,
      0b00000,
      0b01100,
      0b00100,
      0b00100,
      0b00100,
      0b01110
      
};

boolean SCROLL_FLAG = true;             // This variable is used to turn scrolling on or off.
int SCROLL_WAIT = 1000;                 // This variable is used to store scrolling wait time before the next scroll operation. 1000 means 1000 milliseconds and this will execute a scrolling operation every 1 second.
char SCROLL_DIRECTION = 'L';            // Direction of scrolling ('L' for left, 'R' for right)
char LCD_ROW_1[] = "This text is for row 0 and longer than 16 characters."; // Initial text for row 1
char LCD_ROW_2[] = "ABCD EFGH"; // Initial text for row 2
char LCD_ROW_1_Temp[17];
char LCD_ROW_2_Temp[17];
bool LCD1_less_16=false;
bool LCD2_less_16=false;



unsigned long lastScrollTime = 0;       // Last scrolling time


void setup() {
  
  lcd.init(); // Initialize LCD
  lcd.backlight(); // Turn on backlight

  lcd.createChar(0,Turkish_ch1); //ç
  lcd.createChar(1,Turkish_ch2); // Ğ
  lcd.createChar(2,Turkish_ch3); //Ş
  lcd.createChar(3,Turkish_ch4); // ğ
  lcd.createChar(4,Turkish_ch5); // İ
  lcd.createChar(5,Turkish_ch6); // i


  lcd.setCursor(0, 0);
  lcd.write((byte)0);
  lcd.write((byte)1);
  lcd.write((byte)2);
  lcd.write((byte)3);
  lcd.write((byte)4);
  lcd.write((byte)5);
  


  delay(4000);
  

   // Control the size of LCD_ROW_1 and LCD_ROW_2 arrays
  if (strlen(LCD_ROW_1) < 16) {
    LCD1_less_16=true;
    for (int i = 0; i < 16; i++) {
      if (i < strlen(LCD_ROW_1)) {
        LCD_ROW_1_Temp[i] = LCD_ROW_1[i];
      } else {
        LCD_ROW_1_Temp[i] = ' ';
      }
    }
    LCD_ROW_1_Temp[16] = '\0'; // Null-terminate the string
  }

if (strlen(LCD_ROW_2) < 16) {
    LCD2_less_16=true;
    for (int i = 0; i < 16; i++) {
      if (i < strlen(LCD_ROW_2)) { // Controlling LCD_ROW_2
        LCD_ROW_2_Temp[i] = LCD_ROW_2[i];
      } else {
        LCD_ROW_2_Temp[i] = ' ';
      }
    }
    LCD_ROW_2_Temp[16] = '\0'; // Null-terminate the string
  }

   if(LCD1_less_16 && !(LCD2_less_16)){
       printInitialText(LCD_ROW_1_Temp, LCD_ROW_2); 
    }
    else if(LCD2_less_16 && !(LCD1_less_16)){
      printInitialText(LCD_ROW_1, LCD_ROW_2_Temp); 
    }
    else if(LCD1_less_16 && LCD2_less_16)
    {
      printInitialText(LCD_ROW_1_Temp, LCD_ROW_2_Temp); 
    }
    else{
      printInitialText(LCD_ROW_1, LCD_ROW_2); 
    }
  delay(1000);
}

void loop() {
  if (millis() - lastScrollTime >= SCROLL_WAIT && SCROLL_FLAG) {
    if(LCD1_less_16 && !(LCD2_less_16)){
       scrollText(LCD_ROW_1_Temp, LCD_ROW_2); 
    }
    else if(LCD2_less_16 && !(LCD1_less_16)){
      scrollText(LCD_ROW_1, LCD_ROW_2_Temp); 
    }
    else if(LCD1_less_16 && LCD2_less_16)
    {
      scrollText(LCD_ROW_1_Temp, LCD_ROW_2_Temp); 
    }
    else{
      scrollText(LCD_ROW_1, LCD_ROW_2); 
    }
    // Perform scrolling if SCROLL_FLAG is true and SCROLL_WAIT time has passed
    lastScrollTime = millis();
  }
}

void printInitialText(char* row1, char* row2) {
  
  
  lcd.setCursor(0, 0);  // Set cursor to row 1, column 0
  // Print first 16 characters of LCD_ROW_1 or pad with spaces if less than 16 characters
 // Print first 16 characters of LCD_ROW_2
  for (int i = 0; i < 16; i++) {

    if(row1[i]=="ç"){
      lcd.write((byte)0);
    }
    else{
     lcd.write(row1[i]);
    }
    
  }
  
  lcd.setCursor(0, 1);  // Set cursor to row 1, column 0
  // Print first 16 characters of LCD_ROW_2
  for (int i = 0; i < 16; i++) {
    lcd.write(row2[i]);
  }
}



void scrollText(char* row1, char* row2) {
  int rowLength;
  char tempChar;
  
  lcd.setCursor(0, 0); // Set cursor to row 0, column 0
  
  // Scroll text on row 0 based on scroll direction
  rowLength = strlen(row1);
  if (SCROLL_DIRECTION == 'L') {
    tempChar = row1[0]; // Store the first character to be shifted out

    // Left shift
    for (int i = 0; i < rowLength - 1; i++) {
      row1[i] = row1[i + 1]; // Shift characters to the left
    }
    row1[rowLength - 1] = tempChar; // Place the first character at the end
  } else if (SCROLL_DIRECTION == 'R') {
    tempChar = row1[rowLength - 1]; // Store the last character to be shifted out

    // Right shift
    for (int i = rowLength - 1; i > 0; i--) {
      row1[i] = row1[i - 1]; // Shift characters to the right
    }
    row1[0] = tempChar; // Place the last character at the beginning
  }
  
  // Print the shifted characters on row 0
  for (int i = 0; i < 16; i++) {
    if (row1[i] != '\0') {
      lcd.write(row1[i]);
    } else {
      lcd.write(' ');
    }
  }

  lcd.setCursor(0, 1); // Set cursor to row 1, column 0
  
  // Scroll text on row 1 based on scroll direction
  rowLength = strlen(row2);
  if (SCROLL_DIRECTION == 'L') {
    tempChar = row2[0]; // Store the first character to be shifted out

    // Left shift
    for (int i = 0; i < rowLength - 1; i++) {
      row2[i] = row2[i + 1]; // Shift characters to the left
    }
    row2[rowLength - 1] = tempChar; // Place the first character at the end
  } else if (SCROLL_DIRECTION == 'R') {
    tempChar = row2[rowLength - 1]; // Store the last character to be shifted out

    // Right shift
    for (int i = rowLength - 1; i > 0; i--) {
      row2[i] = row2[i - 1]; // Shift characters to the right
    }
    row2[0] = tempChar; // Place the last character at the beginning
  }
  
  // Print the shifted characters on row 1
  for (int i = 0; i < 16; i++) {
    if (row2[i] != '\0') {
      lcd.write(row2[i]);
    } else {
      lcd.write(' ');
    }
  }
}