/*
 *    uCam.cpp
 *
 *    http://www.4dsystems.com.au/productpages/uCAM-II/downloads/uCAM-II_datasheet_R_1_4.pdf
 */

#include "uCamII.h"

#define IMAGE_SIZE   45
//#define cameraDebugSerial

// From http://pastebin.com/ZuMu48w7

byte _SYNC_COMMAND[6] = {0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00};
byte _SYNC_ACK_REPLY[6] = {0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};
byte _SYNC_ACK_REPLY_EXT[6] = {0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00};
//byte _SYNC_FINAL_COMMAND[6] = {0xAA, 0x0E, 0x00, 0x00, 0xF5, 0x00};
byte _SYNC_FINAL_COMMAND[6] = {0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};

byte _INITIAL_COMMAND[6] = {0xAA, 0x01, 0x00, 0x07, 0x09, 0x05};
//byte _INITIAL_COMMAND[6] = {0xAA, 0x05, 0x00, 0x07, 0x09, 0x05};
byte _GENERIC_ACK_REPLY[6] = {0xAA, 0x0E, 0x00, 0x00, 0x00, 0x00};
byte _PACK_SIZE[6] = {0xAA, 0x06, 0x08, UCAMII_BUF_SIZE + 6, 0x00, 0x00};
byte _SNAPSHOT[6] = {0xAA, 0x05, 0x00, 0x00, 0x00, 0x00};
byte _GET_PICTURE[6] = {0xAA, 0x04, 0x01, 0x00, 0x00, 0x00};


UCAMII::UCAMII() {
  this->image_pos = 0;
  this->package_no = 0;
}

void UCAMII::setDebug(SoftwareSerial *interface) {
  this->debugSerial = interface;
}

boolean UCAMII::init() {
#ifdef cameraDebugSerial
  if (this->debugSerial) {
    this->debugSerial->println("Intitial is starting to be sent");
  }
#endif
  if (this->attempt_sync()) {
#ifdef cameraDebugSerial
    Serial.println("\n\rCam has ACKED the SYNC");
#endif
    return true;
  }

  return false;

}

int UCAMII::attempt_sync() {
  int attempts = 0;
  byte cam_reply;
  int ack_success = 0;
  int last_reply = 0;

  while (attempts < 20 && ack_success == 0) {
    // Flush
    //while (Serial1.available());
    delay(200);
    //delay(100);
#ifdef cameraDebugSerial
    Serial.println("Sending SYNC...");
#endif
    for (int i = 0; i < 6; i++) {
      Serial1.write(_SYNC_COMMAND[i]);
    }
	attempts++;

    if (this->wait_for_bytes(_SYNC_ACK_REPLY) ) {
      if (this->wait_for_bytes(_SYNC_ACK_REPLY_EXT)) {
        delay(200);
#ifdef cameraDebugSerial
        Serial.println("\r\nSending FINAL SYNC...");
#endif
        for (int i = 0; i < 6; i++) {
          Serial1.write(_SYNC_FINAL_COMMAND[i]);
        }
        return 1;
      }
    }

  }

  return 0;
}


// Return number of packages ready
int UCAMII::numberOfPackages() {
  return this->imageSize / UCAMII_BUF_SIZE;
}


int UCAMII::send_initial() {

  // flush
  while (Serial1.available() > 0) {
    Serial1.read();
  }

  //delay(100);
  delay(50);
  
  if(digitalRead(IMAGE_SIZE))
  _INITIAL_COMMAND[5] = 0x07;
  else
  _INITIAL_COMMAND[5] = 0x05;

#ifdef cameraDebugSerial
  this->debugSerial->println("Sending INITIALISE...");
#endif
  for (int i = 0; i < 6; i++) {
    Serial1.write(_INITIAL_COMMAND[i]);
  }
  // @todo why 500 delay?
  //delay(500);
  delay(100);
  if (this->wait_for_bytes(_GENERIC_ACK_REPLY)) {
#ifdef cameraDebugSerial
    this->debugSerial->println("INITIALISE success");
#endif
    return 1;
  }

#ifdef cameraDebugSerial
  this->debugSerial->println("INITIALISE fail");
#endif

  return 0;
}

boolean UCAMII::wait_for_bytes(byte command[6]) {
  byte cam_reply;
  int i = 0;
  int received;
  short found_bytes;
  found_bytes = 0;
  // @todo millis() wait, millis() wrap around watch out
  // unsigned long start = millis();

#ifdef cameraDebugSerial
  this->debugSerial->print("\r\nWAIT: ");
  for (i = 0; i < 6; i++) {
    this->debugSerial->print("0x");
    this->debugSerial->print(command[i], HEX);
    this->debugSerial->print(" ");
  }
  this->debugSerial->print("\r\nGOT : ");
  i = 0;
#endif

  while (Serial1.available()) {
    cam_reply = Serial1.read();
    if (i < 6 ) {
      if ((cam_reply == command[i]) || command[i] == 0x00) {
        found_bytes++;
        i++;
      }
    }

#ifdef cameraDebugSerial
    this->debugSerial->print("0x");
    this->debugSerial->print(cam_reply, HEX);
    this->debugSerial->print(" ");
#endif
    received++;
    if (found_bytes == 6) {
      return true;
    }
  }
  return false;
}



bool UCAMII::takePicture() {
  if (send_initial()) {
    if (this->set_package_size()) {
      if (this->do_snapshot()) {
        if (get_picture()) {
          return 1;
        }
      }
    }
  }
}


int UCAMII::set_package_size() {

  byte ack[6] = {0xAA, 0x0E, 0x06, 0x00, 0x00, 0x00};

  //delay(100);
  //delay(50);
#ifdef cameraDebugSerial
  Serial.println("Sending packet size...");
#endif

  for (int i = 0; i < 6; i++) {
    Serial1.write(_PACK_SIZE[i]);
#ifdef cameraDebugSerial
    Serial.print(_PACK_SIZE[i], HEX);
    Serial.print(" ");
#endif

  }
  // @todo why 500 delay?
  //delay(500);
  delay(50);
  if (this->wait_for_bytes(ack)) {
#ifdef cameraDebugSerial
    Serial.println("\r\npacket size success");
#endif
    return 1;
  }

#ifdef cameraDebugSerial
  Serial.println("packet size fail");
#endif

  return 0;
}

int UCAMII::do_snapshot() {

  byte ack[6] = {0xAA, 0x0E, 0x05, 0x00, 0x00, 0x00};

  //delay(100);

#ifdef cameraDebugSerial
  Serial.println("Sending snapshot...");
#endif


  for (int i = 0; i < 6; i++) {
    Serial1.write(_SNAPSHOT[i]);
  }
  // @todo why 500 delay?
  //delay(500);
  delay(100);
  if (this->wait_for_bytes(ack)) {
#ifdef cameraDebugSerial
    Serial.println("snapshot success");
#endif
    return 1;
  }

#ifdef cameraDebugSerial
  Serial.println("snapshot fail");
#endif

  return 0;

}

int UCAMII::get_picture() {

  byte ack[6] = {0xAA, 0x0E, 0x04, 0x00, 0x00, 0x00};
  unsigned long imageSize;
  short i;

  //delay(100);

#ifdef cameraDebugSerial
  Serial.println("Sending get picture...");
#endif

  for (int i = 0; i < 6; i++) {
    Serial1.write(_GET_PICTURE[i]);
  }
  // @todo why 500 delay?
  //delay(500);
  delay(50);
  if (this->wait_for_bytes(ack)) {
#ifdef cameraDebugSerial
    Serial.println("picture success");
#endif
    // get the 6 bytes ACK
    for (i = 0; i <= 5; i++) {
      ack[i] = 0;
      while (!Serial1.available());

      ack[i] = Serial1.read();
      // last 3 bytes are the image size
#ifdef cameraDebugSerial
      Serial.print(i, DEC);
      Serial.print(" value: ");
      Serial.println(ack[i], HEX);
#endif
    }

    imageSize = 0;
    imageSize = (imageSize << 8) | ack[5];
    imageSize = (imageSize << 8) | ack[4];
    imageSize = (imageSize << 8) | ack[3];

    this->imageSize = imageSize;
    this->image_pos = this->imageSize;
    this->package_no = 0;
    
    if (imageSize > 0) {
      return 1;
    }
  }

#ifdef cameraDebugSerial
  Serial.println("picture fail");
#endif

  return 0;
}

// @todo return false if time exceeded
int UCAMII::getData() {

  unsigned char high = (unsigned char)(this->package_no >> 8);
  unsigned char low  = this->package_no & 0xff;
  byte my_ack[6] = {0xAA, 0x0E, 0x00, 0x00, low, high};

  int i = 0;
  byte s;
  int bytes;
  
  if(this->image_pos ==0 ) {
    return 0;
  }
  // request bytes
  for (int i = 0; i < 6; i++) {
    Serial1.write(my_ack[i]);
    //Serial.print(my_ack[i],HEX);
    //Serial.print(" ");
  }
    //Serial.print("\n");

  // Set number of bytes we should wait for
  if (this->image_pos < UCAMII_BUF_SIZE) {
    bytes = this->image_pos + 6;
  } else {
    bytes = UCAMII_BUF_SIZE + 6;
  }
#ifdef cameraDebugSerial
    this->debugSerial->print("REMAINING: ");
    this->debugSerial->print(this->image_pos, DEC);
    this->debugSerial->print(" BYTES PER CHUNK: ");
    this->debugSerial->println(bytes, DEC);
#endif

  for (i = 0; i < bytes; i++) {
    while (!Serial1.available()); // wait for bytes
    s = Serial1.read();
    // Skip first 4 and last 2, Page 10 of the datasheet
    if (i >= 4 && i < bytes - 2) {
#ifdef cameraDebugSerial
      this->debugSerial->print("*");
#endif
      this->imgBuffer[i - 4] = s;
      this->image_pos--;
    }
#ifdef cameraDebugSerial
    this->debugSerial->print(this->imgBuffer[s], HEX);
    this->debugSerial->print(" ");
#endif
  }


#ifdef cameraDebugSerial
  this->debugSerial->println("");
#endif

  this->package_no++;
  //if(this->package_no == (this->numberOfPackages()+1))
  //this->package_no = 0;
  if(this->image_pos <= 0) {
    // send the final thank you goodbye package
    my_ack[4] = 0xF0;
    my_ack[5] = 0xF0;
    for (int i = 0; i < 6; i++) {
      Serial1.write(my_ack[i]);
      //Serial.print(my_ack[i],HEX);
      //Serial.print(" ");
    }
      //Serial.print("\n");
  }
  return bytes-6;
}



