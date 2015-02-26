#include "shader.h"

#include "../helpers.h"

bool Shader::addVertexShader(std::string filename) {
    m_currentShaderFilenames[VERTEX_SHADER].reset(new std::string(filename));
    return addShader(filename, GL_VERTEX_SHADER);
}

bool Shader::addFragmentShader(std::string filename) {
    m_currentShaderFilenames[FRAGMENT_SHADER].reset(new std::string(filename));
    return addShader(filename, GL_FRAGMENT_SHADER);
}

bool Shader::addTessControlShader(std::string filename) {
    m_currentShaderFilenames[TESS_CONTROL_SHADER].reset(new std::string(filename));
    return addShader(filename, GL_TESS_CONTROL_SHADER);
}

bool Shader::addTessEvaluationShader(std::string filename) {
    m_currentShaderFilenames[TESS_EVAL_SHADER].reset(new std::string(filename));
    return addShader(filename, GL_TESS_EVALUATION_SHADER);
}

bool Shader::addGeometryShader(std::string filename) {
    m_currentShaderFilenames[GEOMETRY_SHADER].reset(new std::string(filename));
    return addShader(filename, GL_GEOMETRY_SHADER);
}

bool Shader::addShader(std::string filename, GLenum type) {
    GLint length;
    GLchar* source = readFile(filename, &length);

    if (!source) {
        std::cerr << "Error reading source from " << filename << '\n';
        return false;
    }

    GLint shader_compiled;

    GLuint shaderId = GL(glCreateShader(type));

    GL(glShaderSource(shaderId, 1, (const char**)&source, &length));

    free(source);

    GL(glCompileShader(shaderId));

    GL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shader_compiled));

    GLsizei log_length = 0;
    GLchar message[4096];
    GL(glGetShaderInfoLog(shaderId, 4096, &log_length, message));

    if (shader_compiled != GL_TRUE)
    {
        std::cerr << "Error compiling shader " << filename << ": " << message << '\n';

        return false;
    }
    else if(strlen(message)) {
        std::cerr << "Althoug compiled with success, shader " << filename << " has following compile output: " << message << '\n';
    }


    GL(glAttachShader(m_program, shaderId));

    return true;
}

bool Shader::link() {
    GL(glLinkProgram(m_program));

    GLint program_linked;
    GL(glGetProgramiv(m_program, GL_LINK_STATUS, &program_linked));
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        GL(glGetProgramInfoLog(m_program, 1024, &log_length, message));

        std::cerr << "Error linking program: " << message;

        return false;
    }

    return true;
}

void Shader::use() {
    GL(glUseProgram(m_program));
}

void Shader::sendTransformations(const mat4& projection, const mat4& view, const mat4& model) {
    mat4 MVP = projection * view * model;

    GL(glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, MVP.data()));

    GL(glUniformMatrix4fv(glGetUniformLocation(m_program, "world"), 1, GL_FALSE, model.data()));

    GL(glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, projection.data()));
    GL(glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, view.data()));
    //    GL(glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, model.data()));
}

void Shader::sendMaterial(const Material &mat)
{
    GL(glUniform3fv(glGetUniformLocation(m_program, "ka"), 1, mat.m_ambientReflectance.data()));
    GL(glUniform3fv(glGetUniformLocation(m_program, "kd"), 1, mat.m_diffuseReflectance.data()));
    GL(glUniform3fv(glGetUniformLocation(m_program, "ks"), 1, mat.m_specularReflectance.data()));

    GL(glUniform1f(glGetUniformLocation(m_program, "shininess"), mat.m_specularExponent));

    mat.m_diffuseTexture.bindToTarget(GL_TEXTURE0);
    mat.m_normalTexture.bindToTarget(GL_TEXTURE1);

}
