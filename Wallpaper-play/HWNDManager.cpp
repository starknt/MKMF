#include "HWNDManager.h"

Handle::Handle() {
	this->display = nullptr;
	this->MSGStatus = TRUE;
	this->MSGWindow = nullptr;
}

Handle handle = Handle();		// 全局变量