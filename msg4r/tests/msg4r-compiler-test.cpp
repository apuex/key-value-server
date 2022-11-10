#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <msg4r.hpp>
#ifdef _WIN32
#include <windows.h>
#endif

template<typename T>
void printf_sizeof_ref(T& r) {
  printf("sizeof(%s) = %zd\n", typeid(r).name(), sizeof(r));
}

int main(int argc, char* argv[]) {
  bool boolean = true;
  uint8_t uint8 = 0;
  uint16_t uint16 = 0;
  uint32_t uint32 = 0;
  uint64_t uint64 = 0;
  int8_t  int8 = 0;
  int16_t int16 = 0;
  int32_t int32 = 0;
  int64_t int64 = 0;
  std::string str = argv[0];
	printf("%s\n", argv[0]);

#ifdef _MSC_VER
  printf("_MSC_VER = %d\n", _MSC_VER);
  printf("sizeof(UINT) = %zd\n", sizeof(UINT));
#endif
  printf("sizeof(uint8_t) = %zd\n", sizeof(uint8_t));

  printf_sizeof_ref(boolean);
  printf_sizeof_ref(int8);
  printf_sizeof_ref(int16);
  printf_sizeof_ref(int32);
  printf_sizeof_ref(int64);
  printf_sizeof_ref(uint8);
  printf_sizeof_ref(uint16);
  printf_sizeof_ref(uint32);
  printf_sizeof_ref(uint64);
  printf_sizeof_ref(argv[0]);
  printf_sizeof_ref(str);

	return 0;
}
