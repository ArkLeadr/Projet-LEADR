
#include <iostream>
#include "Graphics/renderer.h"

#ifdef USE_QT
    #include <QApplication>
    #include "arkMediator.h"
    #include "arkQTGUI/arkMainWindow.h"
#endif

#ifdef USE_SDL2
    #include "Sdl2GUI/mainwindow.h"
#endif



int main(int argc, char * argv[])
{
    int width = 640 + 89 + 45; int height = 480 + 75;

    Renderer renderer( width, height );
    
#ifdef USE_SDL2    
    (void) argc;
    (void) argv;
    
    MainWindow w("DogeGL Next Gen", width, height);
    
    w.setRenderer(&renderer);
    
    w.runLoop();    
#endif
    

#ifdef USE_QT    
    QApplication application( argc, argv );
    
    arkMediatorShPtr mediator_shptr = arkMediator::create( &renderer );

    arkMainWindowShPtr main_window_shptr = arkMainWindow::create( width, height, mediator_shptr );
    
    main_window_shptr->show();
    
    return application.exec();    
#endif

    return 0;
}
