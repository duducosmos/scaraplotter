#include "ObserverControlPanel.h"
#include "ControlPanel.h"




ControlPanel::ControlPanel(const int xpin, const int ypin, const int btnpin,
                           int xmin, int xmax, int ymin, int ymax){

    _xmin = xmin;
    _xmax = xmax;
    _ymin = ymin;
    _ymax = ymax;

    _xpin = xpin;
    _ypin = ypin;
    _btnpin = btnpin;

    pinMode(_xpin, INPUT);
    pinMode(_ypin, INPUT);
    pinMode(_btnpin, INPUT_PULLUP);

}



void ControlPanel::registerOberver(ObserverControlPanel* obs){
    observer = obs;

}


void ControlPanel::unregisterObserver(){
    observer = nullptr;

}

void ControlPanel::_update(){
    int tmp = 1;
    for(int i = 0 ; i < 5; i++){
        tmp += digitalRead(_btnpin);

    }

    tmp /= 5;

    if(tmp == 1) {
        panel_values.is_btn_pressed = false;
        panel_values.time_btn_pressed = 0;
    }else{
        panel_values.is_btn_pressed = true;
        if(panel_values.time_btn_pressed == 0) {
                panel_values.time_btn_pressed = 1.0;
                millis_tr_pressed = millis();
        }else{
                panel_values.time_btn_pressed = millis() - millis_tr_pressed;
        }
    }

    panel_values.x_value = map(analogRead(_xpin), 0, 1023, _xmin, _xmax);
    panel_values.y_value = map(analogRead(_ypin), 0, 1023, _ymin, _ymax);

}


void ControlPanel::_notifyObserver(){
    if(observer != nullptr){
        observer->onReceiveDataFromPanel(this);
    }

}

void ControlPanel::update(){
    _update();
    _notifyObserver();
}
