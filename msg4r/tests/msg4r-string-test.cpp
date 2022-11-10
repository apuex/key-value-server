#define BOOST_TEST_MODULE String Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE(string_test) {
  msg4r::string_parser parse;
  std::string s1 = "hello, world!";
  std::string s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
  s1 = "hi, honey!";
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}
