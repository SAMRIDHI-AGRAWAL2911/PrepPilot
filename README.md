# PrepPilot: The DSA Hub

PrepPilot is a high-performance, centralized tracking platform and ledger designed to streamline and monitor Data Structures and Algorithms (DSA) interview preparation. Instead of scattering progress across multiple coding platforms, PrepPilot aggregates your solved problems, monitors difficulty distributions, tracks performance analytics, and pinpoints algorithmic bottlenecks to optimize your technical interview study strategy.

## Features (Implemented So Far)

*   **Centralized DSA Ledger:** Log, update, and track coding problems across various platforms with custom metrics.
*   **High-Performance Backend Engine:** Built entirely in C++ to minimize latency and handle rapid analytical requests.
*   **Difficulty & Topic Mapping:** Categorize problems by difficulty (Easy, Medium, Hard) and specific algorithmic patterns (e.g., Bit Manipulation, Dynamic Programming, Graph Theory).
*   **Performance Analytics:** Internal tracking logic to identify areas requiring reinforcement and measure progress over time.
*   **Clean REST API Interface:** Lightweight JSON-based endpoints communicating natively over HTTP.

## Tech Stack

*   **Backend Core:** C++ (Modern C++17/C++20 standards)
*   **Networking/HTTP Server:** `cpp-httplib` (A header-only, multi-threaded HTTP library)
*   **Data Serialization:** JSON integration for seamless request/response payloads
*   **Default Port:** `8080`

```text
├── backend/
│   ├── httplib.h              # Single-header multi-threaded HTTP library
│   ├── main.cpp               # Server entry point, routing, and logic
│   ├── models.hpp             # DSA problem structures and analytics data models
│   ├── models.exe             # Compiled binary artifact
│   └── preppilot_backend.exe  # Main backend engine executable
│
├── Frontend/
│   └── index.html             # Client-facing analytics dashboard UI
│
└── README.md
