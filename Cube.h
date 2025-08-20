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

using namespace std;

// Default for this cube will be Green facing foward with White on top

// Each edge can have their position (edge slot), and their oreientation 0 (correct) 1 (flipped)
struct Edge{
    char position;
    uint32_t orientation = 0;
};

// Each corner can have their position (edge slot), and their orientation 0 (correct), 1 (rotated 1), 2 (rotated 2). 1 and 2 are wrong since corners have parity of 3 (J Perm)
struct Corner{
    char position;
    uint32_t orientation = 0;
};

struct Node{
    pair<vector<Edge>, vector<Corner>> state;
    uint32_t depth = 0;
};

class Cube {
public:

    // Construct Cube with the edges and corners
    Cube(std::vector<Edge> Edges, std::vector<Corner> Corners) : edges(Edges), corners(Corners) {}

    // Used to get the edges
    vector<Edge> get_edges(){
        return edges;
    }

    // Used to get the corners
    vector<Corner> get_corners(){
        return corners;
    }

    // Helper functions to print out the state of the cube
    void print_cube_state();

    // Function to execute an algorithm on the cube
    void algorithm(const string &algorithm);

    string Thistlethwaite(unordered_map<uint64_t, uint32_t> &G0_pruning_table, unordered_map<uint64_t, uint32_t> &G1_pruning_table, unordered_map<uint64_t, uint32_t> &G2_pruning_table, unordered_map<uint64_t, uint32_t> &G3_pruning_table){
        string sol1 = G0(G0_pruning_table);
        string sol2 = G1(G1_pruning_table);
        string sol3 = G2(G2_pruning_table);
        string sol4 = G3(G3_pruning_table);
        string solution = sol1 + sol2 + sol3 + sol4;
        return solution;
    }

    // Function to set the cube to a certain state given by the edges and the corners
    void set_cube(const vector<Edge> &new_edges, const vector<Corner> &new_corners);

    // The next 18 functions just define all the moves on the cube
    void R();

    void U();

    void L();

    void F();

    void D();

    void B();

    void R2(){
        R();
        R();
    }

    void R_prime(){
        R();
        R();
        R();
    }

    void U2(){
        U();
        U();
    }

    void U_prime(){
        U();
        U();
        U();
    }

    void L2(){
        L();
        L();
    }

    void L_prime(){
        L();
        L();
        L();
    }

    void F2(){
        F();
        F();
    }

    void F_prime(){
        F();
        F();
        F();
    }

    void D2(){
        D();
        D();
    }

    void D_prime(){
        D();
        D();
        D();
    }

    void B2(){
        B();
        B();
    }

    void B_prime(){
        B();
        B();
        B();
    }

    // find the G0 solution
    string G0(unordered_map<uint64_t, uint32_t> &pruning_table); // Got it to work on 7/22! Fixed on 7/26!

    // find the G1 solution
    string G1(unordered_map<uint64_t, uint32_t> &pruning_table); // Got it to work on 7/26!

    // find the G2 solution
    string G2(unordered_map<uint64_t, uint32_t> &pruning_table); // Got it to work on 7/27! Fixed on 

    // find the G3 solution
    string G3(unordered_map<uint64_t, uint32_t> &pruning_table); // Got it work on 7/27

private:

    vector<Edge> edges;
    vector<Corner> corners;

    /* 
    Ok, let's say that I have my pruning table done. 
    If I want to actually find the set of moves that gets me to the solved state via IDA*, I would need to start by having my 
    root node be the current state that my cube is in. I then set the threshold to be equal to that state by looking it up. 
    I then would apply all the moves within that subset to that root node state and see which states come out of it. 
    I can then see what their heuristic value is by looking it up from the table and will prune if it is greater than my threshold. 
    If I have not found the solved state. I will update the threshold to be the minimum value that I pruned and start from the root node. 
    I will continue this until I find the goal state and make note of the edges (moves) that I took to get there.
    */

    /*
    High-Level Steps for Using IDA* with a Pruning Table
    Start with the current scrambled state as the root node.

    Look up the heuristic from the pruning table for this state. This is your initial threshold:

    You begin a depth-limited DFS (depth-first search) from the current state.

    You pass down the current depth g and compute f = g + h, where:

        g is the number of moves from the start,

        h is the pruning table value (heuristic estimate to goal).

    Prune if f > threshold:

        If so, do not explore this branch, but track the smallest f that was pruned — this will become your next threshold.

    Goal Check:

        If h == 0, then you've reached the solved state (since you built the pruning table with distances from the goal).

    You then return the path (i.e., the moves) you took to reach the goal.

    Repeat the above process, increasing the threshold to the smallest pruned f each time, until the solution is found.

    */
    

};

void read_in(ifstream &cin, vector<vector<uint32_t>> &faces, unordered_map<uint64_t, uint32_t> &G0_table, unordered_map<uint64_t, uint32_t> &G1_map, unordered_map<uint64_t, uint32_t> &G2_map, unordered_map<uint64_t, uint32_t> &G3_map);

void initialize_solved_state(vector<Edge> &edges, vector<Corner> &corners);

// These following functions will be used for the pruning table

uint32_t binary_string_to_int(const vector<uint32_t> binary_string);

uint64_t large_string_to_int(const string large_string);

uint32_t encode_edge_orientations(const vector<Edge> &edges); // use binary representation

uint32_t encode_corner_orientations(const vector<Corner> &corners); // use ternary representation

uint32_t G1_encode_M_slice_edges(const vector<Edge> &edges);

uint64_t G2_encode_corner_permutations(const vector<Corner> &corners);

uint32_t G2_encode_E_slice_edges(const vector<Edge> &edges);

uint32_t G2_encode_S_slice_edges(const vector<Edge> &edges);

uint32_t G3_encode_edge_permutations(const vector<Edge> &edges);

uint32_t G3_encode_corner_permutations(const vector<Corner> &corners);

string get_inverse_moves(const string &algorithm);

bool operator==(const Edge &lhs, const Edge &rhs);

bool operator==(const Corner &lhs, const Corner &rhs);

