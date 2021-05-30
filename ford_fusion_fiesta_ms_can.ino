#include <SPI.h>
#include <mcp2515.h>

struct can_frame can_msg;
MCP2515 mcp2515(10);

void setup() {
  while (!Serial);
  Serial.begin(115200);
  SPI.begin();
  pinMode(A0, INPUT_PULLUP);  // right arrow
  pinMode(A1, INPUT_PULLUP);  // airbag
  pinMode(A2, INPUT_PULLUP);  // left arrow
  pinMode(A3, INPUT_PULLUP);  // doors
  
  mcp2515.reset();
  mcp2515.setBitrate (CAN_125KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  can_msg.can_dlc = 8;
  can_msg.data[0] = 0x00;
  can_msg.data[1] = 0x00;
  can_msg.data[2] = 0x00;
  can_msg.data[3] = 0x00;
  can_msg.data[4] = 0x00;
  can_msg.data[5] = 0x00;
  can_msg.data[6] = 0x00;
  can_msg.data[7] = 0x00;
}

void sendArrowsStatus() {
  can_msg.can_id  = 0x265;
  bool has_right_arrow = digitalRead(A0) == LOW;
  bool has_left_arrow = digitalRead(A2) == LOW;
  if (has_left_arrow || has_right_arrow) {
    if (has_left_arrow) {
      if (has_right_arrow) {
        can_msg.data[0] = 0x60;
      } else {
        can_msg.data[0] = 0x20;
      }
    } else {
        can_msg.data[0] = 0x40;
    }
  } else {
    can_msg.data[0] = 0x00;
  }
  mcp2515.sendMessage(&can_msg);
}

void sendAirbagStatus() {
  can_msg.can_id  = 0x460;
  can_msg.data[0] = 0x0;
  can_msg.data[1] = 0x0;
  can_msg.data[2] = 0x0;
  can_msg.data[3] = 0x0;
  can_msg.data[4] = 0x00; // (digitalRead(A1) == LOW) ? 0x00 : 0xC0;
  can_msg.data[5] = 0x0;
  can_msg.data[6] = 0x0;
  can_msg.data[7] = 0x0;
  mcp2515.sendMessage(&can_msg);
}

void sendDoorsStatus() {
  can_msg.can_id  = 0x433;
  can_msg.data[2] = 0x00;
  can_msg.data[4] = 0x20;
  if (digitalRead(A3) != LOW) {
    can_msg.data[0] = 0x80;
  } else {
    can_msg.data[0] = 0x00;
  }
  mcp2515.sendMessage(&can_msg);
}

void loop() {
  sendArrowsStatus();
  sendAirbagStatus();
  sendDoorsStatus();
}
