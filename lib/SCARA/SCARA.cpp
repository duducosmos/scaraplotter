#include <SCARA.h>


SCARA::SCARA(Servo servo_left, Servo servo_right, Servo servo_updow, Arms arms):
_arms(arms), _servo_left(servo_left),
_servo_right(servo_right), _servo_updown(servo_updow)
{
    t0 = millis();
    _start_direction();

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

void SCARA::_start_direction(){
    direction.dx = 0;
    direction.dy = 0;
    direction.xr = 0;
    direction.yr = 0;
    direction.i = 0;
    direction.j = 0;
    direction.fxy = 0;
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

void SCARA::set_direction(double x0, double y0, double xf, double yf){
    direction.dy = yf - y0;
    if(direction.dy < 0){
        direction.j = -1;
    }else {
        direction.j = 1;
    }

    direction.dy = abs(direction.dy);

    direction.dx = xf - x0;

    if(direction.dx < 0){
        direction.i = -1;
    }else{
        direction.i = 1;
    }

    direction.dx = abs(direction.dx);

    direction.fxy = direction.dx - direction.dy;
}

void SCARA::line2(double x0, double y0, double xf, double yf){

    direction.dx = abs(xf - x0);
    direction.dy = abs(yf - y0);
    direction.i = x0 < xf ? 1: -1;
    direction.j = y0 < yf ? 1: -1;
    double x = x0;
    double y = y0;
    long over = 0;
    move(x, y);
    if(direction.dx > direction.dy){
        for(int i = 0; i < (int) direction.dx; i++){

            x += direction.i;
            over += direction.dy;
            if(over >= direction.dx){
                over -= direction.dx;
                y += direction.j;
            }
            Serial.print("x: ");
            Serial.print(x);
            Serial.print(", y: ");
            Serial.println(y);
            move(x, y);
        }
    }else{
        for(int i=0; i < (int) direction.dy; i++){

            y += direction.j;
            over += direction.dx;
            if(over >= direction.dy){
                over -= direction.dy;
                x += direction.i;

            }
            Serial.print("x: ");
            Serial.print(x);
            Serial.print(", y: ");
            Serial.println(y);
            move(x, y);
        }
    }

    move(xf, yf);

}



void SCARA::lineBresenham(double x0, double y0, double xf, double yf){
    direction.dx = abs(xf - x0);
    direction.dy = abs(yf - y0);
    direction.i = x0 < xf ? 1: -1;
    direction.j = y0 < yf ? 1: -1;
    float error = direction.dx + direction.dy;
    float e2;
    double x  = x0;
    double y = y0;


    while(true){
        move(x, y);
        if( x == xf && y == yf){
            break;
        }
        Serial.print("x: ");
        Serial.print(x);
        Serial.print(", y: ");
        Serial.println(y);

        e2 = 2 * error;
        if(e2 >= direction.dy){
            error += direction.dy;
            x += direction.i;
        }
        if(e2 <= direction.dx){
            error += direction.dx;
            y += direction.j;
        }


    }

}

void SCARA::lineMidPoint(double x0, double y0, double xf, double yf){


    direction.dx = xf - x0;
    direction.dy = yf - y0;

    direction.step = 1;

    int n = (int) direction.dx / direction.step;
    double d = direction.dy - direction.dx / 2;
    double x = x0;
    double y = y0;
    move(x, y);

    int i = 0;


    while(i <  n){
        x = x + direction.step;
        if(d < 0){
            d = d + direction.dy;
        }else{
            d = d + direction.dy - direction.dx;
            y = y + direction.step;
        }
        move(x, y);

        Serial.print("x: ");
        Serial.print(x);
        Serial.print(", y: ");
        Serial.println(y);

        i++;

    }


}



void SCARA::lineDDA(double x0, double y0, double xf, double yf){

    direction.dx = xf - x0;
    direction.dy = yf - y0;
    if(abs(direction.dx) >= abs(direction.dy)){
        direction.step = abs(direction.dx);
    }else{
        direction.step = abs(direction.dy);
    }

    direction.dx = direction.dx / direction.step;
    direction.dy = direction.dy / direction.step;

    int i = 0;
    double x = x0;
    double y = y0;

    while(i <= direction.step){

        Serial.print("x: ");
        Serial.print(x);
        Serial.print(", y: ");
        Serial.println(y);
        move(x, y);
        x = x + direction.dx;
        y = y + direction.dy;
        i = i + 1;
    }

}

void SCARA::line(double x0, double y0, double xf, double yf){
    if(yf > get_ymax()){
        yf = get_ymax();
    }

    if(y0 > get_ymax()){
        y0 = get_ymax();
    }

    if(y0 < 0){
        y0 = 0;
    }

    if(yf < 0){
        yf = 0;
    }

    if(x0 < 0){
        x0 = 0;
    }

    if(xf < 0){
        xf = 0;
    }

    /*
    TODO: Verify xmax;
    */


    _start_direction();
    set_direction(x0, y0, xf, yf);


    /*
    TODO: Move to x0, y0, with Up
    */

    move(x0, y0);

    double nx = abs(direction.xr - direction.dx) / steps;
    double ny = abs(direction.yr - direction.dy) / steps;


    while((direction.xr != direction.dx) || (direction.yr != direction.dy)){

        direction.xr =  direction.xr + nx;
        //direction.fxy = direction.fxy - direction.dy;

        direction.yr = direction.yr + ny;
        //direction.fxy = direction.fxy + direction.dx;



        move(x0 + direction.xr * direction.i, y0 + direction.yr * direction.j);

        /*
        Serial.println("");
        Serial.print("xr: ");
        Serial.println(x0 + direction.xr * direction.i);
        Serial.println(nx);

        Serial.print("yr: ");
        Serial.println(y0 + direction.yr * direction.j);
        Serial.println(ny);
        Serial.println(direction.yr);
        */
    }
    //move(get_xmean(), get_ymax());
    //Serial.println("Fim");

}


void SCARA::drawnumber(int n, double x0, double y0, int scale){

    double base = 15.0;
    double altura = 30.0;


    up_state = true;
    _servo_updown.write(toup);
    move(x0, y0 + altura);
    up_state = false;
    _servo_updown.write(todown);

    double dois[5][4] = {{x0, y0 + altura, x0 + base, y0 + altura},
                      {x0 + base, y0 + altura, x0 + base, y0 + altura / 2.0},
                      {x0 + base, y0 + altura / 2.0, x0, y0 + altura / 2.0},
                      {x0, y0 + altura / 2.0, x0, y0},
                      {x0, y0, x0 + base, y0},
                    };

    for(int i=0; i < 5; i++){
        Serial.print("i: ");
        Serial.println(i);
        lineDDA(dois[i][0], dois[i][1], dois[i][2], dois[i][3]);
        Serial.println(" ");


    }
    up_state = true;
    _servo_updown.write(toup);

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
        lineDDA(rect[i][0], rect[i][1], rect[i][2], rect[i][3]);       
    }






    /*



    lineBresenham(x0, y0, x0, y0 + height);
    lineBresenham(x0, y0 + height, x0 + width, y0 + height);
    lineBresenham(x0 + width, y0 + height, x0 + width, y0);
    lineBresenham(x0 + width, y0, x0, y0);
    */

    /*


    line2(x0, y0, x0, y0 + height);
    line2(x0, y0 + height, x0 + width, y0 + height);
    line2(x0 + width, y0 + height, x0 + width, y0);
    line2(x0 + width, y0, x0, y0);




    line(x0, y0, x0, y0 + height);
    line(x0, y0 + height, x0 + width, y0 + height);
    line(x0 + width, y0 + height, x0 + width, y0);
    line(x0 + width, y0, x0, y0);
    */

    up_state = true;
    _servo_updown.write(toup);
}


void SCARA::move(double x, double y){
    int _beta = beta(x, y);
    int _alpha = alpha(x, y);

    Serial.println("");



    Serial.print("Beta: ");
    Serial.println(SERVOFAKTORRIGHT * _beta + SERVORIGHTNULL);
    Serial.print("Alpha: ");
    Serial.println(SERVOFAKTORLEFT * _alpha + SERVOLEFTNULL);
    Serial.println("");


    t0 = millis();
    while(millis() - t0 <= dtime);

    _servo_right.write(SERVOFAKTORRIGHT * _beta + SERVORIGHTNULL);
    _servo_left.write(SERVOFAKTORLEFT * _alpha + SERVOLEFTNULL);


}

double SCARA::thetaB(double x, double y){
    double tmp = abs(x - _arms.AB / 2.0);
    if(tmp == 0.0){
        return 90;
    }
    tmp = atan2(y, tmp) * deg_rad;

    return tmp;
}

double SCARA::thetaD(double x, double y){

    double _tmp = be(x, y);

    double tmp = _tmp * _tmp - _arms.DE *  _arms.DE + _arms.BD * _arms.BD;


    double tmp2 = 2.0 * _tmp * _arms.BD;

    tmp = tmp / tmp2;

    tmp = acos(tmp) * deg_rad;

    return tmp;
}

double SCARA::be(double x, double y){

    double abx = abs(x - _arms.AB / 2.0 );
    return sqrt(abx * abx + y * y);

}

double SCARA::beta(double x, double y){

    return 180.0 -  thetaB(x, y) - thetaD(x, y);
}

double SCARA::thetaA(double x, double y){
    double tmp = abs(x + _arms.AB / 2.0);
    if(tmp == 0.0){
        return 90;
    }

    return atan2(y, tmp) * deg_rad;
}

double SCARA::thetaC(double x, double y){

    double _tmp = ae(x, y);
    double tmp = _tmp * _tmp +  _arms.CE * _arms.CE - _arms.AC * _arms.AC;
    tmp = tmp / (2.0 * _tmp * _arms.CE);
    return acos(tmp) * deg_rad;
}


double SCARA::ae(double x, double y){

    double tmp = (x + _arms.AB / 2.0);
    return sqrt(tmp * tmp + y * y);
}

double SCARA::alpha(double x, double y){

    return thetaA(x, y) + thetaC(x, y);
}
