#include <Arduino.h>

#ifndef BUTTONPANEL_H
#define BUTTONPANEL_H

class ObserverButtonPanel;

enum Button {button00, button01, button02, button03, button04, noButton};

struct ButtonRange {
    int maxvalue;
    int minvalue;
    Button button;
};

struct TimeRangePressed {
    unsigned long time;
    Button button;
};

class ButtonPanel{
private:
    ButtonRange btn[5];
    int dataPort;
    int get_inputPort();
    Button button;
    void _notifyObserver();
    ObserverButtonPanel* observer;
    TimeRangePressed tr_pressed;
    unsigned long millis_tr_pressed;
    void _update_tr_pressed();

public:

    ButtonPanel(int dataPort);
    ButtonPanel(int dataPort, ButtonRange btn00, ButtonRange btn01,
                ButtonRange btn02, ButtonRange btn03, ButtonRange btn04);
    TimeRangePressed time_range_pressed();
    Button button_pressed();
    void registerOberver(ObserverButtonPanel*);
    void unregisterObserver();
    void update();
};

#endif
