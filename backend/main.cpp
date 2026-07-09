#ifdef _WIN32
  #define _WIN32_WINNT 0x0A00 
#endif

#include "httplib.h"
#include "models.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

std::vector<Problem> problems_db;
int global_id_counter = 1;

void seed_starter_catalog() {
    problems_db.push_back({global_id_counter++, 1, "Two Sum", "", "Easy", "Arrays", "Two Pointers", "Not Started", "https://leetcode.com/problems/two-sum/", "", "", "O(N)", "O(N)", 0, 0});
    problems_db.push_back({global_id_counter++, 1, "Reverse Linked List", "", "Easy", "Linked List", "Iterative", "Not Started", "https://leetcode.com/problems/reverse-linked-list/", "", "", "O(N)", "O(1)", 0, 0});
    problems_db.push_back({global_id_counter++, 1, "Longest Common Subsequence", "", "Hard", "DP", "Grid DP", "Not Started", "https://leetcode.com/problems/longest-common-subsequence/", "", "", "O(N*M)", "O(N*M)", 0, 0});
    problems_db.push_back({global_id_counter++, 1, "Number of Islands", "", "Medium", "Graph", "DFS/BFS", "Not Started", "https://leetcode.com/problems/number-of-islands/", "", "", "O(M*N)", "O(M*N)", 0, 0});
    problems_db.push_back({global_id_counter++, 1, "Valid Parentheses", "", "Easy", "Stack", "Linear Scan", "Not Started", "https://leetcode.com/problems/valid-parentheses/", "", "", "O(N)", "O(N)", 0, 0});
}

DashboardStats compute_telemetry() {
    DashboardStats stats;
    double confidence_sum = 0.0;

    for (const auto& prob : problems_db) {
        if (!prob.platform.empty()) {
            stats.total_solved++;
            if (prob.difficulty == "Easy") stats.easy++;
            else if (prob.difficulty == "Medium") stats.medium++;
            else if (prob.difficulty == "Hard") stats.hard++;
            
            confidence_sum += prob.confidence;
        }
    }
    if (stats.total_solved > 0) {
        stats.avg_confidence = confidence_sum / stats.total_solved;
    }
    return stats;
}

std::string get_json_value(const std::string& body, const std::string& key) {
    size_t pos = body.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    size_t start = body.find(":", pos);
    size_t val_start = body.find_first_not_of(" \t\n\r\"", start + 1);
    size_t val_end = body.find_first_of(",}\"\n\r", val_start);
    return body.substr(val_start, val_end - val_start);
}

int main() {
    seed_starter_catalog();
    httplib::Server svr;

    auto set_cors_headers = [](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    };

    svr.Options(R"(.*)", [set_cors_headers](const httplib::Request&, httplib::Response& res) {
        set_cors_headers(res);
        res.status = 200;
    });

    // MODULE 1: Authentication Endpoints [cite: 256]
    svr.Post("/register", [set_cors_headers](const httplib::Request& req, httplib::Response& res) {
        set_cors_headers(res);
        std::cout << "Registering user structure: " << get_json_value(req.body, "email") << std::endl;
        res.status = 201;
        res.set_content("{\"message\":\"Registration successful!\"}", "application/json");
    });

    svr.Post("/login", [set_cors_headers](const httplib::Request& req, httplib::Response& res) {
        set_cors_headers(res);
        // Returns a simulated JWT verification token signature string back to client storage [cite: 90]
        res.status = 200;
        res.set_content("{\"token\":\"mock-jwt-token-signature-xyz\",\"name\":\"Samridhi\"}", "application/json");
    });

    // API: GET /problems
    svr.Get("/problems", [set_cors_headers](const httplib::Request&, httplib::Response& res) {
        set_cors_headers(res);
        std::stringstream ss;
        ss << "[";
        for (size_t i = 0; i < problems_db.size(); ++i) {
            const auto& p = problems_db[i];
            ss << "{\"id\":" << p.id 
               << ",\"name\":\"" << p.name 
               << "\",\"platform\":\"" << p.platform 
               << "\",\"difficulty\":\"" << p.difficulty 
               << "\",\"topic\":\"" << p.topic 
               << "\",\"pattern\":\"" << p.pattern
               << "\",\"status\":\"" << p.status
               << "\",\"link\":\"" << p.link
               << "\",\"notes\":\"" << p.notes
               << "\",\"mistakes\":\"" << p.mistakes
               << "\",\"time_comp\":\"" << p.time_comp
               << "\",\"space_comp\":\"" << p.space_comp
               << "\",\"confidence\":" << p.confidence
               << ",\"time_taken\":" << p.time_taken << "}";
            if (i < problems_db.size() - 1) ss << ",";
        }
        ss << "]";
        res.set_content(ss.str(), "application/json");
    });

    // API: POST /problems
    svr.Post("/problems", [set_cors_headers](const httplib::Request& req, httplib::Response& res) {
        set_cors_headers(res);
        std::string body = req.body;
        std::string target_name = get_json_value(body, "name");
        std::string incoming_platform = get_json_value(body, "platform");
        std::string new_note = get_json_value(body, "notes");
        std::string raw_time = get_json_value(body, "time_taken");

        auto it = std::find_if(problems_db.begin(), problems_db.end(), [&](const Problem& p) {
            return p.name == target_name;
        });

        if (it != problems_db.end()) {
            if (it->platform.empty()) it->platform = incoming_platform;
            else if (it->platform.find(incoming_platform) == std::string::npos) it->platform += ", " + incoming_platform;
            
            it->status = "Solved";
            it->time_taken += raw_time.empty() ? 0 : std::stoi(raw_time);
            
            std::string raw_conf = get_json_value(body, "confidence");
            if (!raw_conf.empty()) it->confidence = std::stoi(raw_conf);

            if (!new_note.empty()) {
                if (it->notes.empty()) it->notes = "[" + incoming_platform + "]: " + new_note;
                else it->notes += "\n--------------------\n[" + incoming_platform + "]: " + new_note;
            }
            res.status = 200;
        } else {
            Problem new_problem;
            new_problem.id = global_id_counter++;
            new_problem.user_id = 1;
            new_problem.name = target_name;
            new_problem.platform = incoming_platform;
            new_problem.difficulty = get_json_value(body, "difficulty");
            new_problem.topic = get_json_value(body, "topic");
            new_problem.pattern = get_json_value(body, "pattern");
            new_problem.status = "Solved";
            new_problem.link = get_json_value(body, "link");
            new_problem.time_comp = get_json_value(body, "time_comp");
            new_problem.space_comp = get_json_value(body, "space_comp");
            new_problem.notes = new_note.empty() ? "" : "[" + incoming_platform + "]: " + new_note;
            new_problem.mistakes = get_json_value(body, "mistakes");
            
            std::string conf = get_json_value(body, "confidence");
            new_problem.confidence = conf.empty() ? 3 : std::stoi(conf);
            new_problem.time_taken = raw_time.empty() ? 0 : std::stoi(raw_time);

            problems_db.push_back(new_problem);
            res.status = 201;
        }
        res.set_content("{\"status\":\"success\"}", "application/json");
    });

    // API: GET /stats
    svr.Get("/stats", [set_cors_headers](const httplib::Request&, httplib::Response& res) {
        set_cors_headers(res);
        DashboardStats metrics = compute_telemetry();
        std::stringstream ss;
        ss << "{\"total_solved\":" << metrics.total_solved
           << ",\"easy\":" << metrics.easy
           << ",\"medium\":" << metrics.medium
           << ",\"hard\":" << metrics.hard
           << ",\"avg_confidence\":" << metrics.avg_confidence << "}";
        res.set_content(ss.str(), "application/json");
    });

    std::cout << "PrepPilot Secured Engine active on http://localhost:8080 🚀" << std::endl;
    svr.listen("0.0.0.0", 8080);
}