#include <msg4r.hpp>

namespace msg4r {

template<> decode_state number_parser<float32_t>::operator()(std::istream& is, float32_t& v) {
  if(is.eof()) return decode_state::DECODE_INPROGRESS;
  is.read((char*)&t_ + index_, sizeof(float32_t) - index_);
  auto count = is.gcount();
  if(0 == count) return decode_state::DECODE_INPROGRESS;
  index_ += count;
  if(index_ != sizeof(float32_t)) {
    return decode_state::DECODE_INPROGRESS;
  } else {
    *reinterpret_cast<uint32_t*>(&v) = to_native(*reinterpret_cast<uint32_t*>(&t_));
    index_  = 0; // reset to initial state
    t_      = 0; // reset to initial state
    return decode_state::DECODE_SUCCESS;
  }
}

template<> decode_state number_parser<float64_t>::operator()(std::istream& is, float64_t& v) {
  if(is.eof()) return decode_state::DECODE_INPROGRESS;
  is.read((char*)&t_ + index_, sizeof(float64_t) - index_);
  auto count = is.gcount();
  if(0 == count) return decode_state::DECODE_INPROGRESS;
  index_ += count;
  if(index_ != sizeof(float64_t)) {
    return decode_state::DECODE_INPROGRESS;
  } else {
    *reinterpret_cast<uint64_t*>(&v) = to_native(*reinterpret_cast<uint64_t*>(&t_));
    index_  = 0; // reset to initial state
    t_      = 0; // reset to initial state
    return decode_state::DECODE_SUCCESS;
  }
}

encode_state write(std::ostream& os, const float32_t& v) {
  uint32_t t;
  t = from_native(*reinterpret_cast<const uint32_t*>(&v));
  os.write((char*)&t, sizeof(t));
  return encode_state::ENCODE_SUCCESS;
}

encode_state write(std::ostream& os, const float64_t& v) {
  uint64_t t;
  t = from_native(*reinterpret_cast<const uint64_t*>(&v));
  os.write((char*)&t, sizeof(t));
  return encode_state::ENCODE_SUCCESS;
}

string_parser::string_parser()
    : state_(0), length_(0), index_(0), t_(), length_parser_(), t_parser() {}

string_parser::~string_parser() { }

decode_state string_parser::operator()(std::istream& is, std::string& v) {
  BEGIN_STATE(state_)
  PARSE_STATE(state_, length_parser_, is, length_)
  PARSE_LIST_STATE(state_, t_parser, is, std::string::value_type, t_, push_back, length_, index_)
  END_STATE(state_, t_, v)
}

void string_parser::reset() {
  state_ = 0;  // reset to initial state
  length_ = 0; // reset to initial state
  index_ = 0;  // reset to initial state
  t_.clear();  // reset to initial state
  length_parser_.reset();
  t_parser.reset();
  /* std::cout << "void string_parser::reset()" << std::endl; */
}

encode_state write(std::ostream& os, const std::string& v) {
  MSG4R_SIZE_T length = static_cast<MSG4R_SIZE_T>(v.length());
  write(os, length);
  std::for_each(v.begin(), v.end(), [&](const std::string::value_type& e) {
      write(os, e);
    });
  return encode_state::ENCODE_SUCCESS;
}

std::ostream& operator<<(std::ostream& os, const encode_state& t) {
  switch (t) {
    case encode_state::ENCODE_SUCCESS:
      os << "ENCODE_SUCCESS";
      break;
    case encode_state::ENCODE_WAITING:
      os << "ENCODE_WAITING";
      break;
    case encode_state::ENCODE_FAILURE:
      os << "ENCODE_FAILURE";
      break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const decode_state& t) {
  switch (t) {
    case decode_state::DECODE_SUCCESS:
      os << "DECODE_SUCCESS";
      break;
    case decode_state::DECODE_INPROGRESS:
      os << "DECODE_INPROGRESS";
      break;
    case decode_state::DECODE_FAILURE:
      os << "DECODE_FAILURE";
      break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::string& t) {
  std::operator<<(os, "\"");
  std::operator<<(os, t);
  std::operator<<(os, "\"");
  return os;
}

void print_bytes(std::ostream& os, const char* buff, const size_t length) {
  os << "[ ";
  for (size_t i = 0; i != length; ++i) {
    os << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
       << (static_cast<uint32_t>(buff[i]) & 0xff) << " ";
  }
  os << "]" << std::endl;
}

}
