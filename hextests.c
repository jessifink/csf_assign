// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));
  //
  hex_format_offset(192, buf);
  ASSERT(0 == strcmp(buf, "000000c0"));
  hex_format_offset(16L, buf);
  ASSERT(0 == strcmp(buf, "00000010"));
  hex_format_offset(12L, buf);
  ASSERT(0 == strcmp(buf, "0000000c"));

}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));
  //
  hex_format_byte_as_hex(objs->test_data_1[1], buf);
  ASSERT(0 == strcmp(buf, "65"));
  hex_format_byte_as_hex(objs->test_data_1[2], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c"));
  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21"));
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
  ASSERT('e' == hex_to_printable(objs->test_data_1[1]));
}
