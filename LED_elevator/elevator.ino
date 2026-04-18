// An elevator that reads inputs from buttonArray, stores it into an array and turns on and off lights symbolizing the elevator
// Floor numbers start at 1

// Buttons - for calling the elevator
const int button_1 = A0;
const int button_2 = A1;
const int button_3 = A2;
const int button_4 = A3;
const int button_5 = A4;
const int button_6 = A5;
const int buttonArray[] = {button_1, button_2, button_3, button_4, button_5, button_6};

// Button Lights - to indicate the elevator has been called
const int buttonLED_1 = 2;
const int buttonLED_2 = 3;
const int buttonLED_3 = 4;
const int buttonLED_4 = 5;
const int buttonLED_5 = 6;
const int buttonLED_6 = 7;
const int buttonLEDArray[] = {buttonLED_1, buttonLED_2, buttonLED_3, buttonLED_4, buttonLED_5, buttonLED_6};

// Lights - to indicate the elevator's position
const int elevatorLED_1 = 8;
const int elevatorLED_2 = 9;
const int elevatorLED_3 = 10;
const int elevatorLED_4 = 11;
const int elevatorLED_5 = 12;
const int elevatorLED_6 = 13;
const int elevatorLEDArray[] = {elevatorLED_1, elevatorLED_2, elevatorLED_3, elevatorLED_4, elevatorLED_5, elevatorLED_6};

// Queue Logic
const int queueSize = 6; // Equal to the number of floors
int elevatorQueue[queueSize]; // Initializes a queue the same size as the number of floors
int elevatorPosition = 1; // Starts on first floor

// Functions
void addToQueue(int floorNumber) {
// Looks for the last 0 in the queue and replaces it with floorNumber
  for (int i = 0; i < queueSize; i++) {
    if (elevatorQueue[i] == floorNumber) { // Breaks if floorNumber is already in the queue
      break;
    } else if (elevatorQueue[i] == 0) { // Looks for the first 0 in the queue
      elevatorQueue[i] = floorNumber;
      digitalWrite(buttonLEDArray[floorNumber - 1], HIGH); // Turn on LED indicating button call
      break;
    }
  }
}

void removeFirstFromQueue() {
// Removes the first position in the queue and moves everything else forward
  for (int i = 0; i < queueSize - 1; i++) {
    elevatorQueue[i] = elevatorQueue[i + 1];
  }
  elevatorQueue[queueSize - 1] = 0; // Last position is always empty
}

void setElevatorLEDOn(int floorNumber) {
  // Turn off all LEDS
  for (int i = 0; i < queueSize; i++) {
    digitalWrite(elevatorLEDArray[i], LOW);
  }
  // Turn on a specific floorLED
  if (floorNumber >= 1 && floorNumber <= queueSize) {
    digitalWrite(elevatorLEDArray[floorNumber - 1], HIGH);
  }
}

void moveElevator(int destinationFloorNumber) {
  int direction = destinationFloorNumber > elevatorPosition ? 1 : -1; // 1 if the elevator has to move up, -1 if it has to move down
  int floorsDifference = abs(destinationFloorNumber - elevatorPosition);
  if (destinationFloorNumber >= 1 && destinationFloorNumber <= queueSize) {
    // Read all buttons while moving
    for (int i = 0; i < floorsDifference; i++) { // For each floor the elevator has to travel
      for (int i = 0; i < queueSize; i++) { // For each button in the system
        if (digitalRead(buttonArray[i]) == HIGH) { // Add pushed buttonts to queue
          addToQueue(i + 1);
        }
        Serial.print(elevatorQueue[i]); // And print every element in the queue
      }
      Serial.println();
      Serial.print("Going from: ");
      Serial.print(elevatorPosition);

      elevatorPosition += direction;
      setElevatorLEDOn(elevatorPosition);

      Serial.print(" to: ");
      Serial.println(elevatorPosition);
      delay(1000); // Steps between floors
    }
    delay(1000); // Resting time between queue items (i.e: doors open and close)
  }
  digitalWrite(buttonLEDArray[destinationFloorNumber-1], LOW); // Turns off the floor light in which the elevator stopped
}

void setup() {
  pinMode(elevatorLED_1, OUTPUT);
  pinMode(elevatorLED_2, OUTPUT);
  pinMode(elevatorLED_3, OUTPUT);
  pinMode(elevatorLED_4, OUTPUT);
  pinMode(elevatorLED_5, OUTPUT);
  pinMode(elevatorLED_6, OUTPUT);
  pinMode(buttonLED_1, OUTPUT);
  pinMode(buttonLED_2, OUTPUT);
  pinMode(buttonLED_3, OUTPUT);
  pinMode(buttonLED_4, OUTPUT);
  pinMode(buttonLED_5, OUTPUT);
  pinMode(buttonLED_6, OUTPUT);
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(button_3, INPUT);
  pinMode(button_4, INPUT);
  pinMode(button_5, INPUT);
  pinMode(button_6, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Elevator starts always at the ground floor --> 1
  setElevatorLEDOn(elevatorPosition);
  
  // Read all buttonArray looking for button calls to add to the queue
  for (int i = 0; i < queueSize; i++) {
    if (digitalRead(buttonArray[i]) == HIGH) {
      addToQueue(i + 1);
    }
  }

  // Move the elevator to the first position in the queue
  moveElevator(elevatorQueue[0]);
  
  // Update queue moving all elements one position ahead
  removeFirstFromQueue();

  // Print queue status (just for debugging and better understanding)
  for (int i = 0; i < queueSize; i++) {
    Serial.println(elevatorQueue[i]);
  }
}