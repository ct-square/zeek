#pragma once

#include "Defines.h"
#include "Analyzer.h"

namespace llanalyzer {
class Analyzer;

class AnalyzerSet {
public:
    virtual ~AnalyzerSet() = default;
    virtual Analyzer* dispatch(identifier_t identifier) = 0;
    virtual void reset() = 0;

protected:
    friend class Manager;
};

} // end of llanalyzer namespace