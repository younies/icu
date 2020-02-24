
// © 2020 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html#License

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "intltest.h"
#include "number_decnum.h"
#include <iostream>

class DecNumTest : public IntlTest {
  public:
    DecNumTest() {}

    void runIndexedTest(int32_t index, UBool exec, const char *&name, char *par = NULL);

    void testBasic();
};

extern IntlTest *createDecNumTest() { return new DecNumTest(); }

void DecNumTest::runIndexedTest(int32_t index, UBool exec, const char *&name, char * /*par*/) {
    if (exec) {
        logln("TestSuite DecNumTest: ");
    }

    TESTCASE_AUTO_BEGIN;
    TESTCASE_AUTO(testBasic);
    TESTCASE_AUTO_END;
}

void DecNumTest::testBasic() {
    IcuTestErrorCode status(*this, "DecNum testBasic");

    assertEquals("test DecNum", 1, 2);
}
#endif /* #if !UCONFIG_NO_FORMATTING */
