#include <Arduino.h>

#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H


class ObserverControlPanel;

struct PValues {
    double x_value;
    double y_value;
    bool is_btn_pressed;
    unsigned long time_btn_pressed;

};


class ControlPanel {
    ObserverControlPanel* observer;
    PValues panel_values;
    unsigned long millis_tr_pressed;
    int _xmin;
    int _xmax;
    int _ymin;
    int _ymax;
    int _xpin;
    int _ypin;
    int _btnpin;


    void _notifyObserver();
    void _update();

public:
    ControlPanel(const int xpin, const int ypin, const int btnpin,
                 int xmin, int xmax, int ymin, int ymax);
    void registerOberver(ObserverControlPanel*);
    void unregisterObserver();
    void update();
    PValues get_panel_values(){
        return panel_values;
    }

};

#endif
