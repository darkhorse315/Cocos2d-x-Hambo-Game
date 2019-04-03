#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"

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
	//eglView->setFrameSize(1024, 768);
	//eglView->setFrameSize(1024, 600);
	eglView->setFrameSize(800, 480);
	//eglView->setFrameSize(768, 576);
	//eglView->setFrameSize(800, 280);
    //eglView->setFrameSize(480, 320);
    return CCApplication::sharedApplication()->run();
}

