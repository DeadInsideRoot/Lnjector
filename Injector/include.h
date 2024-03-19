#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;

// Functional
int InjectDLL(DWORD, char*);
int getDLLpath(char*);
int getPID(int*);
int getProc(HANDLE*, DWORD);