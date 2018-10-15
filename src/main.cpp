#include <Arduino.h>
#include <Servo.h>
#include <SCARA.h>


Servo servo_left;
Servo servo_right;
Servo servo_updow;

const Arms arms = {45, 30, 30, 38, 38};
SCARA scara(servo_left, servo_right, servo_updow, arms);

double ymax;

void setup() {
    servo_left.attach(7);
    servo_right.attach(8);
    servo_updow.attach(4);

    Serial.begin(9600);

    //scara.move(22.8, 60);
    ymax = scara.get_ymax();
    double xmean = scara.get_xmean();

    scara.move(xmean, ymax);
    delay(1000);

    scara.lineGoldgerg(40, 50, 20, 20);
    delay(5000);

}

void loop() {
    /*

    for(int i = 10; i < 50; i++){
        for(int j = 10; j < (int) ymax; j++){
            Serial.println("");
            Serial.print("x: ");
            Serial.println(i);
            Serial.print("y: ");
            Serial.println(j);
            scara.move(i, j);
            delay(100);
        }
    }
    */




}
