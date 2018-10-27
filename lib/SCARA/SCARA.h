#include <Arduino.h>
#include <Servo.h>

/*
SCARA - Selective Compilance Assembly Robot Arm.
Parallel Arm robot.
*/

#ifndef SCARA_H
#define SCARA_H
typedef struct {

    /*
    *         (x,y)
    *           E
    *          /\
    *         /  \
    *        /    \
    *       /      \
    *      /        \
    *     /          \
    *  C |            | D
    *    |            |
    *    |            |
    *    |)alpha beta(|
    *    A            B
    *
    *    Arms Configuration
    *
    *   y ^
    *     |
    *     |
    *     |
    *     o-------------------> x
    *    (A)---->0   (B)---->
    */

    double AB;
    double AC;
    double BD;
    double DE;
    double CE;

} Arms;


typedef struct {
    bool is_intersected;
    double xs1;
    double xs2;
} InterCircum;

class SCARA{
private:
    const Arms _arms;
    const double deg_rad = 180.0 / PI;
    const unsigned long dtime = 500;
    const double steps = 10;
    const unsigned long toup = 150;
    const unsigned long todown = 60;
    unsigned long t0;

    bool up_state = false;

    Servo _servo_left;
    Servo _servo_right;
    Servo _servo_updown;


public:

    SCARA(Servo, Servo, Servo, Arms);

    double get_ymax();
    double get_xmean();

    InterCircum get_intersection(double, double, double, double, double, double);

    void move(double x, double y);
    void move_updown();
    void line(double x0, double y0, double xf, double yf);
    void rectangle(double x0, double y0, double width, double height);


};

#endif
