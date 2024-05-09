#ifndef SAVEDATA_HPP
#define SAVEDATA_HPP

#define SAVE_VERSION 0x03

void initSaveData(void);

void storeSaveData(void);

void resetSaveData(void);

unsigned char getSaveDataEntry(int i);

void setSaveDataEntry(int i, unsigned char val);

void setSaveInt(int i, int val);

int getSaveInt(int index);

#endif