#include <Controller.h>
#include <ControlPanel.h>


Controller::Controller(SCARA scara): _scara(scara) {

}

void Controller::onReceiveDataFromPanel(ControlPanel* panel){
    pvalues = panel->get_panel_values();
    _update();


}

float Controller::_serial2float(){

    String inString;

    while (Serial.available() == 0);
    inString = Serial.readStringUntil(10);

    return inString.toFloat();

}

void Controller::_serialmode(){
    float x  = _serial2float();
    Serial.print("x: ");
    Serial.println(x);
    float y = _serial2float();
    Serial.print("y: ");
    Serial.println(y);

    _scara.move(x, y);


}



void Controller::_update(){

    if(_smode == false){
        if(pvalues.is_btn_pressed == true && pvalues.time_btn_pressed > mintime){
            _scara.move_updown();
        }
        _scara.move(pvalues.x_value, pvalues.y_value);

    }else{
        _serialmode();
    }




}
