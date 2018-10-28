#include <Controller.h>
#include <ControlPanel.h>


Controller::Controller(SCARA scara): _scara(scara) {

}

void Controller::onReceiveDataFromPanel(ControlPanel* panel){
    pvalues = panel->get_panel_values();
    _update();


}



void Controller::_update(){
    if(pvalues.is_btn_pressed == true && pvalues.time_btn_pressed > mintime){
        //_scara.move_updown();
        _scara.rectangle(0, 10, 10, 10);
    }


}
