#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

class Property {
  public:
    std::string name;
    std::string value;
    std::string type;

    void print() {
      std::string indent = get_indentation(2);
      println(indent + "{");
      println(indent + "  name: " + name);
      if (is_whitespace(value) == false) {
        println(indent + "  value: " + value);
      }
      if (is_whitespace(type) == false) {
        println(indent + "  type: " + type);
      }
      println(indent + "}");
    }

    static Peek<Property> parse(std::vector<Token> collection, size_t index, Command command) {
      if (collection[index].kind != Kind::PROPERTY) {
        throw collection[index].get_error("Expecting Property");
      }

      auto next = peek<Token>(
        collection,
        index,
        [&command](Token &token) {
          if (command == Command::SELECT) {
            if (token.is_given_marker(Marker::RIGHT_CURLY_BRACE)) {
              return true;
            }

            return token.is_given_kind(Kind::PROPERTY, Kind::LITERAL);
          }

          return token.is_given_kind(Kind::TYPE, Kind::LITERAL); 
        },
        [](Token &token) {
          return token.get_error("Expecting Type or Literal"); 
        }
      );

      Peek<Property> peek;
      peek.node.name = collection[index].value;
      peek.index = next.index;

      if (next.node.is_given_kind(Kind::MARKER, Kind::PROPERTY)) {
        return peek;
      }

      if (next.node.kind == Kind::TYPE) {
        peek.node.type = next.node.value;
        return peek;
      }

      // LITERAL 
      peek.node.value = next.node.value;
      peek.node.type = get_infered_type(next.node.name);
      return peek;
    }
};

struct Statement {
  Command command;
  std::string entity;
  std::vector<Property> properties;

  void print() {
    std::string indent = get_indentation(1);
    println(get_command_name(command) + " " + entity + " {");
    println(indent + "properties: [");
    for (Property prop : properties) prop.print();
    println(indent + "]");
    println("}");
  }
};

class Parser {
  static Peek<Statement> handle_command(std::vector<Token> collection, size_t index) {
    Token command = collection[index];

    if (command.kind != Kind::COMMAND) {
      throw command.get_error("Expecting Command");
    }
    
    auto entity_peek = peek<Token>(
      collection,
      index,
      [](Token &token) { 
        return token.kind == Kind::ENTITY; 
      },
      [](Token &token) {
        return token.get_error("Expecting Entity"); 
      }
    );

    auto brace_peek = peek<Token>(
      collection,
      entity_peek.index,
      [](Token &token) { 
        return token.is_given_marker(Marker::LEFT_CURLY_BRACE); 
      },
      [](Token &token) {
        return token.get_error("Expecting Left Curly Brace"); 
      }
    );

    Peek<Statement> peek;
    peek.node.command = get_command(command.value);
    peek.node.entity = entity_peek.node.value;
    peek.index = brace_peek.index;

    for (size_t i = brace_peek.index + 1; i < collection.size(); i++) {
      Token token = collection[i];

      if (token.is_given_marker(Marker::RIGHT_CURLY_BRACE)) {
        peek.index = i;
        return peek;
      }

      Peek<Property> property_peek = Property::parse(collection, i, peek.node.command);
      peek.node.properties.push_back(property_peek.node);
      i = property_peek.index;
    }

    throw command.get_error("Unterminated Statement");
  }

  public:
    static std::vector<Statement> parse(std::vector<Token> collection) {
      std::vector<Statement> program;

      for (size_t i = 0; i < collection.size(); i++) {
        Token token = collection[i];

        switch (token.kind) {
          case Kind::COMMAND: {
            Peek<Statement> peek = handle_command(collection, i);
            program.push_back(peek.node);
            i = peek.index;
          } break;
          default:
            break;
        }
      }

      return program;
    }
};

#endif