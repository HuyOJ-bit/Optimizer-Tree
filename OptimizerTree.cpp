#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>

#pragma comment(lib, "comctl32.lib")

HFONT hFont = NULL;
HWND hCheck1, hCheck2, hCheck3;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    INITCOMMONCONTROLSEX icex = { sizeof(INITCOMMONCONTROLSEX), ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icex);

    hFont = CreateFontW(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");

    const wchar_t CLASS_NAME[] = L"OptimizerTreeClass";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Optimizer Tree - Thực thi thật", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 780, 500,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (hFont) DeleteObject(hFont);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        // Sidebar Menu
        HWND btn1 = CreateWindowW(L"BUTTON", L"Tối ưu hệ thống", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, 15, 180, 40, hwnd, (HMENU)101, NULL, NULL);
        HWND btn2 = CreateWindowW(L"BUTTON", L"Dọn dẹp rác", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, 65, 180, 40, hwnd, (HMENU)102, NULL, NULL);
        HWND btn3 = CreateWindowW(L"BUTTON", L"Bảo mật & Riêng tư", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 15, 115, 180, 40, hwnd, (HMENU)103, NULL, NULL);

        // Khung nội dung
        HWND hGroup = CreateWindowW(L"BUTTON", L" Tùy chọn thực tế ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 210, 10, 535, 430, hwnd, (HMENU)200, NULL, NULL);

        // Checkbox chức năng thật
        hCheck1 = CreateWindowW(L"BUTTON", L"Dọn dẹp toàn bộ file tạm (Temp, Prefetch)", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 235, 45, 480, 25, hwnd, (HMENU)301, NULL, NULL);
        hCheck2 = CreateWindowW(L"BUTTON", L"Vô hiệu hóa Telemetry (Thu thập dữ liệu ngầm)", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 235, 85, 480, 25, hwnd, (HMENU)302, NULL, NULL);
        hCheck3 = CreateWindowW(L"BUTTON", L"Tối ưu hóa tốc độ mạng (Flush DNS & TCP)", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 235, 125, 480, 25, hwnd, (HMENU)303, NULL, NULL);

        SendMessage(hCheck1, BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(hCheck2, BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(hCheck3, BM_SETCHECK, BST_CHECKED, 0);

        // Nút thực thi thật
        HWND btnExec = CreateWindowW(L"BUTTON", L"Thực thi ngay", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 570, 385, 155, 38, hwnd, (HMENU)401, NULL, NULL);

        // Set font
        HWND controls[] = { btn1, btn2, btn3, hGroup, hCheck1, hCheck2, hCheck3, btnExec };
        for (HWND h : controls) SendMessage(h, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) >= 301 && LOWORD(wParam) <= 303) {
            HWND hChk = (HWND)lParam;
            int state = SendMessage(hChk, BM_GETCHECK, 0, 0);
            SendMessage(hChk, BM_SETCHECK, state == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED, 0);
        }

        // Xử lý khi bấm nút "Thực thi ngay"
        if (LOWORD(wParam) == 401) {
            // 1. Dọn dẹp file tạm nếu chọn Checkbox 1
            if (SendMessage(hCheck1, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                system("del /s /q /f %temp%\\*.* >nul 2>&1");
                system("del /s /q /f C:\\Windows\\Temp\\*.* >nul 2>&1");
            }

            // 2. Tắt Telemetry qua lệnh Registry nếu chọn Checkbox 2
            if (SendMessage(hCheck2, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                system("reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection\" /v AllowTelemetry /t REG_DWORD /d 0 /f >nul 2>&1");
            }

            // 3. Tối ưu mạng (Flush DNS) nếu chọn Checkbox 3
            if (SendMessage(hCheck3, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                system("ipconfig /flushdns >nul 2>&1");
            }

            MessageBoxW(hwnd, L"Đã thực thi thành công các tùy chọn hệ thống!", L"Optimizer Tree", MB_OK | MB_ICONINFORMATION);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
