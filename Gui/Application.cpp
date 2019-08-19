#include "Application.h"
#include "Core/Memory.h"
#include "Widget.h"
#include "Core/Conver.h"

#ifdef WIN32_PLATFORM

rapid::Application::CountType rapid::Application::__Count(0);
rapid::Application::ControlContainerType rapid::Application::__ControlMap;
rapid::Application::ApplicationContainerType rapid::Application::__AppMap;
rapid::Application::CountType rapid::Application::__CreateId(0);
rapid::Application::ControlIDType rapid::Application::__ControlCreateIDMap;
rapid::Application* rapid::Application::__App = new rapid::Application();

rapid::Application *rapid::Application::__get_app(HWND hwnd)
{
    auto it = __AppMap.find(hwnd);
    if(it == __AppMap.end())
    { return nullptr; }
    return it->second;
}

rapid::WinWidget *rapid::Application::__get_widget(HWND hwnd)
{
    return static_cast<WinWidget *>(__get_eventer(hwnd));
}

rapid::EventInterface *rapid::Application::__get_eventer(HWND hwnd)
{
    auto it = __ControlMap.find(hwnd);
    if(it == __ControlMap.end())
    { return nullptr; }
    return it->second;
}

rapid::Application::~Application()
{
    if(__Root != nullptr) delete __Root;
}

int rapid::Application::exec()
{
    MSG msg;
    while(GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    delete __App;
    return static_cast<int>(msg.wParam);
}

LRESULT rapid::Application::win_application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_PAINT:
    {
        WinWidget *widget = __get_widget(hwnd);
        widget->repaint_event();
    }
        break;
    case WM_MOUSEWHEEL:
    {
        MouseEvent *event = static_cast<MouseEvent *>(__get_eventer(hwnd));
        event->mouse_wheel_event();
    }
        break;
    case WM_LBUTTONDOWN:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->left_down_event();
    }
        break;
    case WM_LBUTTONUP:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->left_up_event();
    }
        break;
    case WM_LBUTTONDBLCLK:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->left_click_event();
    }
        break;
    case WM_RBUTTONDOWN:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->right_down_event();
    }
        break;
    case WM_RBUTTONUP:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->right_up_event();
    }
        break;
    case WM_RBUTTONDBLCLK:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->right_click_event();
    }
        break;
    case WM_MBUTTONDOWN:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->middle_down_event();
    }
        break;
    case WM_MBUTTONUP:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->middle_up_event();
    }
        break;
    case WM_MBUTTONDBLCLK:
    {
        ButtonEvent *event = static_cast<ButtonEvent *>(__get_eventer(hwnd));
        event->middle_click_event();
    }
        break;
    case WM_MOUSEMOVE:
    {
        WinWidget *widget = __get_widget(hwnd);
        widget->mouse_move_event();
    }
        break;
    case WM_KEYUP:
    {
        KeyBoardEvent *event = static_cast<KeyBoardEvent *>(__get_eventer(hwnd));
        event->key_up();
    }
        break;
    case WM_KEYDOWN:
    {
        KeyBoardEvent *event = static_cast<KeyBoardEvent *>(__get_eventer(hwnd));
        event->key_down();
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        if(app()->__Root->_Hwnd == nullptr)
        {
            __ControlMap[hwnd] = app()->__Root;
            app()->__Root->_Hwnd = hwnd;
        }
        for(auto it = __ControlCreateIDMap.begin(); it != __ControlCreateIDMap.end(); ++it)
        {
            CreateWindowType *temp = it->second;
            WinWidget *wid = std::get<0>(*temp);
            HWND h = CreateWindow(std::get<1>(*temp), std::get<2>(*temp), std::get<3>(*temp), std::get<4>(*temp), std::get<5>(*temp), std::get<6>(*temp), std::get<7>(*temp), std::get<8>(*temp), std::get<9>(*temp), instance(), nullptr);
            delete temp;
            __ControlMap[h] = wid;
            wid->_Hwnd = h;
        }
        __ControlCreateIDMap.clear();
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void rapid::Application::create_control(rapid::WinWidget *wid, DWORD type, const wchar_t *class_name, rapid::WinWidget *parent)
{
    if(parent == nullptr)
    { parent = app()->__Root; }
    if(parent != nullptr)
    { type |= WS_CHILD; }
    else
    { type |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME; }
    if(class_name == nullptr)
    {
        char* widget_count = new char[11]{'w', 'i', 'd', 'g', 'e', 't', 0, 0, 0, 0, 0};
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
            widget_count[9] = static_cast<char>(0xff);
            widget_count[8] = static_cast<char>(0xff);
            widget_count[7] = static_cast<char>(0xff);
            widget_count[6] = static_cast<char>(__Count - 0xffffff);
        }
        const wchar_t *name = to_utf8(widget_count);
//        SizeType len = strlen(widget_count);
//        WinStringType *name = new WinStringType[len + 1];
//        to_win_code(name, widget_count, len);
        init_wnd_class(widget_count);
        __Count++;
        app()->__Root = wid;
        CreateWindow(name, TEXT(""), type, 0, 0, 0, 0, parent != nullptr ? parent->_Hwnd : nullptr, nullptr, instance(), nullptr);
        delete[] name;
    }
    else
    {
        CreateWindowType *tup = new CreateWindowType(wid, class_name, TEXT(""), type, int(0), int(0), int(0), int(0), parent != nullptr ? parent->_Hwnd : nullptr, nullptr);
        __ControlCreateIDMap[__CreateId] = tup;
        __CreateId++;
        if(app()->__Root->_Hwnd != nullptr)
        {
            SendMessage(app()->__Root->_Hwnd, WM_CREATE, 0, 0);
        }
    }
}

void rapid::Application::create_control(rapid::WinWidget *wid, const wchar_t *class_name, rapid::WinWidget *parent)
{
    create_control(wid, WS_VISIBLE | ES_CENTER, class_name, parent);
}

void rapid::Application::create_control(WinWidget *wid, const char *class_name, WinWidget *parent)
{
    create_control(wid, to_utf8(class_name), parent);
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
    wnd_class.hInstance = instance();
    wnd_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
//    wnd_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//WHITE_BRUSH = 0
    wnd_class.hbrBackground = nullptr;//WHITE_BRUSH = 0
    wnd_class.hbrBackground = nullptr;//WHITE_BRUSH = 0
    wnd_class.lpszMenuName = nullptr;
    wnd_class.lpszClassName = name;
    if(!RegisterClass(&wnd_class))
    {
        MessageBox(nullptr, TEXT("运行程序失败"), name, MB_ICONERROR);
    }
}

#endif
