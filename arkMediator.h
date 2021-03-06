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
#include <vector>

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
    virtual void setRoughnessOffset(double roughnessOffset);
    virtual void setFresnel0( double fresnel0 );
    
    virtual void loadModel( const std::string& model_path );

    virtual void loadLEADRTexture(const std::string & leadr);
    virtual void loadTexture( const std::string & texture);
    virtual void loadDispMap( const std::string & disp_map );
    virtual void loadNormalMap( const std::string & normal_map );
    
    virtual void loadEnvTexture( const std::string & env_path );
    virtual void loadIrradianceMap( const std::string & irr_path );
    
    virtual void setStandardRendering();
    virtual void setWireframeRendering();
    virtual void setDepthRendering();
    virtual void setNormalRendering();
    virtual void setTexcoordRendering();

    virtual void setBRDF( int brdf_choice );
    virtual void setNormalMode( bool normal_mode );
    virtual void reloadShader();

    virtual void setFilteringMode(bool enabled);
    virtual void setDiffuseEnabled(bool enabled);
    virtual void setSpecularDirectEnabled(bool enabled);
    virtual void setSpecularEnvEnabled(bool enabled);
    virtual void setDiffuseDirectEnabled(bool enabled);
    virtual void setDiffuseEnvEnabled(bool enabled);

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
