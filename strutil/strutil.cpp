#include "strutil.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace strutil {

/**
 * ""     -> ""
 * "    " -> ""
 */
vector<string> splitEmpty(const string& str) {
  vector<string> result;
  std::istringstream iss(str);

  copy(std::istream_iterator<string>(iss), std::istream_iterator<string>(),
       back_inserter(result));
  if (result.empty())
    result.push_back("");
  return result;
}

vector<string> splitSep(const string& text, const string& sep) {
  vector<string> result;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != string::npos) {
    result.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  result.push_back(text.substr(start));
  return result;
}

/**
 * @brief Return a list of the words in the string, using sep as the delimiter
 * string.
 *
 * @param sep if "" (default), splits and removes all whitespaces.
 */
vector<string> split(const string& str, const string& sep = "") {
  if (sep == "")
    return splitEmpty(str);
  else
    return splitSep(str, sep);
}

}  // namespace strutil
#include <array>

void test_splitEmpty() {
  std::vector<std::string> expected(1, "");
  assert(strutil::split("       ") == expected);
  assert(strutil::split("") == expected);
  assert(strutil::split(" 		  			 ") == expected);
}

int main() {
  test_splitEmpty();
  std::string str = "      ";
  std::vector<std::string> result = strutil::split(str);
  for (std::vector<std::string>::iterator it = result.begin();
       it != result.end(); ++it) {
    std::cout << "[" << *it << "]\n";
  }
  return 0;
}
