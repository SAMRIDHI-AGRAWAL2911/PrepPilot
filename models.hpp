#pragma once
#include <string>

// Schema structure mapping authenticated user credentials [cite: 230]
struct User {
    int id;
    std::string name;
    std::string email;
    std::string password_hash; // Hashed security credentials [cite: 91, 234]
};

struct Problem {
    int id;
    int user_id; // Relational foreign key binding [cite: 240]
    std::string name;
    std::string platform;
    std::string difficulty;
    std::string topic;
    std::string pattern;
    std::string status;
    std::string link;
    std::string notes;
    std::string mistakes;
    std::string time_comp;
    std::string space_comp;
    int confidence; // Stored natively as 1-5 scalar, rendered as interactive stars on UI
    int time_taken;
};

struct DashboardStats {
    int total_solved = 0;
    int easy = 0;
    int medium = 0;
    int hard = 0;
    double avg_confidence = 0.0;
};