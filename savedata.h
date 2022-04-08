#ifndef SAVEDATA_H
#define SAVEDATA_H

#define SAVE_VERSION 0x02

void initSaveData(void);

void storeSaveData(void);

void resetSaveData(void);

unsigned char getSaveDataEntry(int i);

void setSaveDataEntry(int i, unsigned char val);

#endif