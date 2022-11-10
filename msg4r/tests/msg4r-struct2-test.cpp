#define BOOST_TEST_MODULE switch_states Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

namespace msg4r {

//#pragma pack(push, r1, 1)
//typedef struct __attribute__((packed)) switch_states {
typedef struct MSG4R_PACKED(2) switch_states {
  uint16_t SW0:1;
  uint16_t SW1:1;
  uint16_t SW2:1;
  uint16_t SW3:1;
  uint16_t SW4:1;
  uint16_t SW5:1;
  uint16_t SW6:1;
  uint16_t SW7:1;
  uint16_t SW8:1;
  uint16_t SW9:1;
  uint16_t SW10:1;
  uint16_t SW11:1;
  uint16_t SW12:1;
  uint16_t SW13:1;
  uint16_t SW14:1;
  uint16_t SW15:1;
} switch_states_t;
//#pragma pack(pop, r1)

// for equals tests in testcases.
bool operator==(const switch_states_t& lhs, const switch_states_t& rhs) {
  return (*reinterpret_cast<const uint16_t*>(&lhs))
      == (*reinterpret_cast<const uint16_t*>(&rhs));

}
/*
bool operator==(const switch_states& lhs, const switch_states& rhs) {
  return std::tie( lhs.SW0
                 , lhs.SW1
                 , lhs.SW2
                 , lhs.SW3
                 , lhs.SW4
                 , lhs.SW5
                 , lhs.SW6
                 , lhs.SW7
                 , lhs.SW8
                 , lhs.SW9
                 , lhs.SW10
                 , lhs.SW11
                 , lhs.SW12
                 , lhs.SW13
                 , lhs.SW14
                 , lhs.SW15
                 )
                 == 
         std::tie( rhs.SW0
                 , rhs.SW1
                 , rhs.SW2
                 , rhs.SW3
                 , rhs.SW4
                 , rhs.SW5
                 , rhs.SW6
                 , rhs.SW7
                 , rhs.SW8
                 , rhs.SW9
                 , rhs.SW10
                 , rhs.SW11
                 , rhs.SW12
                 , rhs.SW13
                 , rhs.SW14
                 , rhs.SW15
                 );
}
*/

std::istream& read(std::istream& is, switch_states_t& v) {
  msg4r::number_parser<uint8_t> parse;
  for (size_t i = 0; i != sizeof(v); ++i) {
    parse(is, *(reinterpret_cast<uint8_t*>(&v) + i));
  }
  return is;
}

std::ostream& write(std::ostream& os, const switch_states_t& v) {
  for (size_t i = 0; i != sizeof(v); ++i) {
    msg4r::write(os, *(reinterpret_cast<const uint8_t*>(&v) + i));
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const msg4r::switch_states& v) {
  os << "msg4r::switch_states {"
     << " SW0: " << v.SW0 << ","
     << " SW1: " << v.SW1 << ","
     << " SW2: " << v.SW2 << ","
     << " SW3: " << v.SW3 << ","
     << " SW4: " << v.SW4 << ","
     << " SW5: " << v.SW5 << ","
     << " SW6: " << v.SW6 << ","
     << " SW7: " << v.SW7 << ","
     << " SW8: " << v.SW8 << ","
     << " SW9: " << v.SW9 << ","
     << " SW10: " << v.SW10 << ","
     << " SW11: " << v.SW11 << ","
     << " SW12: " << v.SW12 << ","
     << " SW13: " << v.SW13 << ","
     << " SW14: " << v.SW14 << ","
     << " SW15: " << v.SW15
     << " }";
  return os;
}

} // namespace msg4r

using namespace msg4r;

BOOST_AUTO_TEST_CASE(switch_states_test) {
  msg4r::switch_states_t s1 = {
    0x00,
    0x01,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x01,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00
  };
  msg4r::switch_states_t s2;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  std::string str = ssm.str();
  msg4r::print_bytes(std::cout, str);
  msg4r::read(ssm, s2);
  std::cout << "s1 = " << s1 << std::endl;
  std::cout << "s2 = " << s2 << std::endl;
  BOOST_TEST(s1 == s2);
}

