#include "quotes.h"
#include <vector>
#include <random>       // C++11: better random number generation
#include <chrono>       // C++11: chrono-based seeding
#include <algorithm>    // for std::transform
#include <utility>      // for std::pair

namespace Quotes
{

// ============================================================================
// C++11: std::initializer_list (used implicitly) to initialize vectors
//   This brace-initialization syntax constructs the vector from a list
// ============================================================================

// Motivational quotes shown at startup
static const std::vector<std::string> motivational_quotes = {
    "\"The secret of getting ahead is getting started.\" - Mark Twain",
    "\"It always seems impossible until it's done.\" - Nelson Mandela",
    "\"Small daily improvements are the key to staggering long-term results.\"",
    "\"Success is the sum of small efforts repeated day in and day out.\"",
    "\"Don't watch the clock; do what it does. Keep going.\" - Sam Levenson",
    "\"The only way to do great work is to love what you do.\" - Steve Jobs",
    "\"Start where you are. Use what you have. Do what you can.\"",
    "\"Your future is created by what you do today, not tomorrow.\"",
    "\"Discipline is the bridge between goals and accomplishment.\"",
    "\"A journey of a thousand miles begins with a single step.\" - Lao Tzu",
    "\"Believe you can and you're halfway there.\" - Theodore Roosevelt",
    "\"Today is a good day to start something amazing!\""
};

// Appreciation messages when user completes a routine
static const std::vector<std::string> appreciation_messages = {
    "Awesome job! You crushed it!",
    "Fantastic! Keep that momentum going!",
    "You're on fire! Another one done!",
    "Champion move! Well done!",
    "Bullseye! That's how it's done!",
    "Excellent work! You're making great progress!",
    "Unstoppable! Keep reaching for the stars!",
    "Way to go! You should be proud!"
};

// C++11: std::pair used in the vector - maps category keywords to benefit messages
static const std::vector<std::pair<std::string, std::string>> benefit_messages = {
    {"health",   "Taking care of your health is the best investment. Your body thanks you!"},
    {"exercise", "Exercise boosts energy, mood, and focus. You're building a stronger you!"},
    {"fitness",  "Fitness builds discipline and energy. You're investing in yourself!"},
    {"study",    "Learning compounds over time. Today's study is tomorrow's expertise!"},
    {"learning", "Every bit of learning makes you more capable. Keep growing!"},
    {"work",     "Productivity builds confidence. You're one step closer to your goals!"},
    {"food",     "Good nutrition fuels a great life. Smart choice!"},
    {"sleep",    "Quality rest is a superpower. You'll feel amazing tomorrow!"},
    {"social",   "Staying connected enriches life. Relationships matter!"},
    {"hobby",    "Hobbies keep life exciting and reduce stress. Enjoy!"},
    {"reading",  "Reading expands your mind. One page at a time, you're growing!"},
    {"morning",  "A good morning routine sets the tone for the entire day!"},
    {"meditat",  "Meditation brings clarity and calm. Your mind thanks you!"}
};

// ============================================================================
// C++11: Random number generation
//   std::mt19937 is a high-quality random engine (Mersenne Twister)
//   Much better than the old C-style rand() function
// ============================================================================
static int getRandomIndex(int max_exclusive)
{
    // C++11: seed with chrono clock for unpredictable results
    static std::mt19937 rng(
        static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));

    std::uniform_int_distribution<int> dist(0, max_exclusive - 1);
    return dist(rng);
}

std::string getRandomQuote()
{
    return motivational_quotes[
        getRandomIndex(static_cast<int>(motivational_quotes.size()))];
}

std::string getAppreciationMessage()
{
    return appreciation_messages[
        getRandomIndex(static_cast<int>(appreciation_messages.size()))];
}

// ============================================================================
// Get a real-world benefit message based on the routine's category
//   C++17: Structured bindings [key, message] unpack the pair cleanly
// ============================================================================
std::string getBenefitMessage(const std::string& category)
{
    std::string lower_cat = category;
    std::transform(lower_cat.begin(), lower_cat.end(),
                   lower_cat.begin(), ::tolower);

    // C++17: Structured bindings - unpack std::pair into named variables
    for(const auto& [key, message] : benefit_messages)
    {
        if(lower_cat.find(key) != std::string::npos)
        {
            return message;
        }
    }

    return "Great job completing this routine! Every step counts!";
}

}  // namespace Quotes
