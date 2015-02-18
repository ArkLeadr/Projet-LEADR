//
//  arkMediator.h
//  arkGUI
//
//  Created by anthonycouret on 17/02/2015.
//  Copyright (c) 2015 ac. All rights reserved.
//

#ifndef __arkGUI__arkMediator__
#define __arkGUI__arkMediator__

#include <memory>

#include "arkQTGUI/arkAbstractMediator.h"
#include "Graphics/renderer.h"

class arkMediator;
typedef std::shared_ptr<arkMediator> arkMediatorShPtr;
typedef std::weak_ptr<arkMediator> arkMediatorWkPtr;

class arkMediator : public arkAbstractMediator
{
    public :
    
    static arkMediatorShPtr create( Renderer * renderer );
    ~arkMediator();
    
    virtual void setTesselationFactor( float tesselation_factor );
    virtual void setDispFactor( float disp_factor );
    virtual void setNbSamples( int nb_samples );
    
    virtual void loadModel( const std::string & model_path );
    virtual void loadTexture( const std::string & tex_path );
    virtual void loadEnvTexture( const std::string & env_path );
    virtual void loadIrradianceMap( const std::string & irr_path );
    
    virtual void setStandardRendering();
    virtual void setWireframeRendering();
    virtual void setDepthRendering();
    
    virtual void setLEADRMode( bool leadr_mode );
    virtual void setNormalMode( bool normal_mode );
    
    virtual void initializeGL();
    virtual void resizeGL( int width, int height );
    virtual void paintGL();
    
    virtual void rotateCamera( int mouse_x_rel, int mouse_y_rel );
    virtual void translateCamera( int mouse_x_rel, int mouse_y_rel, int mouse_z_rel );
    virtual void onKeyPress( int qt_key );

    private :
    
    arkMediator( Renderer * renderer );
    arkMediatorWkPtr m_weak_ptr;
    
    Renderer * m_renderer;
    
    float m_tesselation_factor;
    float m_disp_factor;
    float m_nb_samples;
    
    
};

#endif /* defined(__arkGUI__arkMediator__) */
