#include <array>

//#include <ngl/NGLInit.h>

#include "../../include/TestConfig.hpp"

void TestConfig::setUp()
{
  std::cerr<<"Creating OpenGL Context\n";

  m_init = glfwInit();

  // minimum required OpenGL is 4.1 as mac should support it (mavericks and above)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(1024, 720, "", nullptr, nullptr);

  if (!m_window)
  {
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(m_window);
//  ngl::NGLInit::initialize();
}

void TestConfig::verifySetUp() const
{
  /**
   * TODO: should investigate
   * In mac, invoking Catch2 macros at setup stage to verify the setup,
   * results in the following error, even if it's after context creation:
   *
   * libc++abi.dylib: terminating with uncaught exception of type std::logic_error:
   * /Users/hirad/.local/vcpkg/installed/x64-osx/share/OpenImageIO/../../include/catch2/catch.hpp:13052:
   * Internal Catch2 error: No result capture instance
   */
  UNSCOPED_INFO("Failed to create init GLFW need an X server\n");
  CHECK(m_init == GLFW_TRUE);
  CHECK(m_window != nullptr);
}

void TestConfig::tearDown()
{
  std::cerr<<"Teardown removing files\n";
  std::cerr<<"remove OpenGL Context\n";

  glfwTerminate();
}
