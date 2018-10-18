#include <Arduino.h>
#include <Servo.h>
#include <SCARA.h>


Servo servo_left;
Servo servo_right;
Servo servo_updow;

const Arms arms = {43, 79, 79, 75, 75};
SCARA scara(servo_left, servo_right, servo_updow, arms);

double ymax;
double xmean;

void setup() {
    servo_left.attach(7);
    servo_right.attach(8);
    servo_updow.attach(4);

    Serial.begin(9600);

    //servo_left.write(0);
    //servo_right.write(0);

    //scara.move(22.8, 60);
    ymax = scara.get_ymax();
    xmean = scara.get_xmean();

    scara.move(xmean, ymax);
    delay(1000);


}

void loop() {
    scara.rectangle(10, 40, 40, 50);
    delay(5000);
}
