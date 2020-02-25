
// © 2020 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html#License

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "intltest.h"
#include "number_decnum.h"
#include <iostream>

using number::impl::DecNum;

class DecNumTest : public IntlTest {
  public:
    DecNumTest() {}

    void runIndexedTest(int32_t index, UBool exec, const char *&name, char *par = NULL);

    void testMultiply();
};

extern IntlTest *createDecNumTest() { return new DecNumTest(); }

void DecNumTest::runIndexedTest(int32_t index, UBool exec, const char *&name, char * /*par*/) {
    if (exec) {
        logln("TestSuite DecNumTest: ");
    }

    TESTCASE_AUTO_BEGIN;
    TESTCASE_AUTO(testMultiply);
    TESTCASE_AUTO_END;
}

void DecNumTest::testMultiply() {
  //  IcuTestErrorCode status(*this, "DecNum testMultiply");
    UErrorCode status = U_ZERO_ERROR;
    
    StringPiece precision = "0.0001";

    DecNum lhs;
    DecNum rhs;
    lhs.setTo("100", status);
    rhs.setTo("100", status);

    DecNum expected;
    DecNum actual;

    lhs.multiplyBy(rhs, status);
    actual.setTo(lhs, status);

    expected.setTo("10000", status);

    assertEqualsNear("test DecNum", expected, actual, precision);
}
#endif /* #if !UCONFIG_NO_FORMATTING */
