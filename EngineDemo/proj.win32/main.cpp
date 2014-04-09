#include "main.h"
#include "../Classes/AppDelegate.h"
#include "cocos2d.h"
#include "CCGLView.h"

#ifdef WIN32
#ifdef _DEBUG
//#include <vld.h>
#endif
#endif

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	//...
	UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    //AppDelegate app;
    //GLView eglView;
   // eglView.init("Cocos 3D Demos",960,640);
    //return Application::getInstance()->run();

	AppDelegate app;
    return Application::getInstance()->run();
}