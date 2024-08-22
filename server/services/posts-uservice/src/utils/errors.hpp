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

class NotFoundException : public userver::server::handlers::ResourceNotFound {
 public:
  NotFoundException(std::string_view what, std::string_view msg)
      : userver::server::handlers::ResourceNotFound(
            userver::server::handlers::ExternalBody{fmt::format("Failed to find {}:\n {}\n", what, msg)}) {}
};

}  // namespace posts_uservice::errors