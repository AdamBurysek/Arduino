#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9 // Pin pro reset čtečky
#define SS_PIN 10 // Pin pro SPI slave select (SS)

// Vytvoření instance MFRC522 čtečky
MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key; // Inicializace klíče

byte clonedUid[10];    // Pole pro ukládání UID originální karty
bool cardRead = false; // Stav, zda byla karta přečtena

void setup()
{
  Serial.begin(9600); // Spuštění sériové komunikace
  SPI.begin();        // Inicializace SPI rozhraní
  mfrc522.PCD_Init(); // Inicializace RFID čtečky
  Serial.println(F("Přiložte originální kartu k RFID čtečce pro přečtení UID."));
}

void loop()
{
  if (!cardRead)
  {
    // Přečtení originální karty
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
      delay(50);
      return;
    }

    // Uložení UID originální karty
    Serial.print(F("UID originální karty:"));
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      clonedUid[i] = mfrc522.uid.uidByte[i]; // Uložení UID
      Serial.print(clonedUid[i] < 0x10 ? " 0" : " ");
      Serial.print(clonedUid[i], HEX);
    }
    Serial.println();

    // Halt originální kartu
    mfrc522.PICC_HaltA();
    cardRead = true;
    Serial.println(F("UID originální karty bylo přečteno. Přiložte klonovatelnou kartu."));
    delay(1000); // Čas na vyjmutí originální karty
  }

  // Když je přečtena originální karta, čekáme na klonovatelnou kartu
  if (cardRead)
  {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
      delay(50);
      return;
    }

    // Změna UID na klonovatelné kartě
    if (mfrc522.MIFARE_SetUid(clonedUid, mfrc522.uid.size, true))
    {
      Serial.println(F("UID úspěšně zapsáno na klonovanou kartu."));
    }
    else
    {
      Serial.println(F("Chyba při zápisu UID na kartu."));
    }

    // Halt klonovanou kartu
    mfrc522.PICC_HaltA();
    cardRead = false; // Reset stavu pro další klonování
    Serial.println(F("Klonování dokončeno. Přiložte další originální kartu pro klonování."));
  }
}
