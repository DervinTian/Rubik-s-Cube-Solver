#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <random>
#include <array>

#include "Cube.h"

using namespace std;

unordered_map<uint64_t, uint32_t> G0_pruning_table(){
    vector<string> alloted_moves ={"U", "D", "F", "B", "L", "R"}; // alloted moves for G0
    unordered_map<uint64_t, uint32_t> pruning_map;

    vector<Edge> edges;
    vector<Corner> corners;
    initialize_solved_state(edges, corners);

    Cube cube = Cube(edges, corners); // create a cube
    queue<Node> explored_states; // create a search vector to house the states to be searched

    Node node;
    node.state = make_pair(cube.get_edges(), cube.get_corners());
    node.depth = 0;
    explored_states.push(node);

    uint32_t key = encode_edge_orientations(cube.get_edges());
    uint32_t edge_permutation_value = G3_encode_edge_permutations(cube.get_edges());

    unordered_set<uint32_t> edge_permutations;
    edge_permutations.insert(edge_permutation_value);
    bool already_pushed = false;

    pruning_map.insert({key, 0});

    while(explored_states.size() > 0){

        if(pruning_map.size() == 2048){
            return pruning_map;
        }

        Node current_state = explored_states.front();
        explored_states.pop();
        cube.set_cube(current_state.state.first, current_state.state.second);

        for(size_t i = 0; i < alloted_moves.size(); ++i){
            string move = alloted_moves[i];
            cube.algorithm(move);
            Node new_state;
            new_state.state = make_pair(cube.get_edges(), cube.get_corners());
            new_state.depth = current_state.depth + 1;
            
            key = encode_edge_orientations(cube.get_edges());
            edge_permutation_value = G3_encode_edge_permutations(cube.get_edges());

            if(edge_permutations.find(edge_permutation_value) == edge_permutations.end()){
                explored_states.push(new_state);
                already_pushed = true;
            }

            edge_permutations.insert(edge_permutation_value);

            auto it = pruning_map.find(key);
            if(it == pruning_map.end()){
                pruning_map.insert({key, new_state.depth});
                if(!already_pushed){
                    explored_states.push(new_state);
                }

            }

            already_pushed = false;
            
            // Attempt to prune some states that we do need to explore away
            string inverted_moves = get_inverse_moves(move);
            cube.algorithm(inverted_moves);
        }
        // cout << pruning_map.size() << " " << explored_states.size() << '\n';
    }

    // cout << pruning_map.size() <<'\n';
    return pruning_map;
}

unordered_map<uint64_t, uint32_t> G1_pruning_table(){
    vector<string> alloted_moves = {"U2", "D2", "F", "F2", "F'", "B", "B2", "B'", "L", "L2", "L'", "R", "R2", "R'"}; // list of possible moves for G1
    vector<string> alloted_moves_G2 =  {"U2", "D2", "F2", "B2", "L", "R"};
    unordered_map<uint64_t, uint32_t> pruning_map;

    // // Create a random number generator
    // std::random_device rd;  // Seed (non-deterministic if supported)
    // std::mt19937 gen(rd()); // Mersenne Twister engine

    // // Define a distribution
    // std::uniform_int_distribution<> distrib(0, alloted_moves_G2.size() - 1); // Random int from 1 to 100

    vector<Edge> edges;
    vector<Corner> corners;
    initialize_solved_state(edges, corners);

    Cube cube = Cube(edges, corners); // create a cube
    queue<Node> explored_states; // create a search vector to house the states to be searched

    Node node;
    node.state = make_pair(cube.get_edges(), cube.get_corners());
    node.depth = 0;
    explored_states.push(node);

    uint32_t encoded_corner_orientation_value = encode_corner_orientations(cube.get_corners());
    uint32_t encoded_edge_permutation_value = G1_encode_M_slice_edges(cube.get_edges());

    string val = to_string(encoded_corner_orientation_value);
    string ue = to_string(encoded_edge_permutation_value);
    string value = val + "0" + ue;
    uint64_t key = large_string_to_int(value);

    unordered_set<uint32_t> solved_edge_permutations_inserted;
    unordered_map<uint64_t, uint32_t> times_visited;

    pruning_map.insert({key, 0});
    solved_edge_permutations_inserted.insert(key);
    times_visited.insert({key, 1});

    while(explored_states.size() > 0){

        if(pruning_map.size() == 1082565){
            return pruning_map;
        }

        Node current_state = explored_states.front();
        explored_states.pop();
        cube.set_cube(current_state.state.first, current_state.state.second);

        for(size_t i = 0; i < alloted_moves.size(); ++i){
            string move = alloted_moves[i];
            cube.algorithm(move);
            Node new_state;
            new_state.state = make_pair(cube.get_edges(), cube.get_corners());
            new_state.depth = current_state.depth + 1;
            
            encoded_corner_orientation_value = encode_corner_orientations(cube.get_corners());
            encoded_edge_permutation_value = G1_encode_M_slice_edges(cube.get_edges());

            val = to_string(encoded_corner_orientation_value);
            ue = to_string(encoded_edge_permutation_value);
            value = val + "0" + ue;
            key = large_string_to_int(value); 
            
            if(pruning_map.find(key) == pruning_map.end()){
                if(new_state.depth > 8){
                    new_state.depth = 9;
                }
                pruning_map.insert({key, new_state.depth});
                explored_states.push(new_state);
                times_visited.insert({key, 1});
                // if(encoded_edge_permutation_value == 4930){
                //     solved_edge_permutations_inserted.insert(key);
                // }
            }
            else{
                if(times_visited[key] < 15){
                    explored_states.push(new_state);
                    times_visited[key]++;
                }
            }
            
            // Attempt to prune some states that we do need to explore away
            string inverted_moves = get_inverse_moves(move);
            cube.algorithm(inverted_moves);
        }
        // cout << pruning_map.size() << " " << explored_states.size() << " " << solved_edge_permutations_inserted.size() << '\n';
    }

    // unordered_set<uint64_t> all_keys;
    // for(auto it = edge_permutations_inserted.begin(); it != edge_permutations_inserted.end(); ++it){
    //     uint32_t edge_key = *it;
    //     for(auto jt = corner_orientations_inserted.begin(); jt != corner_orientations_inserted.end(); ++jt){
    //         uint32_t corner_key = *jt;
    //         val = to_string(corner_key);
    //         ue = to_string(edge_key);
    //         value = val + "0" + ue;
    //         key = large_string_to_int(value);
    //         all_keys.insert(key);
    //     }  
    // }

    // unordered_set<uint64_t> missed_keys;
    // for(auto it = all_keys.begin(); it != all_keys.end(); ++it){
    //     if(pruning_map.find(*it) == pruning_map.end()){
    //         missed_keys.insert(*it);
    //     }
    // }

    // for(auto it = missed_keys.begin(); it != missed_keys.end(); ++it){
    //     cout << *it << '\n';
    // }

    // cout << pruning_map.size() <<'\n';
    return pruning_map;

}

unordered_map<uint64_t, uint32_t> G2_pruning_table(){
    vector<string> alloted_moves = {"U2", "D2", "F2", "B2", "L", "R"}; // alloted moves for G2
    unordered_map<uint64_t, uint32_t> pruning_map;

    vector<Edge> edges;
    vector<Corner> corners;
    initialize_solved_state(edges, corners);

    Cube cube = Cube(edges, corners); // create a cube
    queue<Node> explored_states; // create a search vector to house the states to be searched
    
    Node node;
    node.state = make_pair(cube.get_edges(), cube.get_corners());
    node.depth = 0;
    explored_states.push(node);

    uint64_t encoded_corner_permutation_value = G2_encode_corner_permutations(cube.get_corners());
    uint32_t encoded_E_edge_permutation_value = G2_encode_E_slice_edges(cube.get_edges());
    uint32_t encoded_S_edge_permutation_value = G2_encode_S_slice_edges(cube.get_edges());
    
    string val = to_string(encoded_E_edge_permutation_value);
    string ue = to_string(encoded_S_edge_permutation_value);
    string the = to_string(encoded_corner_permutation_value);
    string value =  the + "0" + val + "0" + ue;
    uint64_t key = large_string_to_int(value);

    pruning_map.insert({key, 0});


    unordered_set<uint32_t> E_edge_states_visited;
    unordered_set<uint32_t> S_edge_states_visited;
    unordered_set<uint32_t> corner_permutations_visited;
    unordered_set<uint64_t> keys_visited;

    while(explored_states.size() > 0){

        Node current_state = explored_states.front();
        explored_states.pop();
        cube.set_cube(current_state.state.first, current_state.state.second);

        for(size_t i = 0; i < alloted_moves.size(); ++i){
            string move = alloted_moves[i];
            cube.algorithm(move);
            Node new_state;
            new_state.state = make_pair(cube.get_edges(), cube.get_corners());
            new_state.depth = current_state.depth + 1;
            
            encoded_corner_permutation_value = G2_encode_corner_permutations(cube.get_corners());
            encoded_E_edge_permutation_value = G2_encode_E_slice_edges(cube.get_edges());
            encoded_S_edge_permutation_value = G2_encode_S_slice_edges(cube.get_edges());

            E_edge_states_visited.insert(encoded_E_edge_permutation_value);
            S_edge_states_visited.insert(encoded_S_edge_permutation_value);
            corner_permutations_visited.insert(encoded_corner_permutation_value);
            
            val = to_string(encoded_E_edge_permutation_value);
            ue = to_string(encoded_S_edge_permutation_value);
            the = to_string(encoded_corner_permutation_value);
            value =  the + "0" + val + "0" + ue;
            key = large_string_to_int(value);
            keys_visited.insert(key);
            auto it = pruning_map.find(key);
            if(it == pruning_map.end()){
                pruning_map.insert({key, new_state.depth});
                explored_states.push(new_state);
            } 
            
            // Attempt to prune some states that we do need to explore away
            string inverted_moves = get_inverse_moves(move);
            cube.algorithm(inverted_moves);
        }
        cout << pruning_map.size() << " " << explored_states.size() << '\n';
    }

    cout << E_edge_states_visited.size() << " " << S_edge_states_visited.size() << " " << corner_permutations_visited.size() << " " << pruning_map.size() <<'\n';
    return pruning_map;

}

unordered_map<uint64_t, uint32_t> G3_pruning_table(){
    vector<string> alloted_moves = {"U2", "D2", "F2", "B2", "L2", "R2"}; // alloted moves for G2
    unordered_map<uint64_t, uint32_t> pruning_map;

    vector<Edge> edges;
    vector<Corner> corners;
    initialize_solved_state(edges, corners);

    Cube cube = Cube(edges, corners); // create a cube
    queue<Node> explored_states; // create a search vector to house the states to be searched

    Node node;
    node.state = make_pair(cube.get_edges(), cube.get_corners());
    node.depth = 0;
    explored_states.push(node);

    uint64_t edge_key = G3_encode_edge_permutations(cube.get_edges()); // make use of overflow mechanism to get keys
    uint64_t corner_key = G3_encode_corner_permutations(cube.get_corners());

    string val = to_string(edge_key);
    string ue = to_string(corner_key);
    string value =  val + "0" + ue;
    uint64_t key = large_string_to_int(value);

    pruning_map.insert({key, 0});

    unordered_set<uint64_t> keys_visited;

    while(explored_states.size() > 0){

        Node current_state = explored_states.front();
        explored_states.pop();
        cube.set_cube(current_state.state.first, current_state.state.second);

        for(size_t i = 0; i < alloted_moves.size(); ++i){
            string move = alloted_moves[i];
            cube.algorithm(move);
            Node new_state;
            new_state.state = make_pair(cube.get_edges(), cube.get_corners());
            new_state.depth = current_state.depth + 1;

            edge_key = G3_encode_edge_permutations(cube.get_edges()); // make use of overflow mechanism to get keys
            corner_key = G3_encode_corner_permutations(cube.get_corners());

            val = to_string(edge_key);
            ue = to_string(corner_key);
            value =  val + "0" + ue;
            key = large_string_to_int(value);
            
            keys_visited.insert(key);
            auto it = pruning_map.find(key);
            if(it == pruning_map.end()){
                pruning_map.insert({key, new_state.depth});
                explored_states.push(new_state);
            }
            
            // Attempt to prune some states that we do need to explore away
            string inverted_moves = get_inverse_moves(move);
            cube.algorithm(inverted_moves);
        }
        cout << pruning_map.size() << " " << explored_states.size() << '\n';
    }

    cout << pruning_map.size() <<'\n';
    return pruning_map;

}

int main(int argc, char* argv[]){
    if(argc == 0){
        cout << "Not enough arguments!\n";
    }
    if(strcmp(argv[1], "G0") == 0){
        cout << "Calculating Pruning Table for G0\n";
        unordered_map<uint64_t, uint32_t> pruning_map = G0_pruning_table(); // get the pruning table
        for(const auto& pair : pruning_map){
            cout << pair.first << " " << pair.second << '\n';
        }
    }
    else if(strcmp(argv[1], "G1") == 0){
        cout << "Calculating Pruning Table for G1\n";
        unordered_map<uint64_t, uint32_t> pruning_map = G1_pruning_table();
        for(const auto& pair : pruning_map){
            cout << pair.first << " " << pair.second << '\n';
        }
    }
    else if(strcmp(argv[1], "G2") == 0){
        cout << "Calculating Pruning Table for G2\n";
        unordered_map<uint64_t, uint32_t> pruning_map = G2_pruning_table();
        // for(const auto& pair : pruning_map){
        //     cout << pair.first << " " << pair.second << '\n';
        // }
    }
    else if(strcmp(argv[1], "G3") == 0){
        cout << "Calculating Pruning Table for G3\n";
        unordered_map<uint64_t, uint32_t> pruning_map = G3_pruning_table();
        // for(const auto& pair : pruning_map){
        //     cout << pair.first << " " << pair.second << '\n';
        // }
    }
    else{
        cout << "Case Not Found\n";
    }
}