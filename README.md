# Servo auto tracking line

This Arduino code is designed for a line-following robot, where the robot tracks a line on the ground using infrared sensors and adjusts its movement accordingly. The code utilizes servo motors to control the movement of the robot's wheels based on the input from the sensors.

## Hardware Requirements
- Arduino board
- Servo motors (4)
- Tracking line sensor

## Pin Configuration
- `PIN_SERVO_LEFT_FORWARD`: Pin number for the left forward servo motor
- `PIN_SERVO_LEFT_BACKWARD`: Pin number for the left backward servo motor
- `PIN_SERVO_RIGHT_FORWARD`: Pin number for the right forward servo motor
- `PIN_SERVO_RIGHT_BACKWARD`: Pin number for the right backward servo motor
- `PIN_TRACKING_LEFT`: Pin number for the left tracking sensor
- `PIN_TRACKING_CENTER`: Pin number for the center tracking sensor
- `PIN_TRACKING_RIGHT`: Pin number for the right tracking sensor

## Speed Configuration
- `SPEED_FORWARD`: Speed value for forward movement
- `SPEED_STOP`: Speed value for stopping
- `SPEED_TURN_LV1` to `SPEED_TURN_LV4`: Speed values for different levels of turning

## Functions
- `setup()`: Initializes the pin configurations.
- `loop()`: Continuously reads the sensor values and adjusts the motor speeds accordingly.
- `getTrackingSensorValue()`: Reads the values from the tracking sensors and returns a binary representation.
- `configurePins()`: Configures the pins for servo motors and sensor inputs.
- `executeMotor(int speedLeft, int speedRight)`: Controls the movement of the robot based on the specified speeds for the left and right wheels.
- `resetCartAction()`: Stops the movement of the robot.

## Usage
1. Connect the servo motors and the tracking line sensor to the specified pins on the Arduino board
2. Upload the code to the Arduino
3. Place the robot on a surface with a line to follow
4. The robot will continuously adjust its movement to follow the line based on the sensor inputs until all the 3 sensors detect a line, in that case it will stop

## License
This code is provided under the MIT License. Feel free to modify and distribute it according to your project's needs

For more information, visit [Arduino](https://www.arduino.cc/) and [Servo Library](https://www.arduino.cc/en/Reference/Servo)

