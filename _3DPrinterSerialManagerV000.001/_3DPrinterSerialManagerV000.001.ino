/*
 * File: 3DPrinterSerialManager.ino
 * Version: v000.001
 * Date: 2018-04-16
 * Author: Fernando Figueiredo - DaGmaRobotics
 * 
 * 
 * Description: Manages a 3D Printer from a serial port.
 * 
 * 
 * 
 * Licence: GNU GENERAL PUBLIC LICENSE
 */


//Definitions
const String fileName="prog";
const String fileExtention=".gco";

//IOs
#define ciSelect1 2
#define ciSelect2 3
#define ciSelect3 4
#define ciSelect4 5
#define ciStart 6
#define ciReady 7
#define ciOnBoardLED 13

//Routine declarations
void ConfigureSerial();
void StartProgram(int iProgNum);
void CheckSerialPort();
void CheckSerialPort1();
void ProcessMessage(byte bPort);
void ProcessMessage1(byte bPort);
bool xProgSelect[8];
bool xStart;

bool xPrinterReady;
long lLastPrinterCheck;
int iRequestedProgram;

void setup() {
  pinMode(ciSelect1, INPUT);
  pinMode(ciSelect2, INPUT);
  pinMode(ciSelect3, INPUT);
  pinMode(ciSelect4, INPUT);
  pinMode(ciStart, INPUT);
  pinMode(ciReady, OUTPUT);
  pinMode(ciOnBoardLED, OUTPUT);
  pinMode(8, OUTPUT);
  
  ConfigureSerial();
  Serial.println("3D Printer Manager!!!");
  Serial.println("Game ON!!!");
  digitalWrite(8, false);
  digitalWrite(ciOnBoardLED, true);
  digitalWrite(ciReady, true);
  delay(1000);
  digitalWrite(ciOnBoardLED, false);
  digitalWrite(ciReady, false);
  delay(1000);
  digitalWrite(ciOnBoardLED, true);
  digitalWrite(ciReady, true);  
  delay(1000);
  digitalWrite(ciOnBoardLED, false);
  digitalWrite(ciReady, true);
  xPrinterReady = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  CheckSerialPort();
  CheckSerialPort1();
  xProgSelect[0] = digitalRead(ciSelect1);
  xProgSelect[1] = digitalRead(ciSelect2);
  xProgSelect[2] = digitalRead(ciSelect3);
  xProgSelect[3] = digitalRead(ciSelect4);
  xStart = digitalRead(ciStart);
  iRequestedProgram = 0;
  if (xProgSelect[0])
    iRequestedProgram +=1;
  if (xProgSelect[1])
    iRequestedProgram +=2;
  if (xProgSelect[2])
    iRequestedProgram +=4;
  if (xProgSelect[3])
    iRequestedProgram +=8;
  if (xStart and xPrinterReady)
    StartProgram(iRequestedProgram);
}


void ConfigureSerial() {
  Serial.begin(115200);
  Serial.setTimeout(0);
  Serial1.begin(115200);
  Serial1.setTimeout(0);
}


void StartProgram(int iProgNum){
  Serial1.println("M23 " + fileName + String(iProgNum) + fileExtention);
  delay(100);
  Serial1.println("M24");
  digitalWrite(ciReady, false);
  xPrinterReady = false;
}

//Serial Port
byte bPendingMEssage[50];
int iPendingBytes;
byte bRecievedBytes[50];
byte bAuxBytes[4];
int iMessageLength;
String sRecievedMessage;
//Serial Port1
byte bPendingMEssagePort1[50];
int iPendingBytesPort1;
byte bRecievedBytesPort1[50];
byte bAuxBytesPort1[4];
int iMessageLengthPort1;
String sRecievedMessagePort1;


/*
   Check if there is any received byte in serial Port
   Buffering is done by software i.e. serial timeout is 0
   Any recieved byte is stored in an array and processor is passed on
   for an othe task
*/
void CheckSerialPort() {
  //Check if there is info available in serial Port
  if (Serial.available()) {
    iMessageLength = Serial.readBytes(bRecievedBytes, 12);
    if (iPendingBytes + iMessageLength <= 100) {
      for (int i = 0; i < iMessageLength; i++) {
        //Add new bytes to buffer
        bPendingMEssage[iPendingBytes] = bRecievedBytes[i];
        iPendingBytes ++;
        if (bRecievedBytes[i] == (byte)'\r' || bRecievedBytes[i] == (byte)'\n') {
          //process message and clear buffer
          ProcessMessage(0);
          iPendingBytes = 0;
        }
      }
    }
    else {
      iPendingBytes = 0;
    }
  }
}


/*
  Processar mensagens recebidas
*/
void ProcessMessage(byte bPort) {
  String sRecievedMessage = String((char *)bPendingMEssage);
  sRecievedMessage = sRecievedMessage.substring(0, iPendingBytes - 1);
  //Program request
  if (sRecievedMessage.substring(0, 2) == "rp" || sRecievedMessage.substring(0, 2) == "RP") {
    iRequestedProgram = sRecievedMessage.substring(2, iPendingBytes - 1).toInt();
    StartProgram(iRequestedProgram);
  }
  else if (sRecievedMessage == "p?" || sRecievedMessage == "P?") {
    Serial.println("Selected Program = " + String(iRequestedProgram));
  }
  else{
    Serial1.println(sRecievedMessage);
  }
}







/*
   Check if there is any received byte in serial Port
   Buffering is done by software i.e. serial timeout is 0
   Any recieved byte is stored in an array and processor is passed on
   for an othe task
*/
void CheckSerialPort1() {
  //Check if there is info available in serial Port
  if (Serial1.available()) {
    iMessageLengthPort1 = Serial1.readBytes(bRecievedBytesPort1, 12);
    if (iPendingBytesPort1 + iMessageLengthPort1 <= 100) {
      for (int i = 0; i < iMessageLengthPort1; i++) {
        //Add new bytes to buffer
        bPendingMEssagePort1[iPendingBytesPort1] = bRecievedBytesPort1[i];
        iPendingBytesPort1 ++;
        if (bRecievedBytesPort1[i] == (byte)'\r' || bRecievedBytesPort1[i] == (byte)'\n') {
          //process message and clear buffer
          ProcessMessagePort1(0);
          iPendingBytesPort1 = 0;
        }
      }
    }
    else {
      iPendingBytesPort1 = 0;
    }
  }
}

/*
  Processar mensagens recebidas
*/
void ProcessMessagePort1(byte bPort) {
  String sRecievedMessagePort1 = String((char *)bPendingMEssagePort1);
  sRecievedMessagePort1 = sRecievedMessagePort1.substring(0, iPendingBytesPort1 - 1);
  Serial.println(sRecievedMessagePort1);
  //Fast commands
  //Power Off
  if (sRecievedMessagePort1 == "Done printing file") {
    digitalWrite(ciReady, true);
    xPrinterReady = true;
  }

  
  

}
  

