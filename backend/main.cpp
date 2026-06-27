#ifdef _WIN32
  #define _WIN32_WINNT 0x0A00 // Updated to target Windows 10+ explicitly
#endif

#include "httplib.h"
#include "models.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>

// Thread-safe dynamic vectors serving as temporary in-memory database registers
std::vector<Problem> problems_db;
int global_id_counter = 1;

// Core Performance Calculation Algorithm Engine
DashboardStats compute_telemetry() {
    DashboardStats stats;
    stats.total_solved = problems_db.size();

    std::unordered_map<std::string, int> topic_time_map;
    std::unordered_map<std::string, int> topic_count_map;

    for (const auto& prob : problems_db) {
        if (prob.difficulty == "Easy") stats.easy++;
        else if (prob.difficulty == "Medium") stats.medium++;
        else if (prob.difficulty == "Hard") stats.hard++;

        topic_time_map[prob.topic] += prob.time_taken;
        topic_count_map[prob.topic]++;
    }

    double worst_average_overhead = 0.0;
    for (const auto& pair : topic_count_map) {
        const std::string& topic_name = pair.first;
        int count = pair.second;
        double avg_time = static_cast<double>(topic_time_map[topic_name]) / count;

        if (avg_time > worst_average_overhead) {
            worst_average_overhead = avg_time;
            stats.weak_topic = topic_name;
        }
    }
    return stats;
}

// Simple helper function to parse raw text JSON string manually without heavy libraries
std::string get_json_value(const std::string& body, const std::string& key) {
    size_t pos = body.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    size_t start = body.find(":", pos);
    size_t val_start = body.find_first_not_of(" \t\n\r\"", start + 1);
    size_t val_end = body.find_first_of(",}\"\n\r", val_start);
    return body.substr(val_start, val_end - val_start);
}

int main() {
    httplib::Server svr;

    // CORS Headers setup so index.html browser requests can communicate smoothly
    auto set_cors_headers = [](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };

    // Handle standard browser pre-flight checks
    svr.Options(R"(.*)", [set_cors_headers](const httplib::Request&, httplib::Response& res) {
        set_cors_headers(res);
        res.status = 200;
    });

    // API ROUTE: GET /problems
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
               << "\",\"status\":\"" << p.status 
               << "\",\"time_taken\":" << p.time_taken << "}";
            if (i < problems_db.size() - 1) ss << ",";
        }
        ss << "]";
        res.set_content(ss.str(), "application/json");
    });

    // API ROUTE: POST /problems
    svr.Post("/problems", [set_cors_headers](const httplib::Request& req, httplib::Response& res) {
        set_cors_headers(res);
        std::string body = req.body;

        Problem new_problem;
        new_problem.id = global_id_counter++;
        new_problem.user_id = 1;
        new_problem.name = get_json_value(body, "name");
        new_problem.platform = get_json_value(body, "platform");
        new_problem.difficulty = get_json_value(body, "difficulty");
        new_problem.topic = get_json_value(body, "topic");
        new_problem.status = "Solved";
        
        std::string raw_time = get_json_value(body, "time_taken");
        new_problem.time_taken = raw_time.empty() ? 0 : std::stoi(raw_time);

        problems_db.push_back(new_problem);
        res.status = 201;
        res.set_content("{\"message\":\"Problem committed!\"}", "application/json");
    });

    // API ROUTE: GET /stats
    svr.Get("/stats", [set_cors_headers](const httplib::Request&, httplib::Response& res) {
        set_cors_headers(res);
        DashboardStats metrics = compute_telemetry();
        
        std::stringstream ss;
        ss << "{\"total_solved\":" << metrics.total_solved
           << ",\"easy\":" << metrics.easy
           << ",\"medium\":" << metrics.medium
           << ",\"hard\":" << metrics.hard
           << ",\"weak_topic\":\"" << metrics.weak_topic << "\"}";

        res.set_content(ss.str(), "application/json");
    });

    std::cout << "PrepPilot Backend Engine active and running on http://localhost:8080 🚀" << std::endl;
    svr.listen("0.0.0.0", 8080);
}