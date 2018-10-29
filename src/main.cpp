#include <Arduino.h>
#include <Servo.h>
#include <SCARA.h>
#include <ControlPanel.h>
#include <Controller.h>

Servo servo_left;
Servo servo_right;
Servo servo_updow;

const Arms arms = {44.2, 44.2, 44.2, 47.5, 47.5};
SCARA scara(servo_left, servo_right, servo_updow, arms);

ControlPanel controlPanel(A0, A1, 11, -20, 20, 10, 80);

Controller controller(scara);

double ymax;
double xmean;

void setup() {
    servo_left.attach(7);
    servo_right.attach(8);
    servo_updow.attach(4);

    controlPanel.registerOberver(&controller);


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

    controlPanel.update();
    delay(1);

    //scara.rectangle(-10, ymax / 2, 30, 30);
    //delay(1000);

}
