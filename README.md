# PrepPilot: The DSA Hub

PrepPilot is a high-performance, centralized tracking platform and ledger designed to streamline and monitor Data Structures and Algorithms (DSA) interview preparation. Instead of scattering progress across multiple coding platforms, PrepPilot aggregates your solved problems, monitors difficulty distributions, tracks performance analytics, and pinpoints algorithmic bottlenecks to optimize your technical interview study strategy.

---

## Features (Implemented So Far)

*   **Centralized DSA Ledger:** Log, update, and track coding problems across various platforms with custom metrics.
*   **Robust User Login & Authentication Flow:** A properly engineered end-to-end authentication system backed by functional C++ server endpoints and validation logic ensuring secure account registration and gated dashboard access.
*   **Web-Native Notification Matrix:** Completely eliminates disruptive native system browser popups by utilizing a custom, built-in toast notification layout running clean UI status transitions.
*   **Smart Autocomplete Query Processor:** Scans input sub-strings against an in-memory database of curated starter placement questions to automate tracking configurations and reduce friction.
*   **Cross-Platform Deduplication Matrix:** Normalizes incoming lists dynamically across LeetCode, GeeksforGeeks, NeetCode, and Codeforces, merging historical duplicates into a single-row registry.
*   **Dynamic Selection Complexity Registry:** Eliminates character input clutter by tracking algorithms inside bounded dropdown arrays, supporting real-time addition of custom notations safely.
*   **High-Performance Backend Engine:** Built entirely in C++ to minimize latency and handle rapid analytical requests.
*   **Interactive Star Ratings & Masked Input Controls:** Features a premium UI dashboard with clickable star ratings and secure password fields containing functional text/hidden visibility toggles.
*   **Clean REST API Interface:** Lightweight JSON-based endpoints communicating natively over HTTP.
---

## Tech Stack

* **Backend Core:** C++ (Modern C++17/C++20 standards)
* **Networking/HTTP Server:** `cpp-httplib` (A header-only, multi-threaded HTTP library)
* **Data Serialization:** Custom JSON integration for seamless request/response payloads
* **Frontend UI Layout:** HTML5, CSS3, and Tailwind CSS (Asynchronous Web Fetch API)
* **Default Port:** `8080`

---

## 📊 Evolutionary Iteration Blueprint

### v1.0 — The Basic Ledger Setup (Core CRUD)
* Established fundamental network routing configurations using standard template headers.
* Implemented temporary, volatile thread-safe in-memory array tracking registers (`std::vector`).
* Developed interface anchors managing basic entries: title inputs, basic duration logs, and primary difficulty trackers.

### v2.0 — Cross-Platform Telemetry Integration
* Refactored core memory schemas to hold tracking metadata elements (Big-O properties, mistake logs).
* Programmed a data optimization loop running linear scanning search checks (`std::find_if`) with custom predicate lambdas to catch double entries and merge cross-platform progress.
* Created multi-field display inspectors revealing detailed analytical statistics blocks on element selection triggers.

### v3.0 — The Secure Smart Dashboard
* Pre-loaded an internal dictionary mapping top curated placement DSA targets to instantiate active lookups immediately.
* Structured token validations across display paths to protect dashboard components, alongside browser autofill guards to secure form data.
* Upgraded metrics widgets to process confidence tracking calculations natively as star strings instead of raw integers.

### v4.0 — Integrated Web-Native UX & Production-Ready Authentication Engine (Current Release)
*   **Fully-Engineered Login & Authentication Flow:** Built a robust, end-to-end authentication matrix, establishing completely functional backend endpoints and rigorous validation logic to handle secure user registration and portal access properly.
*   **Web-Native Notification Matrix:** Completely stripped out disruptive native system browser `alert()` popups. Engineered a custom, built-in toast notification system directly into the website layout using Tailwind CSS transitions for smooth, self-dismissing status feedback.
*   **Unified Side-by-Side Authentication Portal:** Re-architected the access framework by merging the `/login` and `/register` client-side workflows into a single, cohesive, tab-switched authentication card component.
*   **Interactive Password Visibility Controls:** Built responsive input field mask filters (`👁️` / `🙈`) into both authentication panels to prevent typing mistakes and secure local screen space.
*   **Asynchronous State Routing Synchronization:** Calibrated frontend JavaScript execution loops to handle backend network responses smoothly—delaying app view transitions until built-in toast alerts finish projecting so the user experience is flawless.
*   **Form Lifecycle State Reset Guards:** Integrated strict layout scrubbing filters that clean all input buffers, unlock dynamic form fields, and reset confidence ratings back to standard matrices when changing tracking views.

---

## Project Structure

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
└── README.md                  # System documentation
