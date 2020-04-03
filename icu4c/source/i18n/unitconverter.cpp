// © 2020 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "charstr.h"
#include "measunit_impl.h"
#include "unicode/errorcode.h"
#include "unicode/measunit.h"
#include "unicode/stringpiece.h"
#include "unitconverter.h"

U_NAMESPACE_BEGIN

namespace {

const ConversionRateInfo &
extractConversionInfo(StringPiece source,
                      const MaybeStackVector<ConversionRateInfo> &conversionRateInfoList,
                      UErrorCode &status) {
    for (size_t i = 0, n = conversionRateInfoList.length(); i < n; ++i) {
        if (conversionRateInfoList[i]->sourceUnit.toStringPiece() == source)
            return *(conversionRateInfoList[i]);
    }

    status = U_INTERNAL_PROGRAM_ERROR;
    return ConversionRateInfo();
}

MeasureUnit extractBaseUnit(const MeasureUnit &source,
                            const MaybeStackVector<ConversionRateInfo> &conversionRateInfoList,
                            UErrorCode &status) {
    MeasureUnit result;
    int32_t count;
    const auto singleUnits = source.splitToSingleUnits(count, status);
    if (U_FAILURE(status)) return result;

    for (int i = 0; i < count; ++i) {
        const auto &singleUnit = singleUnits[i];
        int32_t dimensionality = singleUnit.getDimensionality(status);
        // Extract `ConversionRateInfo` using the absolute unit. For example: in case of `square-meter`,
        // we will use `meter`
        const auto singleUnitImpl = SingleUnitImpl::forMeasureUnit(singleUnit, status);
        const auto &rateInfo =
            extractConversionInfo(singleUnitImpl.identifier, conversionRateInfoList, status);
        if (U_FAILURE(status)) return result;

        // Multiply the power of the singleUnit by the power of the baseUnit. For example, square-hectare
        // must be p4-meter. (NOTE: hectare --> square-meter)
        auto baseUnit = MeasureUnit::forIdentifier(rateInfo.baseUnit.toStringPiece(), status);
        if (dimensionality != 1) {
            if (baseUnit.getComplexity(status) == UMEASURE_UNIT_SINGLE) {
                baseUnit = baseUnit.withDimensionality(
                    dimensionality * baseUnit.getDimensionality(status), status);
            } else {
                // Single units like volt, joule, newton, and knots map to
                // compound base units. Fortunately square-volt, square-joule,
                // square-newton, and square-knots don't generally make sense.
                // We treat them as illegal.
                status = U_ILLEGAL_ARGUMENT_ERROR;
                return MeasureUnit();
                // TODO(younies,hugovdm): (dimensionality = -1) might appear
                // though? one-per-joule, one-per-volt? Consider this. Find some
                // reasonable real-world usages that would trigger this problem.
            }
        }

        result = result.product(baseUnit, status);
    }

    return result;
}

} // namespace

UnitsMatchingState U_I18N_API
checkUnitsState(const MeasureUnit &source, const MeasureUnit &target,
                const MaybeStackVector<ConversionRateInfo> &conversionRateInfoList, UErrorCode &status) {
    auto sourceBaseUnit = extractBaseUnit(source, conversionRateInfoList, status);
    auto targetBaseUnit = extractBaseUnit(target, conversionRateInfoList, status);

    if (U_FAILURE(status)) return UNCONVERTIBLE;

    if (sourceBaseUnit == targetBaseUnit) return CONVERTIBLE;
    if (sourceBaseUnit == targetBaseUnit.reciprocal(status)) return RECIPROCAL;

    return UNCONVERTIBLE;
}

U_NAMESPACE_END

#endif /* #if !UCONFIG_NO_FORMATTING */