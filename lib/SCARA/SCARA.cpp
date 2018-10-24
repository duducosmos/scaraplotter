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
    double tmp = _arms.AC * _arms.AC + _arms.AB * _arms.AB / 4.0;
    tmp = tmp - _arms.CE * _arms.CE;
    double _delta = 4.0 * _arms.AC * _arms.AC - 4.0 * tmp;
    double ymax = max((2.0 * _arms.AC + sqrt(_delta)) / 2.0,
                      (2.0 * _arms.AC - sqrt(_delta)) / 2.0);
    return ymax;
}

double SCARA::get_xmean(){
    return _arms.AB / 2.0;

}


CircIntersec SCARA::_get_coord_intersec(double x0, double y0, double x1,
                                        double y1, double r0, double r1){
        if(y0 == y1){
            y0 += 0.01;
        }
        double dx = x1 - x0;
        double dy = y1 - y0;
        double d = sqrt(dx*dx + dy*dy);
        double x = (d - r1*r1 / d + r0*r0 / d) / 2.0;
        double s = r0 * r0 - x * x;
        CircIntersec circint;
        if(s < 0){
            circint.is_intesected = false;
            circint.x = 0;
            circint.y = 0;
            return circint;

        }

        double y = sqrt(s);

        circint.is_intesected = true;
        circint.x = (x * dx - y * dy) / d + x0;
        circint.y = (x * dy + y * dx) / d + y0;

        return circint;
}

MotorAngles SCARA::_coordinate_to_angles(double xe, double ye){

    MotorAngles motor_angle;
    CircIntersec circint0 = _get_coord_intersec(-_arms.AB / 2.0, 0, xe, ye,
                                                _arms.AC, _arms.CE);

    if(circint0.is_intesected == false){
        motor_angle.in_limit = false;
        motor_angle.alpha = 90;
        motor_angle.beta = 90;
        return motor_angle;
    }


    CircIntersec circint1 = _get_coord_intersec(xe, ye, _arms.AB / 2.0, 0,
                                                _arms.BD, _arms.DE);

    if(circint1.is_intesected == false){
        motor_angle.in_limit = false;
        motor_angle.alpha = 90;
        motor_angle.beta = 90;
        return motor_angle;
    }


    motor_angle.in_limit = true;

    motor_angle.alpha = 180.0 - atan2(circint0.y, - _arms.AB / 2.0 - circint0.x) * deg_rad;

    motor_angle.beta = atan2(circint1.y, circint1.x -  _arms.AB / 2.0) * deg_rad;

    Serial.println(" ");

    Serial.print("Alpha: ");
    Serial.print(motor_angle.alpha );
    Serial.print(", Beta: ");
    Serial.println(motor_angle.beta);
    return motor_angle;




}


void SCARA::line(double x0, double y0, double xf, double yf){
    /* Using DDA algorithm */

    double dx = xf - x0;
    double dy = yf - y0;
    double step;
    if(abs(dx) >= abs(dy)){
        step = abs(dx);
    }else{
        step = abs(dy);
    }

    dx = dx / step;
    dy = dy / step;

    int i = 0;
    double x = x0;
    double y = y0;

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

    double rect[4][4] = {{x0, y0, x0, y0 + height},
                         {x0, y0 + height, x0 + width, y0 + height},
                         {x0 + width, y0 + height, x0 + width, y0},
                         {x0 + width, y0, x0, y0}
                     };
    for(int i=0; i<4;i++){
        line(rect[i][0], rect[i][1], rect[i][2], rect[i][3]);
    }


    up_state = true;
    _servo_updown.write(toup);
}


void SCARA::move(double x, double y){
    t0 = millis();
    while(millis() - t0 <= dtime);

    MotorAngles motor_angle = _coordinate_to_angles(x, y);

    if(motor_angle.in_limit == true){
        _servo_left.write(motor_angle.alpha);
        _servo_right.write(motor_angle.beta);
    }




}
