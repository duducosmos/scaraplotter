#include <Arduino.h>
#include <SCARA.h>
#include <ControlPanel.h>
#include <ObserverControlPanel.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller: public ObserverControlPanel{
private:
    SCARA _scara;
    const unsigned long mintime = 100;
    PValues pvalues;

    void _update();

public:
    Controller(SCARA);
    void onReceiveDataFromPanel(ControlPanel*) override;

};

#endif
