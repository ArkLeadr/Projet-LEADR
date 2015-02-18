#include <iostream>

#ifdef USE_QT
    #include <QApplication>
    #include "QtGUI/mainwindow.h"
#endif

#ifdef USE_SDL2
    #include "Sdl2GUI/mainwindow.h"
#endif

#include "Graphics/renderer.h"

int main(int argc, char *argv[])
{
    Renderer renderer(640, 480);

    #ifdef USE_SDL2

    (void) argc;
    (void) argv;

    MainWindow w("DogeGL Next Gen", 640, 480);

    w.setRenderer(&renderer);

    w.runLoop();

    #endif


    #ifdef USE_QT

    QApplication app(argc, argv);



    MainWindow w;
    w.setRenderer(&renderer); //DO NOT FORGET

    w.show();

    return(app.exec());

    #endif

    return 0;
}

