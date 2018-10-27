#include "ObserverControlPanel.h"
#include "ControlPanel.h"




ControlPanel::ControlPanel(const int xpin, const int ypin, const int btnpin){

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


void ControlPanel::_notifyObserver(){
    if(observer != nullptr){
        observer->onReceiveDataFromPanel(this);
    }

}

void ControlPanel::update(){
    _notifyObserver();
}
