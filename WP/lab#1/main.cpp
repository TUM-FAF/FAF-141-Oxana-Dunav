#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

//Define the id values of the windows
#define IDC_InputArea            100
#define IDC_OutputArea           101
#define IDC_SendButton           102
#define IDC_ResetButton          103
#define IDC_ChangeColorButton    104
#define IDC_ChangeFontButton     105
#define IDC_ChangeBkButton      106


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(255,228,196));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("WP lab1"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = NULL ;
    PAINTSTRUCT ps ;
    RECT rect ;
    static HWND hwndInputArea, hwndOutputArea, hwndSendButton, hwndResetButton, hwndChangeColorButton, hwndChangeBkButton, hwndChangeFontButton;
    static int cxCoord, cyCoord;
    HFONT fontOutputText, buttonFont;
    int screenW;
    int screenH;
    LRESULT textSize;
    char textData[1000];
    char* fonts[] = {"Tahoma", "Lucida Calligraphy", "Courier New", "Monotype Corsiva", "Calibri"};
    COLORREF colors[] = {RGB(255, 0, 0), RGB(0,0,205), RGB(199,21,133), RGB(139,69,19), RGB(0,100,0)};
    COLORREF bks[] = {RGB(176,196,222), RGB(175,238,238), RGB(255,215,0), RGB(224,255,255), RGB(255,215,0)};
    static int fontNr = 0, colorNr = 0, bkNr = 0;
    static COLORREF usedColor;
    static COLORREF usedBk;
    static bool colorBtn, bkBtn;

    switch (message)                  /* handle the messages */
    {

        case WM_CREATE: {
            hwndInputArea = CreateWindowEx(
                            WS_EX_OVERLAPPEDWINDOW,                      // style
                            TEXT("EDIT"),
                            TEXT(" "),
                            WS_VISIBLE | WS_CHILD| ES_MULTILINE|
                            ES_AUTOVSCROLL | WS_VSCROLL,
                            0, 0, 0, 0,                                  //left, top, width, height
                            hwnd,                                        //parent window handle
                            (HMENU)IDC_InputArea,                        // the id of the textbox
                            GetModuleHandle(NULL),
                            NULL);                                       // extra bits not needed


            hwndOutputArea = CreateWindowEx(
                            WS_EX_OVERLAPPEDWINDOW,                      // style
                            TEXT("EDIT"),
                            TEXT(" "),
                            WS_VISIBLE | WS_CHILD| ES_MULTILINE|
                            ES_AUTOVSCROLL | WS_VSCROLL| ES_READONLY,
                            0, 0, 0, 0,                                  //left, top, width, height
                            hwnd,                                        //parent window handle
                            (HMENU)IDC_OutputArea,
                            GetModuleHandle(NULL),
                            NULL);                                       // extra bits not needed

            hwndSendButton = CreateWindowEx(
                            NULL,
                            TEXT("BUTTON"),
                            TEXT("Send"),
                            WS_VISIBLE | WS_CHILD| BS_DEFPUSHBUTTON| BS_OWNERDRAW,
                            0, 0, 0, 0,                                  //left, top, width, height
                            hwnd,                                        //parent window handle
                            (HMENU)IDC_SendButton,
                            GetModuleHandle(NULL),
                            NULL);                                       // extra bits not needed

            hwndResetButton = CreateWindowEx(
                            NULL,
                            TEXT("BUTTON"),
                            TEXT("Reset"),
                            WS_VISIBLE | WS_CHILD| BS_DEFPUSHBUTTON| BS_OWNERDRAW,
                            0, 0, 0, 0,                                  //left, top, width, height
                            hwnd,                                        //parent window handle
                            (HMENU)IDC_ResetButton,
                            GetModuleHandle(NULL),
                            NULL);                                       // extra bits not needed


            hwndChangeColorButton = CreateWindowEx(
                            NULL,
                            TEXT("BUTTON"),
                            TEXT("Change Color"),
                            WS_VISIBLE | WS_CHILD| BS_DEFPUSHBUTTON,
                            0, 0, 0, 0,                                  //left, top, width, height
                            hwnd,                                        //parent window handle
                            (HMENU)IDC_ChangeColorButton,
                            GetModuleHandle(NULL),
                            NULL);                                       // extra bits not needed

            hwndChangeFontButton = CreateWindowEx(
                            NULL,
                            TEXT("BUTTON"),
                            TEXT("Change Font"),
                            WS_VISIBLE | WS_CHILD| BS_DEFPUSHBUTTON,
                            0, 0, 0, 0,                                  //left, top, width, height
                            hwnd,                                        //parent window handle
                            (HMENU)IDC_ChangeFontButton,
                            GetModuleHandle(NULL),
                            NULL);                                       // extra bits not needed


            hwndChangeBkButton = CreateWindowEx(
                            NULL,
                            TEXT("BUTTON"),
                            TEXT("Background"),
                            WS_VISIBLE | WS_CHILD|BS_DEFPUSHBUTTON,
                            0, 0, 0, 0,                                  //left, top, width, height
                            hwnd,                                        //parent window handle
                            (HMENU)IDC_ChangeBkButton,
                            GetModuleHandle(NULL),
                            NULL);

            HGDIOBJ defaultFont = GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hwndInputArea, WM_SETTEXT, NULL, (LPARAM)"Enter your text...");
            SendMessage(hwndInputArea, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE,0));
            SendMessage(hwndChangeColorButton, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE,0));
            SendMessage(hwndChangeBkButton, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE,0));
            SendMessage(hwndChangeFontButton, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE,0));
            SetFocus(hwndInputArea);
            break;
        }

        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);
            SetBkColor(hdc, RGB(255,228,196));
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(0,0,128));
            DrawText(hdc, TEXT("Done with Pride and Prejudice by Oxana"), -1, &rect, DT_SINGLELINE| DT_CENTER| DT_VCENTER);
            EndPaint(hwnd, &ps);

            break;
        }

        case WM_DRAWITEM: {
            if ((UINT)wParam == IDC_SendButton) {
                LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                SIZE size;
                char szBtnText[] = "Send";

                GetTextExtentPoint32(lpdis->hDC, szBtnText, strlen(szBtnText), &size);
                SetTextColor(lpdis->hDC, RGB(250, 250, 250));
                SetBkColor(lpdis->hDC, RGB(255,160,122));

                ExtTextOut(
                    lpdis->hDC,
                    ((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,
                    ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,
                    ETO_OPAQUE | ETO_CLIPPED,
                    &lpdis->rcItem,
                    szBtnText,
                    strlen(szBtnText),
                    NULL);

                DrawEdge(
                    lpdis->hDC,
                    &lpdis->rcItem,
                    (lpdis->itemState & ODS_SELECTED ? EDGE_SUNKEN : EDGE_RAISED ),
                    BF_RECT);
                    return TRUE;
            }
            else if((UINT)wParam == IDC_ResetButton) {
                LPDRAWITEMSTRUCT lpdis = (DRAWITEMSTRUCT*)lParam;
                SIZE size;
                char szResetBtnText[] = "Reset";

                GetTextExtentPoint32(lpdis->hDC, szResetBtnText, strlen(szResetBtnText), &size);
                SetTextColor(lpdis->hDC, RGB(250, 250, 250));
                SetBkColor(lpdis->hDC, RGB(255,160,122));

                ExtTextOut(
                    lpdis->hDC,
                    ((lpdis->rcItem.right - lpdis->rcItem.left) - size.cx) / 2,
                    ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2,
                    ETO_OPAQUE | ETO_CLIPPED,
                    &lpdis->rcItem,
                    szResetBtnText,
                    strlen(szResetBtnText),
                    NULL);

                DrawEdge(
                    lpdis->hDC,
                    &lpdis->rcItem,
                    (lpdis->itemState & ODS_SELECTED ? EDGE_SUNKEN : EDGE_RAISED ),
                    BF_RECT);
                return TRUE;
            }
            break;
        }

        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
            mmi->ptMinTrackSize.x = 300;
            mmi->ptMinTrackSize.y = 350;
            mmi->ptMaxTrackSize.x = 700;
            mmi->ptMaxTrackSize.y = 450;
            break;
        }

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_SendButton:
                {
                    textSize = SendMessage(hwndInputArea, WM_GETTEXT, 1000, (LPARAM)textData); // text size
                    textData[textSize] = _T('\0'); // initialization with null character
                    SendMessage(hwndOutputArea, EM_REPLACESEL, 0, (LPARAM)textData); // add input text to window
                    SendMessage(hwndOutputArea, EM_REPLACESEL, 0, (LPARAM)" ");
                    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
                    fontOutputText = CreateFont(20,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("Monotype Corsiva"));
                    SendMessage(hwndOutputArea, WM_SETFONT, (WPARAM)fontOutputText, TRUE);
                    break;
                }

                case IDC_ResetButton:
                {
                    SendMessage(hwndOutputArea, WM_SETTEXT, NULL, NULL);
                    break;
                }

                case IDC_ChangeColorButton: {
                    colorBtn = 1;
                    InvalidateRect(hwndOutputArea, NULL, TRUE);
                    break;
                }


                case IDC_ChangeFontButton: {
                    fontOutputText = CreateFont(20,0,0,0,0,0,0,0,0,0,0,0,0,TEXT(fonts[fontNr]));
                    SendMessage(hwndOutputArea, WM_SETFONT, (WPARAM)fontOutputText, TRUE);
                    fontNr++;
                    fontNr = fontNr % 5;
                    break;
                }

                case IDC_ChangeBkButton: {
                    bkBtn = 1;
                    InvalidateRect(hwndOutputArea, NULL, TRUE);
                    break;
                }
            }
            break;


        case WM_SYSCOMMAND:
        {
            switch(wParam)
            {
                case SC_MINIMIZE:
                {
                    return MessageBox(NULL, TEXT("Minimize Button doesn't work"), TEXT("Oh noooooo!"), MB_OKCANCEL | MB_ICONINFORMATION);
                }

                case SC_MAXIMIZE:
                {
                    screenW = GetSystemMetrics(SM_CXSCREEN);
                    screenH = GetSystemMetrics(SM_CYSCREEN);

                    GetWindowRect(hwnd, &rect);
                    SetWindowPos(
                        hwnd, 0,
                        (screenW - rect.right) / 10 * (rand() % 11),
                        (screenH - rect.bottom) / 10 * (rand() % 11),
                        0, 0,
                        SWP_NOZORDER|SWP_NOSIZE);
                    break;
                }

                case SC_CLOSE:
                {
                    if(MessageBox(hwnd, "Do you want to quit?", "Alert", MB_YESNO) == IDYES)
                    {
                        exit(1);
                    }
                    break;
                }

                default:
                    return DefWindowProc(hwnd, message, wParam, lParam);
            }
            break;

            case WM_SIZE:
            {
                cxCoord = LOWORD(lParam); // 544
                cyCoord = HIWORD(lParam); // 375
                MoveWindow(hwndInputArea, 15, 30, cxCoord-200, cyCoord/3, TRUE);
                MoveWindow(hwndOutputArea, 15, cyCoord/2+30, cxCoord-200, cyCoord/3, TRUE);

                MoveWindow(hwndSendButton, cxCoord-150, cyCoord/3-80, 110, 40, TRUE);
                MoveWindow(hwndResetButton, cxCoord-150, cyCoord/3-25, 110, 40, TRUE);

                MoveWindow(hwndChangeColorButton, cxCoord-150, cyCoord/2+30, 110, 30, TRUE);
                MoveWindow(hwndChangeBkButton, cxCoord-150, cyCoord/2+70, 110, 30, TRUE);
                MoveWindow(hwndChangeFontButton, cxCoord-150, cyCoord/2+115, 110, 30, TRUE);

                break;
            }

            case WM_CTLCOLOREDIT: {
               if(IDC_InputArea == GetDlgCtrlID((HWND)lParam))
               {
                    HDC hdcStatic = (HDC) wParam;
                    SetBkColor(hdcStatic, RGB(255,160,122));
                    return (INT_PTR)CreateSolidBrush(RGB(255,160,122));
                }
                break;
            }

            case WM_CTLCOLORSTATIC:
            {
                if(colorBtn == true && (HWND)lParam == hwndOutputArea) {

                    HBRUSH hbr = (HBRUSH) DefWindowProc(hwnd, message, wParam, lParam);
                    SetTextColor((HDC) wParam, colors[colorNr]);
                    usedColor = colors[colorNr];
                    colorNr++;
                    colorNr = colorNr % 5;
                    colorBtn = 0;
                    return (BOOL) hbr;
                }
                else if(bkBtn == true && (HWND)lParam == hwndOutputArea)
                {
                    HDC hdcStatic = (HDC) wParam;
                    SetBkColor(hdcStatic, bks[bkNr]);
                    usedBk = bks[bkNr];
                    bkNr++;
                    bkNr = bkNr % 5;
                    bkBtn = 0;
                    return (INT_PTR)CreateSolidBrush(usedBk);
                }
                break;
            }

            case WM_DESTROY:
            {
                PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
                break;
            }

            default:                      /* for messages that we don't deal with */
                return DefWindowProc (hwnd, message, wParam, lParam);
        }

        return 0;
    }
}

