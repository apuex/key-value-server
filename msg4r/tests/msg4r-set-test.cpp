#define BOOST_TEST_MODULE Set Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

using namespace msg4r;

BOOST_AUTO_TEST_CASE(string_test) {
  msg4r::set_parser<msg4r::number_parser<uint16_t> > parse;
  std::set<uint16_t> s1 = {0xcafe, 0xbeef};
  std::set<uint16_t> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
  s1 = { 0xdead, 0xface};
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}
