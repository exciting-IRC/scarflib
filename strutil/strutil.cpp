#include "strutil.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace strutil {

/**
 * ""           -> ""
 * ".\t..\t..." -> "" (. is whitespace)
 * " a  b  c"   -> "a", "b", "c"
 * "a b c"      -> "a", "b", "c"
 * "a"          -> "a"
 */
vector<string> splitEmptySep(const string& str) {
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
  std::size_t start = 0U, end = text.find(sep);
  while (end != std::string::npos) {
    result.push_back(text.substr(start, end - start));
    start = end + sep.length();
    end = text.find(sep, start);
  }
  result.push_back(text.substr(start, end));
  return result;
}

/**
 * @brief Return a list of the words in the string, using sep as the delimiter
 * string.
 *
 * @param sep if "" (default), splits and removes all whitespaces.
 *
 * "a b c" " " -> "a", "b", "c"
 * "a,b,c" ","  -> "a", "b", "c"
 */
vector<string> split(const string& str, const string& sep = "") {
  if (sep == "")
    return splitEmptySep(str);
  else
    return splitSep(str, sep);
}

}  // namespace strutil

void test_splitEmptySep() {
  {
    std::vector<std::string> expected(1, "");
    assert(strutil::split("       ") == expected);
    assert(strutil::split("") == expected);
    assert(strutil::split(" 		  			") == expected);
  }
  {
    const std::string tmp[3] = {"a", "b", "c"};
    std::vector<std::string> expected(tmp, tmp + 3);
    assert(strutil::split("a b c") == expected);
    assert(strutil::split("a   b    c") == expected);
  }
  {
    std::vector<std::string> expected(1, "a");
    assert(strutil::split("a") == expected);
  }
}

void test_splitSep() {
  {
    const std::string tmp[3] = {"a", "b", "c"};
    std::vector<std::string> expected(tmp, tmp + 3);
    assert(strutil::split("a b c", " ") == expected);

    assert(strutil::split("a,b,c", ",") == expected);
    assert(strutil::split("a!!b!!c", "!!") == expected);
    assert(strutil::split("a->b->c", "->") == expected);
  }
  {
    const std::string tmp = "a!!b!!c";
    assert(strutil::split(tmp, "->") == std::vector<std::string>(1, tmp));
  }
  {
    const std::string tmp[4] = {"a", "", "b", "c"};
    std::vector<std::string> expected(tmp, tmp + 4);
    assert(strutil::split("a,,b,c", ",") == expected);
    assert(strutil::split("a->->b->c", "->") == expected);
  }
}

int main() {
  std::string str = "a b c";
  std::vector<std::string> result = strutil::split(str, " ");
  for (std::vector<std::string>::iterator it = result.begin();
       it != result.end(); ++it) {
    std::cout << "[" << *it << "]\n";
  }
  test_splitEmptySep();
  test_splitSep();
  return 0;
}
