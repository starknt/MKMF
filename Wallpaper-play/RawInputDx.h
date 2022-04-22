#pragma once
#ifndef RAWINPUT_H
#define RAWINPUT_H
#include "HWNDManager.h"

BOOL RegMouseRawInput(HWND hWnd);

BOOL RegKeyboardRawInput(HWND hWnd);

void HandleMouseMessage(const RAWMOUSE& rawMouse, HWND target);

void HandleKeyboardMessage(const RAWKEYBOARD& rawKeyboard, HWND target);

#endif // !RAWINPUT_H
