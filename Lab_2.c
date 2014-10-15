// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings. 
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to 
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF &
	BKBUG_ON & COE_ON & ICS_PGx1 &
	FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)

	// ******************************************************************************************* //
	// Configuration bits for CONFIG2 settings.
	// Make sure "Configuration Bits set in code." option is checked in MPLAB.

	_CONFIG2(IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
	IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT)

	// ******************************************************************************************* //

	// Varible used to indicate that the current configuration of the keypad has been changed,
	// and the KeypadScan() function needs to be called.

	volatile char scanKeypad;
volatile long int counter = 0;
volatile long int press = 0;
volatile int state = 0;
// ******************************************************************************************* //

int main(void)
{
	char key;


	//32 bit timer set to 2 second interrupt
	TMR3 = 0;
	TMR5 = 0;
	PR3 = 0b1100000111111111;
	PR5 = 0b000001;
	IFS1bits.T5IF = 0;
	IEC1bits.T5IE = 1;
	T4CONbits.T32 = 1;
	T4CONbits.TON = 1;
	T4CONbits.TCKPS0 = 1;
	T4CONbits.TCKPS1 = 1;


	// TODO: Initialize and configure IOs, LCD (using your code from Lab 1), 
	// UART (if desired for debugging), and any other configurations that are needed.

	LCDInitialize();
	KeypadInitialize();

	// TODO: Initialize scanKeypad variable.

	while (1)
	{
		// TODO: Once you create the correct keypad driver (Part 1 of the lab assignment), write
		// the C program that use both the keypad and LCD drivers to implement the 4-digit password system.
		char database[4] = { 1, 2, 3, 4 };
		int valid = 1;

		case (state){

/*********************************************************/
//if no press print ENTER once press detected, proceed to state 1
		case(0) :
			valid = 1;
			press = 0;

			if (scanKeypad == 0){
				LCDMoveCursor(0, 0);
				LCDPrintString("Enter");
			}
				if (scanKeypad == 1){
					TMR5 = 0;
					count = 0;
					scanKeypad = 0;
					state = 1;
				}
				break;
/*********************************************************/

//Detects press when released goes to state 2
		case(1):
			if(scanKeypad==1){
				TMR5 = 0;
				count = 0;
				scanKeypad = 0;
				state = 2;
			}
//If * is pressed goes to state 3 that checks number of presses
			else if(key=='*'){
				state = 3;
			}

			break;

/********************************************************/

		case(2) :
//upon release, counts a press
			press = press + 1;
//sets scanned button to key
			key = KeypadScan();
//prints key on second row
			if (key != -1) {
				LCDMoveCursor(1, (press-1));
				LCDPrintChar(key);
			}

//checks key with database if match then valid ==1 if ever not valid REMAIN unvalid
			if ((key - database[press - 1] == 0) && valid != 0){
				valid = 1;
			}
//if invalid char, valid ==0
			else if(key == '#' || key == '*'){
				valid = 0;
			}
//if no match valid =0
			else valid =0;

//if any press did not match the database on the fourth press print bad and return to ENTER state
			if (valid == 0 && press == 4){
				LCDClear();
				LCDMoveCursor(0, 0);
				LCDPrintString("Bad")

//wait 2 seconds then proceed to ENTER state
				if (count == 2){
					TMR5 = 0;
					count = 0;
					scanKeypad = 0;
					state = 0;
				}
			}

//if all keys match database print good and return to ENTER state
			else if (valid == 1 && press == 4){
				LCDClear();
				LCDMoveCursor(0, 0);
				LCDPrintString("Good");

//wait 2 seconds then proceed to ENTER state
				if (count == 2)
					TMR5 = 0;
					count = 0;
					scanKeypad = 0;
					state = 0;
			}

//if a press is detected go to state 1
			if(scanKeypad == 1)
					state = 1;
			

		}
		break;
/*************************************************************/
		case(3):
//if key is * and on the second press go to state 4 
			if(press == 1){
				state = 4;
			{
//if not continue on to state 2
			else{
				TMR5 = 0;
				count = 0;
				scanKeypad = 0;
				state = 2;
			}

		break;
/*************************************************************/		
		case(4):
//Print Set Mode and wait until key is pressed
			LCDClear();
			LCDMoveCursor(0, 0);
			LCDPrintString("Set Mode");

			if (scanKeypad == 1){
				TMR5 = 0;
				count = 0;					
				scanKeypad = 0;
				state = 5;
		break;

/*************************************************************/

//Detects press when released goes to state 6
		case(5):
			if(scanKeypad==1){
				TMR5 = 0;
				count = 0;
				scanKeypad = 0;
				state = 6;
			}
/*************************************************************/

		case(6):
//count press and add key presed to the appropriate location on the array
			press = press +1;

//prints key on second row
			if (key != -1) {
				LCDMoveCursor(1, (press-1));
				LCDPrintChar(key);
			}

//if invalid char, valid ==0
			if(key == '#' || key == '*'){
				valid = 0;			}
			else valid =1;

//THIS WILL CURRENTLY OVERRIDE THE DATABASE RATHER THAN KEEP EACH NEW PASSWORD
			if(press != 4 && valid != 0){
				database[press-1]= KeypadScan();
			}

//once  5 keys are pressed & last key is # with valid keys return to ENTER state
			if(press == 5 && key == '#' && valid =1){
				LCDClear();
				LCDMoveCursor(0, 0);
				LCDPrintString("Valid");
				TMR5=0;
				count=0;
//wait 2 seconds then proceed to ENTER state
				if (count == 2){
					TMR5 = 0;
					count = 0;
					scanKeypad = 0;
					state = 0;
				}
			}
//if five keys are pressed & keys are not valid print invalid
			else if(press == 5){
				LCDClear();
				LCDMoveCursor(0, 0);
				LCDPrintString("Invalid");
				TMR5=0;
				count =0;

//wait 2 seconds then proceed to ENTER state
				if (count == 2){
					TMR5 = 0;
					count = 0;
					scanKeypad = 0;
					state = 0;
				}
			}

//wait for another press
			if(scanKeypad =1){
				state = 5;
			}
	}
	return 0;
}

// ******************************************************************************************* //
// Defines an interrupt service routine that will execute whenever any enable
// input change notifcation is detected.
// 
//     In place of _ISR and _ISRFAST, we can directy use __attribute__((interrupt))
//     to inform the compiler that this function is an interrupt. 
//
//     _CNInterrupt is a macro for specifying the interrupt for input change 
//     notification.
//
// The functionality defined in an interrupt should be a minimal as possible
// to ensure additional interrupts can be processed. 
void __attribute__((interrupt)) _CNInterrupt(void)
{
	// TODO: Clear interrupt flag
	IFS1bits.CNIF = 0;

	// TODO: Detect if *any* key of the keypad is *pressed*, and update scanKeypad
	// variable to indicate keypad scanning process must be executed.
	scanKeypad == 1;

}

void __attribute__((interrupt, auto_psv)) _T5Interrupt(void){
	IFS1bits.T5IF = 0;
	counter = counter + 1;
}

// ******************************************************************************************* //

