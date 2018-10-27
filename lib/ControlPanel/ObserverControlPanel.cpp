#include "ControlPanel.h"
#include "ObserverControlPanel.h"

void ObserverControlPanel::attachPanel(ControlPanel *panel){
    panel->registerOberver(this);
}
