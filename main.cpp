// --- main.cpp ---
#include "music_player_logic.h"
#include "music_player_gui.h"

int main() {
    MusicPlayerLogic logic;
    MusicPlayerGUI gui(&logic);

    while (gui.isWindowOpen()) {
        gui.handleEvents(); // GUI handles events and interacts with logic
        logic.updateLogic(); // Logic updates its internal state
        gui.render(); // GUI renders based on logic's current state
    }

    return 0;
}