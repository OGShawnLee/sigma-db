#ifndef LEXER_h
#define LEXER_h

#include "utils.h"
#include "types.h"

struct Token {
  Kind kind;
  std::string name;
  std::string value;
  std::string line;

  void print() const {
    println(get_kind_name(kind) + " {");
    if (is_whitespace(name) == false) {
      println("  name: " + name);
    }
    println("  value: " + value);
    println("}");
  }

  std::runtime_error get_error(std::string message) {
    println("error at: " + line);
    return std::runtime_error(message + " found: " + value);
  }

  bool is_given_marker(Marker marker) const {
    return kind == Kind::MARKER && name == get_marker_name(marker);
  }

  bool is_given_kind(Kind kind) const {
    return this->kind == kind;
  }

  bool is_given_kind(Kind kind_a, Kind kind_b) const {
    return this->kind == kind_a || this->kind == kind_b;
  }
};

class Lexer {
  static Peek<Token> get_str_literal(std::string line, size_t index) {
    std::string buffer = "";

    for (size_t i = index + 1; i < line.length(); i++) {
      char character = line[i];

      if (is_marker(character)) {
        Marker marker = get_marker(character);

        if (marker == Marker::DOUBLE_QUOTE) {
          Token token;
          token.kind = Kind::LITERAL;
          token.name = get_literal_name(Literal::STRING);
          token.value = buffer;
          return Peek<Token> { token, i };
        }
      }

      buffer += character;
    }

    throw std::runtime_error("Unterminated String Literal");
  }

  static Token handle_buffer(std::string buffer) {
    Token token;
    token.value = buffer;

    if (is_command(buffer)) {
      token.kind = Kind::COMMAND;
    } else if (is_bool_literal(buffer)) {
      token.kind = Kind::LITERAL;
      token.name = get_literal_name(Literal::BOOL);
    } else if (is_int_literal(buffer)) {
      token.kind = Kind::LITERAL;
      token.name = get_literal_name(Literal::INT);
    } else if (is_entity(buffer)) {
      token.kind = Kind::ENTITY;
    } else if (is_type(buffer)) {
      token.kind = Kind::TYPE;
      token.name = get_type_name(buffer);
    } else {
      token.kind = Kind::PROPERTY;
    }

    return token;
  }

  public:
    static std::vector<Token> lex_file(std::string file_name) {
      std::vector<Token> collection;
      each_line(file_name, [&collection](std::string line) {
        std::vector<Token> tokens = lex_line(line);
        collection.insert(collection.end(), tokens.begin(), tokens.end());
      });
      return collection;
    }
    
    static std::vector<Token> lex_line(std::string line) {
      std::vector<Token> collection;
      std::string buffer;

      line += ' ';
      
      for (size_t i = 0; i < line.length(); i++) {
        char character = line[i];
        
        if (is_whitespace(character)) {
          if (is_whitespace(buffer)) continue;

          Token token = handle_buffer(buffer);
          token.line = line;
          collection.push_back(token);
          buffer = "";

          continue;
        }

        if (is_marker(character)) {
          Marker marker = get_marker(character);

          if (is_whitespace(buffer) == false) {
            Token token = handle_buffer(buffer);
            token.line = line;
            collection.push_back(token);
            buffer = "";
          }

          switch (marker) {
            case Marker::DOUBLE_QUOTE: {
              Peek<Token> str_peek = get_str_literal(line, i);
              str_peek.node.line = line;
              collection.push_back(str_peek.node);
              i = str_peek.index;
            } break;
            default:
              Token token;
              token.kind = Kind::MARKER;
              token.name = get_marker_name(marker);
              token.value = character;
              token.line = line;
              collection.push_back(token);
              break;
          }

          continue;
        }

        buffer += character;
      }

      return collection;
    }
};

#endif