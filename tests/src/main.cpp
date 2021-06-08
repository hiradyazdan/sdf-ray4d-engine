#define CATCH_CONFIG_RUNNER

#include "../include/TestConfig.hpp"

int main(int argc, char **argv)
{
  auto &&testConfig = TestConfig();

  testConfig.setUp();

  auto &&TESTS_RUN = Catch::Session().run(argc, argv);

//  testConfig.verifySetUp();

  TestConfig::tearDown();

  return TESTS_RUN;
}
