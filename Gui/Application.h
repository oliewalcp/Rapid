#ifndef APPLICATION_H
#define APPLICATION_H

#include "GuiConfig.h"
#include <map>
#include <atomic>

namespace rapid
{

class WinWidget;
class EventInterface;

class Application
{
private:
    using CountType = std::atomic<SizeType>;
    using ApplicationContainerType = std::map<HWND, Application*>;
    using ControlContainerType = std::map<HWND, EventInterface*>;
    using CreateWindowType = std::tuple<WinWidget*, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU>;
    using ControlIDType = std::map<SizeType, CreateWindowType*>;
    using RootType = WinWidget*;

    RootType __Root = nullptr;

    static CountType __Count;
    static ControlContainerType __ControlMap;
    static ApplicationContainerType __AppMap;
    static ControlIDType __ControlCreateIDMap;
    static CountType __CreateId;
    static Application *__App;

    friend class WinWidget;

    static Application* __get_app(HWND hwnd);
    static WinWidget *__get_widget(HWND hwnd);
    static EventInterface *__get_eventer(HWND hwnd);

//    Application() { }
//    Application(HWND hwnd)
//    { __AppMap[hwnd] = this; }
public:
    ~Application();
    static Application* app()
    { return __App; }
    static int exec();
    static int loop()
    { return exec(); }

#ifdef WIN32_PLATFORM

    static HINSTANCE instance()
    { return GetModuleHandle(nullptr); }

    static LRESULT CALLBACK win_application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static void create_control(WinWidget *wid, DWORD type, const wchar_t *class_name, WinWidget *parent);
    static void create_control(WinWidget *wid, const wchar_t *class_name, WinWidget *parent);
    static void create_control(WinWidget *wid, const char *class_name = nullptr, WinWidget *parent = nullptr);
    static void init_wnd_class(const char *app_name);

#endif

};

};

#endif // APPLICATION_H
