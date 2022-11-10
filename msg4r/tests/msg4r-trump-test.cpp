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

typedef struct struct1_parser {
  DECLARE_PARSER_FOR(struct1)
public:
  struct1_parser();
  virtual ~struct1_parser();

private:
  msg4r::string_parser parse_string_;
  msg4r::number_parser<float32_t> parse_float32_;
  msg4r::number_parser<float64_t> parse_float64_;
  msg4r::map_parser<msg4r::string_parser, msg4r::string_parser> parse_string_string_map_;
} struct1_parser_t;

BEGIN_IMPLEMENT_PARSER(struct1_parser)
  PARSE_FIELD(parse_string_, is, v.name)
  PARSE_FIELD(parse_float32_, is, v.salary)
  PARSE_VALIDATE_FIELD(parse_float64_, is, v.capital, (v.capital > 300000000.00))
  PARSE_FIELD(parse_string_, is, v.title)
  PARSE_FIELD(parse_string_string_map_, is, v.props)
END_IMPLEMENT_PARSER()

struct1_parser::struct1_parser()
    : state_(),
      parse_string_(),
      parse_float32_(),
      parse_float64_(),
      parse_string_string_map_() {}

struct1_parser::~struct1_parser() {}

void struct1_parser::reset() {
  state_ = 0;
  parse_string_.reset();
  parse_float32_.reset();
  parse_float64_.reset();
  parse_string_string_map_.reset();
  parse_string_.reset();
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
  msg4r::struct1_parser_t parse_struct1;
  msg4r::struct1_t s1 = {
    "Donald J. Trump",
    35.6f,
    12345678901.234,
    "President",
    { {"mother", "fucker"}, { "dead", "face" } }
  };
  msg4r::struct1_t s2 = {
    "",
    0,
    0,
    "",
    {}
  };
  std::stringstream ssm;
  std::stringstream segment1;
  std::stringstream segment2;
  std::stringstream segment3;
  std::stringstream segment4;
  msg4r::decode_state state;
  msg4r::write(ssm, s1);
  // initial pos
  std::istream::pos_type pos = ssm.tellg();
  // get length
  ssm.seekg(0, ssm.end);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(70));
  ssm.seekg(pos);
  
  std::string str = ssm.str();
  std::cout << "complete message:" << std::endl; 
  msg4r::print_bytes(std::cout, str);

  std::cout << "splitted to 4 segment:" << std::endl;
  char buff1[13];
  char buff2[17];
  char buff3[17];
  char buff4[24];
  ssm.seekg(0);
  ssm.read(buff1, sizeof(buff1));
  ssm.read(buff2, sizeof(buff2));
  ssm.read(buff3, sizeof(buff3));
  ssm.read(buff4, sizeof(buff4));

  std::cout << "segment 1:" << std::endl; 
  segment1.write(buff1, sizeof(buff1));
  msg4r::print_bytes(std::cout, segment1.str());
  segment1.seekg(0);
  
  std::cout << "segment 2:" << std::endl; 
  segment2.write(buff2, sizeof(buff2));
  msg4r::print_bytes(std::cout, segment2.str());
  segment2.seekg(0);

  std::cout << "segment 3:" << std::endl; 
  segment3.write(buff3, sizeof(buff3));
  msg4r::print_bytes(std::cout, segment3.str());
  segment3.seekg(0);

  std::cout << "segment 4:" << std::endl; 
  segment4.write(buff4, sizeof(buff4));
  msg4r::print_bytes(std::cout, segment4.str());
  segment4.seekg(0);

  state = parse_struct1(segment1, s2);
  std::cout << "parse segment 1 => " << state << std::endl; 
  BOOST_TEST(decode_state::DECODE_INPROGRESS == state);
  
  state = parse_struct1(segment2, s2);
  std::cout << "parse segment 2 => " << state << std::endl; 
  BOOST_TEST(decode_state::DECODE_INPROGRESS == state);
  
  state = parse_struct1(segment3, s2);
  std::cout << "parse segment 3 => " << state << std::endl; 
  BOOST_TEST(decode_state::DECODE_INPROGRESS == state);
  
  state = parse_struct1(segment4, s2);
  std::cout << "parse segment 4 => " << state << std::endl; 
  BOOST_TEST(decode_state::DECODE_SUCCESS == state);

  std::cout << "s1 = " << s1 << std::endl;
  std::cout << "s2 = " << s2 << std::endl;
  std::cout << "(s1 == s2) => " << (s1 == s2) << std::endl;
  BOOST_TEST(s1 == s2);
}

