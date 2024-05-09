#include "savedata.hpp"

#include "cheats.hpp"
#include "HW05Lib.hpp"

extern "C" {

#include "print.h"

}

unsigned char saveBuffer[128];

void loadSaveData(void) {
    for (int i = 0; i < sizeof(saveBuffer); i++) {
        saveBuffer[i] = GAMEPAK_RAM[i];
    }
}

void storeSaveData(void) {
    for (int i = 0; i < sizeof(saveBuffer); i++) {
        GAMEPAK_RAM[i] = saveBuffer[i];
    }
}

void initSaveData(void) {
    loadSaveData();
    if (saveBuffer[0] != SAVE_VERSION) {
        resetSaveData();
    } else {
        mgba_printf("Save File Version: %d", saveBuffer[0]);
    }
}

void resetSaveData(void) {
    saveBuffer[0] = SAVE_VERSION;
    saveBuffer[1] = 0; // Initial Palette
    saveBuffer[3] = 1; // Number of Palettes Unlocked
    saveBuffer[5] = 0; // Cheats Enabled
    setSaveInt(8, 0);
    for (int i = 0; i < NUM_CHEATS; i++) {
        saveBuffer[32+i] = 0;
    }
    storeSaveData();
}

unsigned char getSaveDataEntry(int i) {
    return saveBuffer[i];
}

void setSaveDataEntry(int i, unsigned char val) {
    if (i != 0) {
        saveBuffer[i] = val;
        if (i < 64) {
            GAMEPAK_RAM[i] = val;
        }
    }
}

void setSaveInt(int index, int val) {
    for (int i = 0; i < sizeof(int); i++) {
        mgba_printf("save byte %d", i);
        setSaveDataEntry(index+i, *(((char*)(&val))+i));
    }
}

int getSaveInt(int index) {
    int val;
    for (int i = 0; i < sizeof(int); i++) {
        *((char*)(&val)+i) = getSaveDataEntry(index+i);
    }
    return val;
}
