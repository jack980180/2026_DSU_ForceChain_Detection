#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct ParticleData {
    int id;
    double x, y, r;
    int is_spanning;
};

void print_usage() {
    std::cout << "Usage: ./bin/plotter -i <input.csv> [-o img/output.svg] [-w width] [-y height]\n";
}

int main(int argc, char* argv[]) {
    std::string input_csv = "";
    std::string output_svg = "img/particles_plot.svg"; // <-- Changed default path
    double width = 200.0;
    double height = 100.0;

    // 1. Parse Arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-i" && i + 1 < argc) input_csv = argv[++i];
        else if (arg == "-o" && i + 1 < argc) output_svg = argv[++i];
        else if (arg == "-x" && i + 1 < argc) width = std::stod(argv[++i]);
        else if (arg == "-y" && i + 1 < argc) height = std::stod(argv[++i]);
        else if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        }
    }

    if (input_csv.empty()) {
        std::cerr << "Error: Input CSV is required. Use -i <file.csv>\n";
        print_usage();
        return 1;
    }

    // Redirect input csv file to results/, svg file to img/
    if (input_csv.find('/') == std::string::npos) {
        input_csv = "results/" + input_csv;
    }
    if (output_svg.find('/') == std::string::npos) {
        output_svg = "img/" + output_svg;
    }

    // 2. Read CSV
    std::vector<ParticleData> particles;
    std::ifstream file(input_csv);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << input_csv << "\n";
        return 1;
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        ParticleData p;

        std::getline(ss, token, ','); p.id = std::stoi(token);
        std::getline(ss, token, ','); p.x = std::stod(token);
        std::getline(ss, token, ','); p.y = std::stod(token);
        std::getline(ss, token, ','); p.r = std::stod(token);

        // Read the 5th column (default to 0 if it fails for some reason)
        if (std::getline(ss, token, ',')) {
            p.is_spanning = std::stoi(token);
        } else {
            p.is_spanning = 0;
        }

        particles.push_back(p);
    }
    file.close();

    // 3. Generate SVG File
    std::ofstream svg(output_svg);
    if (!svg.is_open()) {
        std::cerr << "Error: Could not create output file " << output_svg << "\n";
        return 1;
    }

    // Write SVG Header
    svg << "<svg width=\"" << width << "\" height=\"" << height 
        << "\" viewBox=\"0 0 " << width << " " << height 
        << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    
    // Draw Background (White)
    svg << "  <rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";

    // Draw Particles
    for (const auto& p : particles) {
        // Red if it's in the force chain, Blue if it's a normal particle
        std::string fill_color = p.is_spanning ? "rgba(255, 50, 50, 0.8)" : "rgba(0, 150, 255, 0.3)";
        std::string stroke_color = p.is_spanning ? "darkred" : "black";
        std::string stroke_width = p.is_spanning ? "1.5" : "0.5";

        svg << "  <circle cx=\"" << p.x << "\" cy=\"" << (height - p.y)
            << "\" r=\"" << p.r
            << "\" fill=\"" << fill_color
            << "\" stroke=\"" << stroke_color
            << "\" stroke-width=\"" << stroke_width << "\"/>\n";
    }

    // Write SVG Footer
    svg << "</svg>\n";
    svg.close();

    std::cout << "Successfully rendered " << particles.size() << " particles to " << output_svg << "\n";
    return 0;
}