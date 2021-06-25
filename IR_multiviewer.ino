#include <IRremote.h>

#define IR_SEND_PIN 3

#include <IRremote.h>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    /*
     * The IR library setup. That's all!
     */
    IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin

    Serial.print(F("Ready to send IR signals at pin "));
    Serial.println(IR_SEND_PIN);
}


void loop() {
  uint16_t sAddress = 0x80;
uint8_t sRepeats = 4;
  // put your main code here, to run repeatedly:
while (Serial.available()){
       String r = Serial.readString();
       r.trim();
       if (r == "quad"){
        Serial.println("0x8");
            uint8_t sCommand = 0x8;
            IrSender.sendNEC(sAddress, sCommand, sRepeats);

       }
       else if (r == "Cam1"){
        Serial.println("0x7");
         uint8_t sCommand = 0x7;
            IrSender.sendNEC(sAddress, sCommand, sRepeats);
       }
       else if (r == "Cam2"){
        Serial.println("0x9");
         uint8_t sCommand = 0x9;
            IrSender.sendNEC(sAddress, sCommand, sRepeats);
       }
       else if (r == "Cam3"){
         uint8_t sCommand = 0x5;
            IrSender.sendNEC(sAddress, sCommand, sRepeats);
       }
       else if (r == "Cam4"){
        Serial.println("0x1B");
         uint8_t sCommand = 0x1B;
            IrSender.sendNEC(sAddress, sCommand, sRepeats);
       }
    }
}
