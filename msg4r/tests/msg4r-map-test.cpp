#define BOOST_TEST_MODULE Map Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

using namespace msg4r;

BOOST_AUTO_TEST_CASE(string_test) {
  msg4r::map_parser<
                    msg4r::string_parser,
                    msg4r::string_parser
                   > parse;
  std::map<std::string, std::string> s1 = { {"hello", "world"}, {"bob", "alice"} };
  std::map<std::string, std::string> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
  s1 = { { "hi", "there" }, { "good", "night" } };
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint16_test) {
  msg4r::map_parser<
                    msg4r::number_parser<uint16_t>,
                    msg4r::number_parser<uint16_t>
                   > parse;
  std::map<uint16_t, uint16_t> s1 = { {0xcafe, 0xbeef} };
  std::map<uint16_t, uint16_t> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
  s1 = { { 0xdead, 0xface } };
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint32_test) {
  msg4r::map_parser<
                    msg4r::number_parser<uint32_t>,
                    msg4r::number_parser<uint32_t>
                   > parse;
  std::map<uint32_t, uint32_t> s1 = { {0xcafe, 0xbeef} };
  std::map<uint32_t, uint32_t> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
  s1 = { { 0xdead, 0xface } };
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

