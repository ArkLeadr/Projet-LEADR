#include "scene.h"

#include "Utils/timer.h"

Scene::Scene(int width, int height) :
    m_width(width),
    m_height(height),
    reducePass(m_width, m_height, "reduce.frag"),
    finalScreenPass(m_width, m_height, "quadFbo.frag"),
    toLuminancePass(m_width, m_height, "toLuminance.frag"),
    tonemapPass(m_width, m_height, "tonemap.frag"),
    leadrScreenpass(m_width, m_height),
	tessFactor(1),
    nbSample(1),
    userDisplacementFactor(0)
{
}

void Scene::initScene() {
    s.addVertexShader("simple.vert");
    s.addFragmentShader("simple.frag");
    s.addTessControlShader("simple_tesc.glsl");
    s.addTessEvaluationShader("simple_tese.glsl");
    s.link();


    renewableShadersList.push_back(&(s));
    renewableShadersList.push_back(&(reducePass.getShader()));
    renewableShadersList.push_back(&(finalScreenPass.getShader()));
    renewableShadersList.push_back(&(toLuminancePass.getShader()));
    renewableShadersList.push_back(&(tonemapPass.getShader()));
    renewableShadersList.push_back(&(skybox.getShader()));

    renewableShadersList.push_back(&(leadrScreenpass.getShader()));


    camera.setProperties(vec3{0.f, 0.f, -1.f}, vec3{0.f, 0.f, 1.f}, vec3{0.f, 1.f, 0.f});

//    mainModel.loadFromFile("plan.obj");
    mainModel.loadFromFile("hi_sphere.obj");
//    mainModel.loadFromFile("SimpleModel/demo.dae");
//    mainModel.loadFromFile("cubenorm.obj");

//    mainModel.loadFromFile("pillow/Pillow_Obj.obj");

    basicLamp.loadFromFile("hi_sphere.obj");

    basicLampShader.addVertexShader("line.vert");
    basicLampShader.addFragmentShader("line.frag");
    basicLampShader.link();


//    texture.loadFromFile("roundstones.jpg"); // EXAMPLE
    texture.loadFromFile("disp_data/wall002_512x512.jpg");

//    normalMap.loadFromFile("roundstones_norm.jpg");// EXAMPLE
    normalMap.loadFromFile("disp_data/wall002_nmap2_512x512.jpg");

    mainModel.overrideNormalTexture(normalMap);

    dogeMap.loadFromFile("disp_data/wall002_hmap2_512x512.jpg");
//    dogeMap.loadFromMaterialColor(Color3f{0, 0, 0});


    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_X, "Ryfjallet_512/posx.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_X, "Ryfjallet_512/negx.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Y, "Ryfjallet_512/posy.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Y, "Ryfjallet_512/negy.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::POSITIVE_Z, "Ryfjallet_512/posz.jpg");
    cubemap.loadFaceFromFile(Cubemap::Face::NEGATIVE_Z, "Ryfjallet_512/negz.jpg");

    skybox.feedCubemap(cubemap);

    envmap.loadFromFile("grace_probe.float");

    fbo.reset(new FBO(m_width, m_height, 3));

    importCoeffs("grace_probe.leadrshc", shc);

    computeMatrixRepresentation(shc);

//    printCoeffs(shc.coeffs);

//    printMatricesToGlslDeclaration(shc);

//    importLeadrTextures("tex1", "tex2", leadr1, leadr2);
//    importLeadrTextures("disp_data/wall002_hmap2_512x512.leadr1", "disp_data/wall002_hmap2_512x512.leadr2", leadr1, leadr2);

//    importLeadrTextures("wgnoise.leadr1", "wgnoise.leadr2", leadr1, leadr2);
//    importLeadrTextures("pillow/silk_bump.leadr1", "pillow/silk_bump.leadr2", leadr1, leadr2);
//    importLeadrTextures("moon_sea.leadr1", "moon_sea.leadr2", leadr1, leadr2);

    importLeadrTexture("pillow/silk_bump.leadr", leadr1, leadr2);
}

void Scene::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    projection = mat4::Projection(70, (float) m_width / (float) m_height, 0.1f, 1000.f);

    fbo.reset(new FBO(width, height, 3));

    finalScreenPass.resize(width, height);

    toLuminancePass.resize(width, height);

    tonemapPass.resize(width, height);

    leadrScreenpass.resize(width, height);

    std::cerr << "FBO shall be: " << width << " " << height << '\n';
}

void Scene::reloadShaders()
{
    for (Shader* pShader : renewableShadersList) {
        pShader->reload();
    }
}

static bool isPowerOfTwo(int val) {
    return (val > 0) && !(val & (val - 1));
}

#define ENABLE_HDR 1

void Scene::render()
{
    //Bed

    theta += 0.005f;
    //        A += 0.03;

    float x = cosf(theta);
    float z = -sinf(theta);

    //position = {A*x, 5.f*sin(theta*3.f), A*z};


    forward.normalize();

    camera.lookAt(position, position + forward, up);

    /* AT LAST: DA RENDERING */


    /* Do your model transformations */
    mat4 cubeTransformation = mat4::Identity();
//        cubeTransformation.translate(5,0,0);
//        cubeTransformation.rotate(normalize({0.f, 0.f, 1.f}), -90.f);

//    cubeTransformation.scale(0.01, 0.01, 0.01);

    /* Get your lights ready */
    DirLight dirLight{vec3{2, 2, 2}, vec3{0, -1, -1}};
    PointLight pointLight{vec3{0, 1, 0}, vec3{x*10, 0, z*10}};


    fbo->bind();

    GLuint attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3,  attachments);


    glClearColor(0.f, 0.f, 0.f, 0); // BLACK
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//    skybox.render(projection, camera.getPureViewNoTranslation());
    envmap.render(projection, camera.getPureViewNoTranslation());
//        skybox.render(projection, camera.getView());

    FBO::unbind();


    /* Bind a shader */
    s.use();

    /* Send data to the shader */
    glUniform1i(glGetUniformLocation(s.getProgramId(), "texSampler"), 0); //Texture unit 0 is for base images.
    glUniform1i(glGetUniformLocation(s.getProgramId(), "normalMapSampler"), 1);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "dispMapSampler"), 2); //...
    glUniform1i(glGetUniformLocation(s.getProgramId(), "shadowMapSampler"), 3);
//        texture.bindToTarget(GL_TEXTURE0);
    dogeMap.bindToTarget(GL_TEXTURE2);

    glUniform1i(glGetUniformLocation(s.getProgramId(), "leadr1"), 4);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "leadr2"), 5);

    leadr1.bindToTarget(GL_TEXTURE4);
    leadr2.bindToTarget(GL_TEXTURE5);

    glUniform1i(glGetUniformLocation(s.getProgramId(), "cubeMapSampler"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.getId());


    glUniform3fv(glGetUniformLocation(s.getProgramId(), "dirLight.direction"), 1, dirLight.m_direction.data());
    glUniform3fv(glGetUniformLocation(s.getProgramId(), "dirLight.color"), 1, dirLight.m_color.data());

    glUniform3fv(glGetUniformLocation(s.getProgramId(), "pointLight.position"), 1, pointLight.m_position.data());
    glUniform3fv(glGetUniformLocation(s.getProgramId(), "pointLight.color"), 1, pointLight.m_color.data());

    glUniform3fv(glGetUniformLocation(s.getProgramId(), "eyePosition"), 1, position.data());


    glUniform1f(glGetUniformLocation(s.getProgramId(), "userDisplacementFactor"), userDisplacementFactor);
    glUniform1f(glGetUniformLocation(s.getProgramId(), "tessFactor"), tessFactor);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "nbSample"), nbSample);
    glUniform1f(glGetUniformLocation(s.getProgramId(), "roughnessOffset"), roughnessOffset);
    glUniform1f(glGetUniformLocation(s.getProgramId(), "fresnel0"), fresnel0);

    glUniform1i(glGetUniformLocation(s.getProgramId(), "filtering"), filtering);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "diffuse"), diffuse);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "specularDirect"), specularDirect);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "specularEnv"), specularEnv);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "diffuseDirect"), diffuseDirect);
    glUniform1i(glGetUniformLocation(s.getProgramId(), "diffuseEnv"), diffuseEnv);

    glUniform1i(glGetUniformLocation(s.getProgramId(), "currentBRDF"), currentBRDF);


    /* SHADOW PASS */
    glPolygonOffset(1.1f, 4.0f);
    glEnable(GL_POLYGON_OFFSET_FILL);

    mat4 lightProjection = mat4::Projection(70, 1.f, 0.1f, 1000.f);
    mat4 lightView = mat4::LookAt(vec3{15.f, 15.f, 15.f}, vec3{0, 0, 0}, vec3{0, 1, 0});

    mat4 lightMVP = lightProjection * lightView;

    s.sendTransformations(lightProjection, lightView, cubeTransformation);

    shadowmap.bind();

    mainModel.drawAsPatch(lightProjection, lightView, cubeTransformation, &s);

    shadowmap.unbind();

    glDisable(GL_POLYGON_OFFSET_FILL);

    /* FIRE! */
    s.sendTransformations(projection, camera.getView(), cubeTransformation);
    glUniformMatrix4fv(glGetUniformLocation(s.getProgramId(), "lightMVP"), 1, GL_FALSE, lightMVP.data());

    glUniform3fv(glGetUniformLocation(s.getProgramId(), "shc_env"), 9, (float*) shc.coeffs);

    glUniformMatrix4fv(glGetUniformLocation(s.getProgramId(), "hred"), 1, GL_FALSE, shc.hred.data());
    glUniformMatrix4fv(glGetUniformLocation(s.getProgramId(), "hgreen"), 1, GL_FALSE, shc.hgreen.data());
    glUniformMatrix4fv(glGetUniformLocation(s.getProgramId(), "hblue"), 1, GL_FALSE, shc.hblue.data());

    shadowmap.bindShadowMapToTarget(GL_TEXTURE3);

    fbo->bind();
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mainModel.drawAsPatch(projection, camera.getView(), cubeTransformation, &s);
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Shader::unbind();


    mat4 lampTransformation = mat4::Identity();

    lampTransformation.translate(pointLight.m_position);

    basicLampShader.use();
    basicLampShader.sendTransformations(projection, camera.getView(), lampTransformation);
    glUniform1i(glGetUniformLocation(basicLampShader.getProgramId(), "overrideColor"), true);
    glUniform3fv(glGetUniformLocation(basicLampShader.getProgramId(), "userColor"), 1, pointLight.m_color.data());

    basicLamp.drawAsTriangles();

    Shader::unbind();

    /* And so on for every other model... */
    /* Currently handmade, for developing/testing purposes */
    /* Will hopefully become moar professional in the near future */


    base.draw(projection, camera.getView());

    FBO::unbind();

    glClearColor(0.f, 0.f, 0.f, 0); // BLACK
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


#if ENABLE_HDR

    unsigned int closestWidth = m_width;
    unsigned int closestHeight = m_height;

    if (!isPowerOfTwo(closestWidth)) {
        float f = (float)(closestWidth - 1);
        closestWidth = 1U << ((*(unsigned int*)(&f) >> 23) - 126 - 1);
    }
    if (!isPowerOfTwo(closestHeight)) {
        float f = (float)(closestHeight - 1);
        closestHeight = 1U << ((*(unsigned int*)(&f) >> 23) - 126 - 1);
    }

    SingleColorFBO toLuminanceFbo(closestWidth, closestHeight);

    toLuminanceFbo.bind();

    glClear(GL_DEPTH_BUFFER_BIT);

    fbo->getTexture(0).bindToTarget(GL_TEXTURE0);
    toLuminancePass.resize(closestWidth, closestHeight);

    toLuminancePass.fire();

    toLuminanceFbo.bindToTarget(GL_TEXTURE0);

    int n = 0;

    int numLevels = 1 + floor(log2(std::max(closestWidth, closestHeight)));

    float onePixel[4] = {0};

    toLuminanceFbo.bindToTarget(GL_TEXTURE0);

    glGenerateMipmap(GL_TEXTURE_2D);

    onePixel[0] = 0;
    onePixel[1] = 0;
    onePixel[2] = 0;
    onePixel[3] = 0;

    glGetTexImage(GL_TEXTURE_2D, numLevels - 1, GL_RGBA, GL_FLOAT, onePixel);

//        std::cerr << "Mean RGB - " << onePixel[0] << ' ' << onePixel[1] << ' ' << onePixel[2] << ' ' << onePixel[3] << '\n';

    FBO::unbind();

    fbo->getTexture(fboTexId).bindToTarget(GL_TEXTURE0);

    Shader& tonemapShader = tonemapPass.getShader();
    tonemapShader.use();

    glUniform1f(glGetUniformLocation(tonemapShader.getProgramId(), "logAvLum"), onePixel[3]);
    glUniform1f(glGetUniformLocation(tonemapShader.getProgramId(), "gamma"), gamma);
    glUniform1f(glGetUniformLocation(tonemapShader.getProgramId(), "keyValue"), keyValue);
    tonemapPass.fire();

#else
    FBO::unbind();

    fbo->getTexture(fboTexId).bindToTarget(GL_TEXTURE0);
//        shadowFbo.getTexture(0).bindToTarget(GL_TEXTURE0);
    finalScreenPass.fire();
#endif


    Shader::unbind();
}

void Scene::renderLeadrQuadOnly()
{
    fbo->bind();

    GLuint attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3,  attachments);


    glClearColor(0.f, 0.f, 0.f, 0); // BLACK
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    Shader& leadr_s = leadrScreenpass.getShader();

    /* Bind a shader */
    leadr_s.use();

    glUniform1i(glGetUniformLocation(leadr_s.getProgramId(), "leadr1"), 4);
    glUniform1i(glGetUniformLocation(leadr_s.getProgramId(), "leadr2"), 5);
    leadr1.bindToTarget(GL_TEXTURE4);
    leadr2.bindToTarget(GL_TEXTURE5);


    fbo->bind();

    leadrScreenpass.fire();

//    mainModel.drawAsPatch(projection, camera.getView(), cubeTransformation, &s);

    Shader::unbind();


    FBO::unbind();

    glClearColor(0.f, 0.f, 0.f, 0); // BLACK
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    fbo->getTexture(fboTexId).bindToTarget(GL_TEXTURE0);

    finalScreenPass.fire();

    Shader::unbind();
}
