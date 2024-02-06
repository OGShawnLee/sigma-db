#include "lexer.h"

int main() {
  each_line("main.pql", [](std::string line) {
    std::vector<Token> tokens = Lexer::lex_line(line);
    for (Token token : tokens) token.print();
  });
  return 0;
}