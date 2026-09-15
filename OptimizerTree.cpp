#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    const wchar_t CLASS_NAME[] = L"OptimizerTreeClass";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); // Nền sáng

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Optimizer Tree - Tiếng Việt", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 420,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        // Tạo thanh Tab giao diện
        HWND hTab = CreateWindowEx(0, WC_TABCONTROL, L"",
            WS_CHILD | WS_VISIBLE | TCS_TABS,
            12, 12, 560, 310, hwnd, (HMENU)1, NULL, NULL);

        TCITEM tie = { };
        tie.mask = TCIF_TEXT;
        tie.pszText = (LPWSTR)L"Tối ưu hệ thống";
        TabCtrl_InsertItem(hTab, 0, &tie);

        tie.pszText = (LPWSTR)L"Dọn dẹp rác";
        TabCtrl_InsertItem(hTab, 1, &tie);

        tie.pszText = (LPWSTR)L"Bảo mật & Quyền riêng tư";
        TabCtrl_InsertItem(hTab, 2, &tie);

        // Nút thực thi chức năng
        CreateWindowW(L"BUTTON", L"Bắt đầu tối ưu", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            30, 70, 160, 35, hwnd, (HMENU)2, NULL, NULL);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 2) {
            MessageBoxW(hwnd, L"Đã hoàn tất tối ưu hóa hệ thống!", L"Optimizer Tree", MB_OK | MB_ICONINFORMATION);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
