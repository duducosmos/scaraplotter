#include "ObserverButtonPanel.h"
#include "ButtonPanel.h"

void ObserverButtonPanel::attachButtonPanel(ButtonPanel *buttonpanel){
    buttonpanel->registerOberver(this);
}
