#ifndef DF93EBE0_978C_4659_B58F_CB5154FBE8BB
#define DF93EBE0_978C_4659_B58F_CB5154FBE8BB

#include "heater_controller.h"
#include "chronothermo.h"
#include "restart_controller.h"

namespace Controllers {
    extern HeaterController heaterController;
    extern Chronothermo chronothermo;
    extern RestartController restartController;
    void init();
    void updateFromSettings();
}

#endif /* DF93EBE0_978C_4659_B58F_CB5154FBE8BB */
