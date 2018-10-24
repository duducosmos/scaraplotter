#include <Arduino.h>
#include <Servo.h>
#include <SCARA.h>
#include <ButtonPanel.h>
#include <Controller.h>

Servo servo_left;
Servo servo_right;
Servo servo_updow;

//Arms arms = {43.5, 44., 44., 43., 43.};
Arms arms = {44, 45, 45, 45, 45};
SCARA scara(servo_left, servo_right, servo_updow, arms);

ButtonPanel buttonsPanel(A0);

Controller controller(scara);

double ymax;
double xmean;


void setup() {

    pinMode(11, INPUT_PULLUP);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);

    servo_left.attach(7);
    servo_right.attach(8);
    servo_updow.attach(4);

    controller.attachButtonPanel(&buttonsPanel);

    Serial.begin(9600);

    servo_updow.write(180);

    ymax = scara.get_ymax();
    xmean = scara.get_xmean();

    scara.move(0, ymax);
    //delay(5000);
    servo_updow.write(90);
    //delay(5000);


}

double error_y(double x){
    double a = 8.0 / 34.0;

    double b = 20 * a;

    return a * x + b;
}

void loop() {

    //buttonsPanel.update();
    delay(1);
    if(digitalRead(11) == LOW){
        scara.move_updown();
    }
    double x = map(analogRead(A0), 0,1023, -20, 20);
    double y = map(analogRead(A1), 0, 1023, 0, ymax);
    double delta_y = error_y(x);

    Serial.print("x: ");
    Serial.print(x);
    Serial.print(", y: ");
    Serial.print(y);

    Serial.print(", delta_y: ");
    Serial.println(delta_y);

    scara.move(x, y);
    delay(100);



    //scara.rectangle(xmean, ymax / 2, 20, 20);
    //scara.line(xmean, ymax/2, xmean, ymax);



}
