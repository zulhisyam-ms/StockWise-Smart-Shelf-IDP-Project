Data analytics:
import streamlit as st
import pandas as pd


# Initial data
if 'data' not in st.session_state:
   st.session_state.data = pd.DataFrame(columns=["Water", "Snack", "Instant Noodle"])


# Function to add values to the table
def add_values():
   new_value1 = st.number_input("Enter value for Water", value=0, key='Water')
   new_value2 = st.number_input("Enter value for Snack", value=0, key='Snack')
   new_value3 = st.number_input("Enter value for Instant Noodle", value=0, key='Instant Noodle')
   if st.button("Add to Table"):
       new_row = pd.DataFrame({"Water": [new_value1], "Snack": [new_value2], "Instant Noodle": [new_value3]})
       st.session_state.data = pd.concat([st.session_state.data, new_row], ignore_index=True)


# Function to plot the line chart
def plot_chart():
   if st.button("Plot Line Chart"):
       st.line_chart(st.session_state.data)


st.title("Analysis Website for total sale")


# Adding values to the table
add_values()


# Display the table
st.write("Table of Values")
st.write(st.session_state.data)


# Plotting the line chart
plot_chart()


Website for real-time tracking:
import streamlit as st
import pandas as pd
import time
from datetime import datetime
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import serial


# Ensure pyserial is imported correctly
try:
   import serial
except ImportError:
   st.error("Please install pyserial by running `pip install pyserial`")
   exit()


# Set up the serial connection (adjust the COM port and baud rate as necessary)
try:
   ser = serial.Serial('COM9', 9600, timeout=0.1)  # Replace 'COM9' with your Arduino's serial port
   ser1 = serial.Serial('COM10', 9600, timeout=0.1)  # Replace 'COM9' with your Arduino's serial port
   ser2 = serial.Serial('COM11', 9600, timeout=0.1)  # Replace 'COM9' with your Arduino's serial port






except serial.SerialException:
   st.error("Could not open serial port. Please check the port name and try again.")
   exit()


# Initialize Streamlit app
st.title('StockWise')
st.header('CoopMart Kolej 10')


# Initialize empty DataFrames for each product
product1_df = pd.DataFrame(columns=["Timestamp", "Product Name", "Price", "Stock", "Action"])
product2_df = pd.DataFrame(columns=["Timestamp", "Product Name", "Price", "Stock", "Action"])
product3_df = pd.DataFrame(columns=["Timestamp", "Product Name", "Price", "Stock", "Action"])


# Dictionary to store daily sales
daily_sales = {
   "Water": {"Total Sell": 0, "Total Price": 0.0},
   "Snack": {"Total Sell": 0, "Total Price": 0.0},
   "Instant Noodle": {"Total Sell": 0, "Total Price": 0.0}
}




# Function to read and parse serial data
def read_serial_data():
   try:
       line = ser.readline().decode('utf-8').strip()


       line1 = ser1.readline().decode('utf-8').strip()


       line2 = ser2.readline().decode('utf-8').strip()


       if line.startswith("Product Name:"):
           name = line.split(": ")[1]
           price = float(ser.readline().decode('utf-8').strip().split(": RM")[1])
           availability = int(ser.readline().decode('utf-8').strip().split(": ")[1])
           action = "Out"  # Assuming every scan results in an 'Out' action
           return datetime.now(), name, price, availability, action


       if line1.startswith("Product Name:"):
           name = line1.split(": ")[1]
           price = float(ser1.readline().decode('utf-8').strip().split(": RM")[1])
           availability = int(ser1.readline().decode('utf-8').strip().split(": ")[1])
           action = "Out"  # Assuming every scan results in an 'Out' action
           return datetime.now(), name, price, availability, action


       if line2.startswith("Product Name:"):
           name = line2.split(": ")[1]
           price = float(ser2.readline().decode('utf-8').strip().split(": RM")[1])
           availability = int(ser2.readline().decode('utf-8').strip().split(": ")[1])
           action = "Out"  # Assuming every scan results in an 'Out' action
           return datetime.now(), name, price, availability, action






   except serial.SerialException as e:
       st.error(f"Serial error: {e}")
   except Exception as e:
       st.error(f"Error reading serial data: {e}")
   return None




# Function to send email alert
def send_email(product_name):
   try:
       # SMTP server configuration
       smtp_server = 'smtp.gmail.com'
       smtp_port = 587
       sender_email = '209997@student.upm.edu.my'
       sender_password = 'Zulhisyam4'
       receiver_email = '212213@student.upm.edu.my'


       # Create a SMTP session
       s = smtplib.SMTP(smtp_server, smtp_port)
       s.starttls()  # Start TLS encryption


       # Login to Gmail
       s.login(sender_email, sender_password)


       # Email content
       subject = 'COOP MART INVENTORY MANAGEMENT DATA'


       # Customize message based on product_name
       if product_name == 'Water':
           message = f"""
           Dear worker,


           This is a reminder to fill up the Stock for Water.


           Best regards,
           Your Manager
           """
       elif product_name == 'Snack':
           message = f"""
           Dear worker,


           This is a reminder to fill up the Stock for Snack.


           Best regards,
           Your Manager
           """
       elif product_name == 'Instant Noodle':
           message = f"""
           Dear worker,


           This is a reminder to fill up the Stock for Instant Noodle.


           Best regards,
           Your Manager
           """
       else:
           # Default message for unknown products
           message = """
           Dear worker,


           This is a reminder to fill up the Stock for a product.


           Best regards,
           Your Manager
           """


       # Construct the message
       msg = MIMEMultipart()
       msg['From'] = sender_email
       msg['To'] = receiver_email
       msg['Subject'] = subject


       # Attach the message body
       msg.attach(MIMEText(message, 'plain'))


       # Send the email
       s.sendmail(sender_email, receiver_email, msg.as_string())


       # Quit the session
       s.quit()
   except Exception as e:
       st.error(f"Error sending email: {e}")




def update_tables():
   global product1_df, product2_df, product3_df


   data = read_serial_data()
   if data:
       timestamp, product_name, price, availability, action = data


       if product_name == "Water":
           # Check if this is the first time availability reaches 0
           if availability == 0 and ("Water" not in product1_df["Product Name"].values or
                                     product1_df.loc[product1_df["Product Name"] == "Water", "Stock"].iloc[-1] != 0):
               product1_df = pd.concat(
                   [pd.DataFrame([data], columns=["Timestamp", "Product Name", "Price", "Stock", "Action"]),
                    product1_df], ignore_index=True)
               product1_table.table(product1_df)
       elif product_name == "Snack":
           # Check if this is the first time availability reaches 0
           if availability == 0 and ("Snack" not in product2_df["Product Name"].values or
                                     product2_df.loc[product2_df["Product Name"] == "Snack", "Stock"].iloc[-1] != 0):
               product2_df = pd.concat(
                   [pd.DataFrame([data], columns=["Timestamp", "Product Name", "Price", "Stock", "Action"]),
                    product2_df], ignore_index=True)
               product2_table.table(product2_df)
       elif product_name == "Instant Noodle":
           # Check if this is the first time availability reaches 0
           if availability == 0 and ("Instant Noodle" not in product3_df["Product Name"].values or
                                     product3_df.loc[product3_df["Product Name"] == "Instant Noodle", "Stock"].iloc[
                                         -1] != 0):
               product3_df = pd.concat(
                   [pd.DataFrame([data], columns=["Timestamp", "Product Name", "Price", "Stock", "Action"]),
                    product3_df], ignore_index=True)
               product3_table.table(product3_df)


       # Update daily sales summary
       update_daily_sales(product_name, 1, price)


       # Send reStock alert if availability is low
       if availability <= 1:
           send_email(product_name)


       # Update summary table
       summary_data = []
       for product_name, sales_info in daily_sales.items():
           summary_data.append({
               "Product Name": product_name,
               "Total Sell": sales_info["Total Sell"],
               "Total Price": sales_info["Total Price"]
           })
       summary_df = pd.DataFrame(summary_data)
       summary_table.table(summary_df)




# Function to update daily sales summary
def update_daily_sales(product_name, quantity, price):
   global daily_sales
   daily_sales[product_name]["Total Sell"] += quantity
   daily_sales[product_name]["Total Price"] += (quantity * price)




# Create placeholders for displaying product information
product1_placeholder = st.empty()
product2_placeholder = st.empty()
product3_placeholder = st.empty()
summary_placeholder = st.empty()  # Placeholder for summary table


# Set up the layout for the tables
with st.container():
   st.subheader("Product Details")


   with st.expander("Water"):
       product1_table = st.empty()


   with st.expander("Snack"):
       product2_table = st.empty()


   with st.expander("Instant Noodle"):
       product3_table = st.empty()


   st.subheader("Today's Sales Summary")
   summary_table = st.empty()




# Function to update the tables and send email alerts
def update_tables():
   global product1_df, product2_df, product3_df


   data = read_serial_data()
   if data:
       timestamp, product_name, price, availability, action = data
       availability -= 1  # Simulate sale decrementing availability by 1


       if product_name == "Water":
           product1_df = pd.concat(
               [pd.DataFrame([data], columns=["Timestamp", "Product Name", "Price", "Stock", "Action"]), product1_df],
               ignore_index=True)
           product1_table.table(product1_df)
       elif product_name == "Snack":
           product2_df = pd.concat(
               [pd.DataFrame([data], columns=["Timestamp", "Product Name", "Price", "Stock", "Action"]), product2_df],
               ignore_index=True)
           product2_table.table(product2_df)
       elif product_name == "Instant Noodle":
           product3_df = pd.concat(
               [pd.DataFrame([data], columns=["Timestamp", "Product Name", "Price", "Stock", "Action"]), product3_df],
               ignore_index=True)
           product3_table.table(product3_df)


       # Update daily sales summary
       update_daily_sales(product_name, 1, price)


       # Send reStock alert if availability is low
       if availability <= 1:
           send_email(product_name)


       # Update summary table
       summary_data = []
       for product_name, sales_info in daily_sales.items():
           summary_data.append({
               "Product Name": product_name,
               "Total Sell": sales_info["Total Sell"],
               "Total Price": sales_info["Total Price"]
           })
       summary_df = pd.DataFrame(summary_data)
       summary_table.table(summary_df)




# Main loop to read data from serial and update the transaction log
while True:
   update_tables()
   time.sleep(0.1)


First partition:
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


Second partition:
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
int led1 = 6;
int led2 = 7;


#define RST_PIN 9
#define SS_PIN 10


byte readCard[4];
String MasterTag = "EC39F12B"; // REPLACE this Tag ID with your Tag ID!!!
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
  {"EC39F12B", "Snack", 1.00, 5}
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
  lcd.print("Snack");
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


Third partition:
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
int led1 = 6;
int led2 = 7;


#define RST_PIN 9
#define SS_PIN 10


byte readCard[4];
String MasterTag = "EE45159F"; // REPLACE this Tag ID with your Tag ID!!!
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
  {"EE45159F", "Instant Noodle", 5.00, 5}
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
  lcd.print("Instant Noodle");
  lcd.setCursor(0, 1);
  lcd.print("RM5.00");
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

