#include <Arduino.h>
#include <Servo.h>
#include <SCARA.h>


Servo servo_left;
Servo servo_right;
Servo servo_updow;

const Arms arms = {44, 30, 30, 38, 38};
SCARA scara(servo_left, servo_right, servo_updow, arms);

double ymax;
double xmean;

void setup() {
    servo_left.attach(7);
    servo_right.attach(8);
    servo_updow.attach(4);

    Serial.begin(9600);

    //scara.move(22.8, 60);
    ymax = scara.get_ymax();
    xmean = scara.get_xmean();

    scara.move(xmean, ymax);
    delay(1000);


}

void loop() {

    scara.rectangle(15, 20, 30, 30);
    delay(5000);

}
