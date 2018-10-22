#include <Arduino.h>
#include <SCARA.h>
#include <ButtonPanel.h>
#include <ObserverButtonPanel.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller: public ObserverButtonPanel{
private:
    SCARA _scara;
    TimeRangePressed button_pressed;
    const unsigned long mintime = 100;

    void _update();

public:
    Controller(SCARA);
    void onReceiveDataFromButtonPanel(ButtonPanel*) override;

};

#endif
