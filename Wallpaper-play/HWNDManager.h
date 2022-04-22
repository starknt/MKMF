#pragma once
#ifndef HWNDMANAGER_H
#define HWNDMANAGER_H
#include <Windows.h>
#include <cstdio>
class Handle
{
	public:
		Handle();
		HWND display;
		HWND MSGWindow;
		BOOL MSGStatus;
};

extern Handle handle;

#endif // !HWNDMANAGER_H