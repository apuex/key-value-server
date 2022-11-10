#define BOOST_TEST_MODULE JSON Test
#include <boost/test/included/unit_test.hpp>

#include <ostream>
#include <boost/json/value_to.hpp>
#include <boost/json/src.hpp>

typedef struct my_struct {
  my_struct() : type_url_("type.googleapis.com/my_struct"), b_(), i_(), f_() {}
  my_struct(bool v) : type_url_("type.googleapis.com/my_struct/bool"), b_(v), i_(), f_() {}
  my_struct(int v) : type_url_("type.googleapis.com/my_struct/int"), b_(), i_(v), f_() {}
  my_struct(double v) : type_url_("type.googleapis.com/my_struct/double"), b_(), i_(), f_(v) {}
  my_struct(const my_struct& t) : type_url_(t.type_url_), b_(t.b_), i_(t.i_), f_(t.f_) {}

  my_struct& operator=(const my_struct& t) { 
    this->b_ = t.b_;
    this->i_ = t.i_;
    this->f_ = t.f_;
    return *this;
  }

  bool operator==(const my_struct& t) const { 
    return std::tie(type_url_, b_, i_, f_) == std::tie(t.type_url_, t.b_, t.i_, t.f_);
  }

  const std::string type_url_;
  bool b_;
  int i_;
  double f_;
} my_struct_t;

std::ostream& operator<<(std::ostream& os, const my_struct& t) {
  os << "struct my_struct { type_url = \"" << t.type_url_ << "\", i = " << t.i_ << " }";
  return os;
}

void tag_invoke(
  boost::json::value_from_tag,
  boost::json::value& jv,
  my_struct_t const & t) {
  jv = {
    {"@type", t.type_url_ },
    {"b", t.b_ },
    {"i", t.i_ },
    {"f", t.f_ }
  };
}

my_struct_t tag_invoke(
  boost::json::value_to_tag<my_struct_t>,
  const boost::json::value& jv) {
  boost::json::object const& obj = jv.as_object();
  if(obj.at("@type") == "type.googleapis.com/my_struct/bool") {
    return my_struct_t(boost::json::value_to<bool>(obj.at("b")));
  } else if(obj.at("@type") == "type.googleapis.com/my_struct/int") {
    return my_struct_t(boost::json::value_to<int>(obj.at("i")));
  } else if(obj.at("@type") == "type.googleapis.com/my_struct/double") {
    return my_struct_t(boost::json::value_to<double>(obj.at("f")));
  } else {
    return my_struct_t(boost::json::value_to<int>(obj.at("i")));
  }
}

template <typename T>
void check(T t) {
  BOOST_TEST(boost::json::value_to<T>(boost::json::value_from(t)) == t);
}

BOOST_AUTO_TEST_CASE(first_test) {
  double i = 1234;
  my_struct_t s1{i};
  my_struct_t s2{i};
  BOOST_TEST(s1 == s2);
  check(i);
  check(s1);
  auto str = boost::json::serialize(boost::json::value_from(s1));
  std::cout << str << std::endl;
  boost::json::value jv = boost::json::parse(str);
  s2 = boost::json::value_to<my_struct_t>(jv);
  BOOST_TEST(s1 == s2);
}


