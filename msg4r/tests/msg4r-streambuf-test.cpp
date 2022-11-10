#define BOOST_TEST_MODULE Streambuf Test
#ifdef _WIN32
#include <windows.h>
#endif
#include <boost/test/included/unit_test.hpp>

#include <msg4r.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/streambuf.hpp>

BOOST_AUTO_TEST_CASE(read_bytes_test) {
  uint8_t buf_out[] = { 0xfe, 0xca };
  uint8_t buf_in[4] = { 0 };
  boost::asio::streambuf sb_out;
  boost::asio::streambuf sb_in;
  std::ostream os(&sb_out);
  os.write(reinterpret_cast<const char*>(buf_out), sizeof(buf_out));
  msg4r::print_bytes(std::cout,
                     boost::asio::buffers_begin(sb_out.data()),
                     boost::asio::buffers_begin(sb_out.data()) + sb_out.size());

  std::copy(boost::asio::buffers_begin(sb_out.data()),
            boost::asio::buffers_begin(sb_out.data()) + sb_out.size(),
            boost::asio::buffers_begin(sb_in.prepare(sb_out.size())));
  sb_in.commit(sb_out.size());
  BOOST_TEST(sb_in.size() == sb_out.size());

  std::istream is(&sb_in);
  is.read(reinterpret_cast<char*>(buf_in), sizeof(buf_in));
  std::cout << "is.eof() = " << is.eof() << std::endl;
  std::cout << "is.bad() = " << is.bad() << std::endl;
  std::cout << "is.good() = " << is.good() << std::endl;
  std::cout << "is.fail() = " << is.fail() << std::endl;
  std::cout << "is.tellg() = " << is.tellg() << std::endl;
  std::cout << "is.gcount() = " << is.gcount() << std::endl;
}

BOOST_AUTO_TEST_CASE(segment_test) {
  const size_t BUFF_SIZE = 8;
  uint8_t buf_out1[] = { 0xfe, 0xca };
  uint8_t buf_out2[] = { 0xbe, 0xba };
  uint8_t buf_out3[] = { 0xad, 0xde };
  uint8_t buf_out4[] = { 0xef, 0xbe };

  boost::asio::streambuf sb_out;
  auto segment1 = sb_out.prepare(BUFF_SIZE);
  std::copy(buf_out1,
            buf_out1 + sizeof(buf_out1),
            boost::asio::buffers_begin(segment1));
  sb_out.commit(sizeof(buf_out1));

  auto segment2 = sb_out.prepare(BUFF_SIZE);
  std::copy(buf_out2,
            buf_out2 + sizeof(buf_out2),
            boost::asio::buffers_begin(segment2));
  sb_out.commit(sizeof(buf_out2));

  auto segment3 = sb_out.prepare(BUFF_SIZE);
  std::copy(buf_out3,
            buf_out3 + sizeof(buf_out3),
            boost::asio::buffers_begin(segment3));
  sb_out.commit(sizeof(buf_out3));

  auto segment4 = sb_out.prepare(BUFF_SIZE);
  std::copy(buf_out4,
            buf_out4 + sizeof(buf_out4),
            boost::asio::buffers_begin(segment4));
  sb_out.commit(sizeof(buf_out4));

  msg4r::print_bytes(std::cout,
                     boost::asio::buffers_begin(sb_out.data()),
                     boost::asio::buffers_begin(sb_out.data()) + sb_out.size());

  uint16_t word;
  std::istream is(&sb_out);
  is.read(reinterpret_cast<char*>(&word), sizeof(word));
  BOOST_TEST(0xcafe == word);
  is.read(reinterpret_cast<char*>(&word), sizeof(word));
  BOOST_TEST(0xbabe == word);
  is.read(reinterpret_cast<char*>(&word), sizeof(word));
  BOOST_TEST(0xdead == word);
  is.read(reinterpret_cast<char*>(&word), sizeof(word));
  BOOST_TEST(0xbeef == word);
  std::cout << "is.eof() = " << is.eof() << std::endl;
  std::cout << "is.bad() = " << is.bad() << std::endl;
  std::cout << "is.good() = " << is.good() << std::endl;
  std::cout << "is.fail() = " << is.fail() << std::endl;
  std::cout << "is.tellg() = " << is.tellg() << std::endl;
  std::cout << "is.gcount() = " << is.gcount() << std::endl;
}

BOOST_AUTO_TEST_CASE(string_test) {
  std::string str_out("hello, world!");
  std::string str_in;
  boost::asio::streambuf sb_out;
  boost::asio::streambuf sb_in;
  std::ostream os(&sb_out);
  os << str_out;
  msg4r::print_bytes(std::cout,
    boost::asio::buffers_begin(sb_out.data()),
    boost::asio::buffers_begin(sb_out.data()) + sb_out.size());

  std::copy(boost::asio::buffers_begin(sb_out.data()),
    boost::asio::buffers_begin(sb_out.data()) + sb_out.size(),
    boost::asio::buffers_begin(sb_in.prepare(sb_out.size())));
  sb_in.commit(sb_out.size());
  BOOST_TEST(sb_in.size() == sb_out.size());

  std::istream is(&sb_in);
  std::getline(is, str_in);
  std::cout << str_in << std::endl;
  std::cout << "is.eof() = " << is.eof() << std::endl;
  std::cout << "is.bad() = " << is.bad() << std::endl;
  std::cout << "is.good() = " << is.good() << std::endl;
  std::cout << "is.fail() = " << is.fail() << std::endl;
  std::cout << "is.tellg() = " << is.tellg() << std::endl;
  std::cout << "is.gcount() = " << is.gcount() << std::endl;
}

BOOST_AUTO_TEST_CASE(uint16_streambuf_test) {
  std::vector<uint16_t> s1 = { 0xcafe, 0xbeef };
  std::vector<uint16_t> s2;
  msg4r::vector_parser<msg4r::number_parser<uint16_t> > uint16_vector_parser;

  boost::asio::streambuf sb_out;
  boost::asio::streambuf sb_in;
  std::ostream os(&sb_out);
  msg4r::write(os, s1);
  msg4r::print_bytes(std::cout,
    boost::asio::buffers_begin(sb_out.data()),
    boost::asio::buffers_begin(sb_out.data()) + sb_out.size());

  std::copy(boost::asio::buffers_begin(sb_out.data()),
    boost::asio::buffers_begin(sb_out.data()) + sb_out.size(),
    boost::asio::buffers_begin(sb_in.prepare(sb_out.size())));
  sb_in.commit(sb_out.size());
  BOOST_TEST(sb_in.size() == sb_out.size());

  std::istream is(&sb_out);
  std::cout << "is.eof() = " << is.eof() << std::endl;
  std::cout << "is.bad() = " << is.bad() << std::endl;
  std::cout << "is.good() = " << is.good() << std::endl;
  std::cout << "is.fail() = " << is.fail() << std::endl;
  std::cout << "is.tellg() = " << is.tellg() << std::endl;
  std::cout << "is.gcount() = " << is.gcount() << std::endl;
  uint16_vector_parser(is, s2);
  BOOST_TEST(s1 == s2);
}

BOOST_AUTO_TEST_CASE(uint16_stringstream_test) {
  std::vector<uint16_t> s1 = { 0xcafe, 0xbeef };
  std::vector<uint16_t> s2;
  msg4r::vector_parser<msg4r::number_parser<uint16_t> > uint16_vector_parser;
  std::stringstream ssm;
  msg4r::write(ssm, s1);
  uint16_vector_parser(ssm, s2);
  BOOST_TEST(s1 == s2);

  s1 = { 0xdead, 0xface};
  s2.clear();
  msg4r::write(ssm, s1);
  uint16_vector_parser(ssm, s2);
  BOOST_TEST(s1 == s2);
}
