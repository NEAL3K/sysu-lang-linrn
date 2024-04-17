#include "SYsULexer.h" // 确保这里的头文件名与您生成的词法分析器匹配
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
using namespace std;

// 映射定义，将ANTLR的tokenTypeName映射到clang的格式
std::unordered_map<std::string, std::string> tokenTypeMapping = {
  { "Int", "int" },
  { "Identifier", "identifier" },
  { "LeftParen", "l_paren" },
  { "RightParen", "r_paren" },
  { "RightBrace", "r_brace" },
  { "LeftBrace", "l_brace" },
  { "LeftBracket", "l_square" },
  { "RightBracket", "r_square" },
  { "Constant", "numeric_constant" },
  { "Return", "return" },
  { "Semi", "semi" },
  { "EOF", "eof" },
  { "Equal", "equal" },
  { "Plus", "plus" },
  { "Minus", "minus" },
  { "Comma", "comma" },
  { "Void", "void" },
  { "If", "if" },
  { "Else", "else" },
  { "Equalequal", "equalequal" },
  { "Ampamp", "ampamp" },
  { "Pipepipe", "pipepipe" },
  { "Const", "const" },
  { "Star", "star" },
  { "Slash", "slash" },
  { "Percent", "percent" },
  { "Greater", "greater" },
  { "Less", "less" },
  { "While", "while" },
  { "Break", "break" },
  { "Continue", "continue" },
  { "Lessequal", "lessequal" },
  { "Greaterequal", "greaterequal" },
  { "Exclaimequal", "exclaimequal" },
  { "Exclaim", "exclaim" },

  // 在这里继续添加其他映射
};

std::string location = "";
std::string Line = "1";
bool startOfLine = false;
bool leadingSpace = false;

void
print_token(const antlr4::Token* token,
            const antlr4::CommonTokenStream& tokens,
            std::ofstream& outFile,
            const antlr4::Lexer& lexer)
{
  auto& vocabulary = lexer.getVocabulary();

  auto tokenTypeName =
    std::string(vocabulary.getSymbolicName(token->getType()));

  if (tokenTypeName.empty())
    tokenTypeName = "<UNKNOWN>"; // 处理可能的空字符串情况

  if (tokenTypeMapping.find(tokenTypeName) != tokenTypeMapping.end()) {
    tokenTypeName = tokenTypeMapping[tokenTypeName];
  }

  if (tokenTypeName == "LineAfterPreprocessing") {
    std::string text = token->getText(); // 获取词法符号的文本

    std::regex rgx("#\\s*(\\d+)\\s*\"([^\"]+)\"[\\s\\d]*");
    std::smatch match;
    if (std::regex_search(text, match, rgx) && match.size() == 3) {
      Line = match[1].str();
      location = match[2].str();
      // std::cout << Line << location << endl;
      // 因为最后一行预编译行会让Line++，所以先减去一行
      int tempLine = std::stoi(Line);
      tempLine--;
      Line = std::to_string(tempLine);
    }
    return;
  }

  if (tokenTypeName == "Newline") {
    int tempLine = std::stoi(Line);
    tempLine++;
    Line = std::to_string(tempLine);
    startOfLine = true;
    return;
  }

  if (tokenTypeName == "Whitespace") {
    leadingSpace = true;
    return;
  }

  std::string CharPositionInLine =
    std::to_string(token->getCharPositionInLine() + 1);
  std::cout << Line << location << endl;
  std::string locInfo =
    "\tLoc=<" + location + ":" + Line + ":" + CharPositionInLine + ">";

  if (token->getText() != "<EOF>")
    outFile << tokenTypeName << " '" << token->getText() << "'\t";
  else{
    outFile << tokenTypeName << " '"
            << "'";
    return;
  }
  if (startOfLine) {
    outFile << " [StartOfLine]";
    startOfLine = false;
  }
  if (leadingSpace) {
    outFile << " [LeadingSpace]";
    leadingSpace = false;
  }

  outFile << locInfo << std::endl;
}

int
main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <input> <output>\n";
    return -1;
  }

  std::ifstream inFile(argv[1]);
  if (!inFile) {
    std::cout << "Error: unable to open input file: " << argv[1] << '\n';
    return -2;
  }

  std::ofstream outFile(argv[2]);
  if (!outFile) {
    std::cout << "Error: unable to open output file: " << argv[2] << '\n';
    return -3;
  }

  std::cout << "程序 '" << argv[0] << std::endl;
  std::cout << "输入 '" << argv[1] << std::endl;
  std::cout << "输出 '" << argv[2] << std::endl;

  antlr4::ANTLRInputStream input(inFile);
  SYsULexer lexer(&input);

  antlr4::CommonTokenStream tokens(&lexer);
  tokens.fill();

  for (auto&& token : tokens.getTokens()) {
    print_token(token, tokens, outFile, lexer);
  }
}
