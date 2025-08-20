#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <random>
#include <queue>
#include <stack>

#include "Cube.h"

// Dictionary {green : 2, white : 3, red : 5, blue : 7, yellow : 11, orange : 13}

// Read in the color of the faces as well as the scramble, faces not really needed that much as of now 7/19
void read_in(ifstream &cin, vector<vector<uint32_t>> &faces, unordered_map<uint64_t, uint32_t> &G0_table, unordered_map<uint64_t, uint32_t> &G1_map, unordered_map<uint64_t, uint32_t> &G2_map, unordered_map<uint64_t, uint32_t> &G3_map){

    vector<uint32_t> temp_store;
    uint32_t counter = 0;
    uint64_t y = 0;
    uint32_t x = 0;
    string temp;
    getline(cin, temp); // Skip this line (# Line)
    while(cin >> y >> x){
        G0_table.insert({y, x}); // read in each string from the line and store it in the pruning table
    }

    cin.clear(); // Clear the error flag
    getline(cin, temp); // Skip this line (# Line)

    while(cin >> y >> x){
        G1_map.insert({y, x});
    }

    cin.clear(); // Clear the error flag
    getline(cin, temp); // Skip this line (# Line)

    while(cin >> y >> x){
        G2_map.insert({y, x});
    }

    cin.clear(); // Clear the error flag
    getline(cin, temp); // Skip this line (# Line)

    while(cin >> y >> x){
        G3_map.insert({y, x});
    }

    cin.clear(); // Clear the error flag
    getline(cin, temp); // Skip this line (# Line)

    while(cin >> x){
        counter++;
        temp_store.push_back(x);
        if(counter == 9){
            counter = 0;
            faces.push_back(temp_store); // update the faces
            temp_store.clear();
        }
    }
}

// Initialize the edges and corners to be in the solved state
void initialize_solved_state(vector<Edge> &edges, vector<Corner> &corners){
    // Default read into a solved state
    vector<char> indices = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C'};
    for(size_t i = 0; i < indices.size(); ++i){ // Go through all the corners, then the edges in the ascending order
        if(i < 8){
            Corner corner;
            corner.position = indices[i];
            corners.push_back(corner); // initialize a solved corner and push it to the Corners vector
        }
        else{
            Edge edge;
            edge.position = indices[i];
            edges.push_back(edge); // initialize a solved edge and push it to the Edges vector
        }
    }
}

// Print out the state of the cube
void Cube::print_cube_state(){
    vector<char> indices = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C'};
    for(size_t i = 0; i < indices.size(); ++i){
        cout << indices[i] << " "; // print out the individual indices for each of the pieces
    }
    cout << '\n';
    for(size_t i = 0; i < corners.size(); ++i){
        cout << corners[i].position << " "; // print out the positions for each of the corners
    }
    for(size_t i = 0; i < edges.size(); ++i){
        cout << edges[i].position << " "; // print out the positions for each of the edges
    }
    cout << '\n';
    for(size_t i = 0; i < corners.size(); ++i){
        cout << corners[i].orientation << " "; // print out the orientations for each of the corners
    }
    for(size_t i = 0; i < edges.size(); ++i){
        cout << edges[i].orientation << " "; // print out the orientations for each of the edges
    }
    cout << '\n';
}

// Perform an algorithm (Hard-Coded)
void Cube::algorithm(const string &algorithm){
    stringstream alg(algorithm);
    string move;
    vector<string> moves;
    while(alg >> move){
        moves.push_back(move); // read in the algorithm string into a vector of values that we can iterate through
    }

    for(size_t i = 0; i < moves.size(); ++i){ // just apply the moves, it's kinda just hardcoded in
        if(moves[i] == "R"){
            R();
        }
        else if(moves[i] == "R2"){
            R2();
        }
        else if(moves[i] == "R'"){
            R_prime();
        }
        else if (moves[i] == "U"){
            U();
        }
        else if (moves[i] == "U2"){
            U2();
        }
        else if (moves[i] == "U'"){
            U_prime();
        }
        else if(moves[i] == "L"){
            L();
        }
        else if(moves[i] == "L2"){
            L2();
        }
        else if(moves[i] == "L'"){
            L_prime();
        }
        else if(moves[i] == "F"){
            F();
        }
        else if(moves[i] == "F2"){
            F2();
        }
        else if(moves[i] == "F'"){
            F_prime();
        }
        else if(moves[i] == "D"){
            D();
        }
        else if(moves[i] == "D2"){
            D2();
        }
        else if(moves[i] == "D'"){
            D_prime();
        }
        else if(moves[i] == "B"){
            B();
        }
        else if(moves[i] == "B2"){
            B2();
        }
        else if(moves[i] == "B'"){
            B_prime();
        }
        else{
            cout << "Unknown Move specified\n";
        }
    }
}

/*
The following functions will require some cube theory to define edge orientations and corner twists etc.
Here is the linked paper that I referenced: https://www.jaapsch.net/puzzles/thistle.htm
*/


// Perform an R Move
void Cube::R(){
    // Corner with position = 'b' gets it set equal to 'f'
    // Corner with position = 'f' gets it set equal to 'h'
    // Corner with position = 'h' gets it set equal to 'd'
    // Corner with position = 'd' gets it set equal to 'b'

    // Edge with position = '2' gets it set equal to '5'
    // Edge with position = '5' gets it set equal to 'B'
    // Edge with position = 'B' gets it set equal to '9'
    // Edge with position = '9' gets it set equal to '2'

    // Note: We do not change the orientation of the edges or the corners, as this is our definition, everything is defined to be this way (Cube Theory)

    vector<size_t> indices;
    for(size_t i = 0; i < edges.size(); ++i){
        if(edges[i].position == '2'){
            indices.push_back(i);
        }
        else if(edges[i].position == '5'){
            indices.push_back(i);
        }
        else if(edges[i].position == 'B'){
            indices.push_back(i);
        }
        else if(edges[i].position == '9'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(edges[index].position == '2'){
            edges[index].position = '5';
        }
        else if(edges[index].position == '5'){
            edges[index].position = 'B';
        }
        else if(edges[index].position == 'B'){
            edges[index].position = '9';
        }
        else if(edges[index].position == '9'){
            edges[index].position = '2';
        }
    }

    indices.clear();

    for(size_t i = 0; i < corners.size(); ++i){
        if(corners[i].position == 'b'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'f'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'h'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'd'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(corners[index].position == 'b'){
            corners[index].position = 'f';
        }
        else if(corners[index].position == 'f'){
            corners[index].position = 'h';
        }
        else if(corners[index].position == 'h'){
            corners[index].position = 'd';
        }
        else if(corners[index].position == 'd'){
            corners[index].position = 'b';
        }
    }
}

// Perform a U Move
void Cube::U(){
    // Corner with position = 'b' gets it set equal to 'a', orientation +2
    // Corner with position = 'a' gets it set equal to 'e', orientation +1
    // Corner with position = 'e' gets it set equal to 'f', orientation +2
    // Corner with position = 'f' gets it set equal to 'b', orientation +1

    // Edge with position = '1' gets it set equal to '6', orientation +1
    // Edge with position = '6' gets it set equal to '7', orientation +1
    // Edge with position = '7' gets it set equal to '5', orientation +1
    // Edge with position = '5' gets it set equal to '1', orientation +1

    vector<size_t> indices;
    for(size_t i = 0; i < edges.size(); ++i){
        if(edges[i].position == '1'){
            indices.push_back(i);
        }
        else if(edges[i].position == '6'){
            indices.push_back(i);
        }
        else if(edges[i].position == '7'){
            indices.push_back(i);
        }
        else if(edges[i].position == '5'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(edges[index].position == '1'){
            edges[index].position = '6';
            edges[index].orientation = (edges[index].orientation + 1) % 2;
        }
        else if(edges[index].position == '6'){
            edges[index].position = '7';
            edges[index].orientation = (edges[index].orientation + 1) % 2;
        }
        else if(edges[index].position == '7'){
            edges[index].position = '5';
            edges[index].orientation = (edges[index].orientation + 1) % 2;
        }
        else if(edges[index].position == '5'){
            edges[index].position = '1';
            edges[index].orientation = (edges[index].orientation + 1) % 2;
        }
    }

    indices.clear();

    for(size_t i = 0; i < corners.size(); ++i){
        if(corners[i].position == 'b'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'f'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'a'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'e'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(corners[index].position == 'b'){
            corners[index].position = 'a';
            corners[index].orientation = (corners[index].orientation + 2) % 3;
        }
        else if(corners[index].position == 'a'){
            corners[index].position = 'e';
            corners[index].orientation = (corners[index].orientation + 1) % 3;
        }
        else if(corners[index].position == 'e'){
            corners[index].position = 'f';
            corners[index].orientation = (corners[index].orientation + 2) % 3;
        }
        else if(corners[index].position == 'f'){
            corners[index].position = 'b';
            corners[index].orientation = (corners[index].orientation + 1) % 3;
        }
    }
}

// Perform a F move
void Cube::F(){
    // Corner with position = 'a' gets it set equal to 'b', orientation +2
    // Corner with position = 'b' gets it set equal to 'd', orientation +1
    // Corner with position = 'd' gets it set equal to 'c', orientation +2
    // Corner with position = 'c' gets it set equal to 'a', orientation +1

    // Edge with position = '1' gets it set equal to '2'
    // Edge with position = '2' gets it set equal to '3'
    // Edge with position = '3' gets it set equal to '4'
    // Edge with position = '4' gets it set equal to '1'

    vector<size_t> indices;
    for(size_t i = 0; i < edges.size(); ++i){
        if(edges[i].position == '1'){
            indices.push_back(i);
        }
        else if(edges[i].position == '2'){
            indices.push_back(i);
        }
        else if(edges[i].position == '3'){
            indices.push_back(i);
        }
        else if(edges[i].position == '4'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(edges[index].position == '1'){
            edges[index].position = '2';
        }
        else if(edges[index].position == '2'){
            edges[index].position = '3';
        }
        else if(edges[index].position == '3'){
            edges[index].position = '4';
        }
        else if(edges[index].position == '4'){
            edges[index].position = '1';
        }
    }

    indices.clear();

    for(size_t i = 0; i < corners.size(); ++i){
        if(corners[i].position == 'a'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'b'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'd'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'c'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(corners[index].position == 'a'){
            corners[index].position = 'b';
            corners[index].orientation = (corners[index].orientation + 2) % 3;
        }
        else if(corners[index].position == 'b'){
            corners[index].position = 'd';
            corners[index].orientation = (corners[index].orientation + 1) % 3;
        }
        else if(corners[index].position == 'd'){
            corners[index].position = 'c';
            corners[index].orientation = (corners[index].orientation + 2) % 3;
        }
        else if(corners[index].position == 'c'){
            corners[index].position = 'a';
            corners[index].orientation = (corners[index].orientation + 1) % 3;
        }
    }
}

// Perform a L move
void Cube::L(){
    // Corner with position = 'a' gets it set equal to 'c'
    // Corner with position = 'c' gets it set equal to 'g'
    // Corner with position = 'g' gets it set equal to 'e'
    // Corner with position = 'e' gets it set equal to 'a'

    // Edge with position = '4' gets it set equal to '8'
    // Edge with position = '8' gets it set equal to 'C'
    // Edge with position = 'C' gets it set equal to '6'
    // Edge with position = '6' gets it set equal to '4'

    vector<size_t> indices;
    for(size_t i = 0; i < edges.size(); ++i){
        if(edges[i].position == '4'){
            indices.push_back(i);
        }
        else if(edges[i].position == '8'){
            indices.push_back(i);
        }
        else if(edges[i].position == 'C'){
            indices.push_back(i);
        }
        else if(edges[i].position == '6'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(edges[index].position == '4'){
            edges[index].position = '8';
        }
        else if(edges[index].position == '8'){
            edges[index].position = 'C';
        }
        else if(edges[index].position == 'C'){
            edges[index].position = '6';
        }
        else if(edges[index].position == '6'){
            edges[index].position = '4';
        }
    }

    indices.clear();

    for(size_t i = 0; i < corners.size(); ++i){
        if(corners[i].position == 'a'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'c'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'g'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'e'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(corners[index].position == 'a'){
            corners[index].position = 'c';
        }
        else if(corners[index].position == 'c'){
            corners[index].position = 'g';
        }
        else if(corners[index].position == 'g'){
            corners[index].position = 'e';
        }
        else if(corners[index].position == 'e'){
            corners[index].position = 'a';
        }
    }
}

// Perform a D move
void Cube::D(){
    // Corner with position = 'c' gets it set equal to 'd', orientation +2
    // Corner with position = 'd' gets it set equal to 'h', orientation +1
    // Corner with position = 'h' gets it set equal to 'g', orientation +2
    // Corner with position = 'g' gets it set equal to 'c', orientation +1

    // Edge with position = '3' gets it set equal to '9', orientation +1
    // Edge with position = '9' gets it set equal to 'A', orientation +1
    // Edge with position = 'A' gets it set equal to '8', orientation +1
    // Edge with position = '8' gets it set equal to '3', orientation +1

    vector<size_t> indices;
    for(size_t i = 0; i < edges.size(); ++i){
        if(edges[i].position == '3'){
            indices.push_back(i);
        }
        else if(edges[i].position == '9'){
            indices.push_back(i);
        }
        else if(edges[i].position == 'A'){
            indices.push_back(i);
        }
        else if(edges[i].position == '8'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(edges[index].position == '3'){
            edges[index].position = '9';
            edges[index].orientation = (edges[index].orientation + 1) % 2;
        }
        else if(edges[index].position == '9'){
            edges[index].position = 'A';
            edges[index].orientation = (edges[index].orientation + 1) % 2;
        }
        else if(edges[index].position == 'A'){
            edges[index].position = '8';
            edges[index].orientation = (edges[index].orientation + 1) % 2;
        }
        else if(edges[index].position == '8'){
            edges[index].position = '3';
            edges[index].orientation = (edges[index].orientation + 1) % 2;
        }
    }

    indices.clear();

    for(size_t i = 0; i < corners.size(); ++i){
        if(corners[i].position == 'c'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'd'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'h'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'g'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(corners[index].position == 'c'){
            corners[index].position = 'd';
            corners[index].orientation = (corners[index].orientation + 2) % 3;
        }
        else if(corners[index].position == 'd'){
            corners[index].position = 'h';
            corners[index].orientation = (corners[index].orientation + 1) % 3;
        }
        else if(corners[index].position == 'h'){
            corners[index].position = 'g';
            corners[index].orientation = (corners[index].orientation + 2) % 3;
        }

        else if(corners[index].position == 'g'){
            corners[index].position = 'c';
            corners[index].orientation = (corners[index].orientation + 1) % 3;
        }
    }
}

// Perform a B move
void Cube::B(){
    // Corner with position = 'f' gets it set equal to 'e', orientation +2
    // Corner with position = 'e' gets it set equal to 'g', orientation +1
    // Corner with position = 'g' gets it set equal to 'h', orientation +2
    // Corner with position = 'h' gets it set equal to 'f', orientation +1

    // Edge with position = '7' gets it set equal to 'C'
    // Edge with position = 'C' gets it set equal to 'A'
    // Edge with position = 'A' gets it set equal to 'B'
    // Edge with position = 'B' gets it set equal to '7'

    vector<size_t> indices;
    for(size_t i = 0; i < edges.size(); ++i){
        if(edges[i].position == '7'){
            indices.push_back(i);
        }
        else if(edges[i].position == 'C'){
            indices.push_back(i);
        }
        else if(edges[i].position == 'A'){
            indices.push_back(i);
        }
        else if(edges[i].position == 'B'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(edges[index].position == '7'){
            edges[index].position = 'C';
        }
        else if(edges[index].position == 'C'){
            edges[index].position = 'A';
        }
        else if(edges[index].position == 'A'){
            edges[index].position = 'B';
        }
        else if(edges[index].position == 'B'){
            edges[index].position = '7';
        }
    }

    indices.clear();

    for(size_t i = 0; i < corners.size(); ++i){
        if(corners[i].position == 'f'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'e'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'g'){
            indices.push_back(i);
        }
        else if(corners[i].position == 'h'){
            indices.push_back(i);
        }
    }

    for(size_t i = 0; i < indices.size(); ++i){
        uint32_t index = indices[i];
        if(corners[index].position == 'f'){
            corners[index].position = 'e';
            corners[index].orientation = (corners[index].orientation + 2) % 3;
        }
        else if(corners[index].position == 'e'){
            corners[index].position = 'g';
            corners[index].orientation = (corners[index].orientation + 1) % 3;
        }
        else if(corners[index].position == 'g'){
            corners[index].position = 'h';
            corners[index].orientation = (corners[index].orientation + 2) % 3;
        }
        else if(corners[index].position == 'h'){
            corners[index].position = 'f';
            corners[index].orientation = (corners[index].orientation + 1) % 3;
        }
    }
}

uint32_t encode_edge_orientations(const vector<Edge> &edges){ // since edges are either 0 or 1, we can use binary values as an encoded value
    uint32_t encoded_value = 0;
    uint32_t binary_factor = 0;
    for(int i = edges.size() - 2; i > -1; --i){ // Only need to encode the first 11 since the last edge is dependent on the previous edges, meaning that given 11 edges, we only have 1 valid state for that last edge
        uint32_t orientation = edges[i].orientation;
        uint32_t amount_to_encode = static_cast<uint32_t>(pow(2, binary_factor)) * orientation; // calculate the decimal value from the binary string, 
        encoded_value = encoded_value + amount_to_encode;
        binary_factor++;
    }
    return encoded_value;
}

uint32_t encode_corner_orientations(const vector<Corner> &corners){
    uint32_t encoded_value = 0;
    uint32_t ternary_factor = 0;
    for(int i = corners.size() - 2; i > -1; --i){ // Only need to encode the first 7 since the last edge is dependent on the previous corners, meaning that given 7 edges, we only have 1 valid state for that last edge
        uint32_t orientation = corners[i].orientation;
        uint32_t amount_to_encode = static_cast<uint32_t>(pow(3, ternary_factor)) * orientation; // calculate the decimal value from the ternary string, 
        encoded_value = encoded_value + amount_to_encode;
        ternary_factor++;
    }
    return encoded_value;   
}

// Give it a set of new edges, and set the cube equal to that set of new edges (primarily used when going through all the different possibilities)
void Cube::set_cube(const vector<Edge> &new_edges, const vector<Corner> &new_corners){
    for(size_t i = 0; i < new_edges.size(); ++i){
        edges[i] = new_edges[i];
    }

    for(size_t i = 0; i < new_corners.size(); ++i){
        corners[i] = new_corners[i];
    }
}

string get_inverse_moves(const string &algorithm){ // get the inverse of the moves/algorithm (Hardcoded)
    // may need to use a vector or sstream in the future, not quite sure as of no 7/19
    stringstream moves(algorithm);
    string move;
    vector<string> inverted_moves;
    string inverted_algorithm;
    while(moves >> move){
        if(move == "R"){
            inverted_moves.push_back("R'");
        }
        else if(move == "R2"){
            inverted_moves.push_back("R2");
        }
        else if(move == "R'"){
            inverted_moves.push_back("R");
        }
        else if (move == "U"){
            inverted_moves.push_back("U'");
        }
        else if (move == "U2"){
            inverted_moves.push_back("U2");
        }
        else if (move == "U'"){
            inverted_moves.push_back("U");
        }
        else if(move == "L"){
            inverted_moves.push_back("L'");
        }
        else if(move == "L2"){
            inverted_moves.push_back("L2");
        }
        else if(move == "L'"){
            inverted_moves.push_back("L");
        }
        else if(move == "F"){
            inverted_moves.push_back("F'");
        }
        else if(move == "F2"){
            inverted_moves.push_back("F2");
        }
        else if(move == "F'"){
            inverted_moves.push_back("F");
        }
        else if(move == "D"){
            inverted_moves.push_back("D'");
        }
        else if(move == "D2"){
            inverted_moves.push_back("D2");
        }
        else if(move == "D'"){
            inverted_moves.push_back("D");
        }
        else if(move == "B"){
            inverted_moves.push_back("B'");
        }
        else if(move == "B2"){
            inverted_moves.push_back("B2");
        }
        else if(move == "B'"){
            inverted_moves.push_back("B");
        }
        else{
            cout << "Unknown Move specified\n";
        }
    }

    for(int i = inverted_moves.size() - 1; i > -1; --i){ // convert it to a valid string
        inverted_algorithm = inverted_algorithm + " " + inverted_moves[i];
    }

    return inverted_algorithm;
}

bool operator==(const Edge &lhs, const Edge &rhs){ // operator overload for edge equality
    return (lhs.orientation == rhs.orientation) && (lhs.position == rhs.position);
}

bool operator==(const Corner &lhs, const Corner &rhs){ // operator overload for corner equality
    return (lhs.orientation == rhs.orientation) && (lhs.position == rhs.position);
}

// Cube will be in it's initial scrambled state and we need to orient all the edges in G0
string Cube::G0(unordered_map<uint64_t, uint32_t> &pruning_table){
    vector<string> alloted_moves = {"U", "D", "F", "B", "L", "R"}; // list of possible moves for G0
    // vector<string> alloted_moves = {"L", "R", "F", "B", "U", "D"}; // list of possible moves for G0
    // cout << "Searching for a solution for G0...\n";

    string G0_solution; // string to hold the solution
    Node original_state;
    original_state.state = make_pair(get_edges(), get_corners()); // keep track of the original state of the cube, will need to reset to this after each iteration
    Node current_node; // this is to keep track of the current state of the cube, helps keep track of new states to explore


    // Key calculation
    // ----------------------------------------------------------------------------------------//

    uint32_t encoded_edge_state = encode_edge_orientations(get_edges()); // get the scrambled state of the edges

    // ----------------------------------------------------------------------------------------//
    uint32_t threshold = pruning_table[encoded_edge_state] + 0; // get the initial threshold

    vector<string> temp_solution = {""}; //going to be used to store all of the things that we are searching
    stack < pair< Node, vector<string> > > explored_states; // all the states that we need to explore

    // uint32_t iteration_counter = 0;

    while(encoded_edge_state != 0){ // Go while we have not found the right state (essentially just go forever, since this will never be true)
        uint32_t smallest_pruned_score = pow(2, 32) - 1; // just set to the max value for uint32_t
        explored_states.push(make_pair(original_state, temp_solution)); // At the start of a new iteration, put the cube in the original state

        while(!(explored_states.empty())){ // while we still have states to explore
            pair< Node, vector<string> > current_state = explored_states.top();
            explored_states.pop(); // get rid of that state from the search queue
            current_node = current_state.first; // get the next state
            set_cube(current_node.state.first, current_node.state.second); // set the cube to that state

            for(size_t i = 0; i < alloted_moves.size(); ++i){ // go through all of the alloted moves and apply them to the state
                temp_solution = current_state.second; // get the vector of moves that it took to get to this state
                string move = alloted_moves[i]; // get the move to be applied
                temp_solution.push_back(move); // push in the move that we just applied
                algorithm(move); // apply that move
                current_node.depth = temp_solution.size() - 1; // update depth of the node (same as the number of moves it took to get there)
                current_node.state = make_pair(get_edges(), get_corners()); // get the state out of that move
                // iteration_counter++;

                // Key calculation
                // ----------------------------------------------------------------------------------------//
                encoded_edge_state = encode_edge_orientations(get_edges()); // encode the value of the edges as a result
                // ----------------------------------------------------------------------------------------//

                if(encoded_edge_state == 0){ // encoded_edge_state will be 0 when the edges are all oriented properly
                    for(size_t j = 0; j < temp_solution.size(); ++j){
                        G0_solution = G0_solution + " " + temp_solution[j]; // turn the solution into a valid string
                    }
                    // cout << iteration_counter << "!\n";
                    return G0_solution; // return that string
                }

                uint32_t f_score = pruning_table[encoded_edge_state]; // get the f_score (heuristic)
                uint32_t combined_heuristic = f_score +  current_node.depth;
                if(combined_heuristic > threshold){ // prune and store the pruned value if it is the smallest so far
                    if(combined_heuristic < smallest_pruned_score){ 
                        smallest_pruned_score = combined_heuristic; // update the smallest value if f_score is small
                    }
                } 
                else{ // add the state of the cube to the stack so that we can search it later as well as the moves that it took to get there (we can track the solution this way)
                    explored_states.push(make_pair(current_node, temp_solution)); // push this state to the search queue
                }

                string inverted_moves = get_inverse_moves(move); 
                algorithm(inverted_moves); // inverse the move to explore the next move
            }
        }

        threshold = smallest_pruned_score; // after going through all the alloted moves, update the threshold
        temp_solution.clear(); // once this iteration is over, we start back at the top with a new threshold, so our set of moves will be nothing since we are starting at the original again.
        temp_solution.push_back(""); // Do this to initialize temp_solution with a value, avoids Undefined Behavior
    }

    return "Solution Not Found"; // Will never happen, so just return "Solution Not Found" since there is no solution
}

uint32_t G1_encode_M_slice_edges(const vector<Edge> &edges){
    unordered_map<char, uint32_t> edge_to_prime;
    edge_to_prime['1'] = 2;
    edge_to_prime['2'] = 3;
    edge_to_prime['3'] = 5;
    edge_to_prime['4'] = 7;
    edge_to_prime['5'] = 11;
    edge_to_prime['6'] = 13;
    edge_to_prime['7'] = 17;
    edge_to_prime['8'] = 19;
    edge_to_prime['9'] = 23;
    edge_to_prime['A'] = 29;
    edge_to_prime['B'] = 31;
    edge_to_prime['C'] = 37;

    uint32_t edge_1_encoded = edge_to_prime[edges[0].position];
    uint32_t edge_3_encoded = edge_to_prime[edges[2].position];
    uint32_t edge_7_encoded = edge_to_prime[edges[6].position];
    uint32_t edge_A_encoded = edge_to_prime[edges[9].position];

    return edge_1_encoded * edge_3_encoded * edge_7_encoded * edge_A_encoded;
}

string Cube::G1(unordered_map<uint64_t, uint32_t> &pruning_table){
    // Cube will initially already be at a solved G0 state, meaning all edge orientation will be 0
    vector<string> alloted_moves = {"U2", "D2", "F", "F2", "F'", "B", "B2", "B'", "L", "R"}; // list of possible moves for G1
    // vector<string> alloted_moves = {"L", "R", "F", "B", "U2", "D2" }; // list of possible moves for G0
    // cout << "Searching for a solution for G1...\n";

    string G1_solution; // string to hold the solution
    Node original_state;
    original_state.state = make_pair(get_edges(), get_corners()); // keep track of the original state of the cube, will need to reset to this after each iteration
    Node current_node;

    // Key calculation
    // ----------------------------------------------------------------------------------------//

    uint32_t encoded_corner_orientation_value = encode_corner_orientations(get_corners());
    uint32_t encoded_edge_permutation_value = G1_encode_M_slice_edges(get_edges());

    string val = to_string(encoded_corner_orientation_value);
    string ue = to_string(encoded_edge_permutation_value);
    string value = val + "0" + ue;
    uint64_t key = large_string_to_int(value);

    // ----------------------------------------------------------------------------------------//

    uint32_t threshold = pruning_table[key] + 0; // get the initial threshold

    // uint32_t iteration_counter = 0;
    // uint32_t empty = 0;
    // uint32_t reset = 0;

    vector<string> temp_solution = {""}; //going to be used to store all of the things that we are searching
    stack< pair< Node, vector<string> > > explored_states; // all the states that we need to explore
    while(key != 4930){ // Go while we have not found the right state (essentially just go forever, since this will never be true)
        uint32_t smallest_pruned_score = pow(2, 32) - 1; // just set to the max value for uint32_t
        explored_states.push(make_pair(original_state, temp_solution)); // At the start of a new iteration, put the cube in the original state
        while(!(explored_states.empty())){ // while we still have states to explore
            pair< Node, vector<string> > current_state = explored_states.top();
            explored_states.pop(); // get rid of that state from the search queue
            current_node = current_state.first; // get the next state
            set_cube(current_node.state.first, current_node.state.second); // set the cube to that state
            for(size_t i = 0; i < alloted_moves.size(); ++i){ // go through all of the alloted moves and apply them to the state
                temp_solution = current_state.second; // get the vector of moves that it took to get to this state
                string move = alloted_moves[i]; // get the move to be applied
                temp_solution.push_back(move); // push in the move that we just applied
                algorithm(move); // apply that move
                current_node.state = make_pair(get_edges(), get_corners()); // get the state out of that move
                current_node.depth = temp_solution.size() - 1;

                // iteration_counter++;

                // Key calculation
                // ----------------------------------------------------------------------------------------//
            
                uint32_t encoded_corner_orientation_value = encode_corner_orientations(get_corners());
                uint32_t encoded_edge_permutation_value = G1_encode_M_slice_edges(get_edges());
            
                string val = to_string(encoded_corner_orientation_value);
                string ue = to_string(encoded_edge_permutation_value);
                string value = val + "0" + ue;
                uint64_t key = large_string_to_int(value); // encode the value of the edges as a result
                // cout << key << '\n';

                // ----------------------------------------------------------------------------------------//

                if(key == 4930){ // key will be 4930 when the corners are all oriented properly and edges in E sliced are correctly placed
                    for(size_t j = 0; j < temp_solution.size(); ++j){
                        G1_solution = G1_solution + " " + temp_solution[j]; // turn the solution into a valid string
                    }
                    // cout << iteration_counter << "!\n";
                    // cout << empty << "?\n";
                    // cout << reset << "%\n";
                    // cout << explored_states.size() << "***\n";
                    return G1_solution; // return that string
                }

                uint32_t f_score = 0;
                if(pruning_table.find(key) == pruning_table.end()){
                    // empty++;
                    f_score = 9;
                }
                else{
                    f_score = pruning_table[key];
                }

                // uint32_t f_score = pruning_table[key]; // get the f_score (heuristic)
                uint32_t combined_heuristic = f_score + current_node.depth;
                if(combined_heuristic > threshold){ // prune and store the pruned value if it is the smallest so far
                    if(combined_heuristic < smallest_pruned_score){ 
                        smallest_pruned_score = combined_heuristic; // update the smallest value if f_score is small
                    }
                } 
                else{ // add the state of the cube to the queue so that we can search it later as well as the moves that it took to get there (we can track the solution this way)
                    explored_states.push(make_pair(current_node, temp_solution)); // push this state to the search queue
                }
                string inverted_moves = get_inverse_moves(move); 
                algorithm(inverted_moves); // inverse the move to explore the next move

            }
        }
        // reset++;
        threshold = smallest_pruned_score; // after going through all the alloted moves, update the threshold
        temp_solution.clear(); // once this iteration is over, we start back at the top with a new threshold, so our set of moves will be nothing since we are starting at the original again.
        temp_solution.push_back(""); // Do this to initialize temp_solution with a value, avoids Undefined Behavior
    }

    return "Solution Not Found"; // Will never happen, so just return "Solution Not Found" since there is no solution
}

uint32_t binary_string_to_int(const vector<uint32_t> binary_string){
    uint32_t encoded_value = 0;
    uint32_t binary_factor = 0;
    for(int i = binary_string.size() - 1; i > -1; --i){ // Only need to encode the first 11 since the last edge is dependent on the previous edges, meaning that given 11 edges, we only have 1 valid state for that last edge
        uint32_t amount_to_encode = static_cast<uint32_t>(pow(2, binary_factor)) * binary_string[i]; // calculate the decimal value from the binary string, 
        encoded_value = encoded_value + amount_to_encode;
        binary_factor++;
    }
    return encoded_value;
}

uint64_t large_string_to_int(const string large_string){
    uint64_t encoded_value = 0;
    uint32_t base_10_factor = 0;
    for(int i = large_string.size() - 1; i > -1; --i){ 
        uint64_t amount_to_encode = pow(10, base_10_factor) * (large_string[i] - '0'); // calculate the decimal value from the binary string, 
        encoded_value = encoded_value + amount_to_encode;
        base_10_factor++;
    }
    return encoded_value;
}

uint64_t G2_encode_corner_permutations(const vector<Corner> &corners){
    unordered_map<char, uint32_t> corner_to_val;
    corner_to_val['a'] = 1;
    corner_to_val['b'] = 2;
    corner_to_val['c'] = 3;
    corner_to_val['d'] = 4;
    corner_to_val['e'] = 5;
    corner_to_val['f'] = 6;
    corner_to_val['g'] = 7;
    corner_to_val['h'] = 8;

    uint64_t encoded_value = 0;
    uint32_t base_8_factor = 0;
    for(int i = corners.size() - 2; i > -1; --i){ 
        uint64_t amount_to_encode = pow(8, base_8_factor) * corner_to_val[corners[i].position]; 
        encoded_value = encoded_value + amount_to_encode;
        base_8_factor++;
    }
    return encoded_value;
}

uint32_t G2_encode_E_slice_edges(const vector<Edge> &edges){
    unordered_map<char, uint32_t> edge_to_prime;
    edge_to_prime['1'] = 2;
    edge_to_prime['2'] = 3;
    edge_to_prime['3'] = 5;
    edge_to_prime['4'] = 7;
    edge_to_prime['5'] = 11;
    edge_to_prime['6'] = 13;
    edge_to_prime['7'] = 17;
    edge_to_prime['8'] = 19;
    edge_to_prime['9'] = 23;
    edge_to_prime['A'] = 29;
    edge_to_prime['B'] = 31;
    edge_to_prime['C'] = 37;

    uint32_t edge_2_encoded = edge_to_prime[edges[1].position];
    uint32_t edge_4_encoded = edge_to_prime[edges[3].position];
    uint32_t edge_B_encoded = edge_to_prime[edges[10].position];
    uint32_t edge_C_encoded = edge_to_prime[edges[11].position];

    return edge_2_encoded * edge_4_encoded * edge_B_encoded * edge_C_encoded;
}

uint32_t G2_encode_S_slice_edges(const vector<Edge> &edges){
    unordered_map<char, uint32_t> edge_to_prime;
    edge_to_prime['1'] = 2;
    edge_to_prime['2'] = 3;
    edge_to_prime['3'] = 5;
    edge_to_prime['4'] = 7;
    edge_to_prime['5'] = 11;
    edge_to_prime['6'] = 13;
    edge_to_prime['7'] = 17;
    edge_to_prime['8'] = 19;
    edge_to_prime['9'] = 23;
    edge_to_prime['A'] = 29;
    edge_to_prime['B'] = 31;
    edge_to_prime['C'] = 37;

    uint32_t edge_5_encoded = edge_to_prime[edges[4].position];
    uint32_t edge_6_encoded = edge_to_prime[edges[5].position];
    uint32_t edge_8_encoded = edge_to_prime[edges[7].position];
    uint32_t edge_9_encoded = edge_to_prime[edges[8].position];

    return edge_5_encoded * edge_6_encoded * edge_8_encoded * edge_9_encoded;
}

string Cube::G2(unordered_map<uint64_t, uint32_t> &pruning_table){
    // Cube will initially already be at a solved G0 state, meaning all edge orientation will be 0
    vector<string> alloted_moves = {"U2", "D2", "F2", "B2", "L", "L2", "L'", "R", "R2", "R'"}; // list of possible moves for G2
    // vector<string> alloted_moves = {"L", "R", "F", "B", "U2", "D2" }; // list of possible moves for G0
    // cout << "Searching for a solution for G2...\n";

    string G2_solution; // string to hold the solution
    Node original_state;
    original_state.state = make_pair(get_edges(), get_corners()); // keep track of the original state of the cube, will need to reset to this after each iteration
    Node current_node;

    // Key calculation
    // ----------------------------------------------------------------------------------------//

    uint32_t encoded_corner_permutation_value = G2_encode_corner_permutations(get_corners());
    uint32_t encoded_L_edge_permutation_value = G2_encode_E_slice_edges(get_edges());
    uint32_t encoded_R_edge_permutation_value = G2_encode_S_slice_edges(get_edges());

    string val = to_string(encoded_L_edge_permutation_value);
    string ue = to_string(encoded_R_edge_permutation_value);
    string the = to_string(encoded_corner_permutation_value);
    string value =  the + "0" + val + "0" + ue;
    uint64_t key = large_string_to_int(value);

    // ----------------------------------------------------------------------------------------//

    uint32_t threshold = pruning_table[key] + 0; // get the initial threshold

    // uint32_t iteration_counter = 0;

    vector<string> temp_solution = {""}; //going to be used to store all of the things that we are searching
    stack< pair< Node, vector<string> > > explored_states; // all the states that we need to explore
    while(key != 342391024087062491){ // Go while we have not found the right state (essentially just go forever, since this will never be true)
        uint32_t smallest_pruned_score = pow(2, 32) - 1; // just set to the max value for uint32_t
        explored_states.push(make_pair(original_state, temp_solution)); // At the start of a new iteration, put the cube in the original state
        while(!(explored_states.empty())){ // while we still have states to explore
            pair< Node, vector<string> > current_state = explored_states.top();
            explored_states.pop(); // get rid of that state from the search queue
            current_node = current_state.first; // get the next state
            set_cube(current_node.state.first, current_node.state.second); // set the cube to that state
            for(size_t i = 0; i < alloted_moves.size(); ++i){ // go through all of the alloted moves and apply them to the state
                temp_solution = current_state.second; // get the vector of moves that it took to get to this state
                string move = alloted_moves[i]; // get the move to be applied
                temp_solution.push_back(move); // push in the move that we just applied
                algorithm(move); // apply that move
                current_node.state = make_pair(get_edges(), get_corners()); // get the state out of that move
                current_node.depth = temp_solution.size() - 1;

                // iteration_counter++;
                
                // Key calculation
                // ----------------------------------------------------------------------------------------//
                encoded_corner_permutation_value = G2_encode_corner_permutations(get_corners());
                encoded_L_edge_permutation_value = G2_encode_E_slice_edges(get_edges());
                encoded_R_edge_permutation_value = G2_encode_S_slice_edges(get_edges());
            
                val = to_string(encoded_L_edge_permutation_value);
                ue = to_string(encoded_R_edge_permutation_value);
                the = to_string(encoded_corner_permutation_value);
                value =  the + "0" + val + "0" + ue;
                key = large_string_to_int(value); // encode the value of the edges as a result
                // ----------------------------------------------------------------------------------------//

                if(key == 342391024087062491){ // key will be 4930 when the corners are all oriented properly and edges in E sliced are correctly placed
                    for(size_t j = 0; j < temp_solution.size(); ++j){
                        G2_solution = G2_solution + " " + temp_solution[j]; // turn the solution into a valid string
                    }
                    // cout << iteration_counter << "!\n";
                    return G2_solution; // return that string
                }
                uint32_t f_score = pruning_table[key]; // get the f_score (heuristic)
                uint32_t combined_heuristic = f_score + current_node.depth;
                if(combined_heuristic > threshold){ // prune and store the pruned value if it is the smallest so far
                    if(combined_heuristic < smallest_pruned_score){ 
                        smallest_pruned_score = combined_heuristic; // update the smallest value if f_score is small
                    }
                } else{ // add the state of the cube to the queue so that we can search it later as well as the moves that it took to get there (we can track the solution this way)
                    explored_states.push(make_pair(current_node, temp_solution)); // push this state to the search queue
                }
                string inverted_moves = get_inverse_moves(move); 
                algorithm(inverted_moves); // inverse the move to explore the next move
            }
        }
        threshold = smallest_pruned_score; // after going through all the alloted moves, update the threshold
        temp_solution.clear(); // once this iteration is over, we start back at the top with a new threshold, so our set of moves will be nothing since we are starting at the original again.
        temp_solution.push_back(""); // Do this to initialize temp_solution with a value, avoids Undefined Behavior
    }

    return "Solution Not Found"; // Will never happen, so just return "Solution Not Found" since there is no solution
}

uint32_t G3_encode_edge_permutations(const vector<Edge> &edges){ // I guess just use base 12 integer coding right?
    unordered_map<char, uint32_t> edge_to_val;
    edge_to_val['1'] = 0;
    edge_to_val['2'] = 1;
    edge_to_val['3'] = 2;
    edge_to_val['4'] = 3;
    edge_to_val['5'] = 4;
    edge_to_val['6'] = 5;
    edge_to_val['7'] = 6;
    edge_to_val['8'] = 7;
    edge_to_val['9'] = 8;
    edge_to_val['A'] = 9;
    edge_to_val['B'] = 10;
    edge_to_val['C'] = 11;

    uint32_t encoded_value = 0;
    uint32_t base_12_factor = 0;
    for(int i = edges.size() - 2; i > -1; --i){ 
        uint32_t amount_to_encode = static_cast<uint32_t>(pow(12, base_12_factor)) * edge_to_val[edges[i].position]; // calculate the decimal value from the binary string, 
        encoded_value = encoded_value + amount_to_encode;
        base_12_factor++;
    }
    return encoded_value; 
}

uint32_t G3_encode_corner_permutations(const vector<Corner> &corners){ // I guess just use base 12 integer coding right?
    unordered_map<char, uint32_t> corner_to_val;
    corner_to_val['a'] = 0;
    corner_to_val['b'] = 1;
    corner_to_val['c'] = 2;
    corner_to_val['d'] = 3;
    corner_to_val['e'] = 4;
    corner_to_val['f'] = 5;
    corner_to_val['g'] = 6;
    corner_to_val['h'] = 7;

    uint32_t encoded_value = 0;
    uint32_t base_8_factor = 0;
    for(int i = corners.size() - 2; i > -1; --i){ 
        uint32_t amount_to_encode = static_cast<uint32_t>(pow(8, base_8_factor)) * corner_to_val[corners[i].position]; // calculate the decimal value from the binary string, 
        encoded_value = encoded_value + amount_to_encode;
        base_8_factor++;
    }
    return encoded_value; 
}

string Cube::G3(unordered_map<uint64_t, uint32_t> &pruning_table){
    // Cube will initially already be at a solved G0 state, meaning all edge orientation will be 0
    vector<string> alloted_moves = {"U2", "D2", "F2", "B2", "L2", "R2"}; // list of possible moves for G3
    // cout << "Searching for a solution for G3...\n";

    string G3_solution; // string to hold the solution
    Node original_state;
    original_state.state = make_pair(get_edges(), get_corners()); // keep track of the original state of the cube, will need to reset to this after each iteration
    Node current_node;

    // Key calculation
    // ---------------------------------------------------------------------------------------------------- //

    uint64_t edge_key = G3_encode_edge_permutations(get_edges()); // make use of overflow mechanism to get keys
    uint64_t corner_key = G3_encode_corner_permutations(get_corners());

    string val = to_string(edge_key);
    string ue = to_string(corner_key);
    string value =  val + "0" + ue;
    uint64_t key = large_string_to_int(value);
    
    // ---------------------------------------------------------------------------------------------------- //

    uint32_t threshold = pruning_table[key] + 0; // get the initial threshold

    // uint32_t iteration_counter = 0;

    vector<string> temp_solution = {""}; //going to be used to store all of the things that we are searching
    stack< pair< Node, vector<string> > > explored_states; // all the states that we need to explore
    while(key != 980784693042798){ // Go while we have not found the right state (essentially just go forever, since this will never be true)
        uint32_t smallest_pruned_score = pow(2, 32) - 1; // just set to the max value for uint32_t
        explored_states.push(make_pair(original_state, temp_solution)); // At the start of a new iteration, put the cube in the original state
        while(!(explored_states.empty())){ // while we still have states to explore
            pair< Node, vector<string> > current_state = explored_states.top();
            explored_states.pop(); // get rid of that state from the search queue
            current_node = current_state.first; // get the next state
            set_cube(current_node.state.first, current_node.state.second); // set the cube to that state
            for(size_t i = 0; i < alloted_moves.size(); ++i){ // go through all of the alloted moves and apply them to the state
                temp_solution = current_state.second; // get the vector of moves that it took to get to this state
                string move = alloted_moves[i]; // get the move to be applied
                temp_solution.push_back(move); // push in the move that we just applied
                algorithm(move); // apply that move
                current_node.state = make_pair(get_edges(), get_corners()); // get the state out of that move
                current_node.depth = temp_solution.size() - 1;

                // iteration_counter++;
                
                // Key calculation
                // ---------------------------------------------------------------------------------------------------- //

                edge_key = G3_encode_edge_permutations(get_edges()); // make use of overflow mechanism to get keys
                corner_key = G3_encode_corner_permutations(get_corners());
    
                val = to_string(edge_key);
                ue = to_string(corner_key);
                value =  val + "0" + ue;
                key = large_string_to_int(value);

                // ---------------------------------------------------------------------------------------------------- //

                if(key == 980784693042798){ // key will be 4930 when the corners are all oriented properly and edges in E sliced are correctly placed
                    for(size_t j = 0; j < temp_solution.size(); ++j){
                        G3_solution = G3_solution + " " + temp_solution[j]; // turn the solution into a valid string
                    }
                    // cout << iteration_counter << "!\n";
                    return G3_solution; // return that string
                }
                uint32_t f_score = pruning_table[key]; // get the f_score (heuristic)
                uint32_t combined_heuristic = f_score + current_node.depth;
                if(combined_heuristic > threshold){ // prune and store the pruned value if it is the smallest so far
                    if(combined_heuristic < smallest_pruned_score){ 
                        smallest_pruned_score = combined_heuristic; // update the smallest value if f_score is small
                    }
                } else{ // add the state of the cube to the queue so that we can search it later as well as the moves that it took to get there (we can track the solution this way)
                    explored_states.push(make_pair(current_node, temp_solution)); // push this state to the search queue
                }
                string inverted_moves = get_inverse_moves(move); 
                algorithm(inverted_moves); // inverse the move to explore the next move
            }
        }
        threshold = smallest_pruned_score; // after going through all the alloted moves, update the threshold
        temp_solution.clear(); // once this iteration is over, we start back at the top with a new threshold, so our set of moves will be nothing since we are starting at the original again.
        temp_solution.push_back(""); // Do this to initialize temp_solution with a value, avoids Undefined Behavior
    }

    return "Solution Not Found"; // Will never happen, so just return "Solution Not Found" since there is no solution
}