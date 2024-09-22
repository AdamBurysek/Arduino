#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9 // Pin pro reset čtečky
#define SS_PIN 10 // Pin pro SPI slave select (SS)

// Vytvoření instance MFRC522 čtečky
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
    Serial.begin(9600); // Spuštění sériové komunikace
    SPI.begin();        // Inicializace SPI rozhraní
    mfrc522.PCD_Init(); // Inicializace RFID čtečky
    Serial.println(F("Přiložte kartu k RFID čtečce"));
}

void loop()
{
    // Kontrola, zda je přítomna nová karta
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
        delay(50); // Malé zpoždění, aby nedocházelo k zahlcení
        return;
    }

    // Vypsání UID karty
    Serial.print(F("UID karty:"));
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); // Pokud je hodnota menší než 0x10, přidej nulu
        Serial.print(mfrc522.uid.uidByte[i], HEX);                // Vypisuje UID v hexadecimálním formátu
    }
    Serial.println();

    // Halt PICC (zastavení komunikace s kartou)
    mfrc522.PICC_HaltA();
}
