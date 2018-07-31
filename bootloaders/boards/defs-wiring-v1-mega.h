/* $Id: defs-wiring-v1-mega.h 1178 2011-06-11 06:02:07Z bhagman $
||
|| @author         Brett Hagman <bhagman@roguerobotics.com>
|| @url            http://wiring.org.co/
|| @url            http://www.roguerobotics.com/
||
|| @description
|| | STK500v2 bootloader hardware definition file for Wiring V1 128/Wiring Mini.
|| #
||
|| @license Please see License.txt for license information.
||
*/

// BOOT_BAUD_RATE = Bitrate for bootloader
// -ve values tell the usart initialization to use U2X
// We use these values to force rounding
// @ 16MHz, best match for 115200 = -117647
// @ 16MHz, best match for 57600 = -57142

// BOOT_USART = USART to use on MCU (0 = first/only, 1 = second, 2 = third...)
//#define BOOT_USART 0


//////////////////////////////////////////////////
// Debug parameters
//////////////////////////////////////////////////

// DEBUG = If defined, enable debugging output
// (may be best to define in makefile)
//#define DEBUG

// DEBUG_USART = USART to use for debugging purposes
// Can be any hardware USART, or SOFT_PORT - a software serial output
// (Soft serial parameters defined in makefile - see RogueRobotics.mk)
//#define DEBUG_USART USART1


//////////////////////////////////////////////////
// LED parameters
//////////////////////////////////////////////////

// D48 = PORTG0
#define LED_PORT          PORTG
#define LED_PIN           PING0

// Number of times to flash LED on boot (one time)
// LED_FLASH_TIME is in milliseconds
#define NUM_LED_FLASHES 2
#define LED_FLASH_TIME  50

