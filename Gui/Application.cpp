#include "Application.h"
#include "Core/Memory.h"

#ifdef WIN32_PLATFORM

rapid::Application* rapid::Application::__App = new rapid::Application();
SizeType rapid::Application::__Count = 0;

int rapid::Application::exec()
{
    MSG msg;
    while(GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT rapid::Application::win_application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

HWND rapid::Application::create_control(WinWidget *wid, const char *class_name, HWND parent)
{
    DWORD type = WS_VISIBLE | ES_CENTER;
    if(parent != nullptr)
    { type |= WS_CHILD; }
    else
    { type |= WS_MINIMIZEBOX | WS_SYSMENU; }
    const char *result_class_name = nullptr;
    if(class_name == nullptr)
    {
        char* widget_count;
        if(application()->__AppName == nullptr)
        {
            widget_count = new char[11]{'w', 'i', 'd', 'g', 'e', 't', 0, 0, 0, 0, 0};
        }
        else
        {
            SizeType len = strlen(application()->__AppName);
            widget_count = new char[len + 1];
            mem_copy(widget_count, application()->__AppName, static_cast<unsigned long>(len));
        }
        if(__Count <= 0xff)
        {
            widget_count[6] = static_cast<char>(__Count);
        }
        else if(__Count <= 0xffff)
        {
            widget_count[7] = static_cast<char>(0xff);
            widget_count[6] = static_cast<char>(__Count - 0xff);
        }
        else if(__Count <= 0xffffff)
        {
            widget_count[8] = static_cast<char>(0xff);
            widget_count[7] = static_cast<char>(0xff);
            widget_count[6] = static_cast<char>(__Count - 0xffff);
        }
        else if(__Count <= 0xffffffff)
        {
            widget_count[8] = static_cast<char>(0xff);
            widget_count[8] = static_cast<char>(0xff);
            widget_count[7] = static_cast<char>(0xff);
            widget_count[6] = static_cast<char>(__Count - 0xffffff);
        }
        result_class_name = widget_count;
        init_wnd_class(widget_count);
        __Count++;
    }
    else
    {
        result_class_name = class_name;
    }
    SizeType len = strlen(result_class_name);
    WinStringType *name = new WinStringType[len];
    to_win_code(name, result_class_name, len);
    HWND result = CreateWindowExW(0, name, TEXT(""), type, 0, 0, 0, 0, parent, nullptr, application()->instance(), nullptr);
    application()->__ControlMap[result] = wid;
    return result;
}

void rapid::Application::init_wnd_class(const char *app_name)
{
    SizeType len = strlen(app_name);
    WinStringType *name = new WinStringType[len];
    to_win_code(name, app_name, len);
    WNDCLASS wnd_class;
    wnd_class.style = CS_HREDRAW | CS_VREDRAW;
    wnd_class.lpfnWndProc = win_application_proc;
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hInstance = application()->instance();
    wnd_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
//    wnd_class.hbrBackground = (HBRUSH)GetStockObject(0);//WHITE_BRUSH = 0
    wnd_class.hbrBackground = nullptr;//WHITE_BRUSH = 0
    wnd_class.lpszMenuName = nullptr;
    wnd_class.lpszClassName = name;
    if(!RegisterClass(&wnd_class))
    {
        MessageBox(nullptr, TEXT("运行程序失败"), name, MB_ICONERROR);
    }
}

#endif
