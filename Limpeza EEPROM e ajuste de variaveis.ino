#include <EEPROM.h>

#define espacoEEPROM 1000   //1Kb para todos os Arduinos com ATMega328P

byte   variavelByte;
int    variavelInt;
long   variavelLong;
String variavelString;


void EEPROMWriteInt(int address, int value); //2 Bytes
int  EEPROMReadInt(int address);

void EEPROMWriteLong(int address, long value); //4 Bytes
long EEPROMReadLong(int address);

void EEPROMWriteStr(int address, String value); //n Bytes
void EEPROMConcatStr(int address, String value);
String EEPROMReadStr(int address);

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch Iniciado");

  //Limpeza da EEPROM (Executado apenas para limpar a memoria. IMPORTANTE: Apos a primeira carga, comentar este trecho e efetuar a segunda carga em seguida.)
  //for (int nL = 0; nL < espacoEEPROM; nL++) { 
  //    EEPROM.write(nL, 0);
  //}  //Comentar até aqui.
  
  variavelByte = EEPROM.read(0);        //Posicao 0 (zero) da EEPROM
  variavelInt  = EEPROMReadInt(1);      //Posicao 1 e 2 da EEPROM
  variavelLong = EEPROMReadLong(3);     //Posicao 3, 4, 5 e 6 da EEPROM
  variavelString = EEPROMReadStr(100);  //A Partir da Posicao 100 da EEPROM
}

void loop() {

  variavelByte++;
  variavelInt += 5;
  variavelLong += 10;
  variavelString = "Soma dos das variaves = ";
  variavelString += String(variavelByte + variavelInt + variavelLong);

  EEPROM.write(0, variavelByte);        //Posicao 0 (zero) da EEPROM
  EEPROMWriteInt(1, variavelInt);       //Posicao 1 e 2 da EEPROM
  EEPROMWriteLong(3, variavelLong);     //Posicao 3, 4, 5 e 6 da EEPROM
  EEPROMWriteStr(100, variavelString);  //A Partir da Posicao 100 da EEPROM

  Serial.print("variavelByte: ");
  Serial.println(variavelByte);

  Serial.print("variavelInt: ");
  Serial.println(variavelInt);

  Serial.print("variavelLong: ");
  Serial.println(variavelLong);

  Serial.print("variavelString: ");
  Serial.println(variavelString);

  Serial.println();
  delay(2000);
}

void EEPROMWriteInt(int address, int value) {
   byte hiByte = highByte(value);
   byte loByte = lowByte(value);

   EEPROM.write(address, hiByte);
   EEPROM.write(address + 1, loByte);   
}

int EEPROMReadInt(int address) {
   byte hiByte = EEPROM.read(address);
   byte loByte = EEPROM.read(address + 1);
   
   return word(hiByte, loByte); 
}

void EEPROMWriteLong(int address, long value) {
   byte four = (value & 0xFF);
   byte three = ((value >> 8) & 0xFF);
   byte two = ((value >> 16) & 0xFF);
   byte one = ((value >> 24) & 0xFF);

   EEPROM.write(address, four);
   EEPROM.write(address + 1, three);
   EEPROM.write(address + 2, two);
   EEPROM.write(address + 3, one);
}

long EEPROMReadLong(int address) {
   long four = EEPROM.read(address);
   long three = EEPROM.read(address + 1);
   long two = EEPROM.read(address + 2);
   long one = EEPROM.read(address + 3);

   return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWriteStr(int address, String value) {
int nL;

   for (nL=0; nL < value.length(); nL++)  {
      EEPROM.write(address + nL, value.charAt(nL) );

      if (nL >= 1000) {
         break;
      }
   }

   if (nL < 1000) {
      EEPROM.write(address + nL, (char)0 );
   }
}

void EEPROMConcatStr(int address, String value) {
int nL;
char readByte;
bool nullFound = false;
int strPos = 0;

   for (nL=0; nL < 1000; nL++)  {

      if (!nullFound) {
         readByte = EEPROM.read(address + nL);

         if (readByte == (char)0) {
            nullFound = true;
         }
      } 
      
      if (nullFound) {
         EEPROM.write(address + nL, value.charAt(strPos) );          
         strPos++;

         if ((strPos + 1) > value.length()) {
            EEPROM.write(address + nL + 1, (char)0 );
            break;
         }
      }
   }     
}

String EEPROMReadStr(int address) {
String readStr = "";
char readByte;
int readAddress = address;

   do {
      readByte = EEPROM.read(readAddress);
      readStr += readByte;
      readAddress++;
   } while ( (readByte != (char)0) && (readAddress < (address + 1000)) );
   
   return readStr; 
}