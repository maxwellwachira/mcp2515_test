#include <Arduino.h>

#include <CAN.h> // the OBD2 library depends on the CAN library
#include <OBD2.h>

#define MCP2515_CS 10 //MCP Chip select/ slave select
#define MCP2515_INT 2 //Should be connected to Interrupt pin

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("OBD2 By Sandeep Senior aka Wachira the Master"));

  while (true) {
    Serial.print(F("Attempting to connect to OBD2 CAN bus ... "));

    CAN.setPins(MCP2515_CS, MCP2515_INT);
    CAN.setClockFrequency(8E6);
    delay(1000);

    if (!OBD2.begin()) {
      Serial.println(F("failed!"));

      delay(1000);
    } else {
      Serial.println(F("success"));
      break;
    }
  }

  //If this works, we are safe
  Serial.println();
  Serial.print("VIN = ");
  Serial.println(OBD2.vinRead());
  Serial.print("ECU Name = ");
  Serial.println(OBD2.ecuNameRead());
  Serial.println();
}

void loop() {
  // loop through PIDs 0 to 95, reading and printing the values

  //uncomment for loop to print Sensor Data on Serial Monitor

  // for (int pid = 0; pid < 96; pid++) {
  //   processPid(pid);
  // }
  // Serial.println();


  Serial.println("Reading Confirmed DTCs .......");
  Serial.print("All Confirmed DTC: ");
  String allConfirmedDtcs = OBD2.getAllDTCs(0x03);
  Serial.println(allConfirmedDtcs);
  delay(1000);

  // Serial.println("Reading Pending DTCs .......");
  // Serial.print("All Confirmed DTC: ");
  // String allPendingDtcs = OBD2.getAllDTCs(0x07);
  // Serial.println(allPendingDtcs);
  // delay(1000);

  // Serial.println("Reading Permanent DTCs .......");
  // Serial.print("All permanent DTC: ");
  // String allPermanentDtcs = OBD2.getAllDTCs(0x0a);
  // Serial.println(allPermanentDtcs);

  // wait 5 seconds before next run
  delay(3000);
}

void processPid(int pid) {
  if (!OBD2.pidSupported(pid)) {
    // PID not supported, continue to next one ...
    return;
  }

  // print PID name
  Serial.print(OBD2.pidName(pid));
  Serial.print(F(" = "));

  if (OBD2.pidValueRaw(pid)) {
    // read the raw PID value
    unsigned long pidRawValue = OBD2.pidReadRaw(pid);

    Serial.print(F("0x"));
    Serial.print(pidRawValue, HEX);
  } else {
    // read the PID value
    float pidValue = OBD2.pidRead(pid);

    if (isnan(pidValue)) {
      Serial.print("error");
    } else {
      // print value with units

      Serial.print(pidValue);
      Serial.print(F(" "));
      Serial.print(OBD2.pidUnits(pid));
    }
  }

  Serial.println();
}

