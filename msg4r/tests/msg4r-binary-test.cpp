#define BOOST_TEST_MODULE Binary Test
#include <boost/test/included/unit_test.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/version.hpp>

#include <algorithm>
#include <ostream>

typedef struct my_struct {
  // friend class boost::serialization::access;
  // friend std::ostream& operator<<(std::ostream& os, const my_struct& t);
  explicit my_struct() : i() {}
  explicit my_struct(int iv, std::map<std::string, std::string> mv) : i(iv), m(mv) {}
  explicit my_struct(const my_struct& t) : i(t.i), m(t.m) {}

  my_struct& operator=(const my_struct& t) { 
    this->i = t.i;
    this->m = t.m;
    return *this;
  }

  bool operator==(const my_struct& t) const { 
    return (this->i == t.i && this->m == t.m);
  }

  int i;
  std::map<std::string, std::string> m;
} my_struct_t;

std::ostream& operator<<(std::ostream& os,
                         const std::map<std::string, std::string>& t) {
  os << "{";
  std::for_each(t.begin(), t.end(), [&](const std::pair<std::string, std::string>& e) { 
    os << e.first << ": " << e.second << ";" << std::endl;
    });
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const my_struct& t) {
  os << "struct my_struct { i = " << t.i << "; " << t.m << "; }";
  return os;
}

namespace boost {
namespace serialization {

template <typename Archive>
void serialize(Archive& ar, 
    my_struct& t,
    const unsigned int version) {
  ar& BOOST_SERIALIZATION_NVP(t.i);
  ar& BOOST_SERIALIZATION_NVP(t.m);
}

template <typename Archive>
void serialize(
    Archive& ar,
    std::map<std::string, std::string>& t,
  const unsigned int version) {
  std::for_each(t.begin(), t.end(), [&](const std::pair<std::string, std::string>& e) {
    ar& BOOST_SERIALIZATION_NVP(e.first);
    ar& BOOST_SERIALIZATION_NVP(e.second);
  });
}

} // namespace serialization
} // namespace boost

BOOST_CLASS_VERSION(my_struct, 0x7e)

BOOST_AUTO_TEST_CASE(first_test) {
  int i = 0xdead; 
  std::map<std::string, std::string> m{
      {"hello", "world"},
      {"fuck", "Donald Jr. Trump"},
  };
  my_struct_t s1{i,m};
  my_struct_t s2{i,m};
  //std::cout << s1;
  boost::archive::binary_oarchive oa(std::cout);
  oa << s1;
  BOOST_TEST(s1 == s2);
}


