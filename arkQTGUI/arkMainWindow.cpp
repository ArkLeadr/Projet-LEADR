//
//  arkMainWindow.cpp
//  arkGUI
//
//  Created by anthonycouret on 17/02/2015.
//  Copyright (c) 2015 ac. All rights reserved.
//

#include "arkMainWindow.h"

#include <QDockWidget>

arkMainWindowShPtr arkMainWindow::create
( int width, int height, arkAbstractMediatorShPtr mediator_shptr )
{
    arkMainWindowShPtr arkMainWindow_sh_ptr( new arkMainWindow( width, height, mediator_shptr ) );
    arkMainWindow_sh_ptr->m_weak_ptr = arkMainWindow_sh_ptr;
    return arkMainWindow_sh_ptr;
}

arkMainWindow::arkMainWindow
( int width, int height, arkAbstractMediatorShPtr mediator_shptr ) :
QMainWindow(),
arkMediatorWidget( mediator_shptr )
{
    setGeometry( 0, 0, width, height );
    
    m_menu_bar = arkMenuBar::create( m_mediator_shptr );
    setMenuBar( m_menu_bar.get() );
    
    QGLFormat fmt;
    fmt.setDoubleBuffer(true);
    fmt.setDirectRendering(true);
    fmt.setRgba(true);
    fmt.setStencil(false);
    fmt.setDepth(true);
    fmt.setAlpha(false);
    fmt.setVersion(4, 1);
    fmt.setProfile(QGLFormat::CoreProfile);
    
    if (!(QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_4_1))
    {
        std::cout << "Failed to create a valid Core Opengl 4.1 Profile. \n";
        exit(EXIT_FAILURE);
    }
    
    QDockWidget * dock_ogl_widget = new QDockWidget( "OpenGL Widget" );
    m_ogl_widget = arkOpenGLWidget::create( fmt, m_mediator_shptr );
    dock_ogl_widget->setWidget( m_ogl_widget.get() );
    addDockWidget(Qt::LeftDockWidgetArea, dock_ogl_widget );
    
    QDockWidget * dock_params_widget = new QDockWidget( "Params Widget" );
    m_params_panel = arkParamsPanel::create( m_mediator_shptr );
    dock_params_widget->setWidget( m_params_panel.get() );
    addDockWidget(Qt::RightDockWidgetArea, dock_params_widget );
}

arkMainWindow::~arkMainWindow()
{
    
}


