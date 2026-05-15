#pragma once

#include <string>

// ============================================================================
// Quotes: motivational messages and appreciation
//   - Random quote on startup to keep things positive
//   - Appreciation when routines are completed (user feels rewarded)
//   - Category-specific benefits (real-world advantages of the activity)
// ============================================================================
namespace Quotes
{
    std::string getRandomQuote();
    std::string getAppreciationMessage();
    std::string getBenefitMessage(const std::string& category);
}
