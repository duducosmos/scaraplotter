#include <Arduino.h>
#include <Servo.h>
#include <SCARA.h>
#include <ButtonPanel.h>
#include <Controller.h>

Servo servo_left;
Servo servo_right;
Servo servo_updow;

const Arms arms = {43.5, 44.5, 44.5, 45, 45};
SCARA scara(servo_left, servo_right, servo_updow, arms);

ButtonPanel buttonsPanel(A0);

Controller controller(scara);

double ymax;
double xmean;

void setup() {
    servo_left.attach(7);
    servo_right.attach(8);
    servo_updow.attach(4);

    controller.attachButtonPanel(&buttonsPanel);

    Serial.begin(9600);

    //servo_updow.write(180);

    ymax = scara.get_ymax();
    xmean = scara.get_xmean();

    scara.move(0, ymax);
    //delay(5000);
    //servo_updow.write(90);
    //delay(5000);


}

void loop() {

    //buttonsPanel.update();
    //delay(1);

    scara.rectangle(-10, ymax / 2, 30, 30);
    delay(1000);




}
