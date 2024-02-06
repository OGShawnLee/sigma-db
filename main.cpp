#include "lexer.h"
#include "parser.h"

int main() {
  std::vector<Token> tokens = Lexer::lex_file("main.pql");
  // for (Token token : tokens) token.print();
  std::vector<Statement> output = Parser::parse(tokens);
  for (Statement statement : output) statement.print();
  return 0;
}