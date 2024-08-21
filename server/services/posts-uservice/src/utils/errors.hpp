#pragma once

#include <stdexcept>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/server/http/http_status.hpp>

namespace posts_uservice::errors {
class ValidationException : public userver::server::handlers::ClientError {
 public:
  ValidationException(std::string_view field, std::string_view msg)
      : userver::server::handlers::ClientError(
            userver::server::handlers::ExternalBody{fmt::format("Failed to validate field {}:\n {}\n", field, msg)}) {}
};
}  // namespace posts_uservice::errors