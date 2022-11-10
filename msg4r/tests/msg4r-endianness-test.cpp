#define BOOST_TEST_MODULE Endianness Test
#include <boost/test/included/unit_test.hpp>

#include <boost/endian/buffers.hpp>
#include <boost/endian/arithmetic.hpp>
#include <boost/endian/conversion.hpp>

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
  os << "map {\n";
  std::for_each(t.begin(), t.end(), [&](const std::pair<std::string, std::string>& e) { 
    os << "  " << e.first << " => " << e.second << ";" << std::endl;
    });
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const my_struct& t) {
  os << "struct my_struct {\n  i = " << t.i << ";\n  m = " << t.m << ";\n}";
  return os;
}

BOOST_AUTO_TEST_CASE(first_test) {
  int i = 0xdead; 
  int c = 0x0a414243; 
  boost::endian::little_float32_t f;
  double d = 2.0; 
  uint64_t u = 0xdead; 
  std::map<std::string, std::string> m{
      {"hello", "world"},
      {"fuck", "Donald Jr. Trump"},
  };
  my_struct_t s1{i,m};
  my_struct_t s2{i,m};
  printf("native 0x%08x to little equals 0x%08x\n", i, boost::endian::native_to_little(i));
  printf("native 0x%08x to big    equals 0x%08x\n", i, boost::endian::native_to_big(i));
  printf("native 0x%016zx to little equals 0x%016zx\n", u, boost::endian::native_to_little(u));
  printf("native 0x%016zx to big    equals 0x%016zx\n", u, boost::endian::native_to_big(u));
  f = 1.0f; 
  printf("native %f to little equals %f\n", (float)f, f.value());
  f += 1.0f; 
  printf("native %f to little equals %f\n", (float)f, f.value());
  std::cout.write((char*)&c, sizeof(i));
  std::cout << s1;
  BOOST_TEST(s1 == s2);
}


