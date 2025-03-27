// clothoid_simulator.cpp
#include <iostream>
#include <cmath>
#include <vector>
#include "Clothoids.hh"
#include "Clothoids_fmt.hh"

using namespace G2lib;

struct CarState {
    double x, y, theta;
};

void plot_simulation_interactive(const std::vector<CarState>& path, 
                                 const CarState& start, 
                                 const CarState& end) {
    FILE* gnuplot = popen("gnuplot -persist", "w");
    if (!gnuplot) {
        std::cerr << "Error: Could not open Gnuplot\n";
        return;
    }

    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "set size ratio -1\n");
    fprintf(gnuplot, "plot '-' with lines lc 'green' title 'Path', "
                     "'-' with points pt 7 ps 2 lc 'blue' title 'Start', "
                     "'-' with points pt 7 ps 2 lc 'red' title 'End'\n");

    for (const auto& p : path) {
        fprintf(gnuplot, "%f %f\n", p.x, p.y);
    }
    fprintf(gnuplot, "e\n");

    fprintf(gnuplot, "%f %f\n", start.x, start.y);
    fprintf(gnuplot, "e\n");

    fprintf(gnuplot, "%f %f\n", end.x, end.y);
    fprintf(gnuplot, "e\n");

    fflush(gnuplot);
}

void plot_simulation(const std::vector<CarState>& path, 
                    const CarState& start, 
                    const CarState& end) {
    // Generate GNUplot commands
    std::cout << "set term pngcairo size 1280,960\n";
    std::cout << "set output 'clothoid_sim.png'\n";
    std::cout << "set grid\n";
    std::cout << "set size ratio -1\n";
    std::cout << "plot '-' with lines lc 'green' title 'Path', "
              << "'-' with points pt 7 ps 2 lc 'blue' title 'Start', "
              << "'-' with points pt 7 ps 2 lc 'red' title 'End'\n";
    
    // Path data
    for (const auto& p : path) {
        std::cout << p.x << " " << p.y << "\n";
    }
    std::cout << "e\n";
    
    // Start point
    std::cout << start.x << " " << start.y << "\n";
    std::cout << "e\n";
    
    // End point
    std::cout << end.x << " " << end.y << "\n";
    std::cout << "e\n";
}

int main() {
    // 1. Define start and end points with headings
    CarState start{2.0, 0.5, M_PI/4};  // x, y, heading (rad)
    CarState end{5.0, 3.0, -M_PI/6};    // x, y, heading (rad)
    
    // 2. Create clothoid between points
    ClothoidCurve clothoid{""};
    clothoid.build_G1(start.x, start.y, start.theta,
                    end.x, end.y, end.theta);
    
    // 3. Simulate car movement
    const double velocity = 1.0; // m/s
    const double dt = 0.1;       // time step
    const double total_time = clothoid.length() / velocity;
    
    std::vector<CarState> path;
    
    for(double t = 0; t <= total_time; t += dt) {
        double s = velocity * t;
        double x, y, theta, kappa;
        clothoid.evaluate(s, theta, kappa, x, y);
        std::cout << " | s:" << s << " | theta:" << theta << " | kappa:" << kappa << " | x:" << x << "r| y:" << y << std::endl;
        
        path.push_back({x, y, theta});
        
        // Print car state (debug)
        std::cout << "Time: " << t 
                  << " | Position: (" << x << ", " << y
                  << ") | Heading: " << theta * 180/M_PI << "°\n";
    }
    
    // 4. Generate visualization
    plot_simulation_interactive(path, start, end);
    
    return 0;
}
