//
//  arkMenuBar.cpp
//  arkGUI
//
//  Created by anthonycouret on 17/02/2015.
//  Copyright (c) 2015 ac. All rights reserved.
//

#include "arkMenuBar.h"

arkMenuBarShPtr arkMenuBar::create( arkAbstractMediatorShPtr mediator_shptr )
{
    arkMenuBarShPtr arkMenuBar_sh_ptr( new arkMenuBar( mediator_shptr ) );
    arkMenuBar_sh_ptr->m_weak_ptr = arkMenuBar_sh_ptr;
    return arkMenuBar_sh_ptr;
}

arkMenuBar::arkMenuBar( arkAbstractMediatorShPtr mediator_shptr ) :
QMenuBar(),
arkMediatorWidget( mediator_shptr )
{
    setObjectName( QString::fromUtf8( "LEADR menu bar" ) );
    
    QMenu * file_menu = new QMenu( tr( "&File" ), this );
    QMenu * rendering_menu = new QMenu( tr( "&Rendering" ), this );
    
    QAction * load_model_action = file_menu->addAction( tr( "&Load Model" ) );
    QAction * load_texture_action = file_menu->addAction( tr( "&Load Texture" ) );
    QAction * load_env_action = file_menu->addAction( tr( "&Load Env" ) );
    QAction * load_irr_map_action = file_menu->addAction( tr( "&Load Irr Map" ) );
    
    QAction * standard_action = rendering_menu->addAction( tr( "&Standard" ) );
    QAction * wireframe_action = rendering_menu->addAction( tr( "&Wireframe" ) );
    QAction * depth_action = rendering_menu->addAction( tr( "&Depth" ) );
    
    
    connect( load_model_action, SIGNAL( triggered() ), this, SLOT( loadModel() ) );
    connect( load_texture_action, SIGNAL( triggered() ), this, SLOT( loadTexture() ) );
    connect( load_env_action, SIGNAL( triggered() ), this, SLOT( loadEnvTexture() ) );
    connect( load_irr_map_action, SIGNAL( triggered() ), this, SLOT( loadIrradianceMap() ) );

    connect( standard_action, SIGNAL( triggered() ), this, SLOT( setStandardRendering() ) );
    connect( wireframe_action, SIGNAL( triggered() ), this, SLOT( setWireframeRendering() ) );
    connect( depth_action, SIGNAL( triggered() ), this, SLOT( setDepthRendering() ) );
    
    addMenu( file_menu );
    addMenu( rendering_menu );
}

arkMenuBar::~arkMenuBar()
{
    
}

void arkMenuBar::loadModel()
{
    QStringList files_paths =
    QFileDialog::getOpenFileNames( this, tr( "Select Model files" ), "", tr( "Model Files (*.mtl *.obj)" ) );

    if ( ! files_paths.isEmpty() )
    {
        if (files_paths.length() <= 2)
        {
            std::vector< std::string > files;
            for ( QStringList::iterator it = files_paths.begin(); it != files_paths.end(); ++it )
            {
                files.push_back( it->toStdString() );
            }
            m_mediator_shptr->loadModel( files );
        }
    }
}

void arkMenuBar::loadTexture()
{
    QStringList files_paths =
    QFileDialog::getOpenFileNames( this, tr( "Select Displacement files" ), "", tr( "Model Files (*.jpg *.leadrshc)" ) );

    if ( ! files_paths.isEmpty() )
    {
        if (files_paths.length() <= 2)
        {
            std::vector< std::string > files;
            for ( QStringList::iterator it = files_paths.begin(); it != files_paths.end(); ++it )
            {
                files.push_back( it->toStdString() );
            }
            m_mediator_shptr->loadTexture( files );
        }
    }
}

void arkMenuBar::loadEnvTexture()
{
    QStringList files_paths =
    QFileDialog::getOpenFileNames( this, tr( "Select Environnement file" ), "", tr( "Model Files (*.hdr)" ) );

    if ( ! files_paths.isEmpty() )
    {
        if (files_paths.length() <= 1)
        {
            m_mediator_shptr->loadEnvTexture( files_paths.begin()->toStdString() );
        }
    }
}

void arkMenuBar::loadIrradianceMap()
{
    QStringList files_paths =
    QFileDialog::getOpenFileNames( this, tr( "Select Irradiance file" ), "", tr( "Model Files (*.leadrshc)" ) );

    if ( ! files_paths.isEmpty() )
    {
        if (files_paths.length() <= 1)
        {
            m_mediator_shptr->loadIrradianceMap( files_paths.begin()->toStdString() );
        }
    }
}

void arkMenuBar::setStandardRendering()
{
    m_mediator_shptr->setStandardRendering();
}

void arkMenuBar::setWireframeRendering()
{
    m_mediator_shptr->setWireframeRendering();
}

void arkMenuBar::setDepthRendering()
{
    m_mediator_shptr->setDepthRendering();
}
