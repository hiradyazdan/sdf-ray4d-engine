#pragma once

#include <iostream>

#include <GLFW/glfw3.h> // On linux this needs to come after ngl, otherwise build fails

#include "catch.hpp" // header-only library

class TestConfig
{
  public:
    TestConfig() = default;
    ~TestConfig() = default;
    void setUp();
    void verifySetUp() const;
    static void tearDown();
  private:
    GLFWwindow* m_window;
    int m_init;
};
