# MS-can bus emulator

**Used devices**:
- arduino pro mini
- MCP2515

**Connections**:
MCP2515 | Arduino
------- | -------
INT|D2
SCK|D13
SI|D11
SO|D12
CS|D10
GND|GND
VCC|VCC

Arduino | Description
------- | -------
A0 | arrow right
A1 | airbag
A2 | arrow left
A3 | open doors

The indicator is on when the arduino input is connected to **GND** (INPUT_PULLUP). 

Open doors indicator works as "_driver's door is open_".

Other functions is not implemented.
