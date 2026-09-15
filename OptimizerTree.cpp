#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <string>
#include <vector>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define ID_CLEAN 1001
#define ID_OPTIMIZE 1002
#define ID_TEMP 1003
#define ID_STORAGE 1004
#define ID_TOOLS 1005
#define ID_SETTINGS 1006
#define ID_TIMER 2001

HINSTANCE g_hInst;
HFONT g_font = nullptr, g_bold = nullptr;
COLORREF BG = RGB(18,22,27), PANEL = RGB(25,30,36), TEXT = RGB(235,240,245), MUTED = RGB(150,160,170), ACCENT = RGB(91,190,110);

void Fill(HDC hdc, RECT r, COLORREF c){ HBRUSH b=CreateSolidBrush(c); FillRect(hdc,&r,b); DeleteObject(b); }
void Text(HDC hdc,const wchar_t* s,int x,int y,int size=16,bool bold=false,COLORREF c=TEXT){
    HFONT f=CreateFontW(size,0,0,0,bold?FW_SEMIBOLD:FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH,L"Segoe UI");
    HFONT old=(HFONT)SelectObject(hdc,f); SetTextColor(hdc,c); SetBkMode(hdc,TRANSPARENT); TextOutW(hdc,x,y,s,(int)wcslen(s)); SelectObject(hdc,old); DeleteObject(f);
}
void RoundPanel(HDC hdc, RECT r, COLORREF c, int radius=14){ HBRUSH b=CreateSolidBrush(c); HPEN p=CreatePen(PS_SOLID,1,c); auto ob=(HBRUSH)SelectObject(hdc,b); auto op=(HPEN)SelectObject(hdc,p); RoundRect(hdc,r.left,r.top,r.right,r.bottom,radius,radius); SelectObject(hdc,ob); SelectObject(hdc,op); DeleteObject(b); DeleteObject(p); }
void Button(HDC hdc, RECT r, const wchar_t* label, bool primary=false){ RoundPanel(hdc,r,primary?ACCENT:RGB(38,45,52),10); SIZE sz{}; GetTextExtentPoint32W(hdc,label,(int)wcslen(label),&sz); Text(hdc,label,(r.left+r.right-sz.cx)/2,(r.top+r.bottom-sz.cy)/2-1,14,true,primary?RGB(10,25,14):TEXT); }

bool RemoveDirectoryContents(const std::wstring& dir, unsigned long long& freed){
    std::wstring pat=dir+L"\\*"; WIN32_FIND_DATAW fd{}; HANDLE h=FindFirstFileW(pat.c_str(),&fd); if(h==INVALID_HANDLE_VALUE) return false;
    do{
        if(!wcscmp(fd.cFileName,L".")||!wcscmp(fd.cFileName,L"..")) continue;
        std::wstring p=dir+L"\\"+fd.cFileName;
        if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){ RemoveDirectoryContents(p,freed); RemoveDirectoryW(p.c_str()); }
        else { ULARGE_INTEGER n; n.HighPart=fd.nFileSizeHigh; n.LowPart=fd.nFileSizeLow; SetFileAttributesW(p.c_str(),FILE_ATTRIBUTE_NORMAL); if(DeleteFileW(p.c_str())) freed+=n.QuadPart; }
    }while(FindNextFileW(h,&fd)); FindClose(h); return true;
}
void CleanTemp(HWND hwnd){
    wchar_t path[MAX_PATH]; unsigned long long freed=0;
    if(GetTempPathW(MAX_PATH,path)) RemoveDirectoryContents(path,freed);
    std::wstring msg=L"Đã dọn file tạm. Giải phóng khoảng "+std::to_wstring(freed/1024/1024)+L" MB.";
    MessageBoxW(hwnd,msg.c_str(),L"Optimizer Tree",MB_OK|MB_ICONINFORMATION);
}
void RunAdminTool(HWND hwnd,const wchar_t* args){
    SHELLEXECUTEINFOW si{sizeof(si)}; si.lpVerb=L"runas"; si.lpFile=L"cmd.exe"; si.lpParameters=args; si.nShow=SW_SHOW; 
    if(!ShellExecuteExW(&si)) MessageBoxW(hwnd,L"Không thể chạy công cụ. Windows có thể đã từ chối quyền quản trị.",L"Optimizer Tree",MB_OK|MB_ICONWARNING);
}
void OpenSettings(HWND hwnd,const wchar_t* uri){ ShellExecuteW(hwnd,L"open",uri,nullptr,nullptr,SW_SHOWNORMAL); }

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
    case WM_CREATE: SetTimer(hwnd,ID_TIMER,1000,nullptr); return 0;
    case WM_TIMER: InvalidateRect(hwnd,nullptr,FALSE); return 0;
    case WM_COMMAND:
        switch(LOWORD(wParam)){
            case ID_CLEAN: CleanTemp(hwnd); break;
            case ID_OPTIMIZE: CleanTemp(hwnd); RunAdminTool(hwnd,L"/c sfc /scannow"); break;
            case ID_TEMP: CleanTemp(hwnd); break;
            case ID_STORAGE: OpenSettings(hwnd,L"ms-settings:storagesense"); break;
            case ID_TOOLS: OpenSettings(hwnd,L"ms-settings:appsfeatures"); break;
            case ID_SETTINGS: OpenSettings(hwnd,L"ms-settings:"); break;
        } return 0;
    case WM_PAINT:{
        PAINTSTRUCT ps; HDC hdc=BeginPaint(hwnd,&ps); RECT rc; GetClientRect(hwnd,&rc); Fill(hdc,rc,BG);
        RECT side{0,0,225,rc.bottom}; Fill(hdc,side,PANEL);
        Text(hdc,L"Optimizer",28,26,24,true,TEXT); Text(hdc,L"TREE",28,55,24,true,ACCENT); Text(hdc,L"Windows utility",29,87,12,false,MUTED);
        RECT navs[]={ {20,125,205,170},{20,180,205,225},{20,235,205,280},{20,290,205,335},{20,345,205,390} };
        const wchar_t* labels[]={L"⌂   Dashboard",L"⚡   Optimize",L"♲   Cleanup",L"▣   Storage",L"⚙   Tools"};
        for(int i=0;i<5;i++) Text(hdc,labels[i],36,navs[i].top+14,14,i==0,TEXT);
        Text(hdc,L"v1.0",30,rc.bottom-32,12,false,MUTED);

        Text(hdc,L"Dashboard",255,28,27,true,TEXT); Text(hdc,L"Tối ưu Windows an toàn, nhanh và gọn.",255,67,14,false,MUTED);
        RECT hero{255,105,rc.right-28,215}; RoundPanel(hdc,hero,RGB(29,43,34),18); Text(hdc,L"Ready to optimize",280,128,16,true,TEXT); Text(hdc,L"Dọn file tạm và chạy kiểm tra hệ thống.",280,158,13,false,MUTED); Button(hdc,{rc.right-190,140,rc.right-55,184},L"Optimize Now",true);
        RECT cards[3]={{255,240,rc.right-28,325},{255,340,rc.right-28,425},{255,440,rc.right-28,525}};
        const wchar_t* heads[]={L"CPU",L"Memory",L"Disk"}; const wchar_t* vals[]={L"Windows",L"RAM",L"Storage"};
        for(int i=0;i<3;i++){ RoundPanel(hdc,cards[i],PANEL,14); Text(hdc,heads[i],278,cards[i].top+18,13,true,MUTED); Text(hdc,vals[i],278,cards[i].top+45,20,true,TEXT); Text(hdc,L"Open Windows tools to inspect",278,cards[i].top+69,11,false,MUTED); }
        EndPaint(hwnd,&ps); return 0;
    }
    case WM_DESTROY: KillTimer(hwnd,ID_TIMER); PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd,msg,wParam,lParam);
}

int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE,LPWSTR,int nCmdShow){
    g_hInst=hInstance;
    WNDCLASSEXW wc{sizeof(wc)}; wc.lpfnWndProc=WndProc; wc.hInstance=hInstance; wc.hIcon=LoadIconW(hInstance,MAKEINTRESOURCEW(101)); wc.hCursor=LoadCursor(nullptr,IDC_ARROW); wc.hbrBackground=CreateSolidBrush(BG); wc.lpszClassName=L"OptimizerTreeClass"; RegisterClassExW(&wc);
    HWND hwnd=CreateWindowExW(0,L"OptimizerTreeClass",L"Optimizer Tree",WS_OVERLAPPEDWINDOW,100,80,1050,680,nullptr,nullptr,hInstance,nullptr);
    if(!hwnd) return 0; ShowWindow(hwnd,nCmdShow); UpdateWindow(hwnd);
    MSG msg; while(GetMessageW(&msg,nullptr,0,0)){ TranslateMessage(&msg); DispatchMessageW(&msg); } return (int)msg.wParam;
}
