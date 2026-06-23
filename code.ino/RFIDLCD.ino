#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int led1 = 6;
int led2 = 7;

#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
String MasterTag = "FC7D159F"; // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";

// Product data structure
struct Product {
  String id;
  String name;
  float price;
  int availability;
};

// Create an array of products
Product products[] = {
  {"FC7D159F", "Water", 2.00, 5}
};

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  lcd.begin();
  lcd.backlight();
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  lcd.clear();
  lcd.print("Water");
  lcd.setCursor(0, 1);
  lcd.print("RM2.00");
  lcd.print("  ");
  lcd.print("Stock:5");
  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
  // Wait until a new tag is available
  if (getID()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    bool productFound = false;

    // Debugging: Print scanned tag ID
    Serial.print("Scanned Tag ID: ");
    Serial.println(tagID);

    // Check if the scanned tag matches the product
    for (Product &product : products) {
      if (tagID.equals(product.id)) {
        productFound = true;

        // Update availability if greater than 0
        if (product.availability > 0) {
          product.availability--;
        }

        // Display product information along with availability status
        lcd.clear();
        lcd.print(product.name);
        lcd.setCursor(0, 1);
        lcd.print("RM");
        lcd.print(product.price);
        lcd.print(" Stock:");
        lcd.print(product.availability);

        // Print product information to Serial Monitor
        Serial.print("Product Name: ");
        Serial.println(product.name);
        Serial.print("Price: RM");
        Serial.println(product.price);
        Serial.print("Stock: ");
        Serial.println(product.availability);

        // Wait a short time before updating availability status
        delay(500);

        // Display availability status
        lcd.clear();
        if (product.availability == 0) {
          lcd.print("Out of Stock");
        } else if (product.availability <= 2) {
          lcd.print("Hot-Selling, Please");
          lcd.setCursor(0, 1);
          lcd.print("restock the product");
        } else if (product.availability <= 3) {
          lcd.print("Hot-Selling!");
        } else {
          lcd.print("Not Hot-Selling");
        }

        delay(500);

        // Continue displaying product name, price, availability, and status
        lcd.clear();
        lcd.print(product.name);
        lcd.setCursor(0, 1);
        lcd.print("RM");
        lcd.print(product.price);
        lcd.print(" Stock:");
        lcd.print(product.availability);
        lcd.setCursor(0, 2);
        if (product.availability == 0) {
          lcd.print("Out of Stock");
        } else if (product.availability <= 2) {
          lcd.print("Hot-Selling, restock");
        } else if (product.availability <= 3) {
          lcd.print("Hot-Selling!");
        } else {
          lcd.print("Not Hot-Selling");
        }

        break;
      }
    }

    if (!productFound) {
      lcd.clear();
      lcd.print("Unknown Product");
      Serial.println("Unknown Product");
      delay(500);
    }
  }
}

// Read new tag if available
boolean getID() {
  // Getting ready for Reading PICCs
  if (!mfrc522.PICC_IsNewCardPresent()) { // If a new PICC placed to RFID reader continue
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial()) { // Since a PICC placed get Serial and continue
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase(); // Convert to uppercase
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}
