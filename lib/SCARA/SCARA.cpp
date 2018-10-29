#include <SCARA.h>


SCARA::SCARA(Servo servo_left, Servo servo_right, Servo servo_updow, Arms arms):
_arms(arms), _servo_left(servo_left),
_servo_right(servo_right), _servo_updown(servo_updow)
{
    t0 = millis();

}

void SCARA::move_updown(){
    if(up_state == false){
        up_state = true;
        _servo_updown.write(toup);
    }else{
        up_state = false;
        _servo_updown.write(todown);
    }

}


double SCARA::get_ymax(){
    double b = - 2.0 * _arms.AC;
    double c = (_arms.AB * _arms.AB / 4.0) + _arms.AC * _arms.AC - _arms.CE * _arms.CE;
    double delta = b * b - 4.0 * c;
    double x1 = (- b + sqrt(delta)) / 2.0;
    double x2 = (- b - sqrt(delta)) / 2.0;
    return max(x1, x2);
}

double SCARA::get_xmean(){
    return _arms.AB / 2.0;

}


InterCircum SCARA::get_intersection(double x0, double y0, double r0,
                                    double x1, double y1, double r1)
{
    InterCircum intercircum;
    intercircum.is_intersected = false;

    double dx = x1 - x0;
    double dy = y1 - y0;
    if(dy == 0){
        return intercircum;
    }

    double d = sqrt(dx * dx + dy * dy);

    if(r0 + r1 > d && d > sqrt((r0 - r1) * (r0 -r1))){
        intercircum.is_intersected = true;

        double n = (d + r0 + r1) * ( d + r0 - r1);
        n = n * (d - r0 + r1) * (-d + r0 + r1);
        n = sqrt(n) / 4.0;

        double x = ((x0 + x1) / 2.0) + dx * (r0 * r0 - r1 * r1) / (2.0 * d * d);

        intercircum.xs1 = x + 2 * n * dy / (d * d);
        intercircum.xs2 = x - 2 * n * dy / (d * d);

    }

    return intercircum;
}

void SCARA::move(double x, double y){

    /*

    Serial.print("x: ");
    Serial.print(x);
    Serial.print(", y:");
    Serial.println(y);
    */


    InterCircum intercircum = get_intersection(-_arms.AB / 2.0, 0, _arms.AC,
                                               x, y,  _arms.CE);
    double _alpha;
    double _beta;
    double tmp;

    if(intercircum.is_intersected == true){
        tmp = -_arms.AB / 2.0  - min(intercircum.xs1, intercircum.xs2);
        tmp = tmp / _arms.AC;
        _alpha = acos(tmp) * deg_rad;
        _alpha = 180.0 - _alpha;

        InterCircum intercircum2 = get_intersection(_arms.AB / 2.0, 0, _arms.AC,
                                                   x, y,  _arms.CE);

    /*
       Serial.print("xs1: ");
       Serial.print(intercircum.xs1);
       Serial.print(", xs2: ");
       Serial.println(intercircum.xs2);
       */

        if(intercircum2.is_intersected == true){
           tmp = max(intercircum2.xs1, intercircum2.xs2) - _arms.AB / 2.0;
           tmp = tmp / _arms.BD;
           _beta = acos(tmp) * deg_rad;

           t0 = millis();
           while(millis() - t0 <= dtime);

           /*

           Serial.print("Alpha: ");
           Serial.print(((int) _alpha));
           Serial.print(", Beta: ");
           Serial.println(((int) _beta + 1));
           */

           _servo_right.write(((int) _beta + 1));
           _servo_left.write(((int) _alpha));
        }
    }
}






void SCARA::line(double x0, double y0, double xf, double yf){

    double dx = xf - x0;
    double dy = yf - y0;
    double step;
    double x = x0;
    double y = y0;
    int i = 0;

    if(abs(dx) >= abs(dy)){
        step = abs(dx);
    }else{
        step = abs(dy);
    }

    dx = dx / step;
    dy = dy / step;

    while(i <= step){

        Serial.print("x: ");
        Serial.print(x);
        Serial.print(", y: ");
        Serial.println(y);
        move(x, y);
        x = x + dx;
        y = y + dy;
        i = i + 1;
    }

}


void SCARA::rectangle(double x0, double y0, double width, double height){

    up_state = true;
    _servo_updown.write(toup);
    move(x0, y0);
    up_state = false;
    _servo_updown.write(todown);


    line(x0, y0, x0, y0 + height);
    line(x0, y0 + height, x0 + width, y0 + height);
    line(x0 + width, y0 + height, x0 + width, y0);
    line(x0 + width, y0, x0, y0);

    up_state = true;
    _servo_updown.write(toup);
}
