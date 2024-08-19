#include "create_post.hpp"

#include <fmt/format.h>
#include <cstddef>
#include <userver/clients/http/client.hpp>
#include <userver/clients/http/config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/components/component.hpp>

#include <userver/engine/task/task_processor_fwd.hpp>
#include <userver/utils/async.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>

#include "userver/storages/secdist/exceptions.hpp"

#include <userver/server/http/http_request.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace posts_uservice {

  bool isSymbol(char c) {
    return c != ' ' && c != '\n';
  }

  std::string FormatText(const std::string& input) {
    std::string result;
    
    for(const auto& c : input) {
      if(isSymbol(c)) {
        result.push_back(c);
      }

      if(c == '\n' && result.back() == ' ') {
        result.back() = c;
        continue;
      }

      if(!result.empty() && isSymbol(result.back())) {
        result.push_back(c);
      }
    }
    
    if (!isSymbol(result.back())) {
      result.pop_back();
    }

    return result;
}

namespace {

class CreatePost final : public userver::server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-create-post";

  CreatePost(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& context) : HttpHandlerBase(config,context),
            pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()),
            http_client_{
              context.FindComponent<userver::components::HttpClient>().GetHttpClient()} {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest &request,
      userver::server::request::RequestContext &) const override {

        const auto& userId = request.GetHeader("System-Design-User-Id ");

        const auto& description = FormatText(request.GetArg("text"));
        const auto& media = request.GetArg("media");

        // here u need create field in json with error 
        if(description.length() > 280) {
          auto& response = request.GetHttpResponse();
          response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
          return {};
        }

        //if media isn't image u need send bad request

        //userver::server::http::HttpRequest S3Request();
        // const auto& url = "http://storage.yandexcloud.net/y-media/<ключ_объекта>?<query-параметры>";
        // const auto& data = "data";

        // auto external_response =  http_client_.CreateRequest().put(url, data);

        return "I created post";
  }
private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
  userver::clients::http::Client& http_client_;
};

} // namespace

void AppendCreatePost(userver::components::ComponentList &component_list) {
  component_list.Append<CreatePost>();
}

} // namespace service_template
