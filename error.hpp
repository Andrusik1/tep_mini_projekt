#pragma once

#include <cstdint>
#include <string>
#include <variant>

struct UnknownError {
  enum class Type : std::uint8_t { UNKNOWN };

  Type type = Type::UNKNOWN;
  std::string message;

  UnknownError() = default;

  explicit UnknownError(std::string msg) : message(std::move(msg)) {}
};

struct FileError {
  enum class Type : std::uint8_t { NOT_FOUND, INVALID_TYPE, ACCESS_DENIED, EMPTY };

  Type type;
  std::string path;

  FileError(Type t, std::string p) : type(t), path(std::move(p)) {}
};

struct ParseError {
  enum class Type : std::uint8_t { INVALID_DATA };

  Type type;
  std::string context;

  ParseError(Type t, std::string ctx) : type(t), context(std::move(ctx)) {}
};

using ErrorData = std::variant<UnknownError, FileError, ParseError>;

class Error {
 public:
  Error();

  // WARN: Co się tutaj odpierdala
  template <typename U>
  Error(U&& error_data)
    requires(!std::is_same_v<std::decay_t<U>, Error>)
      : data_(std::forward<U>(error_data)) {}

  const ErrorData& GetData() const { return data_; }

 private:
  ErrorData data_;
};
