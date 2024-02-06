#ifndef LEXER_h
#define LEXER_h

#include "utils.h"
#include "types.h"

struct Token {
  Kind kind;
  std::string name;
  std::string value;

  void print() const {
    println(get_kind_name(kind) + " {");
    if (is_whitespace(name) == false) {
      println("  name: " + name);
    }
    println("  value: " + value);
    println("}");
  }
};

class Lexer {
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
    static std::vector<Token> lex_line(std::string line) {
      std::vector<Token> collection;
      std::string buffer;

      line += ' ';
      
      for (size_t i = 0; i < line.length(); i++) {
        char character = line[i];
        
        if (is_whitespace(character)) {
          if (is_whitespace(buffer)) continue;

          Token token = handle_buffer(buffer);
          collection.push_back(token);
          buffer = "";

          continue;
        }

        if (is_marker(character)) {
          Marker marker = get_marker(character);

          if (is_whitespace(buffer) == false) {
            Token token = handle_buffer(buffer);
            collection.push_back(token);
            buffer = "";
          }

          switch (marker) {
            default:
              Token token;
              token.kind = Kind::MARKER;
              token.name = get_marker_name(marker);
              token.value = character;
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