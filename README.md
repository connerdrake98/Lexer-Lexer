This is a lexer lexer for languages with formal grammar.

It takes in the production rules for a language (formal-grammar languages only) and lexes them (i.e. lexing a lexer.) For now, it only returns errors based on issues with the production rules, and doesn't return a list of all tokens (that functionality is in the works).

Input your production rules in the Language Specifications.txt file and compile main.cpp and LexerLexer.cpp. Run the executable file and the program will analyze your production rules. Syntax for your production rules input must be as follows:

General structure:
[1 - Production rules - non-terminal symbols]
[2 - Production rules - terminal symbols]

Declaring production rules for non-terminal symbols:
[non-terminal symbol] -> [list of non-terminal and terminal symbols] || [option two: list of non-terminal and terminal symbols] || ...

Declaring production rules for terminal symbols:
[Terminal Symbol] = "[Terminal, i.e. string identifier of token]"
