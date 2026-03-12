#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#define BTN_OFFICE 1
#define BTN_HOME   2
#define BTN_VPN    3

const std::string HOSTS_PATH = "C:\\Windows\\System32\\drivers\\etc\\hosts";
// const std::string DOMAIN = "servsky";

std::string GetIPFromConfig(const char* section)
{
    char ip[100];
    GetPrivateProfileStringA(section, "ip", "", ip, 100, ".\\config.ini");
    return std::string(ip);
}

std::string GetDomainFromConfig(const char* section)
{
    char domain[200];
    GetPrivateProfileStringA(section, "dm", "", domain, 200, ".\\config.ini");
    return std::string(domain);
}

void UpdateHosts(std::string ip, std::string domain)
{
    std::ifstream in(HOSTS_PATH);
    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (getline(in, line))
    {
        if (line.find(domain) != std::string::npos)
        {
            lines.push_back(ip + " " + domain);
            found = true;
        }
        else
        {
            lines.push_back(line);
        }
    }

    in.close();

    if (!found)
    {
        lines.push_back(ip + " " + domain);
    }

    std::ofstream out(HOSTS_PATH, std::ios::trunc);

    for (auto &l : lines)
    {
        out << l << "\n";
    }

    out.close();

    MessageBox(NULL, "Hosts updated successfully", "Success", MB_OK);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:

            CreateWindow("BUTTON","Working From Office",
                WS_VISIBLE | WS_CHILD,
                40,40,220,40,
                hwnd,(HMENU)BTN_OFFICE,NULL,NULL);

            CreateWindow("BUTTON","Working From Home (Hamachi)",
                WS_VISIBLE | WS_CHILD,
                40,100,220,40,
                hwnd,(HMENU)BTN_HOME,NULL,NULL);

            CreateWindow("BUTTON","Working with OpenVPN",
                WS_VISIBLE | WS_CHILD,
                40,160,220,40,
                hwnd,(HMENU)BTN_VPN,NULL,NULL);

        break;

        case WM_COMMAND:

            switch(LOWORD(wParam))
            {
                case BTN_OFFICE:
                    UpdateHosts(GetIPFromConfig("office"), GetDomainFromConfig("domain"));
                break;

                case BTN_HOME:
                    UpdateHosts(GetIPFromConfig("home"), GetDomainFromConfig("domain"));
                break;

                case BTN_VPN:
                    UpdateHosts(GetIPFromConfig("vpn"), GetDomainFromConfig("domain"));
                break;
            }

        break;

        case WM_DESTROY:
            PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd,msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    const char CLASS_NAME[] = "NetworkSwitcher";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "ServSky Network Switcher",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,320,280,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd,nCmdShow);

    MSG msg = {};

    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}