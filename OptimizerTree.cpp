#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

HFONT hFont = NULL;
HWND hCheck1, hCheck2, hCheck3;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    INITCOMMONCONTROLSEX icex = { sizeof(INITCOMMONCONTROLSEX), ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icex);

    // Tạo font Segoe UI hiện đại
    hFont = CreateFontW(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");

    const wchar_t CLASS_NAME[] = L"OptimizerTreeClass";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Nền trắng sạch

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Optimizer Tree", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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
        // --- MENU THANH BÊN TRÁI (Sidebar) ---
        HWND btn1 = CreateWindowW(L"BUTTON", L"Tối ưu hệ thống", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            15, 15, 180, 40, hwnd, (HMENU)101, NULL, NULL);
        HWND btn2 = CreateWindowW(L"BUTTON", L"Dọn dẹp rác", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            15, 65, 180, 40, hwnd, (HMENU)102, NULL, NULL);
        HWND btn3 = CreateWindowW(L"BUTTON", L"Bảo mật & Riêng tư", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            15, 115, 180, 40, hwnd, (HMENU)103, NULL, NULL);

        // --- KHUNG NỘI DUNG BÊN PHẢI ---
        HWND hGroup = CreateWindowW(L"BUTTON", L" Tùy chọn tối ưu Windows ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
            210, 10, 535, 430, hwnd, (HMENU)200, NULL, NULL);

        // Các tùy chọn Checkbox mô phỏng phần mềm Optimizer
        hCheck1 = CreateWindowW(L"BUTTON", L"Tắt hiệu ứng chuyển động giao diện", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
            235, 45, 480, 25, hwnd, (HMENU)301, NULL, NULL);
        hCheck2 = CreateWindowW(L"BUTTON", L"Vô hiệu hóa Telemetry và thu thập dữ liệu", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
            235, 85, 480, 25, hwnd, (HMENU)302, NULL, NULL);
        hCheck3 = CreateWindowW(L"BUTTON", L"Tối ưu tốc độ mạng (TCP/IP Stack)", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
            235, 125, 480, 25, hwnd, (HMENU)303, NULL, NULL);

        // Đặt trạng thái mặc định được chọn sẵn cho checkbox
        SendMessage(hCheck1, BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(hCheck2, BM_SETCHECK, BST_CHECKED, 0);

        // Nút thực thi chính
        HWND btnExec = CreateWindowW(L"BUTTON", L"Áp dụng tùy chọn", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            570, 385, 155, 38, hwnd, (HMENU)401, NULL, NULL);

        // Áp dụng font Segoe UI cho toàn bộ các control
        SendMessage(btn1, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(btn2, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(btn3, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hGroup, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hCheck1, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hCheck2, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hCheck3, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(btnExec, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;
    }
    case WM_COMMAND:
        // Xử lý sự kiện bấm Checkbox khi người dùng click vào chữ
        if (LOWORD(wParam) >= 301 && LOWORD(wParam) <= 303) {
            HWND hChk = (HWND)lParam;
            int state = SendMessage(hChk, BM_GETCHECK, 0, 0);
            SendMessage(hChk, BM_SETCHECK, state == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED, 0);
        }
        // Xử lý nút Áp dụng
        if (LOWORD(wParam) == 401) {
            MessageBoxW(hwnd, L"Đã áp dụng các tối ưu thành công!", L"Optimizer Tree", MB_OK | MB_ICONINFORMATION);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
