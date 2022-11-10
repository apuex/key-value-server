#define BOOST_TEST_MODULE DataToString Test
#include <boost/test/included/unit_test.hpp>
#include <boost/variant.hpp>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include <message.hpp>

using namespace kvs;

BOOST_AUTO_TEST_CASE(DataToStringTest) {
  IntegerKVMap map1
  { { 1,  true }
  , { 2,  false }
  , { 3,  0x41 }
  , { 6,  0x41L }
  , { 7,  1.234 }
  , { 8,  std::string("Hello, World!")}
  };

  StringKVMap map2
  { { "Hello",  "World" }
  , { "Cafe", "Babe" }
  , { "Dead", "Face" }
  };

  std::cout << "map1\n=\n" << map1 << std::endl;
  std::cout << "map2\n=\n" << map2 << std::endl;
}

