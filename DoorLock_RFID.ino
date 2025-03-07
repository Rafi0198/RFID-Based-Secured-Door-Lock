#include <SPI.h>
#include <MFRC522.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x3F, 16, 2);

#define SS_PIN 10
#define RST_PIN 9
byte card_pin = 6;
byte ring_pin = 7;
byte relay = 11;

#define card "40 EF 81 7C"
#define tag "F0 58 C0 80"

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME 2 RAFI'S DOOR LOCK");
  lcd.setCursor(0, 1);
  lcd.print("SCAN YOUR CARD  ");
  
  pinMode(card_pin, OUTPUT);
  pinMode(ring_pin, OUTPUT);
  pinMode(relay, OUTPUT);

}

void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == card) //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access to this card");
    Serial.println();
    digitalWrite(6, HIGH);
    digitalWrite(relay, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("CARD SCANNED    ");
    lcd.setCursor(0, 1);
    lcd.print("ACCESS GRANTED  ");

    delay(3000);
    digitalWrite(6, LOW);
    digitalWrite(relay, LOW);
    lcd.print("WELCOME 2 RAFI'S DOOR LOCK");
    lcd.setCursor(0, 1);
    lcd.print("SCAN YOUR CARD  ");

  }
  else if (content.substring(1) == tag)
  {
    Serial.println("Authorized access to this tag");
    Serial.println();
    digitalWrite(7, HIGH);
    digitalWrite(relay, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("CARD SCANNED    ");
    lcd.setCursor(0, 1);
    lcd.print("ACCESS GRANTED  ");

    delay(3000);
    digitalWrite(7, LOW);
    digitalWrite(relay, LOW);

    lcd.setCursor(0, 0);
    lcd.print("WELCOME 2 RAFI'S DOOR LOCK");
    lcd.setCursor(0, 1);
    lcd.print("SCAN YOUR CARD  ");

  }
  else
  {
    Serial.println(" Access denied");

    lcd.setCursor(0, 0);
    lcd.print("CARD SCANNED!   ");
    lcd.setCursor(0, 1);
    lcd.print("ACCESS DENIED!  ");
    delay(3000);

    lcd.setCursor(0, 0);
    lcd.print("WELCOME 2 RAFI'S DOOR LOCK");
    lcd.setCursor(0, 1);
    lcd.print("SCAN YOUR CARD  ");
  }
}
