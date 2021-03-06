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

void arkMediator::setRoughnessOffset(double roughnessOffset)
{
    m_renderer->setRoughnessOffset(roughnessOffset);
}

void arkMediator::setFresnel0(double fresnel0)
{
    m_renderer->setFresnel0(fresnel0);
}

void arkMediator::loadModel( const std::string &model_path )
{
    m_renderer->loadModel( model_path );
}

void arkMediator::loadLEADRTexture(  const std::string & leadr  )
{
    m_renderer->loadLEADRTexture( leadr );
}
void arkMediator::loadTexture(  const std::string & texture )
{
    m_renderer->loadTexture( texture );
}
void arkMediator::loadDispMap(  const std::string & disp_map  )
{
    m_renderer->loadDispMap( disp_map );
}
void arkMediator::loadNormalMap(  const std::string & normal_map  )
{
    m_renderer->loadTexture( normal_map );
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

void arkMediator::setBRDF(int brdf_choice )
{
    m_renderer->setBRDF( brdf_choice );
}

void arkMediator::setNormalMode( bool normal_mode )
{
    m_renderer->setNormalMode( normal_mode );
}

void arkMediator::reloadShader()
{
    m_renderer->reloadShader();
}

void arkMediator::setFilteringMode(bool enabled)
{
    m_renderer->setFilteringMode(enabled);
}

void arkMediator::setDiffuseEnabled(bool enabled)
{
    m_renderer->setDiffuseEnabled(enabled);
}

void arkMediator::setSpecularDirectEnabled(bool enabled)
{
    m_renderer->setSpecularDirectEnabled(enabled);
}

void arkMediator::setSpecularEnvEnabled(bool enabled)
{
    m_renderer->setSpecularEnvEnabled(enabled);
}

void arkMediator::setDiffuseDirectEnabled(bool enabled)
{
    m_renderer->setDiffuseDirectEnabled(enabled);
}

void arkMediator::setDiffuseEnvEnabled(bool enabled)
{
    m_renderer->setDiffuseEnvEnabled(enabled);
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
