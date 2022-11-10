#pragma once

#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <boost/endian/arithmetic.hpp>
#include <boost/endian/conversion.hpp>

namespace msg4r {

#ifndef MSG4R_SIZE_T
#define MSG4R_SIZE_T uint32_t
#endif

#ifdef MSG4R_BIG_ENDIAN
#define from_native boost::endian::native_to_big
#define to_native boost::endian::big_to_native
#else
#define from_native boost::endian::native_to_little
#define to_native boost::endian::little_to_native
#endif

#define float32_t float
#define float64_t double

#ifdef _MSC_VER
#define MSG4R_PACKED(n) _declspec(align((n)))
#else
#define MSG4R_PACKED(n) __attribute__((packed))
#endif

#define DECLARE_PARSER_FOR(value)                               \
public:                                                         \
  typedef value value_type;                                     \
  msg4r::decode_state operator()(std::istream& is, value_type& v);     \
  void reset();                                                 \
private:                                                        \
  int state_;                                                   \

#define DECLARE_DYNAMIC_PARSER_FOR(value, value_ptr_type)       \
public:                                                         \
  typedef value value_type;                                     \
  msg4r::decode_state operator()(std::istream& is, value_type& v);     \
  msg4r::decode_state operator()(std::istream& is, value_ptr_type& v); \
  void reset();                                                 \
private:                                                        \
  int state_;                                                   \

#define IMPLEMENT_PTR_PARSER(parser, value_ptr_type)                   \
  msg4r::decode_state parser::operator()(std::istream& is, value_ptr_type& v) { \
  value_type& value = *dynamic_cast<value_type*>(v.get());             \
  return operator()(is, value);                                        \
}                                                                      \

#define BEGIN_IMPLEMENT_PARSER(parser)                                 \
  msg4r::decode_state parser::operator()(std::istream& is, value_type& v) {     \
  msg4r::decode_state field_state;                                            \
  switch (state_) {                                                    \
  case 0:                                                              \

#define PARSE_FIELD(parser, stream, field)           \
  case __LINE__:                                     \
    state_ = __LINE__;                               \
    field_state = parser(stream, field);             \
    /* std::cout << __FILE__ << "(" << std::dec <<  __LINE__ << "): PARSE_FIELD field_state = " << field_state << std::endl; */ \
    if (msg4r::decode_state::DECODE_SUCCESS != field_state) \
      return field_state;                            \
    state_ += 1;                                     \

#define INIT_DYNAMIC_FIELD_PARSER(init_action, type_field) \
  case __LINE__:                                           \
    state_ = __LINE__;                                     \
    init_action(type_field);                               \
    state_ += 1;                                           \

#define PARSE_DYNAMIC_FIELD(parser, stream, field)   \
  case __LINE__:                                     \
    state_ = __LINE__;                               \
    field_state = (*parser)(stream, field);          \
    if (msg4r::decode_state::DECODE_SUCCESS != field_state) \
      return field_state;                            \
    v = field;                                       \
    state_ += 1;                                     \

#define PARSE_VALIDATE_FIELD(parser, stream, field, validate_expr)    \
  case __LINE__:                                                      \
    state_ = __LINE__;                                                \
    field_state = parser(stream, field);                              \
    if (msg4r::decode_state::DECODE_SUCCESS != field_state)                  \
      return field_state;                                             \
    if (!validate_expr)                                               \
      return msg4r::decode_state::DECODE_FAILURE;                            \
    state_ += 1;                                                      \

#define END_IMPLEMENT_PARSER()           \
  case __LINE__:                         \
    reset();                             \
    return msg4r::decode_state::DECODE_SUCCESS; \
  default:                               \
    return msg4r::decode_state::DECODE_FAILURE; \
  }                                      \
}                                        \

#define BEGIN_STATE(state) switch(state) { case 0:

#define PARSE_STATE(state, op, s, v)  \
  case __LINE__: /* fall through*/    \
    {state = __LINE__;                \
    auto status = op(s, v);           \
    /* std::cout << __FILE__ << "(" << std::dec <<  __LINE__ << "): PARSE_STATE status = " << status << std::endl; */ \
    if (msg4r::decode_state::DECODE_SUCCESS != status) return status;} \

#define PARSE_LIST_STATE(state, op, s, T, t_, add, length, index) \
  case __LINE__: /* fall through*/                 \
    state = __LINE__;                              \
    for (; index != length;) {                     \
      T c;                                         \
      auto status = op(s, c);                      \
      /* std::cout << __FILE__ << "(" << std::dec <<  __LINE__ << "): PARSE_LIST_STATE status = " << status << std::endl; */ \
      if (msg4r::decode_state::DECODE_SUCCESS != status) return status; \
      index += 1;                                  \
      t_.add(c);                                   \
    }                                              \

#define END_STATE(state, t_, v)          \
  case __LINE__: /* fall through*/       \
    state = __LINE__;                    \
    v = t_;                              \
    reset();                             \
  }                                      \
  return msg4r::decode_state::DECODE_SUCCESS;   \


enum class encode_state {
  ENCODE_SUCCESS   = 0,
  ENCODE_WAITING   = 1,
  ENCODE_FAILURE   = 2
};

enum class decode_state {
  DECODE_SUCCESS   = 0,
  DECODE_INPROGRESS = 1,
  DECODE_FAILURE   = 2
};

std::ostream& operator<<(std::ostream& os, const encode_state& t);
std::ostream& operator<<(std::ostream& os, const decode_state& t);

template <typename T>
struct number_parser {
  typedef T value_type;
  number_parser();
  virtual ~number_parser();
  decode_state operator()(std::istream& is, T& v);
  void reset();

  MSG4R_SIZE_T index_; 
  T t_;
};

template<typename T> number_parser<T>::number_parser(): index_(0), t_() { }
template<typename T> number_parser<T>::~number_parser() { }

template<> decode_state number_parser<float32_t>::operator()(std::istream& is, float32_t& v);
template<> decode_state number_parser<float64_t>::operator()(std::istream& is, float64_t& v);

template<typename T>
decode_state number_parser<T>::operator()(std::istream& is, T& v) {
  if(is.eof()) return decode_state::DECODE_INPROGRESS;
  is.read((char*)&t_ + index_, sizeof(T) - index_);
  auto count = is.gcount();
  /*
  std::cout << __FILE__ << "(" << std::dec <<  __LINE__ << "): sizeof(T) = " << sizeof(T)
     << ", index = " << index_
     << ", count = " << count
     << std::endl;
  */
  if(0 == count) return decode_state::DECODE_INPROGRESS;
  index_ += count;
  if(index_ != sizeof(T)) {
    return decode_state::DECODE_INPROGRESS;
  } else {
    v = to_native(t_);
    reset();
    return decode_state::DECODE_SUCCESS;
  }
}

template <typename T>
void number_parser<T>::reset() {
  index_ = static_cast<MSG4R_SIZE_T>(0); // reset to initial state
  t_ = static_cast<T>(0);                // reset to initial state
}

encode_state write(std::ostream& os, const float32_t& v);

encode_state write(std::ostream& os, const float64_t& v);

struct string_parser {
  typedef std::string value_type;
  string_parser();
  virtual ~string_parser();
  decode_state operator()(std::istream& is, std::string& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  std::string t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  number_parser<std::string::value_type> t_parser;
};

decode_state read(std::istream& is, std::string& v);
encode_state write(std::ostream& os, const std::string& v);

std::ostream& operator<<(std::ostream& os, const std::string& t);

/**
 * write integers and numbers. T = { int8_t, ... }
 */
template<typename T>
encode_state write(std::ostream& os, const T& v) {
  T t = from_native(v);
  os.write((char*)&t, sizeof(t));
  return encode_state::ENCODE_SUCCESS;
}

template<typename ParserType>
struct vector_parser {
  typedef typename ParserType::value_type element_type;
  typedef std::vector<element_type> value_type;
  vector_parser();
  virtual ~vector_parser();
  decode_state operator()(std::istream& is, std::vector<element_type>& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  std::vector<element_type> t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  ParserType t_parser_;
};

template<typename ParserType>
vector_parser<ParserType>::vector_parser()
    : state_(0),
      length_(0),
      index_(0),
      t_(), length_parser_(), t_parser_() {}

template<typename ParserType>
vector_parser<ParserType>::~vector_parser() {}

template <typename ParserType>
void vector_parser<ParserType>::reset() {
  state_ = 0;   // reset to initial state
  length_ = 0;  // reset to initial state
  index_ = 0;   // reset to initial state
  t_.clear();   // reset to initial state
  length_parser_.reset();
  t_parser_.reset();
}

template <typename ParserType>
decode_state vector_parser<ParserType>::operator()(std::istream& is, std::vector<element_type>& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser_, is, element_type, t_, push_back, length_, index_)
  END_STATE(state_, t_, v)
}

template<typename T>
encode_state write(std::ostream& os, const std::vector<T>& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](const T& e) {
      write(os, e);
    });
  return encode_state::ENCODE_SUCCESS;
}

template <typename ParserType>
struct list_parser {
  typedef typename ParserType::value_type element_type;
  typedef std::list<element_type> value_type;
  list_parser();
  virtual ~list_parser();
  decode_state operator()(std::istream& is, value_type& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  value_type t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  ParserType t_parser_;
};

template <typename ParserType>
list_parser<ParserType>::list_parser()
    : state_(0), length_(0), index_(0), t_(), length_parser_(), t_parser_() {}

template <typename ParserType>
list_parser<ParserType>::~list_parser() {}

template <typename ParserType>
void list_parser<ParserType>::reset() {
  state_ = 0;   // reset to initial state
  length_ = 0;  // reset to initial state
  index_ = 0;   // reset to initial state
  t_.clear();   // reset to initial state
  length_parser_.reset();
  t_parser_.reset();
}

template <typename ParserType>
decode_state list_parser<ParserType>::operator()(std::istream& is,
                                             value_type& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser_, is, element_type, t_,
                   push_back, length_, index_)
  END_STATE(state_, t_, v)
}

template<typename T>
encode_state write(std::ostream& os, const std::list<T>& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](const T& e) {
      write(os, e);
    });
  return encode_state::ENCODE_SUCCESS;
}

template <typename ParserType>
struct set_parser {
  typedef typename ParserType::value_type element_type;
  typedef std::set<element_type> value_type;
  set_parser();
  virtual ~set_parser();
  decode_state operator()(std::istream& is, value_type& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  value_type t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  ParserType t_parser_;
};

template <typename ParserType>
set_parser<ParserType>::set_parser()
    : state_(0), length_(0), index_(0), t_(), length_parser_(), t_parser_() {}

template <typename ParserType>
set_parser<ParserType>::~set_parser() {}

template <typename ParserType>
void set_parser<ParserType>::reset() {
  state_ = 0;   // reset to initial state
  length_ = 0;  // reset to initial state
  index_ = 0;   // reset to initial state
  t_.clear();   // reset to initial state
  length_parser_.reset();
  t_parser_.reset();
}

template <typename ParserType>
decode_state set_parser<ParserType>::operator()(std::istream& is, value_type& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser_, is, element_type, t_,
                   insert, length_, index_)
  END_STATE(state_, t_, v)
}

template<typename T>
encode_state write(std::ostream& os, const std::set<T>& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](const T& e) {
      write(os, e);
    });
  return encode_state::ENCODE_SUCCESS;
}

template<typename KParserType, typename VParserType>
struct pair_parser {
  typedef typename KParserType::value_type first_type;
  typedef typename VParserType::value_type second_type;
  typedef std::pair<first_type, second_type> value_type;
  pair_parser();
  virtual ~pair_parser();
  decode_state operator()(std::istream& is, value_type& v);
  void reset();

  int state_;
  first_type k_;
  second_type v_;
  KParserType k_parser_;
  VParserType v_parser_;
};

template<typename KParserType, typename VParserType>
pair_parser<KParserType, VParserType>::pair_parser()
    : state_(0),
      k_(), v_(), k_parser_(), v_parser_() {}

template<typename KParserType, typename VParserType>
pair_parser<KParserType, VParserType>::~pair_parser() {}

template<typename KParserType, typename VParserType>
void pair_parser<KParserType, VParserType>::reset() {
  state_ = 0;   // reset to initial state
  k_parser_.reset();
  v_parser_.reset();
}

template<typename KParserType, typename VParserType>
decode_state pair_parser<KParserType, VParserType>::operator()(std::istream& is, value_type& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, k_parser_, is, k_)
  PARSE_STATE(state_, v_parser_, is, v_)
  END_STATE(state_, std::make_pair(k_, v_), v)
}

template<typename K, typename V>
encode_state write(std::ostream& os, const std::pair<K, V>& v) {
  write(os, v.first);
  write(os, v.second);
  return encode_state::ENCODE_SUCCESS;
}

template<typename KParserType, typename VParserType>
struct map_parser {
  typedef typename KParserType::value_type first_type;
  typedef typename VParserType::value_type second_type;
  typedef std::pair<first_type, second_type> element_type;
  typedef std::map<first_type, second_type> value_type;
  typedef pair_parser<KParserType, VParserType> ParserType;
  map_parser();
  virtual ~map_parser();
  decode_state operator()(std::istream& is, value_type& v);
  void reset();

  int state_;
  MSG4R_SIZE_T length_;
  MSG4R_SIZE_T index_;
  value_type t_;
  number_parser<MSG4R_SIZE_T> length_parser_;
  ParserType t_parser_;
};

template<typename KParserType, typename VParserType>
map_parser<KParserType, VParserType>::map_parser()
    : state_(0), length_(0), index_(0), t_(), length_parser_(), t_parser_() {}

template<typename KParserType, typename VParserType>
map_parser<KParserType, VParserType>::~map_parser() {}

template<typename KParserType, typename VParserType>
void map_parser<KParserType, VParserType>::reset() {
  state_ = 0;   // reset to initial state
  length_ = 0;  // reset to initial state
  index_ = 0;   // reset to initial state
  t_.clear();   // reset to initial state
  length_parser_.reset();
  t_parser_.reset();
}

template<typename KParserType, typename VParserType>
decode_state map_parser<KParserType, VParserType>::operator()(std::istream& is, value_type& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser_, is, element_type, t_,
                   insert, length_, index_)
  END_STATE(state_, t_, v)
}

template<typename K, typename V>
encode_state write(std::ostream& os, const std::map<K, V>& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.size());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](const std::pair<K, V>& e) {
      write(os, e.first);
      write(os, e.second);
    });
  return encode_state::ENCODE_SUCCESS;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os,
                         const std::pair<K, V>& t) {
  os << t.first << ": " << t.second;
  return os;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os,
                         const std::map<K, V>& t) {
  os << "std::map { ";
  for(auto e = t.begin(); e != t.end();) {
    os << *e;
    e++;
    if(e != t.end()) os << ", ";
  }
  os << " }";
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os,
                         const std::vector<T>& t) {
  os << "std::vector { ";
  for(auto e = t.begin(); e != t.end();) {
    os << *e;
    e++;
    if(e != t.end()) os << ", ";
  }
  os << " }";
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os,
                         const std::list<T>& t) {
  os << "std::list { ";
  for(auto e = t.begin(); e != t.end();) {
    os << *e;
    e++;
    if(e != t.end()) os << ", ";
  }
  os << " }";
  return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os,
                         const std::set<T>& t) {
  os << "std::set { ";
  for(auto e = t.begin(); e != t.end();) {
    os << *e;
    e++;
    if(e != t.end()) os << ", ";
  }
  os << " }";
  return os;
}

template<typename T>
void print_bytes(std::ostream& os, const T& str) {
  os << "[ ";
  std::for_each(str.begin(), str.end(), [&](const typename T::value_type& e) {
    os << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
       << (static_cast<uint32_t>(e) & 0xff) << " ";
  });
  os << "]" << std::endl;
}

void print_bytes(std::ostream& os, const char* buff, const size_t length);
template <typename Iterator>
void print_bytes(std::ostream& os, const Iterator begin, const Iterator end) {
  os << "[ ";
  std::for_each(begin, end, [&](const char& e) {
    os << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
       << (static_cast<uint32_t>(e) & 0xff) << " ";
  });
  os << "]" << std::endl;
}

/*
template<typename K, typename V>
constexpr bool operator==(const std::pair<K, V>& lhs,
                         const std::pair<K, V>& rhs) {
  return std::tie(lhs.first, lhs.second) == std::tie(rhs.first, rhs.second);
}
*/

}
