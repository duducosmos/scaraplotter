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
    double step;
    int i;
    int j;
} Direction;

class SCARA{
private:
    const Arms _arms;
    const double deg_rad = 180.0 / PI;
    const unsigned long dtime = 500;
    const double steps = 10;
    const unsigned long toup = 150;
    const unsigned long todown = 60;
    unsigned long t0;

    // When in calibration mode, adjust the following factor until the servos move exactly 90 degrees

    unsigned long SERVOFAKTORLEFT = 1;
    unsigned long SERVOFAKTORRIGHT = 1;
    // Zero-position of left and right servo
    // When in calibration mode, adjust the NULL-values so that the servo arms are at all times parallel
    // either to the X or Y axis
    unsigned long SERVOLEFTNULL = 0;
    unsigned long SERVORIGHTNULL = 0;
    bool up_state = false;

    Direction direction;

    Servo _servo_left;
    Servo _servo_right;
    Servo _servo_updown;

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
    void move_updown();

    void circle(double xm, double ym, int r);

    void lineMidPoint(double x0, double y0, double xf, double yf);

    void line(double x0, double y0, double xf, double yf);
    void line2(double x0, double y0, double xf, double yf);
    void lineDDA(double x0, double y0, double xf, double yf);
    void lineBresenham(double x0, double y0, double xf, double yf);
    void rectangle(double x0, double y0, double width, double height);

    void drawnumber(int, double x0, double y0, int scale);


};

#endif
