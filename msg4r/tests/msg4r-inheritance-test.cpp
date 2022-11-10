#define BOOST_TEST_MODULE data_point Test
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <sstream>

namespace msg4r {

enum class data_point_type: uint8_t {
  DI = 0,
  AI = 1,
  SI = 2,
};

std::ostream& operator<<(std::ostream& os, const data_point_type& v) {
  switch(v) {
    case data_point_type::DI:
      os << "data_point_type::DI";
      break;
    case data_point_type::AI:
      os << "data_point_type::AI";
      break;
    case data_point_type::SI:
      os << "data_point_type::SI";
      break;
    default:
      os << "data_point_type::UNKNOWN";
      break;
  }
  return os;
}

class data_point {
protected:
  data_point(const data_point_type t);
public:
  typedef std::shared_ptr<data_point> data_point_ptr;
  virtual ~data_point();
  const data_point_type data_type;
};

data_point::data_point(const data_point_type v): data_type(v) {}
data_point::~data_point() {}

class di_point: public data_point {
public:
  di_point();
  di_point(uint32_t v);
  virtual ~di_point();
  uint32_t value;
};

di_point::di_point(): data_point(data_point_type::DI), value() {}
di_point::di_point(uint32_t v): data_point(data_point_type::DI), value(v) {}
di_point::~di_point() {}

// for equals tests in testcases.
bool operator==(const di_point& lhs, const di_point& v) {
  return std::tie( lhs.data_type
                 , lhs.value
                 )
                 == 
         std::tie( v.data_type
                 , v.value
                 );
}

std::ostream& operator<<(std::ostream& os, const di_point& v) {
  // os.unsetf(std::ios::fixed | std::ios::scientific);
  os << std::fixed
     << "msg4r::di_point"
     << " {"
     << " data_type: " << v.data_type << ","
     << std::boolalpha
     << " value: " << static_cast<bool>(v.value)
     << " }";
  return os;
}

class abstract_parser {
public:
  abstract_parser() {}
  virtual ~abstract_parser() {}
  virtual decode_state operator()(std::istream& is, data_point::data_point_ptr& v) = 0;
};
typedef std::shared_ptr<abstract_parser> data_point_parser_ptr;

class di_parser: public abstract_parser {
  DECLARE_DYNAMIC_PARSER_FOR(di_point, data_point::data_point_ptr)
public:
  di_parser();
  virtual ~di_parser();
private:
  msg4r::number_parser<uint32_t> parse_uint32_;
};

IMPLEMENT_PTR_PARSER(di_parser, data_point::data_point_ptr)
BEGIN_IMPLEMENT_PARSER(di_parser)
  PARSE_FIELD(parse_uint32_, is, v.value)
END_IMPLEMENT_PARSER()

di_parser::di_parser()
    : state_(),
      parse_uint32_() {}

di_parser::~di_parser() {}

void di_parser::reset() {
  state_ = 0;
  parse_uint32_.reset();
}

class ai_point: public data_point {
public:
  ai_point();
  ai_point(double v);
  virtual ~ai_point();
  double value;
};

ai_point::ai_point(): data_point(data_point_type::AI), value() {}
ai_point::ai_point(double v): data_point(data_point_type::AI), value(v) {}
ai_point::~ai_point() {}

// for equals tests in testcases.
bool operator==(const ai_point& lhs, const ai_point& v) {
  return std::tie( lhs.data_type
                 , lhs.value
                 )
                 == 
         std::tie( v.data_type
                 , v.value
                 );
}

std::ostream& operator<<(std::ostream& os, const ai_point& v) {
  os << std::fixed
     << "msg4r::ai_point"
     << " {"
     << " data_type: " << v.data_type << ","
     << std::fixed
     << " value: " << v.value
     << " }";
  return os;
}

typedef struct ai_parser: public abstract_parser {
  DECLARE_DYNAMIC_PARSER_FOR(ai_point, data_point::data_point_ptr)
public:
  ai_parser();
  virtual ~ai_parser();

private:
  msg4r::number_parser<float64_t> parse_float64_;
} ai_parser_t;

IMPLEMENT_PTR_PARSER(ai_parser, data_point::data_point_ptr)
BEGIN_IMPLEMENT_PARSER(ai_parser)
  PARSE_FIELD(parse_float64_, is, v.value)
END_IMPLEMENT_PARSER()

ai_parser::ai_parser()
    : state_(),
      parse_float64_() {}

ai_parser::~ai_parser() {}

void ai_parser::reset() {
  state_ = 0;
  parse_float64_.reset();
}

class si_point: public data_point {
public:
  si_point();
  si_point(const std::string& v);
  virtual ~si_point();
  std::string value;
};

si_point::si_point(): data_point(data_point_type::SI), value() {}
si_point::si_point(const std::string& v): data_point(data_point_type::SI), value(v) {}
si_point::~si_point() {}

  // for equals tests in testcases.
bool operator==(const si_point& lhs, const si_point& v) {
  return std::tie( lhs.data_type
                 , lhs.value
                 )
                 == 
         std::tie( v.data_type
                 , v.value
                 );
}

std::ostream& operator<<(std::ostream& os, const si_point& v) {
  // os.unsetf(std::ios::fixed | std::ios::scientific);
  os << std::fixed
     << "msg4r::si_point"
     << " {"
     << " data_type: " << v.data_type << ","
     << " value: " << v.value
     << " }";
  return os;
}

std::ostream& operator<<(std::ostream& os, const data_point& v) {
  switch(v.data_type) {
    case data_point_type::DI:
      os << dynamic_cast<const di_point&>(v);
      break;
    case data_point_type::AI:
      os << dynamic_cast<const ai_point&>(v);
      break;
    case data_point_type::SI:
      os << dynamic_cast<const si_point&>(v);
      break;
    default:
      break;
  }
  return os;
}
typedef struct si_parser: public abstract_parser {
  DECLARE_DYNAMIC_PARSER_FOR(si_point, data_point::data_point_ptr)
public:
  si_parser();
  virtual ~si_parser();

private:
  msg4r::string_parser parse_string_;
} si_parser_t;

IMPLEMENT_PTR_PARSER(si_parser, data_point::data_point_ptr)
BEGIN_IMPLEMENT_PARSER(si_parser)
  PARSE_FIELD(parse_string_, is, v.value)
END_IMPLEMENT_PARSER()

si_parser::si_parser()
    : state_(),
      parse_string_() {}

si_parser::~si_parser() {}

void si_parser::reset() {
  state_ = 0;
  parse_string_.reset();
}

class data_point_parser {
  DECLARE_PARSER_FOR(data_point::data_point_ptr)
public:
  data_point_parser();
  virtual ~data_point_parser();

private:
  data_point_type data_type_;
  data_point::data_point_ptr data_point_;
  data_point_parser_ptr parser_ptr_;

  inline void init_action(const data_point_type data_type) {
    switch(data_type) {
      case data_point_type::DI:
        data_point_ = std::make_shared<di_point>();
        parser_ptr_ = std::make_shared<di_parser>();
        break;
      case data_point_type::AI:
        data_point_ = std::make_shared<ai_point>();
        parser_ptr_ = std::make_shared<ai_parser>();
        break;
      case data_point_type::SI:
        data_point_ = std::make_shared<si_point>();
        parser_ptr_ = std::make_shared<si_parser>();
        break;
    }
  }

  msg4r::number_parser<data_point_type> parse_uint8_;
  msg4r::number_parser<float64_t> parse_float64_;
  msg4r::string_parser parse_string_;
};

BEGIN_IMPLEMENT_PARSER(data_point_parser)
  PARSE_FIELD(parse_uint8_, is, data_type_)
  INIT_DYNAMIC_FIELD_PARSER(init_action, data_type_)
  PARSE_DYNAMIC_FIELD(parser_ptr_, is, data_point_)
END_IMPLEMENT_PARSER()

data_point_parser::data_point_parser()
    : state_(),
      parse_uint8_(),
      parse_float64_(),
      parse_string_() {}

  data_point_parser::~data_point_parser() {}

void data_point_parser::reset() {
  state_ = 0;
  parse_uint8_.reset();
  parse_float64_.reset();
  parse_string_.reset();
}

std::ostream& write(std::ostream& os, const di_point& v) {
  msg4r::write(os, v.data_type);
  msg4r::write(os, v.value);
  return os;
}

std::ostream& write(std::ostream& os, const ai_point& v) {
  msg4r::write(os, v.data_type);
  msg4r::write(os, v.value);
  return os;
}

std::ostream& write(std::ostream& os, const si_point& v) {
  msg4r::write(os, v.data_type);
  msg4r::write(os, v.value);
  return os;
}

} // namespace msg4r

using namespace msg4r;

BOOST_AUTO_TEST_CASE(ai_data_point_test) {
  msg4r::data_point_parser parse_data_point;
  msg4r::ai_point a1 = { 35.6 };
  msg4r::di_point d1 = { true };
  msg4r::si_point s1 = { "Hello, World!" };
  msg4r::data_point::data_point_ptr a2;
  msg4r::data_point::data_point_ptr d2;
  msg4r::data_point::data_point_ptr s2;
  std::stringstream ssm;
  msg4r::decode_state state;
  msg4r::write(ssm, a1);
  // initial pos
  std::istream::pos_type pos = ssm.tellg();
  // get length
  ssm.seekg(0, ssm.end);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(9));
  ssm.seekg(pos);
  
  std::string str = ssm.str();
  std::cout << "complete message:" << std::endl; 
  msg4r::print_bytes(std::cout, str);

  state = parse_data_point(ssm, a2);
  std::cout << "parse ssm => " << state << std::endl;
  BOOST_TEST(decode_state::DECODE_SUCCESS == state);

  std::cout << "a1 = " << a1 << std::endl;
  std::cout << "a2 = " << (*a2) << std::endl;
  std::cout << "(a1 == a2) => "
            << std::boolalpha
            << (a1 == dynamic_cast<ai_point&>(*a2)) << std::endl;
  BOOST_TEST(a1 == dynamic_cast<ai_point&>(*a2));
}

BOOST_AUTO_TEST_CASE(di_data_point_test) {
  msg4r::data_point_parser parse_data_point;
  msg4r::di_point d1 = { true };
  msg4r::data_point::data_point_ptr d2;
  std::stringstream ssm;
  msg4r::decode_state state;
  msg4r::write(ssm, d1);
  // initial pos
  std::istream::pos_type pos = ssm.tellg();
  // get length
  ssm.seekg(0, ssm.end);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(5));
  ssm.seekg(pos);

  std::string str = ssm.str();
  std::cout << "complete message:" << std::endl;
  msg4r::print_bytes(std::cout, str);

  state = parse_data_point(ssm, d2);
  std::cout << "parse ssm => " << state << std::endl;
  BOOST_TEST(decode_state::DECODE_SUCCESS == state);

  std::cout << "d1 = " << d1 << std::endl;
  std::cout << "d2 = " << (*d2) << std::endl;
  std::cout << "(d1 == d2) => "
            << std::boolalpha
            << (d1 == dynamic_cast<di_point&>(*d2)) << std::endl;
  BOOST_TEST(d1 == dynamic_cast<di_point&>(*d2));
}

BOOST_AUTO_TEST_CASE(si_data_point_test) {
  msg4r::data_point_parser parse_data_point;
  msg4r::si_point s1 = { "Hello, World!" };
  msg4r::data_point::data_point_ptr s2;
  std::stringstream ssm;
  msg4r::decode_state state;
  msg4r::write(ssm, s1);
  // initial pos
  std::istream::pos_type pos = ssm.tellg();
  // get length
  ssm.seekg(0, ssm.end);
  BOOST_TEST(ssm.tellg() == static_cast<std::istream::pos_type>(16));
  ssm.seekg(pos);

  std::string str = ssm.str();
  std::cout << "complete message:" << std::endl;
  msg4r::print_bytes(std::cout, str);

  state = parse_data_point(ssm, s2);
  std::cout << "parse ssm => " << state << std::endl;
  BOOST_TEST(decode_state::DECODE_SUCCESS == state);

  std::cout << "s1 = " << s1 << std::endl;
  std::cout << "s2 = " << (*s2) << std::endl;
  std::cout << "(s1 == s2) => "
            << std::boolalpha
            << (s1 == dynamic_cast<si_point&>(*s2)) << std::endl;
  BOOST_TEST(s1 == dynamic_cast<si_point&>(*s2));
}
