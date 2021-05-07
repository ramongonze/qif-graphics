#include "gui.h"

Gui::Gui(){
    menu = GuiMenu();
    prior = GuiPrior();
    channel = GuiChannel();
    posteriors = GuiPosteriors();
}