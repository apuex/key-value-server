#define BOOST_TEST_MODULE List Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

using namespace msg4r;

BOOST_AUTO_TEST_CASE(string_test) {
  msg4r::list_parser<msg4r::number_parser<uint16_t> > parse;
  std::list<uint16_t> s1 = {0xcafe, 0xbeef};
  std::list<uint16_t> s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  BOOST_TEST(s1 == s2);
  s1 = { 0xdead, 0xface};
  s2.clear();
  ssm.str("");
  msg4r::write(ssm, s1);
  parse(ssm, s2);
  std::cout << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}
