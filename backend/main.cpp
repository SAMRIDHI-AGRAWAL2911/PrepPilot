#ifdef _WIN32
  #define _WIN32_WINNT 0x0A00 // Targets modern Windows 10+ environments
#endif

#include "httplib.h"
#include "models.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>

std::vector<Problem> problems_db;
int global_id_counter = 1;

// Refactored Phase 2 Compute Layout Matrix Engine
DashboardStats compute_telemetry() {
    DashboardStats stats;
    stats.total_solved = problems_db.size();

    if (problems_db.empty()) return stats;

    std::unordered_map<std::string, int> topic_time_map;
    std::unordered_map<std::string, int> topic_count_map;
    double total_confidence_accumulator = 0.0;

    for (const auto& prob : problems_db) {
        if (prob.difficulty == "Easy") stats.easy++;
        else if (prob.difficulty == "Medium") stats.medium++;
        else if (prob.difficulty == "Hard") stats.hard++; // Handles critical Hard counter tracking

        topic_time_map[prob.topic] += prob.time_taken;
        topic_count_map[prob.topic]++;
        total_confidence_accumulator += prob.confidence;
    }

    stats.avg_confidence = total_confidence_accumulator / problems_db.size();

    double worst_average_overhead = 0.0;
    for (const auto& pair : topic_count_map) {
        double avg_time = static_cast<double>(topic_time_map[pair.first]) / pair.second;
        if (avg_time > worst_average_overhead) {
            worst_average_overhead = avg_time;
            stats.weak_topic = pair.first;
        }
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
    httplib::Server svr;

    auto set_cors_headers = [](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };

    svr.Options(R"(.*)", [set_cors_headers](const httplib::Request&, httplib::Response& res) {
        set_cors_headers(res);
        res.status = 200;
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

    // API: POST /problems (Includes Multi-Platform Redundancy Resolution Verification)
    svr.Post("/problems", [set_cors_headers](const httplib::Request& req, httplib::Response& res) {
        set_cors_headers(res);
        std::string body = req.body;

        std::string target_name = get_json_value(body, "name");
        std::string incoming_platform = get_json_value(body, "platform");

        // Algorithmic Scan: Check if problem exists in database vector register
        auto it = std::find_if(problems_db.begin(), problems_db.end(), [&](const Problem& p) {
            return p.name == target_name;
        });

        if (it != problems_db.end()) {
            // Found duplicate match! Append the new platform if it isn't listed yet
            if (it->platform.find(incoming_platform) == std::string::npos) {
                it->platform += ", " + incoming_platform;
            }
            // Update parameters with the latest submission values
            it->status = get_json_value(body, "status");
            it->notes = get_json_value(body, "notes");
            it->mistakes = get_json_value(body, "mistakes");
            
            std::string raw_conf = get_json_value(body, "confidence");
            if (!raw_conf.empty()) it->confidence = std::stoi(raw_conf);

            std::string raw_time = get_json_value(body, "time_taken");
            if (!raw_time.empty()) it->time_taken += std::stoi(raw_time); // Accumulate duration overhead

            res.status = 200;
            res.set_content("{\"message\":\"Problem registration updated with cross-platform indicators.\"}", "application/json");
        } else {
            // Problem is distinct. Process full instantiation entry pipeline maps
            Problem new_problem;
            new_problem.id = global_id_counter++;
            new_problem.user_id = 1;
            new_problem.name = target_name;
            new_problem.platform = incoming_platform;
            new_problem.difficulty = get_json_value(body, "difficulty");
            new_problem.topic = get_json_value(body, "topic");
            new_problem.pattern = get_json_value(body, "pattern");
            new_problem.status = get_json_value(body, "status");
            new_problem.link = get_json_value(body, "link");
            new_problem.notes = get_json_value(body, "notes");
            new_problem.mistakes = get_json_value(body, "mistakes");
            new_problem.time_comp = get_json_value(body, "time_comp");
            new_problem.space_comp = get_json_value(body, "space_comp");
            
            std::string raw_conf = get_json_value(body, "confidence");
            new_problem.confidence = raw_conf.empty() ? 3 : std::stoi(raw_conf);

            std::string raw_time = get_json_value(body, "time_taken");
            new_problem.time_taken = raw_time.empty() ? 0 : std::stoi(raw_time);

            problems_db.push_back(new_problem);
            res.status = 201;
        }
        res.set_content("{\"message\":\"Data processing successful.\"}", "application/json");
    });

    // API: GET /stats
    svr.Get("/stats", [set_cors_headers](const httplib::Request&, httplib::Response& res) {
        set_cors_headers(res);
        DashboardStats metrics = compute_telemetry();
        
        std::stringstream ss;
        ss << "{\"total_solved\":" << metrics.total_solved
           << ",\"easy\":" << metrics.easy
           << ",\"medium\":" << metrics.medium
           << ",\"hard\":" << metrics.hard // Deliver hard value metrics
           << ",\"avg_confidence\":" << metrics.avg_confidence
           << ",\"weak_topic\":\"" << metrics.weak_topic << "\"}";

        res.set_content(ss.str(), "application/json");
    });

    std::cout << "PrepPilot Duplication Filter Engine online at http://localhost:8080 🚀" << std::endl;
    svr.listen("0.0.0.0", 8080);
}