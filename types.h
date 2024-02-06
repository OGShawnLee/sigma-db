#ifndef TYPES_H
#define TYPES_H

#include <algorithm>
#include <vector>
#include <map>

enum class Command {
  DEFINE,
  INSERT,
  SELECT
};

std::map<std::string, Command> COMMAND_KEY = {
  {"define", Command::DEFINE},
  {"insert", Command::INSERT},
  {"select", Command::SELECT},
};

std::map<Command, std::string> COMMAND_NAME = {
  {Command::DEFINE, "define"},
  {Command::INSERT, "insert"},
  {Command::SELECT, "select"},
};

bool is_command(std::string command) {
  return COMMAND_KEY.count(command) > 0;
}

Command get_command(std::string buffer) {
  if (is_command(buffer)) {
    return COMMAND_KEY.at(buffer);
  }

  throw std::runtime_error("Invalid Command: " + buffer);
}

std::string get_command_name(Command command) {
  return COMMAND_NAME.at(command);
}

bool is_entity(std::string buffer) {
  return isupper(buffer[0]);
}

enum class Kind {
  COMMAND,
  ENTITY,
  LITERAL,
  MARKER,
  PROPERTY,
  TYPE,
};

std::map<Kind, std::string> KIND_NAME = {
  {Kind::COMMAND, "Command"},
  {Kind::ENTITY, "Entity"},
  {Kind::LITERAL, "Literal"},
  {Kind::MARKER, "Marker"},
  {Kind::PROPERTY, "Property"},
  {Kind::TYPE, "Type"},
};

std::string get_kind_name(Kind kind) {
  return KIND_NAME.at(kind);
}

enum class Literal {
  BOOL,
  INT,
  STRING,
};

std::map<std::string, Literal> LITERAL_KEY = {
  {"Boolean Literal", Literal::BOOL},
  {"Integer Literal", Literal::INT},
  {"String Literal", Literal::STRING},
};

std::map<Literal, std::string> LITERAL_NAME = {
  {Literal::BOOL, "Boolean Literal"},
  {Literal::INT, "Integer Literal"},
  {Literal::STRING, "String Literal"},
};

Literal get_literal(std::string name) {
  if (LITERAL_KEY.count(name) > 0) {
    return LITERAL_KEY.at(name);
  }

  throw std::runtime_error("Invalid Literal: " + name);
}

std::string get_literal_name(Literal literal) {
  return LITERAL_NAME.at(literal);
}

bool is_bool_literal(std::string buffer) {
  return buffer == "true" || buffer == "false";
}

bool is_int_literal(std::string buffer) {
  return std::all_of(buffer.begin(), buffer.end(), ::isdigit);
}

enum Marker {
  LEFT_CURLY_BRACE,
  RIGHT_CURLY_BRACE,
  DOUBLE_QUOTE,
};

std::map<char, Marker> MARKER_KEY = {
  {'{', Marker::LEFT_CURLY_BRACE},
  {'}', Marker::RIGHT_CURLY_BRACE},
  {'"', Marker::DOUBLE_QUOTE},
};

std::map<Marker, std::string> MARKER_NAME = {
  {Marker::LEFT_CURLY_BRACE, "Left Curly Brace"},
  {Marker::RIGHT_CURLY_BRACE, "Right Curly Brace"},
  {Marker::DOUBLE_QUOTE, "Double Quote"},
};

bool is_marker(char character) {
  return MARKER_KEY.count(character) > 0;
}

Marker get_marker(char character) {
  if (is_marker(character)) {
    return MARKER_KEY.at(character);
  }

  throw std::runtime_error("Invalid Marker: " + character);
}

std::string get_marker_name(Marker marker) {
  return MARKER_NAME.at(marker);
}

enum class Type {
  BOOL,
  INT,
  STRING,
};

std::map<std::string, Type> TYPE_KEY = {
  {"bool", Type::BOOL},
  {"int", Type::INT},
  {"str", Type::STRING},
};

std::map<Type, std::string> TYPE_NAME = {
  {Type::BOOL, "Boolean"},
  {Type::INT, "Integer"},
  {Type::STRING, "String"},
};

std::map<Literal, std::string> LITERAL_TO_TYPE = {
  {Literal::BOOL, "bool"},
  {Literal::INT, "int"},
  {Literal::STRING, "str"},
};

bool is_type(std::string type) {
  return TYPE_KEY.count(type) > 0;
}

Type get_type(std::string buffer) {
  if (is_type(buffer)) {
    return TYPE_KEY.at(buffer);
  }

  throw std::runtime_error("Invalid Type: " + buffer);
}

std::string get_type_name(std::string buffer) {
  return TYPE_NAME.at(get_type(buffer));
}

std::string get_infered_type(std::string name) {
  Literal literal = get_literal(name);
  return LITERAL_TO_TYPE.at(literal);
}

#endif 