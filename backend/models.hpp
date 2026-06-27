#pragma once
#include <string>

// Blueprint mapping user data properties safely in memory [cite: 230]
struct User {
    int id;
    std::string name;
    std::string email;
    std::string password_hash;
};

// Blueprint for the tracked problem matrix schema [cite: 238]
struct Problem {
    int id;
    int user_id;
    std::string name;
    std::string platform;    // LeetCode, GFG, Codeforces, CodeStudio, etc. [cite: 2]
    std::string difficulty;  // Easy, Medium, Hard [cite: 320]
    std::string topic;       // Arrays, DP, Graphs, etc. [cite: 130, 140, 141]
    std::string status;      // Solved, Attempted, Not Started [cite: 160, 161, 162]
    int time_taken;          // Overhead processing duration metric in minutes [cite: 164]
};

// Container payload structure for compiling analytics telemetry to send to dashboard [cite: 204]
struct DashboardStats {
    int total_solved = 0;
    int easy = 0;
    int medium = 0;
    int hard = 0;
    std::string weak_topic = "None";
};