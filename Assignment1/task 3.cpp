
// Abdalla Fadl Shehata - 20190305


#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
//////////////////////////////////////////////////////////////////
#include <tchar.h>
#include <windows.h>
#include <cmath>
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
           _T("Code::Blocks Template Windows App"),       /* Title Text */
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
//////////////////////////////////////////
int roundd(double num){
	return (int)(num + 0.5);
}
///////////////////////////////////////////
void parametricLine(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color){
    double dx = x2-x1;
    double dy = y2-y1;
    for (double t = 0 ; t < 1;t+=0.001){
        int x = x1+(t*dx);
        int y = y1+(t*dy);
        SetPixel(hdc,x,y,color);
    }
}
//////////////////////////////////////////
void Draw8points(HDC hdc, int xc, int yc, int x, int y, COLORREF color){
	SetPixel(hdc, xc + x, yc + y,RGB(0,0,0));
	SetPixel(hdc, xc + x, yc - y, RGB(0,0,0));
	SetPixel(hdc, xc - x, yc + y, RGB(0,0,0));
	SetPixel(hdc, xc - x, yc - y,  RGB(0,0,0));
	SetPixel(hdc, xc + y, yc + x, RGB(0,0,0));
	SetPixel(hdc, xc + y, yc - x, RGB(0,0,0));
	SetPixel(hdc, xc - y, yc + x, RGB(0,0,0));
	SetPixel(hdc, xc - y, yc - x,  RGB(0,0,0));
	/////////////////////////////////////////////////////////
	parametricLine(hdc,xc,yc,xc + x, yc + y,RGB(0,0,255));
	parametricLine(hdc,xc,yc,xc + x, yc - y, RGB(76,153,0));
	parametricLine(hdc,xc,yc,xc - x, yc + y,RGB(0,255,0));
	parametricLine(hdc,xc,yc,xc - x, yc - y,RGB(0,0,0));
	parametricLine(hdc,xc,yc,xc + y, yc + x,RGB(255,0,0));
	parametricLine(hdc,xc,yc,xc + y, yc - x,RGB(0,255,255));
	parametricLine(hdc,xc,yc,xc - y, yc + x,RGB(255,0,127));
	parametricLine(hdc,xc,yc,xc - y, yc - x,RGB(255,255,0));
}
///////////////////////////////////////////////
void DrawCircleMidPoint(HDC hdc, int xc, int yc, int xe, int ye){
	double r = sqrt(((xe - xc)*(xe - xc)) + ((ye - yc)*(ye - yc)));
	int x = 0;
	double y = r;
	Draw8points(hdc, xc, yc, x, roundd(y),RGB(0,0,0));

	double d = 1 - r;
	while (x < y)
	{
		if (d < 0)
			d += (2 * x + 3);
		else
		{
			d += (2 * (x - y) + 5);
			y--;
		}
		x++;
		Draw8points(hdc, xc, yc, x, roundd(y),RGB(0,0,0));
	}
}
/////////////////////////////////////////////
/*  This function is called by the Windows function DispatchMessage()  */
 int xc,yc,xe,ye;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
       case WM_LBUTTONUP:
           xc = LOWORD(lParam);
           yc = HIWORD(lParam);
           break;
        case WM_RBUTTONDOWN:
           xe = LOWORD(lParam);
           ye = HIWORD(lParam);
          DrawCircleMidPoint(hdc, xc, yc, xe, ye);
           break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
