#include <SCARA.h>


SCARA::SCARA(Servo servo_left, Servo servo_right, Servo servo_updow, Arms arms):
_arms(arms), _servo_left(servo_left),
_servo_right(servo_right), _servo_updow(servo_updow)
{
    t0 = millis();
    _start_direction();

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

void SCARA::lineGoldgerg(double x0, double y0, double xf, double yf){
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


    while((direction.xr != direction.dx) || (direction.yr != direction.dy)){
        if(direction.fxy > 0){
            ++direction.xr;
            direction.fxy = direction.fxy - direction.dy;
        }else{
            ++direction.yr;
            direction.fxy = direction.fxy + direction.dx;

        }


        move(x0 + direction.xr * direction.i, y0 + direction.yr * direction.j);
        Serial.println("");
        Serial.print("xr: ");
        Serial.println(x0 + direction.xr * direction.i);
        Serial.print("yr: ");
        Serial.println(y0 + direction.yr * direction.j);
    }
    //move(get_xmean(), get_ymax());
    Serial.println("Fim");

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

    move(x0, y0);
    double a;
    double b;
    direction.xr = x0;
    direction.yr = y0;
    if(xf == x0 || yf == y0){
        a = 0;

    }else{
        a = (yf - y0) / (xf - x0);
        b = y0 - a * x0;

    }


    while(true){
        if(direction.xr >= xf && direction.yr >= yf){
            break;
        }

        direction.xr += direction.i * step;
        if(a != 0){
            direction.yr = a * direction.xr + b;
        }else{
            direction.yr  += direction.j * step;
        }

        move(direction.xr, direction.yr);
        Serial.println("");
        Serial.print("xr: ");
        Serial.println(direction.xr);
        Serial.print("yr: ");
        Serial.println(direction.yr);
    }


    move(get_xmean(), get_ymax());




}


void SCARA::move(double x, double y){
    int _beta = beta(x, y);
    int _alpha = alpha(x, y);
    /*
    Serial.print("Beta: ");
    Serial.println(_beta);
    Serial.print("Alpha: ");
    Serial.println(_alpha);
    */

    t0 = millis();
    while(millis() - t0 <= dtime);
    _servo_right.write(_beta);
    _servo_left.write(_alpha);


}

double SCARA::thetaB(double x, double y){
    /*
    * $theta_{B} = \tang^{-1}\left(\frac{y}{\bar{AB} - x}\right)$
    */
    double abx = _arms.AB - x;
    abx = atan2(y, abx) * deg_rad;

    return abx;
}

double SCARA::thetaD(double x, double y){
    /*
    * $\theta_{D} = \cos^{-1}\left(
          \frac{\bar{BE}^{2} +\bar{BD}^{2} -\bar{DE}^{2}}{2 \bar{BE}\bar{BD}}
        \right)
       $
    */
    double _tmp = be(x, y);


    double tmp = _tmp * _tmp + _arms.BD *  _arms.BD - _arms.DE * _arms.DE;

    double tmp2 = 2.0 * _tmp * _arms.BD;

    tmp = tmp / tmp2;

    tmp = acos(tmp) * deg_rad;



    return tmp;
}

double SCARA::be(double x, double y){
    /*
    $\bar{BE} = \sqrt{(\bar{AB}^{2}+y^{2})}$
    */
    double abx = _arms.AB - x;
    return sqrt(abx * abx + y * y);

}

double SCARA::beta(double x, double y){
    /*
    $\beta = \theta_{B} + \theta_{D}$
    */
    return 180.0 -  thetaB(x, y) - thetaD(x, y);
}

double SCARA::thetaA(double x, double y){
    /*
    $\theta_{A} = \tan^{-1}\left(\frac{y}{x}\right)$
    */
    return atan2(y, x) * deg_rad;
}

double SCARA::thetaC(double x, double y){
    /*
    $\theta_{C} = \cos^{-1}\left(
    \frac{\bar{AE}^{2}+\bar{AC}^{2}-\bar{CE}^2}{2\bar{AE}\bar{AC}}
    \right)$
    */
    double _tmp = ae(x, y);
    double tmp = _tmp * _tmp + _arms.AC * _arms.AC - _arms.CE * _arms.CE;
    tmp = tmp / (2.0 * _tmp * _arms.AC);
    return acos(tmp) * deg_rad;
}


double SCARA::ae(double x, double y){
    /*
     $\bar{AE} = \sqrt{x^{2} + y^{2}}$
    */
    return sqrt(x * x + y * y);
}

double SCARA::alpha(double x, double y){
    /*
      $\alpha = \theta_{A} + \theta_{C}$
    */
    return thetaA(x, y) + thetaC(x, y);
}
