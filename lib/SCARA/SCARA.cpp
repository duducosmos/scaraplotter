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
        double n = r1*r1-r0*r0 - x1*x1 + x0*x0 -y1*y1 +y0*y0;
        n = n / (2.0 * dy);
        double a = (dx / dy) * (dx / dy) + 1.0;
        double b = 2.0 * y0 * (dx / dy) - 2.0 * n * (dx / dy) - 2.0 * x0;

        double c =  x0 * x0 + y0 * y0 + n * n -r0 * r0 - 2.0 * y0 * n;
        double delta = b * b - 4.0 * a * c;

        CircIntersec circint;

        if(delta < 0){
            circint.is_intesected = false;
            return circint;
        }

        delta = sqrt(delta);

        circint.is_intesected = true;
        circint.xa = (-b + delta) / 2 * a;
        circint.xb = (-b - delta) / 2 * a;
        circint.ya = n - circint.xa * dx / dy;
        circint.yb = n - circint.xb * dx / dy;


        return circint;



}

MotorAngles SCARA::_coordinate_to_angles(double xe, double ye){

    MotorAngles motor_angle;
    CircIntersec circint0 = _get_coord_intersec(xe, ye, 0, 0,
                                                _arms.AC, _arms.CE);

    if(circint0.is_intesected == false){
        motor_angle.in_limit = false;
        motor_angle.alpha = 90;
        motor_angle.beta = 90;
        return motor_angle;
    }


    CircIntersec circint1 = _get_coord_intersec(xe, ye, _arms.AB, 0,
                                                _arms.BD, _arms.DE);

    if(circint1.is_intesected == false){
        motor_angle.in_limit = false;
        motor_angle.alpha = 90;
        motor_angle.beta = 90;
        return motor_angle;
    }

    double xIA;
    double yIA;

    double xIB;
    double yIB;

    if(circint0.xa > circint0.xb){
        xIA = circint0.xb;
        yIA = circint0.yb;
    }else{
        xIA = circint0.xa;
        yIA = circint0.ya;
    }

    if(circint1.xa < circint1.xb){
        xIB = circint1.xb;
        yIB = circint1.yb;
    }else{
        xIB = circint1.xa;
        yIB = circint1.ya;
    }


    motor_angle.in_limit = true;

    motor_angle.alpha = 180.0 - atan2(yIA, xIA)* deg_rad;

    motor_angle.beta = atan2(yIB, xIB - _arms.AB) * deg_rad;

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
