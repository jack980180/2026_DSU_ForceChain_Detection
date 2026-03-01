#include <iostream>
#include <string>
#include "SimulationDriver.h"

// --- Function Prototypes ---
void print_usage();

int main(int argc, char* argv[]) {
    // 1. Default Parameters
    double WIDTH  = 200.0;
    double HEIGHT = 100.0;
    double RADIUS = 2.0;
    double TARGET_PHI = 0.6;
    double FORCE_FACTOR = 1.0; // NEW: Default contact factor (1.0 = exactly touching)

    // Execution Flags & Files
    bool only_generate = false;
    std::string input_csv = "";
    std::string output_csv = "results/relaxed_particles.csv";

    // 2. Parse Command Line Arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if (arg == "-g") {
            only_generate = true;
        } else if (arg == "-i" && i + 1 < argc) {
            input_csv = argv[++i];
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            output_csv = argv[++i];
        } else if ((arg == "-x" || arg == "--width") && i + 1 < argc) {
            WIDTH = std::stod(argv[++i]);
        } else if ((arg == "-y" || arg == "--height") && i + 1 < argc) {
            HEIGHT = std::stod(argv[++i]);
        } else if (arg == "-phi" && i + 1 < argc) {
            TARGET_PHI = std::stod(argv[++i]);
        } else if (arg == "-r" && i + 1 < argc) {
            RADIUS = std::stod(argv[++i]);
        } else if ((arg == "-f" || arg == "--force") && i + 1 < argc) { // <-- NEW FORCE FLAG
            FORCE_FACTOR = std::stod(argv[++i]);
        } else {
            std::cerr << "Unknown or incomplete argument: " << arg << "\n";
            std::cerr << "Type './bin/sim_engine -h' for usage information.\n";
            return 1;
        }
    }

    // Redirect result csv file to results/
    if (output_csv.find('/') == std::string::npos) {
        output_csv = "results/" + output_csv;
    }
    if (!input_csv.empty() && input_csv.find('/') == std::string::npos) {
        input_csv = "results/" + input_csv;
    }

    // Create SimulationDriver object to run the simulation
    SimulationDriver driver(WIDTH, HEIGHT, RADIUS, TARGET_PHI);

    // 2. Generation / Input Branch
    if (!input_csv.empty()) {
        if (!driver.load_particles_from_csv(input_csv)) {
            std::cerr << "Failed to load CSV.\n";
            return 1;
        }
    } else {
        driver.initialize_and_relax();

        if (only_generate) {
            driver.save_particles_to_csv(output_csv);
            std::cout << "Generation complete. Exiting.\n";
            return 0;
        }
    }

    // 3. Analysis Phase
    std::cout << "Scanning for force chains with factor: " << FORCE_FACTOR << "...\n";
    driver.perform_connectivity_scan(FORCE_FACTOR); // Pass the factor to the scan!
    driver.print_results();

    // Save the CSV AFTER the scan so the force chain data (column 5) is included!
    if (!only_generate) {
        driver.save_particles_to_csv(output_csv);
    }

    return 0;
}

// --- Function Implementations ---

void print_usage() {
    std::cout << "\n========================================================\n";
    std::cout << " Force Chain Scanner & Granular Simulation Engine\n";
    std::cout << "========================================================\n";
    std::cout << "Usage: ./bin/sim_engine [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help           Show this help message and exit\n";
    std::cout << "  -g                   Run generation & relaxation only (exports to CSV, skips scan)\n";
    std::cout << "  -i <file.csv>        Load particles from CSV (skips generation & relaxation)\n";
    std::cout << "  -o, --output <file>  Set output CSV filename (default: results/relaxed_particles.csv)\n";
    std::cout << "  -x, --width <width>  Set simulation box width (default: 200.0)\n";
    std::cout << "  -y, --height <height> Set simulation box height (default: 100.0)\n";
    std::cout << "  -phi <fraction>      Set target packing fraction (default: 0.6)\n";
    std::cout << "  -r <radius>          Set particle radius (default: 2.0)\n";
    std::cout << "  -f, --force <factor> Set contact distance threshold factor (default: 1.0)\n";
    std::cout << "\nExamples:\n";
    std::cout << "  ./bin/sim_engine -x 500 -y 200 -phi 0.85\n";
    std::cout << "  ./bin/sim_engine -i jammed.csv -f 0.8 -o strong_network.csv\n";
    std::cout << "========================================================\n\n";
}