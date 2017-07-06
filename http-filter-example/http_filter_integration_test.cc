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
    fake_upstreams_.emplace_back(new FakeUpstream(0, FakeHttpConnection::Type::HTTP1, version_));
    registerPort("upstream_0", fake_upstreams_.back()->localAddress()->ip()->port());
    createTestServer("http-filter-example/envoy.conf", {"http"});
  }

  /**
   * Destructor for an individual integration test.
   */
  void TearDown() override {
    test_server_.reset();
    fake_upstreams_.clear();

  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, HttpFilterSampleIntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(HttpFilterSampleIntegrationTest, Test1) {
  // test something

//  Buffer::OwnedImpl buffer("hello");
//  std::string response;
//  RawConnectionDriver connection(lookupPort("http_filter_sample"), buffer,
//                                 [&](Network::ClientConnection&, const Buffer::Instance& data)
//                                     -> void {
//                                     response.append(TestUtility::bufferToString(data));
//                                     connection.close();
//                                 }, GetParam());
//
//  connection.run();
//  EXPECT_EQ("hello", response);



  Http::TestHeaderMapImpl headers{
      {":method", "GET"}, {":path", "/"}, {":authority", "host"} };

  IntegrationCodecClientPtr codec_client;
  FakeHttpConnectionPtr fake_upstream_connection;
  IntegrationStreamDecoderPtr response(new IntegrationStreamDecoder(*dispatcher_));
  FakeStreamPtr request_stream;

  codec_client = makeHttpConnection(lookupPort("http"), Http::CodecClient::Type::HTTP1);
  codec_client->makeHeaderOnlyRequest(headers, *response);
  fake_upstream_connection = fake_upstreams_[0]->waitForHttpConnection(*dispatcher_);
  request_stream = fake_upstream_connection->waitForNewStream();
  request_stream->waitForEndStream(*dispatcher_);
////
  response->waitForEndStream();

//  request_stream.headers()

  codec_client->close();

}
} // Envoy
