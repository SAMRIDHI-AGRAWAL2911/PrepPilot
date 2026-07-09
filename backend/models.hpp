#pragma once
#include <string>

struct User {
    int id;
    std::string name;
    std::string email;
    std::string password_hash;
};

// Expanded Phase 2 structural blueprint mapping interview metrics 
struct Problem {
    int id;
    int user_id;
    std::string name;        // e.g., "Two Sum" [cite: 491]
    std::string platform;    // LeetCode, GFG, Codeforces, etc. [cite: 467, 480]
    std::string difficulty;  // Easy, Medium, Hard [cite: 464, 477]
    std::string topic;       // Arrays, DP, Graphs, etc. [cite: 465, 478]
    std::string pattern;     // e.g., "Two Pointers", "Sliding Window" [cite: 466, 479]
    std::string status;      // Solved, Attempted, Not Started [cite: 468, 482]
    std::string link;        // Problem URL [cite: 481]
    std::string notes;       // General mental notes [cite: 483, 499]
    std::string mistakes;    // Corner cases missed [cite: 500]
    std::string time_comp;   // e.g., "O(N)" [cite: 501]
    std::string space_comp;  // e.g., "O(1)" [cite: 502]
    int confidence;          // Scale 1-5 [cite: 484, 503]
    int time_taken;          // Duration in minutes [cite: 485]
};

struct DashboardStats {
    int total_solved = 0;
    int easy = 0;
    int medium = 0;
    int hard = 0;
    double avg_confidence = 0.0; // Added for Module 6 requirement [cite: 511]
    std::string weak_topic = "None";
};