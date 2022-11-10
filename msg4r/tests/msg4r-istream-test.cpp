#define BOOST_TEST_MODULE IStream Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

using namespace msg4r;

BOOST_AUTO_TEST_CASE(istream1_test) {
  msg4r::string_parser parse_string;
  msg4r::number_parser<uint8_t> parse_number;
  uint8_t b1 = 0x7e;
  uint8_t b2 = 0x00;
  std::string s1 = "01234567";
  std::string s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  msg4r::write(ssm, b1);
  parse_string(ssm, s2);
  BOOST_TEST(!ssm.eof());
  BOOST_TEST(s1 == s2);
  parse_number(ssm, b2);
  BOOST_TEST(!ssm.eof());
  BOOST_TEST(b1 == b2);
}

BOOST_AUTO_TEST_CASE(istream2_test) {
  msg4r::string_parser parse_string;
  msg4r::number_parser<uint8_t> parse_number;
  uint8_t b1 = 0x7e;
  uint8_t b2 = 0x00;
  std::string s1 = "01234567";
  std::string s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  parse_string(ssm, s2);
  BOOST_TEST(!ssm.eof());
  BOOST_TEST(s1 == s2);
  BOOST_TEST(parse_number(ssm, b2) == msg4r::decode_state::DECODE_INPROGRESS);
  BOOST_TEST(ssm.eof());
  // fail some times in the Windows environment
  // VS 2019 is the case:
  // BOOST_TEST(b1 != b2);
}

BOOST_AUTO_TEST_CASE(istream3_test) {
  msg4r::string_parser parse_string;
  msg4r::number_parser<uint8_t> parse_number;
  uint8_t b1 = 0x7e;
  uint8_t b2 = 0x00;
  std::string s1 = "01234567";
  std::string s2;
  std::stringstream ssm;

  // initial pos
  std::istream::pos_type pos = ssm.tellg();
  // get length
  ssm.seekg(0, ssm.end);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(0));

  msg4r::write(ssm, s1);
  // length after write s1
  ssm.seekg(0, ssm.end);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(8 + sizeof(MSG4R_SIZE_T)));

  msg4r::write(ssm, b1);
  // length after write b1
  ssm.seekg(0, ssm.end);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(8 + sizeof(MSG4R_SIZE_T) + 1));

  ssm.seekg(pos);
  parse_string(ssm, s2);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(8 + sizeof(MSG4R_SIZE_T)));
  BOOST_TEST(!ssm.eof());
  BOOST_TEST(s1 == s2);

  parse_number(ssm, b2);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(8 + sizeof(MSG4R_SIZE_T) + 1));
  BOOST_TEST(!ssm.eof());
  BOOST_TEST(b1 == b2);
}

BOOST_AUTO_TEST_CASE(istream4_test) {
  msg4r::number_parser<uint32_t> parse_number;
  uint16_t b1 = 0xcafe;
  uint32_t b2 = 0x00;
  std::stringstream ssm;
  std::istream::pos_type pos;

  msg4r::write(ssm, b1);

  BOOST_TEST(decode_state::DECODE_INPROGRESS == parse_number(ssm, b2));
  pos = ssm.tellg();
  BOOST_TEST(static_cast<std::istream::pos_type>(-1) == pos);
  BOOST_TEST(ssm.eof());
  BOOST_TEST(b1 != b2);
}

BOOST_AUTO_TEST_CASE(istream5_test) {
  std::string s1 = "01234567";
  std::string s2;
  char buff[16];
  char expect[16] = {0x00, 0x08, '0', '1', '2', '3', '4', '5', '6', '7'};
  std::stringstream ssm;
  std::istream::pos_type pos;

  msg4r::write(ssm, s1);
  pos = ssm.tellg();
  BOOST_TEST(static_cast<std::istream::pos_type>(0) == pos);
  BOOST_TEST(0 == ssm.gcount());

  memset(buff, 0, sizeof(buff));
  ssm.read(buff, sizeof(buff));
  pos = ssm.tellg();
  BOOST_TEST(static_cast<std::istream::pos_type>(-1) == pos);
  BOOST_TEST(static_cast<std::istream::pos_type>(8 + sizeof(MSG4R_SIZE_T)) == ssm.gcount());
  BOOST_TEST(ssm.eof());
  msg4r::print_bytes(std::cout, buff, sizeof(buff));
  BOOST_TEST(expect == buff);
}

