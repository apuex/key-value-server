#pragma once
#include <boost/variant.hpp>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>

namespace kvs {

enum class ValueType : int16_t
  { BOOLEAN
  , BYTE
  , SHORT
  , INTEGER
  , LONG 
  , FLOAT
  , DOUBLE
  , STRING
  };

typedef boost::variant
  < bool
  , int8_t 
  , int16_t 
  , int32_t 
  , int64_t 
  , float 
  , double 
  , std::string
  > Value;

typedef std::map<int64_t, Value> IntegerKVMap; 
typedef std::map<std::string, std::string> StringKVMap; 

struct ValueToString : public boost::static_visitor<>
{
 public:
  ValueToString(std::ostream& os) : os_(os) { }
  void operator()(bool c) const { os_ << std::boolalpha << c; }
  void operator()(int8_t c) const { os_ << "\'" << c << "\'"; }
  void operator()(int16_t c) const { os_ << std::dec << c; }
  void operator()(int32_t c) const { os_ << std::dec << c; }
  void operator()(int64_t c) const { os_ << std::dec << c << "L"; }
  void operator()(float d) const { os_ << std::fixed << d << "F"; }
  void operator()(double d) const { os_ << std::fixed << d << "L"; }
  void operator()(std::string s) const { os_ << "\"" << s << "\""; }
 private:
  std::ostream& os_;
};

std::ostream& operator<<(std::ostream& os, const IntegerKVMap& v) {
  ValueToString vts(os);
  bool first = true;
  
  os << "{ ";
  std::for_each(v.begin(), v.end(), [&](const std::pair<int64_t, Value>& e) {
      if(first) {
        first = false;
      } else {
        os << "\n, ";
      }
      os << "{ ";
      os << e.first;
      os << ", ";
      boost::apply_visitor(vts, e.second);
      os << " }";
    });
  os << "\n}";

  return os;
}

std::ostream& operator<<(std::ostream& os, const StringKVMap& v) {
  ValueToString vts(os);
  bool first = true;
  
  os << "{ ";
  std::for_each(v.begin(), v.end(), [&](const std::pair<std::string, std::string>& e) {
      if(first) {
        first = false;
      } else {
        os << "\n, ";
      }
      os << "{ ";
      os << "\"" << e.first << "\"";
      os << ", ";
      os << "\"" << e.second << "\"";
      os << " }";
    });
  os << "\n}";

  return os;
}

} // namespace kvs

