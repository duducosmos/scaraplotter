#include <Arduino.h>
#include <SCARA.h>
#include <ControlPanel.h>
#include <ObserverControlPanel.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller: public ObserverControlPanel{
private:
    SCARA _scara;
    const unsigned long mintime = 50;
    PValues pvalues;
    bool _smode = true;

    void _update();
    void _serialmode();
    float _serial2float();

public:
    Controller(SCARA);
    void onReceiveDataFromPanel(ControlPanel*) override;

};

#endif
