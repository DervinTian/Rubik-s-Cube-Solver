# Rubik-s-Cube-Solver
A crude implementation of the famous Thistlethwaite's Algorithm for solving Rubik's Cubes implemented in C++. The script was also integrated into a macOS app in XCode using Swift.

# Run the Program
Firstly, the user will need to generate pruning tables. This can be done through the pruning_tables.cpp script. The user will need to run it with the command line argument either G0, G1, G2, and G3. The program will only accept one argument at a time. Save the results from each into a larger .txt file. This will be the pruning table values that will be used later.

Once the pruning tables are generated, the user will then go into the main.cpp file, compile and run the script with the following command line arguments. The pruning table .txt file as the first command line argument, and then the Rubik's Cube scramble in scramble notation enclosed in brackets (e.g. "F' U B2 L2 U' L2 U' R2 U B2 R2 U F' R B U' L2 R D L2"). Once this is done, just run the script and the program will output the solution in cube notation (e.g. R F D R Y R B R R B' R L D2 L L F R' R2 F2 Y2 F2 R' U2 R' D2 B2 R' L' R2 D2 R2 D2 B2 R2 B2 R2 D2 F2 U2 F2).
