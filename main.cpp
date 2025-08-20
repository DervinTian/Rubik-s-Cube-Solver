#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <random>
#include <chrono>

#include "Cube.h"

using namespace std;

// Dictionary {green : 2, white : 3, red : 5, blue : 4, yellow : 6, orange : 10}

int main(int argc, char* argv[]){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    vector<Edge> edges;
    vector<Corner> corners;
    vector<vector<uint32_t>> faces;
    string scramble;
    // unordered_map<uint64_t, uint32_t> G0_table;
    unordered_map<uint64_t, uint32_t> G0_table;
    unordered_map<uint64_t, uint32_t> G1_map;
    unordered_map<uint64_t, uint32_t> G2_map;
    unordered_map<uint64_t, uint32_t> G3_map;

    cout << "Reading in Data...\n";
    auto start = std::chrono::high_resolution_clock::now();

    if(argc < 1){
        cerr << "Not enough arguments\n";
        exit(0);
    }

    string filename = argv[1];
    ifstream ifstream(filename);

    scramble = argv[2];

    read_in(ifstream, faces, G0_table, G1_map, G2_map, G3_map);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    initialize_solved_state(edges, corners);
    cout << "Data read in " << duration.count() << " ms\n";

    Cube cube = Cube(edges, corners);
    cout << "Scramble: " << scramble << '\n';
    cube.algorithm(scramble);
    // cube.print_cube_state();
    cout << '\n';

    // start = std::chrono::high_resolution_clock::now();
    // string G0_solution = cube.G0(G0_table);
    // end = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // cout << "The Solution for G0 is: " << G0_solution << '\n';
    // cube.print_cube_state();
    // cout << "Calculated in: " << duration.count() << " ms\n";
    // cout << '\n';

    // // cube.algorithm("R D L2 U F U");
    // // cube.print_cube_state();
    // // cout << '\n';

    // start = std::chrono::high_resolution_clock::now();
    // string G1_solution = cube.G1(G1_map);
    // end = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // cout << "The Solution for G1 is: " << G1_solution << '\n';
    // cube.print_cube_state();
    // cout << "Calculated in: " << duration.count() << " ms\n";
    // cout << '\n';

    // start = std::chrono::high_resolution_clock::now();
    // string G2_solution = cube.G2(G2_map);
    // end = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // cout << "The Solution for G2 is: " << G2_solution << '\n';
    // cube.print_cube_state();
    // cout << "Calculated in: " << duration.count() << " ms\n";
    // cout << '\n';

    // start = std::chrono::high_resolution_clock::now();
    // string G3_solution = cube.G3(G3_map);
    // end = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // cout << "The Solution for G3 is: " << G3_solution << '\n';
    // cube.print_cube_state();
    // cout << "Calculated in: " << duration.count() << " ms\n";
    // cout << '\n';

    start = std::chrono::high_resolution_clock::now();
    string solution = cube.Thistlethwaite(G0_table, G1_map, G2_map, G3_map);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "The Solution is: " << solution << '\n';
    cout << "Calculated in: " << duration.count() << " ms\n";
    cout << '\n';

    cout << "Done!" << '\n';
}