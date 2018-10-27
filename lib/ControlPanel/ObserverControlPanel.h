#include "ControlPanel.h"

#ifndef OBSERVERCONTROLPANEL_H
#define OBSERVERCONTROLPANEL_H

class ObserverControlPanel{
public:
    void attachPanel(ControlPanel *panel);
    virtual void onReceiveDataFromPanel(ControlPanel*) = 0;
};


#endif
