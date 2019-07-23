#ifndef APPLICATION_H
#define APPLICATION_H

#include "GuiConfig.h"
#include <map>


namespace rapid
{

class WinWidget;

class Application
{
private:
    using ControlContainerType = std::map<HWND, WinWidget*>;

    static Application *__App;
    static SizeType __Count;

    char *__AppName = nullptr;
    ControlContainerType __ControlMap;
public:
    static Application* application()
    { return __App; }
    void exit(int status = 0)
    {
        PostQuitMessage(status);
        delete __App;
    }
    int exec();
    void set_app_name(const char *name);
#ifdef WIN32_PLATFORM

    HINSTANCE instance()
    { return GetModuleHandle(nullptr); }

    static LRESULT CALLBACK win_application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static HWND create_control(WinWidget *wid, const char *class_name = nullptr, HWND parent = nullptr);
    static void init_wnd_class(const char *app_name);

#endif

};

};

#endif // APPLICATION_H
