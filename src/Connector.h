//
// Created by Bulcsú on 2/28/2021.
//

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <windows.h>
#include <cstdio>
#include <iostream>
#include <string>

extern STARTUPINFO sti;
extern SECURITY_ATTRIBUTES sats;
extern PROCESS_INFORMATION pi;
extern HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
extern BYTE buffer[2048];
extern DWORD writ, read, available;
extern WINBOOL success;

void connectToEngine(const char* path);
std::string getNextMove(std::string position);
void closeConnection();

#endif //CONNECTOR_H
