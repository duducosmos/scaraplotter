#ifndef OBSERVERBUTTONPANEL_H
#define OBSERVERBUTTONPANEL_H
class ButtonPanel;

class ObserverButtonPanel{
public:
    void attachButtonPanel(ButtonPanel *buttonpanel);
    virtual void onReceiveDataFromButtonPanel(ButtonPanel*) = 0;
};
#endif
