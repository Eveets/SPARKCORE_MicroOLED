/****************************************************************************** 
hardware.cpp
MicroOLED Arduino Library Hardware Interface

Jim Lindblom @ SparkFun Electronics
October 26, 2014
https://github.com/sparkfun/Micro_OLED_Breakout/tree/master/Firmware/Arduino/libraries/SFE_MicroOLED


This file defines the hardware interface(s) for the Micro OLED Breakout. Those
interfaces include SPI, I2C and a parallel bus. Only SPI has been adapted to SparkCore

Adapted by Marc Liyanage
https://github.com/liyanage/Micro_OLED_Breakout/

Formatted for Spark Web IDE Public Library by Steeve Monniere
https://github.com/Eveets/SPARKCORE_MicroOled

Development environment specifics:
  SparkCore
  Micro OLED Breakout v1.0

This code was heavily based around the MicroView library, written by GeekAmmo
(https://github.com/geekammo/MicroView-Arduino-Library), and released under 
the terms of the GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) any later 
version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "MicroOLED.h"

/** \brief Set Up SPI Interface

	Sets up the SPI pins, initializes the Arduino's SPI interface.
**/
void MicroOLED::spiSetup()
{
	// Initialize the SPI library:
	SPI.setClockDivider(SPI_CLOCK_DIV2);	// Fastest SPI clock possible
	SPI.setDataMode(SPI_MODE0);	// CPOL=0 and CPHA=0, SPI mode 0
	pinMode(10, OUTPUT); // Required for setting into Master mode
	SPI.begin();
}

/** \brief Transfer a byte over SPI

	Use the SPI library to transfer a byte. Only used for data OUTPUT.
	This function does not toggle the CS pin. Do that before and after!
**/
void MicroOLED::spiTransfer(byte data)
{
	SPI.transfer(data);	
}
