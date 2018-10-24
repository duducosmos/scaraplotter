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

        /*
        Arms arms;

        String tmp;

        Serial.println("Entre com AB: ");
        while (Serial.available()  == 0){

        }

        tmp = Serial.readString();

        arms.AB = tmp.toDouble();

        Serial.println("Entre com AC: ");
        while (Serial.available()  == 0){

        }
        tmp = Serial.readString();

        arms.AC = tmp.toDouble();



        Serial.println("Entre com BD: ");
        while (Serial.available()  == 0){

        }
        tmp = Serial.readString();

        arms.BD =tmp.toDouble();

        Serial.println("Entre com DE: ");
        while (Serial.available()  == 0){

        }
        tmp = Serial.readString();

        arms.DE = tmp.toDouble();


        Serial.println("Entre com CE: ");
        while (Serial.available()  == 0){

        }
        tmp = Serial.readString();

        arms.CE = tmp.toDouble();


        _scara.set_arms(arms);
        */
        _scara.rectangle(0, 10, 10, 10);
        //_scara.drawnumber(2, 10, 10, 1);
    }
}
