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

typedef struct  {
    bool is_intesected;
    double x;
    double y;
} CircIntersec;

typedef struct {
    double alpha;
    double beta;
    bool in_limit;
} MotorAngles;

class SCARA{

private:
    Arms _arms;
    const double deg_rad = 180.0 / M_PI;
    const unsigned long dtime = 500;
    const double steps = 10;
    const unsigned long toup = 150;
    const unsigned long todown = 60;
    unsigned long t0;

    bool up_state = false;

    Servo _servo_left;
    Servo _servo_right;
    Servo _servo_updown;

    CircIntersec _get_coord_intersec(double x0, double y0, double x1, double y1,
                                     double r0, double r1);

    MotorAngles _coordinate_to_angles(double xe, double ye);

public:

    SCARA(Servo, Servo, Servo, Arms);
    void set_arms(Arms arms){
        _arms = arms;
    }

    double get_ymax();
    double get_xmean();

    void move(double x, double y);
    void move_updown();

    void line(double x0, double y0, double xf, double yf);

    void rectangle(double x0, double y0, double width, double height);

};

#endif
