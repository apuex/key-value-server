#pragma once
#include <boost/variant.hpp>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include <string>

namespace kvs {

enum class ValueType : int16_t
  { BOOLEAN = 1
  , BYTE    = 2
  , SHORT   = 3
  , INTEGER = 4
  , LONG    = 5
  , FLOAT   = 6
  , DOUBLE  = 7
  , STRING  = 8
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

typedef std::set<int64_t> IntegerKeySet; 
typedef std::map<int64_t, Value> IntegerKVMap; 
typedef std::map<std::string, std::string> StringKVMap; 

class ValueToString : public boost::static_visitor<>
{
 public:
  ValueToString(std::ostream& os) : os_(os) { }
  void operator()(bool c) const { os_ << std::boolalpha << c; }
  void operator()(int8_t c) const { os_ << "\'" << c << "\'"; }
  void operator()(int16_t c) const { os_ << std::dec << c; }
  void operator()(int32_t c) const { os_ << std::dec << c; }
  void operator()(int64_t c) const { os_ << std::dec << c << ""; }
  void operator()(float d) const { os_ << std::fixed << d << ""; }
  void operator()(double d) const { os_ << std::fixed << d << ""; }
  void operator()(std::string s) const { os_ << "\"" << s << "\""; }
 private:
  std::ostream& os_;
};

std::ostream& operator<<(std::ostream& os, const ValueType& v) {
  switch(v) {
    case ValueType::BOOLEAN: os << "ValueType::BOOLEAN"; break;
    case ValueType::BYTE: os << "ValueType::BYTE"; break;
    case ValueType::SHORT: os << "ValueType::SHORT"; break;
    case ValueType::INTEGER: os << "ValueType::INTEGER"; break;
    case ValueType::LONG: os << "ValueType::LONG"; break;
    case ValueType::FLOAT: os << "ValueType::FLOAT"; break;
    case ValueType::DOUBLE: os << "ValueType::DOUBLE"; break;
    case ValueType::STRING: os << "ValueType::STRING"; break;
    default: os << "ValueType::UNKNOWN"; break;
  }
  return os;
}

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

enum class MessageType : int16_t
  { VALUES_VO            = 0
  , UPDATE_VALUES_CMD    = 1
  , GET_VALUES_CMD       = 2
  , SUBSCRIBE_VALUES_CMD = 3
  , UNSUBSCRIBE_ALL_CMD  = 5
  };

class ValuesVo {
  IntegerKVMap values;
};

class UpdateValuesCmd {
  IntegerKVMap values;
};

class GetValuesCmd {
  IntegerKeySet keys;
};

class SubscribeValuesCmd {
  IntegerKeySet keys;
};

class UnSubscribeAllCmd {
};

std::ostream& operator<<(std::ostream& os, const MessageType& v) {
  switch(v) {
    case MessageType::VALUES_VO: os << "MessageType::VALUES_VO"; break;
    case MessageType::UPDATE_VALUES_CMD: os << "MessageType::UPDATE_VALUES_CMD"; break;
    case MessageType::GET_VALUES_CMD: os << "MessageType::GET_VALUES_CMD"; break;
    case MessageType::SUBSCRIBE_VALUES_CMD: os << "MessageType::SUBSCRIBE_VALUES_CMD"; break;
    case MessageType::UNSUBSCRIBE_ALL_CMD: os << "MessageType::UNSUBSCRIBE_ALL_CMD"; break;
    default: os << "MessageType::UNKNOWN"; break;
  }
  return os;
}


} // namespace kvs

