
// Send alphanumeric data to the Sparkfun Serial LED Display (COM-09230) using SPI
// Tested using Arduino Pro Mini w/ ATMega168 @ 5V
// July 21, 2009  - Quazar & Busaboi
// No guarantees expressed or implied just a good starting point 
// Based upon the many SPI tutorials on Arduino.cc
//
// "num" specifies the number to display
// "base" specifies the base to use (2-16).
//    Use 2 for binary, 8 for octal, 10 for decimal, or 16 for hex
// "pad" indicates whether leading zeros should be replaced with spaces.
//    pad==0 means spaces ("   0"), pad==1 means zeros ("0000")
//
// Notes: The display's decimal/punctuation indicators are not changed.
// Numbers that don't fit into 4 digits show as " OF " for "Overflow".
// Assumptions: "unsigned short" is assumed to be at least 16b wide.

#define DATAOUT 11 //MOSI
#define DATAIN 12 //MISO - not used, but part of builtin SPI
#define SPICLOCK 13 //sck
#define SLAVESELECT 10 //ss

char spi_transfer(volatile char data)
{
 SPDR = data;                    // Start the transmission
 while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
 {
 };
 return SPDR;                    // return the received byte
}

void setup()
{
 byte clr;
 pinMode(DATAOUT, OUTPUT);
 pinMode(DATAIN, INPUT);
 pinMode(SPICLOCK, OUTPUT);
 pinMode(SLAVESELECT, OUTPUT);
 digitalWrite(SLAVESELECT, HIGH); //disable device
 // SPCR = 01010010
 //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
 //sample on leading edge of clk,system clock/64 
 SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
 clr=SPSR;
 clr=SPDR;
 delay(10);
 write_led_numbers(0x78,0x78,0x78,0x78); //Blank display
 write_led_decimals(0x00); // All decimal points off
}

void write_led_decimals(int value)
{
  digitalWrite(SLAVESELECT, LOW);
  delay(10);
  spi_transfer(0x77);     // Decimal Point OpCode
  spi_transfer(value);    // Decimal Point Values
  digitalWrite(SLAVESELECT, HIGH); //release chip, signal end transfer
}
void write_led_numbers(int digit1, int digit2, int digit3, int digit4)
{
  digitalWrite(SLAVESELECT, LOW);
  delay(10);
  spi_transfer(digit1);    // Thousands Digit
  spi_transfer(digit2);    // Hundreds Digit
  spi_transfer(digit3);    // Tens Digit
  spi_transfer(digit4);    // Ones Digit
  digitalWrite(SLAVESELECT, HIGH); //release chip, signal end transfer
}
void write_led(unsigned short num, unsigned short base, unsigned short pad)
{
   unsigned short digit[4] = { 0, ' ', ' ', ' ' };
   unsigned short place = 0;

   if ( (base<2) || (base>16) || (num>(base*base*base*base-1)) ) {
       write_led_numbers(' ', 0x00, 0x0f, ' ');  // indicate overflow
   } else {
       while ( (num || pad) && (place<4) ) {
           if ( (num>0)  || pad )
               digit[place++] = num % base;
           num /= base;
       }
       write_led_numbers(digit[3], digit[2], digit[1], digit[0]);
   }
}

void loop()
{
for (int i = 0; i < 9999; i++) { 
    write_led (i,10,0);
    delay(100);
  }

}
