#include "ButtonPanel.h"
#include "ObserverButtonPanel.h"

ButtonPanel::ButtonPanel(int dataPort){

    btn[0].minvalue = 0;
    btn[1].minvalue = 85;
    btn[2].minvalue = 170;
    btn[3].minvalue = 230;
    btn[4].minvalue = 294;

    btn[0].maxvalue = 6;
    btn[1].maxvalue = 102;
    btn[2].maxvalue = 201;
    btn[3].maxvalue = 250;
    btn[4].maxvalue = 350;

    btn[0].button = button00;
    btn[1].button = button01;
    btn[2].button = button02;
    btn[3].button = button03;
    btn[4].button = button04;
    pinMode(dataPort, INPUT);

    tr_pressed.button = noButton;
    tr_pressed.time = 0;

}


ButtonPanel::ButtonPanel(int dataPort, ButtonRange btn00, ButtonRange btn01,
                         ButtonRange btn02, ButtonRange btn03,
                         ButtonRange btn04){
    btn[0] = btn00;
    btn[1] = btn01;
    btn[2] = btn02;
    btn[3] = btn03;
    btn[4] = btn04;
    pinMode(dataPort, INPUT);
    tr_pressed.button = noButton;
    tr_pressed.time = 0;
}

TimeRangePressed ButtonPanel::time_range_pressed(){
    return tr_pressed;
}

Button ButtonPanel::button_pressed(){
    for(int i = 0; i < 5; i++){
        int inp = get_inputPort();

        /*
        Serial.print("Aread: ");
        Serial.println(inp);
        */

        if( inp >= btn[i].minvalue and inp <= btn[i].maxvalue){
            return btn[i].button;
        }
    }
    return noButton;
}


void ButtonPanel::registerOberver(ObserverButtonPanel* obs){
    observer = obs;

}


void ButtonPanel::unregisterObserver(){
    observer = nullptr;

}


void ButtonPanel::_notifyObserver(){
    if(observer != nullptr){
        observer->onReceiveDataFromButtonPanel(this);
    }

}

void ButtonPanel::_update_tr_pressed(){
    if(button_pressed() != noButton){
        if(button_pressed() != tr_pressed.button){
            tr_pressed.button = button_pressed();
            tr_pressed.time = 0;
            millis_tr_pressed = millis();

        }else{
            unsigned long deltaTime = millis() - millis_tr_pressed;
            tr_pressed.time += deltaTime;
            millis_tr_pressed = millis();

            /*
            Serial.print("Button: ");
            Serial.print(tr_pressed.button);
            Serial.print(" Time: ");
            Serial.println(tr_pressed.time);
            */

        }
    }else{
        tr_pressed.button = noButton;
        tr_pressed.time = 0;
    }



}

void ButtonPanel::update(){
    _update_tr_pressed();
    _notifyObserver();
}


int ButtonPanel::get_inputPort(){
    return analogRead(dataPort);
}
