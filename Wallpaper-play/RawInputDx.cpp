#include "RawInputDx.h"

BOOL RegMouseRawInput(HWND hWnd) {
    RAWINPUTDEVICE rawInputDevice[1];
    rawInputDevice[0].usUsagePage = 0x01;   // 设备类
    rawInputDevice[0].usUsage = 0x06;       // 鼠标设备
    rawInputDevice[0].dwFlags = RIDEV_EXINPUTSINK;  // 即使窗口失去焦点位置，仍然接收输入消息
    rawInputDevice[0].hwndTarget = hWnd;

    return RegisterRawInputDevices(rawInputDevice, 1, sizeof(rawInputDevice[0])) == TRUE;
}

BOOL RegKeyboardRawInput(HWND hWnd) {
    RAWINPUTDEVICE rawInputDevice[1];
    rawInputDevice[0].usUsagePage = 0x01;   // 设备类
    rawInputDevice[0].usUsage = 0x02;       // 键盘设备
    rawInputDevice[0].dwFlags = RIDEV_EXINPUTSINK;  // 即使窗口失去焦点位置，仍然接收输入消息
    rawInputDevice[0].hwndTarget = hWnd;

    return RegisterRawInputDevices(rawInputDevice, 1, sizeof(rawInputDevice[0])) == TRUE;
}

void HandleMouseMessage(const RAWMOUSE& rawMouse, HWND target) {
    /*
     * 鼠标消息的有关信息
     * Ref :
     * https://docs.microsoft.com/en-us/windows/win32/inputdev/mouse-input
     * https://github.com/rocksdanister/lively/blob/dev-v1.0-fluent-netcore/src/livelywpf/livelywpf/Core/InputForwarding/RawInputDX.xaml.cs
     * 0x20 MouseMove
     * 0x01 MouseLeftButtonDown/MouseLeftButtonUp
     * 0x02 MouseRightButtonDown/MouseRightButtonUp
     * MouseWheel 消息比较特别, 留到后面再去研究
     * MSDN: The delta was set to 120 to allow Microsoft or other vendors to build
                                finer-resolution wheels (a freely-rotating wheel with no notches) to send more
                                messages per rotation, but with a smaller value in each message.
                                Because of this, the value is converted to a float in case a mouse's wheel
                                reports a value other than 120, in which case dividing by 120 would produce
                                a very incorrect value.
     * Ref:
     * https://github.com/ivarboms/game-engine/blob/master/Input/RawInput.cpp
     * http://social.msdn.microsoft.com/forums/en-US/gametechnologiesgeneral/thread/1deb5f7e-95ee-40ac-84db-58d636f601c7/
    */
    POINT point;
    GetCursorPos(&point);
    // 鼠标当前的位置
    UINT32 lParam = MAKELPARAM(point.x, point.y);
    //UINT32 lParam = (UINT32)point.y;
   // lParam <<= 16;
    //lParam = (UINT32)point.x;
    //rawMouse.usButtonData
    switch (rawMouse.usButtonFlags)
    {
    case NULL:                          // MouseMove
        PostMessage(target, WM_MOUSEMOVE, 0x0020, lParam);
        break;
    case RI_MOUSE_LEFT_BUTTON_DOWN:     // MouseLeftButtonDown
        PostMessage(target, WM_LBUTTONDOWN, 0x0001, lParam);
        break;
    case RI_MOUSE_LEFT_BUTTON_UP:       // MouseLeftButtonUp
        PostMessage(target, WM_LBUTTONUP, 0x0001, lParam);
        break;
    case RI_MOUSE_RIGHT_BUTTON_DOWN:    // MouseRightButtonDown
        // 桌面已经存在右键菜单，所以对右键消息不做处理
        break;
    case RI_MOUSE_RIGHT_BUTTON_UP:      // MouseRightButtonUp
        // 桌面已经存在右键菜单，所以对右键消息不做处理
        break;
    case RI_MOUSE_MIDDLE_BUTTON_DOWN:   // MouseMiddleButtonDown
        // 鼠标中键的消息对于页面的交互来说，不是很重要，所以也不做处理
        break;
    case RI_MOUSE_MIDDLE_BUTTON_UP:     // MouseMiddleButtonUp
        // 鼠标中键的消息对于页面的交互来说，不是很重要，所以也不做处理
        break;
    case RI_MOUSE_WHEEL:                // MouseWheel
        break;
    }
}

void HandleKeyboardMessage(const RAWKEYBOARD& rawKeyboard, HWND target) {
    /*
     * Ref:
     * https://docs.microsoft.com/en-us/windows/win32/inputdev/keyboard-input
     * https://github.com/rocksdanister/lively/blob/dev-v1.0-fluent-netcore/src/livelywpf/livelywpf/Core/InputForwarding/RawInputDX.xaml.cs
    
        //ref:
        //https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
        //https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keyup
    */
    /*
        tag: 这样拼装lParam参数可能会无效
    */
    BOOL isPressed = rawKeyboard.Flags != RI_KEY_BREAK;
    UINT lParam = 1u;
    lParam |= (UINT)rawKeyboard.MakeCode << 16;
    lParam |= 1u << 24;
    lParam |= 0u << 29;
    lParam = isPressed ? lParam : (lParam |= 3u << 30);

    PostMessage(target, rawKeyboard.Message, rawKeyboard.VKey, lParam);
}