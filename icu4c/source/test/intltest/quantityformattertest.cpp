// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/*
*******************************************************************************
* Copyright (C) 2014-2016, International Business Machines Corporation and
* others. All Rights Reserved.
*******************************************************************************
*
* File QUANTITYFORMATTERTEST.CPP
*
********************************************************************************
*/
#include "cstring.h"
#include "intltest.h"
#include "quantityformatter.h"
#include "unicode/simpleformatter.h"
#include "unicode/numfmt.h"
#include "unicode/plurrule.h"

#define ASSERT_OK(status) UPRV_BLOCK_MACRO_BEGIN { \
    if(U_FAILURE(status)) { \
        errcheckln(status, #status " = %s @ %s:%d", u_errorName(status), __FILE__, __LINE__); \
        return; \
    } \
} UPRV_BLOCK_MACRO_END

class QuantityFormatterTest : public IntlTest {
public:
    QuantityFormatterTest() {
    }
    void TestBasic();
    void runIndexedTest(int32_t index, UBool exec, const char*& name, char* par = nullptr) override;

private:
};

void QuantityFormatterTest::runIndexedTest(int32_t index, UBool exec, const char* &name, char* /*par*/) {
  TESTCASE_AUTO_BEGIN;
  TESTCASE_AUTO(TestBasic);
  TESTCASE_AUTO_END;
}

void QuantityFormatterTest::TestBasic() {
    UErrorCode status = U_ZERO_ERROR;
#if !UCONFIG_NO_FORMATTING
    QuantityFormatter fmt;
    assertFalse(
            "adding bad variant",
            fmt.addIfAbsent("a bad variant", "{0} pounds", status));
    assertEquals("adding bad variant status", U_ILLEGAL_ARGUMENT_ERROR, status);
    status = U_ZERO_ERROR;
    assertFalse(
            "Adding bad pattern",
            fmt.addIfAbsent("other", "{0} {1} too many placeholders", status));
    assertEquals("adding bad pattern status", U_ILLEGAL_ARGUMENT_ERROR, status);
    status = U_ZERO_ERROR;
    assertFalse("isValid with no patterns", fmt.isValid());
    assertTrue(
            "Adding good pattern with no placeholders",
            fmt.addIfAbsent("zero", "no placeholder", status));
    assertTrue(
            "Adding good pattern",
            fmt.addIfAbsent("other", "{0} pounds", status));
    assertTrue("isValid with other", fmt.isValid());
    assertTrue(
            "Adding good pattern",
            fmt.addIfAbsent("one", "{0} pound", status));

    assertEquals(
            "getByVariant",
            fmt.getByVariant("bad variant")->getTextWithNoArguments(),
            " pounds");
    assertEquals(
            "getByVariant",
            fmt.getByVariant("other")->getTextWithNoArguments(),
            " pounds");
    assertEquals(
            "getByVariant",
            fmt.getByVariant("one")->getTextWithNoArguments(),
            " pound");
    assertEquals(
            "getByVariant",
            fmt.getByVariant("few")->getTextWithNoArguments(),
            " pounds");

    // Test copy constructor
    {
        QuantityFormatter copied(fmt);
        assertEquals(
                "copied getByVariant",
                copied.getByVariant("other")->getTextWithNoArguments(),
                " pounds");
        assertEquals(
                "copied getByVariant",
                copied.getByVariant("one")->getTextWithNoArguments(),
                " pound");
        assertEquals(
                "copied getByVariant",
                copied.getByVariant("few")->getTextWithNoArguments(),
                " pounds");
    }
        
    // Test assignment
    {
        QuantityFormatter assigned;
        assigned = fmt;
        assertEquals(
                "assigned getByVariant",
                assigned.getByVariant("other")->getTextWithNoArguments(),
                " pounds");
        assertEquals(
                "assigned getByVariant",
                assigned.getByVariant("one")->getTextWithNoArguments(),
                " pound");
        assertEquals(
                "assigned getByVariant",
                assigned.getByVariant("few")->getTextWithNoArguments(),
                " pounds");
    }

    // Test format.
    {
        LocalPointer<NumberFormat> numfmt(
                NumberFormat::createInstance(Locale::getEnglish(), status));
        LocalPointer<PluralRules> plurrule(
                PluralRules::forLocale("en", status));
        ASSERT_OK(status);
        FieldPosition pos(FieldPosition::DONT_CARE);
        UnicodeString appendTo;
        assertEquals(
                "format singular",
                UnicodeString("1 pound"),
                fmt.format(
                        1.0,
                        *numfmt,
                        *plurrule,
                        appendTo,
                        pos,
                        status), true);
        appendTo.remove();
        assertEquals(
                "format plural",
                UnicodeString("2 pounds"),
                fmt.format(
                        2.0,
                        *numfmt,
                        *plurrule,
                        appendTo,
                        pos,
                        status), true);
    }
    fmt.reset();
    assertFalse("isValid after reset", fmt.isValid());
#endif
    assertSuccess("", status);
}

extern IntlTest *createQuantityFormatterTest() {
    return new QuantityFormatterTest();
}
