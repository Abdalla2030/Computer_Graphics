#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>

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
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Task 2: Circle with 8 parts"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           600,                 /* and height in pixels */
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

void parametricline(HDC hdc, double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    for (double t = 0; t < 1; t += 0.001) {
        int x = x1 + (dx * t);
        int y = y1 + (dy * t);
        SetPixel(hdc, x, y, RGB(0,0,0));
    }
}

void draw8points(HDC hdc,int x,int y,int xc,int yc)
{
    SetPixel(hdc, xc+x, yc+y, RGB(0,0,0));
    SetPixel(hdc, xc-x, yc+y, RGB(0,0,0));
    SetPixel(hdc, xc+x, yc-y, RGB(0,0,0));
    SetPixel(hdc, xc-x, yc-y, RGB(0,0,0));

    SetPixel(hdc, xc-y, yc+x, RGB(0,0,0));
    SetPixel(hdc, xc+y, yc-x, RGB(0,0,0));
    SetPixel(hdc, xc+y, yc+x, RGB(0,0,0));
    SetPixel(hdc, xc-y, yc-x, RGB(0,0,0));
}

void midpoint(HDC hdc,int xc,int yc,int r)
{
    int x=0;
    int y=r;
    double d=1-r;

    while(x<y){

        if(d<=0){
            d=d+2*x+3;
            x++;
        }

        else{
            d=d+2*(x-y)+5;
            x++;
            y--;
        }

        draw8points(hdc,x,y,xc,yc);
    }

}

const double PI = 3.1415926;

void drawEightLines(HDC hdc, int originX, int originY, int radius)
{

    parametricline(hdc, originX, originY, originX, originY + radius); // down
    parametricline(hdc, originX, originY, originX, originY - radius); // up
    parametricline(hdc, originX, originY, originX + radius, originY); // right
    parametricline(hdc, originX, originY, originX - radius, originY); // left

    // drawing the 4 diagonal lines
    for(int degree = 45; degree < 360; degree += 90){
        double rad = degree * PI / 180.0; // converting to radians

        parametricline(hdc, originX, originY,
                       originX + (radius * sin(rad)), originY + (radius * cos(rad)));
    }

}


/*  This function is called by the Windows function DispatchMessage()  */
int rr, x_1, y_1, x_2, y_2;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);

    switch (message)                  /* handle the messages */
    {

        case WM_LBUTTONDBLCLK:
            x_1 = LOWORD(lParam);
            y_1 = HIWORD(lParam);
            break;

        case WM_RBUTTONDBLCLK:
            x_2 = LOWORD(lParam);
            y_2 = HIWORD(lParam);

            rr = sqrt(pow((x_2-x_1), 2) + pow((y_2-y_1), 2));
            midpoint(hdc, x_1, y_1, rr);
            drawEightLines(hdc, x_1, y_1, rr);

            break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
