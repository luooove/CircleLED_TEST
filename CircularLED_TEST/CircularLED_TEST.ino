#define GLB_CMDMODE 0x00  // Work on 8-bit mode
unsigned char LED[24];
int k = 0;
unsigned int CmdMode = 0b0000000000000000;
int __pinData = 2;
int __pinClock = 0;
unsigned char __state[24];// Led state, brightness for each LED
void latchData()
{
  digitalWrite(__pinData, LOW);
  delayMicroseconds(10);

  for (unsigned char i = 0; i < 8; i++)
  {
    digitalWrite(__pinData, HIGH);
    digitalWrite(__pinData, LOW);

  }
}
// Send 16 bits of data
void sendData(unsigned int data)
{
  for (unsigned char i = 0; i < 16; i++)
  {
    unsigned int state = (data & 0x8000) ? HIGH : LOW;
    digitalWrite(__pinData, state);

    state = digitalRead(__pinClock) ? LOW : HIGH;
    digitalWrite(__pinClock, state);

    data <<= 1;
  }
}
void setData(unsigned char __state[])
{

  sendData(GLB_CMDMODE);

  for (unsigned char i = 0; i < 12; i++)
  {
    // Go forward on __state
    sendData(__state[i]);
  }
  sendData(GLB_CMDMODE);
  for (unsigned char i = 12; i < 24; i++)
  {
    // Go forward on __state
    sendData(__state[i]);
  }
  // Two extra empty bits for padding the command to the correct length
  //  sendData(0x00);
  //  sendData(0x00);

  latchData();
}

void setBits(unsigned int bits)
{

  for (unsigned char i = 0; i < 24; i++)
  {

    if ((bits % 2) == 1)
      __state[i] = 0xff;
    else
      __state[i] = 0x00;
    bits /= 2;
  }

  setData(__state);
}




void setup()
{
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}
unsigned int state;
void loop()
{
  for (int i = 0; i < 24; i++)
  {
    LED[i] = 0;
  }
  LED[k] = 0xff;
  k++;
  if (k == 24)
    k = 0;

  //  //CircularLEDWrite();
  //  delay(40);
  //    for (int i = 0; i <= 1023; i++)
  //  {
  //    setBits(i);
  //    delay(50);
  //  }
  //
  //  for (int i = 0; i < 8; i++)
  //  {
  //    // Bit shift left and update
  //    state <<= 1;
  //    setBits(state);
  //    delay(200);
  //  }
  //    for (int i = 0; i < 8; i++)
  //  {
  //    // Bit shift right and update
  //    state >>= 1;
  //    setBits(state);
  //    delay(200);
  //  }

  //setBits(0x3ff);
  //  delay(1000);
  //  setBits(0b000000000000000);
  //  delay(1000);

  setData(LED);
  delay(100);
}


