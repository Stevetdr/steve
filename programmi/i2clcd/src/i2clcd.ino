
/*  Author:        Vasco Ferraz                                               *
*  Contact:       http://vascoferraz.com/contact/                            *
*  Description:   http://vascoferraz.com/tutorials/hitachi-hd44780-lcd/      *
*****************************************************************************/
 
// Include the Hitachi HD44780 library
#include <LiquidCrystal.h>
 
// Initialize the library with the following Arduino's interface pins: 7, 6, 5, 4, 3, 2
// Number 7 means Arduino's digital I/O port number 7.
// Number 6 means Arduino's digital I/O port number 6.
// And so on... Check the schematics for more information.
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
 
unsigned char i; // Define variable "i" to be used in both loops
 
void setup()
{
// Define the number of LCD's columns and rows
lcd.begin(40,2); // For this tutorial I am using a 16x2 LCD which means that there are 16 columns and 2 rows
lcd.clear();
}

 
void loop()
{
  
// This first loop prints a sequence of "x" on the first row
for (i=0 ; i<=40 ; i++)
 {
  lcd.setCursor(i, 0); // Set the cursor to column i and row 0 (first row)
  lcd.print("x");      // Print a message to the LCD
  delay(100);          // 500ms delay
 }


// This second loop prints a sequence of "x" on the second row
for (i=0 ; i<=40 ; i++)
 {
  lcd.setCursor(i, 1); // Set the cursor to column i and row 1 (second row)
  lcd.print("Y");      // Print a message to the LCD
  delay(100);          // 500ms delay
 }
  delay(300); 
  lcd.clear();         // Clear the LCD  
 
}