#pragma once

#include <windows.h>

#define HINST_THISDLL g_hinst

extern HINSTANCE g_hinst;

void DllAddRef();
void DllRelease();
