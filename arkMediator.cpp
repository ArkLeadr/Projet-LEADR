//
//  arkMediator.cpp
//  arkGUI
//
//  Created by anthonycouret on 17/02/2015.
//  Copyright (c) 2015 ac. All rights reserved.
//

#include "arkMediator.h"

arkMediatorShPtr arkMediator::create( Renderer * renderer )
{
    arkMediatorShPtr arkMediator_sh_ptr( new arkMediator( renderer ) );
    arkMediator_sh_ptr->m_weak_ptr = arkMediator_sh_ptr;
    return arkMediator_sh_ptr;
}

arkMediator::arkMediator( Renderer * renderer )
{
    m_renderer = renderer;
}

arkMediator::~arkMediator()
{
}

void arkMediator::setTesselationFactor( float tesselation_factor )
{
    m_tesselation_factor = tesselation_factor;
    m_renderer->setTesselationFactor( m_tesselation_factor );
}

void arkMediator::setDispFactor( float disp_factor )
{
    m_disp_factor = disp_factor;
    m_renderer->setDispFactor( m_disp_factor );
}

void arkMediator::setNbSamples( int nb_samples )
{
    m_nb_samples = nb_samples;
    m_renderer->setNbSamples( m_nb_samples );
}

void arkMediator::loadModel( const std::vector< std::string > & model_paths )
{
    m_renderer->loadModel( model_paths );
}

void arkMediator::loadTexture( const std::vector< std::string > & tex_paths )
{
    m_renderer->loadTexture( tex_paths );
}

void arkMediator::loadEnvTexture( const std::string & env_path )
{
    m_renderer->loadEnvTexture( env_path );
}

void arkMediator::loadIrradianceMap( const std::string & irr_path )
{
    m_renderer->loadIrradianceMap( irr_path );
}

void arkMediator::setStandardRendering()
{
    m_renderer->setFinalFboTarget(0);
}

void arkMediator::setWireframeRendering()
{
    m_renderer->setWireframeRendering();
}

void arkMediator::setDepthRendering()
{
    m_renderer->setDepthRendering();
}

void arkMediator::setNormalRendering()
{
    m_renderer->setFinalFboTarget(1);
}

void arkMediator::setTexcoordRendering()
{
    m_renderer->setFinalFboTarget(2);
}

void arkMediator::setLEADRMode( bool leadr_mode )
{
    m_renderer->setLEADRMode( leadr_mode );
}

void arkMediator::setNormalMode( bool normal_mode )
{
    m_renderer->setNormalMode( normal_mode );
}

void arkMediator::reloadShader()
{
    m_renderer->reloadShader();
}


void arkMediator::initializeGL()
{
    m_renderer->initializeGL();
}

void arkMediator::resizeGL( int width, int height )
{
    m_renderer->resizeGL( width, height );
}

void arkMediator::paintGL()
{
    m_renderer->paintGL();
}

void arkMediator::rotateCamera( int mouse_x_rel, int mouse_y_rel )
{
    m_renderer->rotateCamera( mouse_x_rel, mouse_y_rel );
}

void arkMediator::translateCamera( int mouse_x_rel, int mouse_y_rel, int mouse_z_rel )
{
    m_renderer->translateCamera( mouse_x_rel, mouse_y_rel, mouse_y_rel );
}

void arkMediator::onKeyPress( int qt_key )
{
    m_renderer->onKeyPress( qt_key );
}
