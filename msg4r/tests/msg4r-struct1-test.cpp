#define BOOST_TEST_MODULE Struct1 Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

namespace msg4r {

typedef struct struct1 {
  uint16_t version;
  uint16_t class_type;
  uint32_t id;
  uint16_t value_type;
  std::string name;
  uint16_t state;
  float64_t analog;
  std::string str;
  std::map<std::string, std::string> props;
} struct1_t;

// for equals tests in testcases.
bool operator==(const struct1& lhs, const struct1& v) {
  return std::tie( lhs.version
                 , lhs.class_type
                 , lhs.id
                 , lhs.value_type
                 , lhs.name
                 , lhs.state
                 , lhs.analog
                 , lhs.str
                 , lhs.props
                 )
                 == 
         std::tie( v.version
                 , v.class_type
                 , v.id
                 , v.value_type
                 , v.name
                 , v.state
                 , v.analog
                 , v.str
                 , v.props
                 );
}

std::istream& read(std::istream& is, struct1_t& v) {
  msg4r::string_parser string_parser;
  msg4r::number_parser<uint16_t> uint16_parser;
  msg4r::number_parser<uint32_t> uint32_parser;
  msg4r::number_parser<float32_t> float32_parser;
  msg4r::number_parser<float64_t> float64_parser;
  msg4r::map_parser<
                    msg4r::string_parser,
                    msg4r::string_parser
                   > map_parser;
  uint16_parser(is, v.version);
  uint16_parser(is, v.class_type);
  uint32_parser(is, v.id);
  uint16_parser(is, v.value_type);
  string_parser(is, v.name);
  uint16_parser(is, v.state);
  float64_parser(is, v.analog);
  string_parser(is, v.str);
  map_parser(is, v.props);
  return is;
}

std::ostream& write(std::ostream& os, const struct1_t& v) {
  msg4r::write(os, v.version);
  msg4r::write(os, v.class_type);
  msg4r::write(os, v.id);
  msg4r::write(os, v.value_type);
  msg4r::write(os, v.name);
  msg4r::write(os, v.state);
  msg4r::write(os, v.analog);
  msg4r::write(os, v.str);
  msg4r::write(os, v.props);
  return os;
}

std::ostream& operator<<(std::ostream& os, const msg4r::struct1& v) {
  os << "msg4r::struct1 {"
     << " version: 0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << v.version << ","
     << " class_type: 0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << v.class_type << ","
     << " id: " << std::dec << v.id << ","
     << " value_type: " << v.value_type << ","
     << " name: " << v.name << ","
     << " state: " << v.state << ","
     << " analog: " << v.analog << ","
     << " str: " << v.str << ","
     << " props: " << v.props << ""
     << " }";
  return os;
}

} // namespace msg4r

using namespace msg4r;

BOOST_AUTO_TEST_CASE(struct1_test) {
  msg4r::struct1_t s1 = {
    0xcafe,
    0xbeef,
    0x00000001,
    0x0001,
    "Donald J. Trump",
    0x01,
    35.6,
    "president",
    { {"mother", "fucker"}, { "dead", "face" } }
  };
  msg4r::struct1_t s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  msg4r::read(ssm, s2);
  std::cout << "s1 = " << s1 << std::endl;
  std::cout << "s2 = " << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

