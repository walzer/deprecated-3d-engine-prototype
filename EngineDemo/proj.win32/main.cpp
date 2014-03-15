#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"

#ifdef WIN32
#ifdef _DEBUG
#include <vld.h>
#endif
#endif

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
   	UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("Cocos 3D Demos");

	eglView->setFrameSize(960, 640);

    // set the design resolution screen size, if you want to use Design Resoulution scaled to current screen, please uncomment next line.
    // eglView.setDesignResolutionSize(480, 320);
    return CCApplication::sharedApplication()->run();
}