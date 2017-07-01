#include "test/integration/integration.h"
#include "test/integration/utility.h"

namespace Envoy {
class HttpFilterSampleIntegrationTest : public BaseIntegrationTest,
                             public testing::TestWithParam<Network::Address::IpVersion> {
public:
  HttpFilterSampleIntegrationTest() : BaseIntegrationTest(GetParam()) {}
  /**
   * Initializer for an individual integration test.
   */
  void SetUp() override {
    createTestServer("http-filter-example/envoy.conf", {"http_filter_sample"});
  }

  /**
   * Destructor for an individual integration test.
   */
  void TearDown() override {
    test_server_.reset();
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, HttpFilterSampleIntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(HttpFilterSampleIntegrationTest, Test1) {
  // test something
}
} // Envoy
