#include "savedata.h"

#include "HW05Lib.h"

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
    saveBuffer[3] = 3; // Number of Palettes Unlocked
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