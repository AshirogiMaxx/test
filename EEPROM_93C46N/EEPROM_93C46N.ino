//
// LED <--> Arduino:
// Anode <--> Digital 8 (via 220 Ohm resistor)
// Cathode <--> GND
//
// 93C46 <--> Arduino:
// CS <--> Digital 53 (SS)
// SCK <--> Digital 52 (SCK)
// DI <--> Digital 51 (MOSI)
// DO <--> Digital 50 (MISO)
// VCC <--> 5V
// GND <--> GND
//

//#include <avr/io.h>
#include <util/delay.h>

#define CLEAN 0b00000000

#define DDR93C46  DDRB
#define PORT93C46 PORTB
#define PIN93C46  PINB

#define SK PB1 //SCK - Digital 52
#define DO PB3 //MISO - Digital 50
#define DI PB2 //MOSI - Digital 51
#define CS PB0 //SS - Digital 53

#define READ  0x02 // 10 00ABCD - 0x02 ADDR
#define EWEN1 0x00 // 00 11XXXX
#define EWEN2 0x30 //           - 0x00 0x30
#define ERASE 0x03 // 11 00ABCD - 0x03 ADDR
#define ERAL1 0x00 // 00 10XXXX
#define ERAL2 0x20 //           - 0x00 0x20
#define WRITE 0x01 // 01 00ABCD - 0x01 ADDR
#define WRAL1 0x00 // 00 01XXXX
#define WRAL2 0x10 //           - 0x00 0x10
#define EWDS1 0x00 // 00 00XXXX
#define EWDS2 0x00 //           - 0x00 0x00

#define SET_CS PORT93C46 |= (1 << CS)
#define CLR_CS PORT93C46 &= ~(1 << CS)

#define SET_SK {PORT93C46 |= (1 << SK); _delay_us(0.01);}
#define CLR_SK PORT93C46 &= ~(1 << SK)

#define SET_DI PORT93C46 |= (1 << DI)
#define CLR_DI PORT93C46 &= ~(1 << DI)

class SPI_93C46N {
private:
  uint8_t Transfer(uint8_t data) {
    SPDR = data;
    while(!(SPSR & (1 << SPIF))); // Wait for SPI interrupt flag
    return SPDR;                  // Return byte gathered from SPI data register
  }
  void Opcode(uint8_t opcode, uint8_t address) {
    SET_CS;
    SPCR &= ~(1 << SPE);               // SPI disable
    _delay_us(1);
    CLR_SK;
    SET_DI;                            // Send start bit
    SET_SK;
    CLR_SK;
    SPCR |= (1 << SPE);                // SPI enable
    Transfer((opcode << 6) | address); // Transmit byte
  }
public:
  SPI_93C46N() {
    SPCR =    CLEAN        // Set SPI control register
           & ~(1 << SPIE)  // SPI interrupt disable
           |  (1 << SPE)   // SPI enable
           & ~(1 << DORD)  // MSB first
           |  (1 << MSTR)  // SPI master device
           & ~(1 << CPOL)  // SPI
           & ~(1 << CPHA)  //  mode 0
           |  (1 << SPR1)  // XX/64 MHz
           & ~(1 << SPR0); //  speed
    SPSR =    CLEAN         // Init SPI status register
           & ~(1 << SPI2X); // SPI double speed
    DDR93C46 =    CLEAN      // Set up inputs/outputs
               |  (1 << SK)  // Serial clock output
               |  (1 << DI)  // Data input (MOSI) output
               |  (1 << CS)  // Chip select output
               & ~(1 << DO); // Data output (MISO) input
    PORT93C46 |= (1 << DO); // Data output (MISO) high
    CLR_DI;
    CLR_CS;    
    CLR_SK;
    _delay_us(1);
  }
  ~SPI_93C46N() {
    //
  }
  uint16_t Read(uint8_t address) {
    uint16_t data = 0;
    Opcode(READ, address);              // READ instruction
    CLR_DI;
    SET_SK;                             // "0" DUMMY bit
    CLR_SK;
    SPCR |=  (1 << CPHA);               // Invert PHA, very important!
    data = Transfer(0);
    data = (data << 8) | (Transfer(0));
    _delay_us(1);
    CLR_CS;
    CLR_DI;
    SPCR &= ~(1 << CPHA);   
    return data;
  }
  void EraseWriteEnable(void) {
    Opcode(EWEN1, EWEN2);   
    CLR_DI;
    CLR_CS;
    SET_CS;
    _delay_us(0.1);
    while(!(PIN93C46 & (1 << DO))); // Wait 1
    CLR_CS; 
  }
  void EraseWriteDisable(void) {
    Opcode(EWDS1, EWDS2);
    CLR_DI;
    CLR_CS;
    SET_CS;
    _delay_us(0.1);
    while(!(PIN93C46 & (1 << DO))); // Wait 1
    CLR_CS; 
  }
  void Erase(uint8_t address) {
    Opcode(ERASE, address);   
    CLR_DI;
    CLR_CS;
    SET_CS;
    _delay_us(0.1);
    while(!(PIN93C46 & (1 << DO))); // Wait 1
    CLR_CS; 
  }
  void EraseAll(void) {
    Opcode(ERAL1, ERAL2);   
    CLR_DI;
    CLR_CS;
    SET_CS;
    _delay_us(0.1);
    while(!(PIN93C46 & (1 << DO))); // Wait 1
    CLR_CS; 
  }
  void Write(uint8_t address, uint16_t data) {
    Opcode(WRITE, address);         // WRITE instruction
    Transfer(data >> 8);            // Write data high byte to addreCS
    Transfer(data & 0xFF);          // Write data low byte to addreCS
    CLR_DI;
    CLR_CS;
    SET_CS;
    _delay_us(0.1);
    while(!(PIN93C46 & (1 << DO))); // Wait "1" 
    CLR_CS;
  } 
  void WriteAll(uint16_t data) {
    Opcode(WRAL1, WRAL2);           // WRITE instruction
    Transfer(data >> 8);            // Write data high byte to addreCS
    Transfer(data & 0xFF);          // Write data low byte to addreCS
    CLR_DI;
    CLR_CS;
    SET_CS;
    _delay_us(0.1);
    while(!(PIN93C46 & (1 << DO))); // Wait "1" 
    CLR_CS;
  }
};

void setup(void) {
  Serial.begin(115200);
  _delay_ms(100);
  Serial.println("EEPROM writing/reading");
  pinMode(8, OUTPUT);
}

void loop() {
  SPI_93C46N _93C46N;
  int i;
  uint16_t data;
  //_93C46N.EraseWriteEnable();
  //for(i = 0; i < 32; i++)
  //_93C46N.Write(i, 0xDEAD);
  //for(i = 32; i < 64; i++)
  //_93C46N.Write(i, 0xBEEF);
  //_93C46N.WriteAll(0xB0AB);
  _93C46N.EraseWriteDisable();
  for(i = 0; i < 64; i++) {
    data = _93C46N.Read(i);
    analogWrite(8, (unsigned char)(data >> 8));
    _delay_ms(50);
    analogWrite(8, (unsigned char)data);
    _delay_ms(50);
    Serial.print(data, HEX);
    Serial.print(" ");
    if((i + 1) % 8 == 0)
      Serial.println();
  }
  analogWrite(8, 0);
  _delay_ms(1000);  
}

