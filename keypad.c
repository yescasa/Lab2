// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "keypad.h"

// ******************************************************************************************* //

void KeypadInitialize() {
	
	// TODO: Configure IOs and Change Notificaiton interrupt for keypad scanning. This 
	// configuration should ensure that if any key is pressed, a change notification interrupt 
	// will be generated.
    //inputs
    TRISBbits.TRISB2 = 1;//cn6//col0
    TRISBbits.TRISB3 = 1;//cn7//col1
    TRISBbits.TRISB5 = 1;//cn27//col2

    //enable internal pull up
    CNPU1bits.CN6PUE = 1;
    CNPU1bits.CN7PUE = 1;
    CNPU2bits.CN27PUE = 1;


    //config interrupt
    CNEN2bits.CN27IE = 1;
    CNEN1bits.CN6IE = 1;
    CNEN1bits.CN7IE = 1;

    //enable interrupt function
    IEC1bits.CNIE = 1;
    

    //outputs
    TRISBbits.TRISB15 = 0;//row3
    TRISBbits.TRISB14 = 0;//row2
    TRISBbits.TRISB13 = 0;//row1
    TRISBbits.TRISB12 = 0;//row0

    //open-drain
    ODCBbits.ODB15 = 1;
    ODCBbits.ODB14 = 1;
    ODCBbits.ODB13 = 1;
    ODCBbits.ODB12 = 1;

}

// ******************************************************************************************* //

char KeypadScan() {
	char key = -1;
        int keysPressed = 0;//keeps track of how many keys were pressed
	
	// TODO: Implement the keypad scanning procedure to detect if exactly one button of the 
	// keypad is pressed. The function should return:
	//
	//      -1         : Return -1 if no keys are pressed.
	//      '0' - '9'  : Return the ASCII character '0' to '9' if one of the 
	//                   numeric (0 - 9) keys are pressed.
	//      '#'        : Return the ASCII character '#' if the # key is pressed. 
	//      '*'        : Return the ASCII character '*' if the * key is pressed. 
	//       -1        : Return -1 if more than one key is pressed simultaneously.
	// Notes: 
	//        1. Only valid inputs should be allowed by the user such that all invalid inputs 
	//           are ignored until a valid input is detected.
	//        2. The user must release all keys of the keypad before the following key press
	//           is processed. This is to prevent invalid keypress from being processed if the 
	//           users presses multiple keys simultaneously.
	//

        if(PORTBbits.RB2==1 && PORTBbits.RB3==1 && PORTBbits.RB5==1) {
            key = -1;
        } else {
            LATBbits.LATB12 = 0;
            if(PORTBbits.RB2 == 0) {
                key = '1';
                keysPressed++;
            }
            if(PORTBbits.RB3==0) {
                key = '2';
                keysPressed++;
            }
            if(PORTBbits.RB5==0) {
                key = '3';
                keysPressed++;
            }
            
            
            LATBbits.LATB13 = 0;
            if(PORTBbits.RB2 == 0) {
                key = '4';
                keysPressed++;
            }
            if(PORTBbits.RB3==0) {
                key = '5';
                keysPressed++;
            }
            if(PORTBbits.RB5==0) {
                key = '6';
                keysPressed++;
            }
            
            
            LATBbits.LATB14 = 0;
            if(PORTBbits.RB2 == 0) {
                key = '7';
                keysPressed++;
            }
            if(PORTBbits.RB3==0) {
                key = '8';
                keysPressed++;
            }
            if(PORTBbits.RB5==0) {
                key = '9';
                keysPressed++;
            }
            
            
            LATBbits.LATB12 = 0;
            if(PORTBbits.RB2 == 0) {
                key = '*';
                keysPressed++;
            }
            if(PORTBbits.RB3==0) {
                key = '0';
                keysPressed++;
            }
            if(PORTBbits.RB5==0) {
                key = '#';
                keysPressed++;
            }

        }

        if(keysPressed > 1) {
            key = -1;
        }
	return key;
}

// ******************************************************************************************* //
