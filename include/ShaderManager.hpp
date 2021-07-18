#pragma once

#include <memory>
#include <unordered_map>
#include <QOpenGLShaderProgram>

/// \file ShaderManager.hpp
/// \brief Simple Shader Manager class
/// \author Teemu Lindborg
/// \version 1.0
/// \date 22/01/17 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 25/04/16

namespace sdfRay4d
{
  class ShaderManager
  {
  public:
    ///
    /// \brief instance Returns an instance of the shader manager and initialises it shader man if it hasn't been initialised before
    /// \return an instance of the shader manager
    ///
    static std::shared_ptr<ShaderManager> instance()
    {
      if(m_instance == nullptr)
        m_instance.reset(new ShaderManager());

      return m_instance;
    }

    ~ShaderManager();
    ///
    /// \brief createShader Creates a shader from a vertex and a fragment shader with the given name
    /// \param _name Name of the shader
    /// \param _vs Name of the vertex shader file inside ./shaders
    /// \param _fs Name of the fragment shader file inside ./shaders
    ///
    void createShader(const std::string &_name, const QString &_vs, const QString &_fs);

    ///
    /// \brief updateShader Replaces the fragment shader in use with the new shader source
    /// \param _shaderCode Shader code of the new fragment shader
    ///
    void updateShader(const char *_shaderCode);

    ///
    /// \brief useShader Sets the active shader to a given one
    /// \param _name Name of the shader to be used
    ///
		void useShader(const std::string &_name);
		QOpenGLShaderProgram* getProgram() const { return m_program; }
  private:
    ///
    /// \brief ShaderManager Ctor hidden as we only want a single instance of this class to exist
    ///
		ShaderManager() : m_program(nullptr), m_fragShader(nullptr) {}
    ///
    /// \brief ShaderManager Copy ctor deleted to avoid problems
    /// \param _rhs
    ///
    ShaderManager(const ShaderManager &_rhs) = delete;
    ///
    /// \brief operator = Assignment operator deleted to avoid problems
    /// \param _rhs
    /// \return
    ///
		ShaderManager& operator= (const ShaderManager &_rhs) = delete;

    ///
    /// \brief m_instance Static pointer to the instance of the shader manager
    ///
    static std::shared_ptr<ShaderManager> m_instance;

    ///
    /// \brief m_shaders Map of the shader programs
    ///
    std::unordered_map<std::string, QOpenGLShaderProgram*> m_shaders;
    ///
    /// \brief m_program Current active shader
    ///
    QOpenGLShaderProgram *m_program;

    ///
    /// \brief m_fragShader Current fragment shader in use
    ///
    QOpenGLShader *m_fragShader;
  };
}
