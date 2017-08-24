/**************************************************************************************/
/*                                 DECLARATIONS                                       */
/**************************************************************************************/

// Librairie CAN
#include <mcp_can.h>
#include <SPI.h>
#define SPI_CS_PIN 9
MCP_CAN CAN(SPI_CS_PIN);

// Affectation
#define BLANC 3
#define ROUGE 4

// Caractéristiques des messages
#define LONGUEUR_DATA 8
#define LENGTH 8

// Déclaration variable
int led_rouge = 0;
int led_blanche = 0;

/**************************************************************************************/
/*                                INITIALISATION                                      */
/**************************************************************************************/

void setup() {
  
  Serial.begin(115200); // Init communication série à 115200 Bauds
  
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {  // Init can bus : baudrate = 500k
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    delay(100);
  } // Fin while
  
  Serial.println("CAN BUS Shield init ok!");
  for(int i = 3; i < 5; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW); 
  }
       
} // Fin setup

unsigned char stmp[LONGUEUR_DATA] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char buf[LONGUEUR_DATA];

/**************************************************************************************/
/*                                BOUCLE INFINIE                                      */
/**************************************************************************************/

void loop() {
  
  unsigned char len = 0;
  
  // Check if data coming
  if(CAN_MSGAVAIL == CAN.checkReceive()) {
    // Read data,  len: data length, buf: data buf
    CAN.readMsgBuf(&len, buf);

    unsigned int canId = CAN.getCanId();
    
    if (buf[0] != 0) {    
      Serial.println("-----------------------------");
      Serial.print("Get data from ID: ");
      Serial.println(canId, DEC);

      // Print the data
      for(int i = 0; i<len; i++) {
        Serial.print(buf[i], DEC);
        Serial.print("\t");
      } // Fin for
    
    Serial.println("");
    } // Fin if

    if(buf[0] == 10) {
      led_blanche = buf[4];
      if (led_blanche == 1) digitalWrite(BLANC, HIGH);
      else digitalWrite(BLANC, LOW);

      // Acknowledgment
      stmp[0] = 11;
      CAN.sendMsgBuf(0x02, 0, LENGTH, stmp);
      stmp[0] = 0;      
    }
    
    if(buf[0] == 100) {
      led_rouge = buf[5];
      if (led_rouge == 1) digitalWrite(ROUGE, HIGH);
      else digitalWrite(ROUGE, LOW);
      
      // Acknowledgment
      stmp[0] = 101;
      CAN.sendMsgBuf(0x02, 0, LENGTH, stmp);
      stmp[0] = 0;
    } // Fin if
  } // Fin CAN receive
  delay(100);
} // Fin loop

// --- Fin du programme ---
