#include <Bluepad32.h>
#include <ESP32Servo.h>

// Motor control pins
#define motorPin1A 12
#define motorPin2A 27
//const int enablePinA = 16;
#define motorPin1B 26
#define motorPin2B 25
//const int enablePinB = 5;


#define continuousServo1Pin 15 
#define continuousServo2Pin 32
#define servo180_1Pin 14
#define servo180_2Pin 4

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

Servo continuousServo1;
Servo continuousServo2;

Servo servo180_1;
Servo servo180_2;

int servoPos = 90;


// ??





// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {
        Serial.println("Unsupported controller");
      }
    }
  }
}





void processGamepad(ControllerPtr ctl) {
    //controlMotor(motorPin1B, motorPin2B, ctl->axisY());
  
    //processPivot(ctl->axisX());
    processAux(ctl->dpad());

    controlMotor(motorPin1B, motorPin2B, ctl->axisY());
    controlMotor(motorPin1A, motorPin2A, ctl->axisRY());

    servoSteer(ctl->axisRX());

    stationaryServo(ctl->x(), ctl->b());
    
    //Serial.print(ctl->axisRY());
    //Serial.print(ctl->axisY());

    
    // if (ctl->a() == 1) {
    //   servo180_1.write(180);
    // } else if (ctl->y() == 1) {
    //   servo180_1.write(0);
    // } else {
    //   servo180_1.write(90);
    // }

    // if (ctl->x() == 1) {
    //   servo180_2.write(180);
    // } else if (ctl->b() == 1) {
    //   servo180_2.write(0);
    // } else {
    //   servo180_2.write(90);
    // }
    
  
    //processBucket(ctl->axisRX());
  
    //processAux(ctl->dpad());


}

void stationaryServo(int ctrlButton1, int ctrlButton2) {
  if (ctrlButton1 == 1) {
      servoPos = servoPos + 15;
  } else if (ctrlButton2 == 1) {
      servoPos = servoPos - 15;
  }
  servo180_2.write(servoPos);
}

void servoSteer(int value) {
  Serial.println(value);
  value = map(value, -512, 512, 0, 180);

  Serial.print("mapped angle is: ");
  Serial.println(value);

  if (value < 70 || value > 110) {
    servo180_1.write(value);
    //Serial.println("angle adjusted");
  } else {
    servo180_1.write(90);
  }
}

void processAux(int dpadValue) {
  if (dpadValue == 1) {
    continuousServo1.write(0);
  } else if (dpadValue == 2) {
    continuousServo1.write(180);
  } else {
    continuousServo1.write(90);
  }
  if (dpadValue == 4) {
    continuousServo2.write(0);
  } else if (dpadValue == 8) {
    continuousServo2.write(180);
  } else {
    continuousServo2.write(90);
  }
}




void controlMotor(int pin1, int pin2, int value)
{
  if (value < -100) {  // reverse speeds
    digitalWrite(pin2, LOW);
    digitalWrite(pin1, HIGH);

  } else if (value > 100) { // stop or forward
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }
  //Serial.println("attempting to control motors");
  //Serial.println(value);
}


// Arduino setup function. Runs in CPU 1
void setup() {
    Serial.begin(115200);
    // Initialize motor and servo pins
    pinMode(motorPin1A, OUTPUT);
    pinMode(motorPin2A, OUTPUT);
    //pinMode(enablePinA, OUTPUT);
    pinMode(motorPin1B, OUTPUT);
    pinMode(motorPin2B, OUTPUT);
    //pinMode(enablePinB, OUTPUT);

    continuousServo1.attach(continuousServo1Pin);
    continuousServo2.attach(continuousServo2Pin);
    servo180_1.attach(servo180_1Pin);
    servo180_2.attach(servo180_2Pin);







    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedController, &onDisconnectedController);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();

    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
    // This call fetches all the controllers' data.
    // Call this function in your main loop.
    bool dataUpdated = BP32.update();
    if (dataUpdated) {
        processControllers();
    }
    // The main loop must have some kind of "yield to lower priority task" event.
    // Otherwise, the watchdog will get triggered.
    // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
    // Detailed info here:
    // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

    //     vTaskDelay(1);
    else { vTaskDelay(1); }
    delay(150);
}
