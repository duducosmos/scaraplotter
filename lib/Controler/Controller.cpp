#include <Controller.h>
#include <ButtonPanel.h>


Controller::Controller(SCARA scara): _scara(scara) {

}

void Controller::onReceiveDataFromButtonPanel(ButtonPanel* buttonsPanel){
    button_pressed = buttonsPanel->time_range_pressed();
    if(button_pressed.time > mintime){
        _update();
    }

}



void Controller::_update(){
    if(button_pressed.button == button00){
        //_scara.move_updown();
        _scara.rectangle(10, _scara.get_ymax() / 2, 20, 20);
        //_scara.drawnumber(2, 10, 40, 1);
    }
}
