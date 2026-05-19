#include "quotes.h"
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <utility>

namespace Spark
{

static const std::vector<std::string> motivators = {
    "\"Remember RCB waited for 18 years to win the trophy.\" - Keshavamurthy",
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

static const std::vector<std::string> praises = {
    "Awesome job! You crushed it!",
    "Fantastic! Keep that momentum going!",
    "You're on fire! Another one done!",
    "Champion move! Well done!",
    "Bullseye! That's how it's done!",
    "Excellent work! You're making great progress!",
    "Unstoppable! Keep reaching for the stars!",
    "Way to go! You should be proud!"
};

static const std::vector<std::pair<std::string, std::string>> perks_map = {
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

static int rollIndex(int max_exclusive)
{
    static std::mt19937 rng(
        static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));

    std::uniform_int_distribution<int> dist(0, max_exclusive - 1);
    return dist(rng);
}

std::string pickMotivator()
{
    return motivators[rollIndex(static_cast<int>(motivators.size()))];
}

std::string pickPraise()
{
    return praises[rollIndex(static_cast<int>(praises.size()))];
}

std::string pickPerks(const std::string& category)
{
    std::string lower_cat = category;
    std::transform(lower_cat.begin(), lower_cat.end(),
                   lower_cat.begin(), ::tolower);

    for(const auto& [key, message] : perks_map)
    {
        if(lower_cat.find(key) != std::string::npos)
            return message;
    }

    return "Great job completing this routine! Every step counts!";
}

}
