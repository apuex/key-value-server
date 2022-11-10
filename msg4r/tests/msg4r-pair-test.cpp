#define BOOST_TEST_MODULE Pair Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

using namespace std;
using namespace msg4r;

BOOST_AUTO_TEST_CASE(string_string_test) {
  msg4r::pair_parser<
                     msg4r::string_parser,
                     msg4r::string_parser
                     > parse;
  std::pair<std::string, std::string> s1 = {"hello", "world"};
  std::pair<std::string, std::string> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1.first == s2.first);
  BOOST_TEST(s1.second == s2.second);
  s1 = {"Bob", "Alice"};
  ssm.str("");
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1.first == s2.first);
  BOOST_TEST(s1.second == s2.second);
}

BOOST_AUTO_TEST_CASE(uint16_string_test) {
  msg4r::pair_parser<
                     msg4r::number_parser<uint16_t>,
                     msg4r::string_parser
                     > parse;
  std::pair<uint16_t, std::string> s1 = {0xcafe, "hello"};
  std::pair<uint16_t, std::string> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
            << s2 << std::endl;
  BOOST_TEST(s1.first == s2.first);
  BOOST_TEST(s1.second == s2.second);
  s1 = {0xbeef, "world"};
  ssm.str("");
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
            << s2 << std::endl;
  BOOST_TEST(s1.first == s2.first);
  BOOST_TEST(s1.second == s2.second);
}
