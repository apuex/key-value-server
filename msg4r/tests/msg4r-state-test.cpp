#define BOOST_TEST_MODULE Struct1 Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

namespace msg4r {

typedef struct struct1 {
  std::string name;
  float32_t salary;
  float64_t capital;
  std::string title;
  std::map<std::string, std::string> props;
} struct1_t;

// for equals tests in testcases.
bool operator==(const struct1& lhs, const struct1& v) {
  return std::tie( lhs.name
                 , lhs.salary
                 , lhs.capital
                 , lhs.title
                 , lhs.props
                 )
                 == 
         std::tie( v.name
                 , v.salary
                 , v.capital
                 , v.title
                 , v.props
                 );
}

std::istream& read(std::istream& is, struct1_t& v) {
  msg4r::string_parser string_parser;
  msg4r::number_parser<float32_t> float32_parser;
  msg4r::number_parser<float64_t> float64_parser;
  msg4r::map_parser<
                    msg4r::string_parser,
                    msg4r::string_parser
                   > map_parser;
  string_parser(is, v.name);
  float32_parser(is, v.salary);
  float64_parser(is, v.capital);
  string_parser(is, v.title);
  map_parser(is, v.props);
  return is;
}

std::ostream& write(std::ostream& os, const struct1_t& v) {
  msg4r::write(os, v.name);
  msg4r::write(os, v.salary);
  msg4r::write(os, v.capital);
  msg4r::write(os, v.title);
  msg4r::write(os, v.props);
  return os;
}

std::ostream& operator<<(std::ostream& os, const msg4r::struct1& v) {
  // os.unsetf(std::ios::fixed | std::ios::scientific);
  os << std::fixed
     << "msg4r::struct1 {"
     << " name: " << v.name << ","
     << " salary: " << v.salary << ","
     << " capital: " << v.capital << ","
     << " title: " << v.title << ","
     << " props: " << v.props << ""
     << " }";
  return os;
}

} // namespace msg4r

using namespace msg4r;

BOOST_AUTO_TEST_CASE(struct1_test) {
  msg4r::struct1_t s1 = {
    "Donald J. Trump",
    35.6f,
    12345678901.234,
    "President",
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
  std::cout << "(s1 == s2) => " << (s1 == s2) << std::endl;
  BOOST_TEST(s1 == s2);
}

