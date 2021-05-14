#include <SPI.h>
#include <mcp2515.h>

struct can_frame can_msg;
MCP2515 mcp2515(10);

void setup() {
  while (!Serial);
  Serial.begin(115200);
  SPI.begin();
  pinMode(A0, INPUT_PULLUP);  // left arrow
  pinMode(A1, INPUT_PULLUP);  // airbag
  pinMode(A2, INPUT_PULLUP);  // right arrow
  pinMode(A3, INPUT_PULLUP);  // doors
  
  mcp2515.reset();
  mcp2515.setBitrate (CAN_125KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  can_msg.can_id  = 0x80;
  can_msg.can_dlc = 8;
  can_msg.data[0] = 0x15;
  can_msg.data[1] = 0x05;
  can_msg.data[2] = 0x0E;
  can_msg.data[3] = 0x0D;
  can_msg.data[4] = 0x1A;
  can_msg.data[5] = 0x1E;
  can_msg.data[6] = 0x00;
  can_msg.data[7] = 0x00;
  mcp2515.sendMessage(&can_msg);
}

void send_arrows_status() {
  bool has_left_arrow = digitalRead(A0) == LOW;
  bool has_right_arrow = digitalRead(A2) == LOW;
  if (has_left_arrow || has_right_arrow) {
    can_msg.can_id  = 0x265;
    if (has_left_arrow) {
      if (has_right_arrow) {
        can_msg.data[0] = 0x60;
      } else {
        can_msg.data[0] = 0x20;
      }
    } else {
        can_msg.data[0] = 0x40;
    }
    mcp2515.sendMessage(&can_msg);
  }
}

void send_airbag_status() {
  can_msg.can_id  = 0x460;
  can_msg.data[0] = 0x0;
  can_msg.data[1] = 0x0;
  can_msg.data[2] = 0x0;
  can_msg.data[3] = 0x0;
  can_msg.data[4] = (digitalRead(A1) == LOW) ? 0x0 : 0xC0;
  can_msg.data[5] = 0x0;
  can_msg.data[6] = 0x0;
  can_msg.data[7] = 0x0;
  mcp2515.sendMessage(&can_msg);
}

void send_doors_status() {
  if (digitalRead(A3) == LOW) {
    can_msg.can_id  = 0x433;
    can_msg.data[0] = 0x80;
    can_msg.data[2] = 0x00;
    can_msg.data[4] = 0x20;
    mcp2515.sendMessage(&can_msg);
  }
}

void loop() {
  send_arrows_status();
  send_airbag_status();
  send_doors_status();
  delay(250);
}
