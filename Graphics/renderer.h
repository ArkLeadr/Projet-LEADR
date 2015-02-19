#ifndef RENDERER_H
#define RENDERER_H



#include "scene.h"
#include <QKeyEvent>
#include <string>

#ifdef USE_SDL2
#include "Sdl2GUI/rendererinterface.h"

class Renderer : public RendererInterface {
#else
class Renderer /*: public RendererInterface*/ {
#endif
public:
    Renderer(int width, int height);

    /*virtual*/ void initializeGL(void);
    /*virtual*/ void resizeGL(int width, int height);
    /*virtual*/ void paintGL(void);

    //virtual void loadModel(const std::string &filename);

    virtual void onKeyPress(int qt_key);

    /*virtual */void reloadShader();
    /*virtual */void toggleWireframe();
    /*virtual */void setFinalFboTarget(int targetIndex);

    bool initGL();
    void initScene();

    void render();

    void rotateCamera(int mouse_x_rel, int mouse_y_rel);
    void translateCamera(int mouse_x_rel, int mouse_y_rel, int mouse_z_rel);

    void gammaChanged(float value);
    void keyValueChanged(float value);

    /** **/
    void setTesselationFactor( float tesselation_factor );
    void setDispFactor( float disp_factor );
    void setNbSamples( int nb_samples );
    
    void loadModel( const std::vector< std::string > & model_paths );
    void loadTexture( const std::vector< std::string > & tex_paths );
    void loadEnvTexture( const std::string & env_path );
    void loadIrradianceMap( const std::string & irr_path );
    
    void setStandardRendering();
    void setWireframeRendering();
    void setDepthRendering();
    
    void setLEADRMode( bool leadr_mode );
    void setNormalMode( bool normal_mode );
    ~Renderer(){};
    /** **/

private:
    int m_width;
    int m_height;

//CURRENT RENDERLOOP DATA: TO REFACTOR !!!!
private:
    Scene* m_scene{nullptr};
};

#endif // RENDERER_H
