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
    double dx;
    double dy;
    double xr;
    double yr;
    double fxy;
    int i;
    int j;
} Direction;

class SCARA{
private:
    const Arms _arms;
    const double deg_rad = 180.0 / PI;
    const unsigned long dtime = 100;
    const double step = 0.1;
    unsigned long t0;

    Direction direction;

    Servo _servo_left;
    Servo _servo_right;
    Servo _servo_updow;

    double thetaB(double x, double y);
    double thetaD(double x, double y);
    double thetaA(double x, double y);
    double thetaC(double x, double y);
    double be(double x, double y);
    double ae(double x, double y);

    void _start_direction();

    void set_direction(double x0, double y0, double xf, double yf);



public:

    SCARA(Servo, Servo, Servo, Arms);

    double get_ymax();
    double get_xmean();

    double beta(double x, double y);
    double alpha(double x, double y);
    void move(double x, double y);

    void line(double x0, double y0, double xf, double yf);


};

#endif
