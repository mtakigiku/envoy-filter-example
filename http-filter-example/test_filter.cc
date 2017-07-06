//
// Created by mtakigiku on 6/21/17.
//
#include <string>

#include "common/common/base64.h"
#include "common/common/logger.h"
#include "common/http/headers.h"
#include "common/http/utility.h"
#include "envoy/server/instance.h"
#include "envoy/ssl/connection.h"
#include "server/config/network/http_connection_manager.h"

#include "envoy/server/instance.h"
//#include "envoy/server/filter_config.h"
#include "server/config/network/http_connection_manager.h"


namespace Envoy{
namespace Http{
namespace TestFilter{

class Config {
 public:
  Config(const Json::Object&){

  }
};
typedef std::shared_ptr<Config> ConfigPtr;

class Instance : public Http::StreamDecoderFilter {
 public:
  Instance(ConfigPtr) {
  }
  ~Instance(){}

  // Http::StreamFilterBase
  void onDestroy() override {}

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(HeaderMap& , bool ) override{
//    auto cb = [](const HeaderEntry& header, void* ) -> void {
//        printf("\t\t%s: %s\n", header.key().c_str(), header.value().c_str());
//    };
//    printf("\tThis_is_decodeHeaders\n");
//    headers.iterate(cb, nullptr);

    // remove some headers
//    printf("\tRemove some headers\n");
//    headers.addStatic(LowerCaseString("warning"), "test-filter");
//    headers.remove(LowerCaseString("user-agent"));
//    headers.iterate(cb, nullptr);

    return FilterHeadersStatus::Continue;
  }

  FilterDataStatus decodeData(Buffer::Instance& , bool ) override{
    return FilterDataStatus::Continue;
  }
  FilterTrailersStatus decodeTrailers(HeaderMap& ) override {
    return FilterTrailersStatus::Continue;
  }
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) override {
    decoder_callbacks_ = &callbacks;
  }

 private:
  StreamDecoderFilterCallbacks* decoder_callbacks_;
  ConfigPtr config_;
};

} // TestFilter
} // Http

namespace Server{
namespace Configuration {

class TestFilterConfig : public HttpFilterConfigFactory {
 public:
  HttpFilterFactoryCb tryCreateFilterFactory(
        HttpFilterType type,
        const std::string& name,
        const Json::Object& config,
        const std::string& ,
        Server::Instance& ) override{
    // Envoy::Server::Configuration::FactoryContext& context) override{

    if (type != HttpFilterType::Decoder || name != "test")
      return nullptr;


    Http::TestFilter::ConfigPtr test_config(
        new Http::TestFilter::Config(config));

    return [test_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
        callbacks.addStreamDecoderFilter(
            Http::StreamDecoderFilterSharedPtr{new Http::TestFilter::Instance(test_config)});
    };

//    return
//        [test_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
//          std::shared_ptr<Http::TestFilter::Instance> instance =
//              std::make_shared<Http::TestFilter::Instance>(test_config);
//          callbacks.addStreamDecoderFilter(
//              Http::StreamDecoderFilterSharedPtr(instance)
//          );
//        };

  }
//  std::string name() override{return "test";}
};
//
//class NamedTestFilterConfig : public NamedHttpFilterConfigFactory {
// public:
//  HttpFilterFactoryCb createFilterFactory(
//      HttpFilterType type,
//      const Json::Object& config,
//      const std::string& ,
//      Server::Instance& ) override{
//    // Envoy::Server::Configuration::FactoryContext& context) override{
//
//    if (type != HttpFilterType::Decoder)
//      return nullptr;
//
//    Http::TestFilter::ConfigPtr test_config(
//        new Http::TestFilter::Config(config));
//
//    return [test_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
//        callbacks.addStreamDecoderFilter(
//            Http::StreamDecoderFilterSharedPtr{new Http::TestFilter::Instance(test_config)});
//    };
//  }
//  std::string name() override{return "test";}
//};

static RegisterHttpFilterConfigFactory<TestFilterConfig> register_;

} // Configuration
} // Server
} // Envoy