
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
bool isInsideCircle(int xc, int yc, int xe, int ye,int xt,int yt){
     double r = sqrt(((xe - xc)*(xe - xc)) + ((ye - yc)*(ye - yc)));
     int dist = (xt - xc) * (xt - xc) + (yt - yc) * (yt - yc);
     if ( dist <= r * r) {
        return true;
    }
     return false;
}
/////////////////////////////////////////
void hermiteCurve(HDC hdc,int x_0,int sx_0,int x_1,int sx_1,int y_0,int sy_0,int y_1,int sy_1,int xc, int yc, int xe, int ye){
    int alf1=2*x_0+1*sx_0-2*x_1+sx_1;
    int beta1=-3*x_0-2*sx_0+3*x_1-sx_1;
    int gamm1=sx_0;
    int sigma1=x_0;

    int alf2=2*y_0+1*sy_0-2*y_1+sy_1;
    int beta2=-3*y_0-2*sy_0+3*y_1-sy_1;
    int gamm2=sy_0;
    int sigma2=y_0;
    for(double t=0;t<=1;t+=0.001){
            double xt=(t*t*t*alf1)+(t*t*beta1)+(t*gamm1)+sigma1;
            double yt=(t*t*t*alf2)+(t*t*beta2)+(t*gamm2)+sigma2;
            if(isInsideCircle(xc,yc,xe,ye,xt,yt)){
                 SetPixel(hdc,xt,yt,RGB(255,50,2));
            }
            else{
                 SetPixel(hdc,xt,yt,RGB(0,0,255));
            }
    }

}
//////////////////////////////////////////
int roundd(double num){
	return (int)(num + 0.5);
}
///////////////////////////////////////////
void Draw8points(HDC hdc, int xc, int yc, int x, int y, COLORREF color){
	SetPixel(hdc, xc + x, yc + y,color);
	SetPixel(hdc, xc + x, yc - y, color);
	SetPixel(hdc, xc - x, yc + y, color);
	SetPixel(hdc, xc - x, yc - y,  color);
	SetPixel(hdc, xc + y, yc + x, color);
	SetPixel(hdc, xc + y, yc - x, color);
	SetPixel(hdc, xc - y, yc + x, color);
	SetPixel(hdc, xc - y, yc - x,  color);
}
///////////////////////////////////////////////
void DrawCircleMidPoint(HDC hdc, int xc, int yc, int xe, int ye){
	double r = sqrt(((xe - xc)*(xe - xc)) + ((ye - yc)*(ye - yc)));
	int x = 0;
	double y = r;
	Draw8points(hdc, xc, yc, x, roundd(y),RGB(0,100,0));

	double d = 1 - r;
	while (x < y)
	{
		if (d < 0){
			d += (2 * x + 3);
		}
		else
		{
			d += (2 * (x - y) + 5);
			y--;
		}
		x++;
		Draw8points(hdc, xc, yc, x, roundd(y),RGB(0,100,0));
	}
}
/////////////////////////////////////////////
/*  This function is called by the Windows function DispatchMessage()  */
int cnt1 = 1 ; // count
int cnt2 = 1 ; // count
int xc,yc,xe,ye; // circle
int x0,sx0,x1,sx1,y0,sy0,y1,sy1;  // curve
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
       case WM_LBUTTONDBLCLK:
           if(cnt1 == 1 ){
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
               cnt1++;
           }
           break;
        case WM_RBUTTONDBLCLK:
            if (cnt1 == 2 ){
               xe = LOWORD(lParam);
               ye = HIWORD(lParam);
               DrawCircleMidPoint(hdc, xc, yc, xe, ye);
               cnt1++ ;
            }
           break;
        case WM_LBUTTONDOWN:
            if(cnt1 == 3){
                  x0=LOWORD(lParam);
                  y0=HIWORD(lParam);
                  cnt1++;
           }
           else if(cnt1 == 5){
                sx0=LOWORD(lParam);
                sy0=HIWORD(lParam);
                cnt1++;
           }
            break ;
          case WM_RBUTTONUP:
            if(cnt1 == 4 ){
                  x1=LOWORD(lParam);
                  y1=HIWORD(lParam);
                  cnt1++;
            }
            else if(cnt1 == 6){
                 sx1=LOWORD(lParam);
                 sy1=HIWORD(lParam);
                 hermiteCurve(hdc,x0,sx0,x1,sx1,y0,sy0,y1,sy1,xc,yc,xe,ye);
                 if(cnt2==4){
                       cnt1 = 1 ;
                       cnt2 = 0 ;
                 }
                 else{
                    cnt1 = 3 ;
                 }
                 cnt2++;
            }
            break ;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
