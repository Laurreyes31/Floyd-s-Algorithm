/*******************************************************************************
 * Filename: shortestpaths.cpp
 * Author  : Laurent Reyes
 * Version : 1.0
 * Date    : December 7th, 2022
 * Description: Finds all the pairs shortest paths using Floyd's Algorithm.
 * Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>

using namespace std;

string alpha[] = {"NULL", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"}; //Creates alphabet vector that will be utilized in the rest of the code
int num_vertices; //Variable that checks how many vertices are in a specific instance
long INF = -1;
long** path_len = new long*;

int len(double val){ //Helper function that helps to find the length in the display table function of the max cell width
    int num = 1;
    while(val != 9 && val > 9){
        val = val / 10;
        num++;
    }
    return num;
}

void display_table(long** const matrix, const string &label,const bool use_letters = false){ //Display table equation given from the coding assignment
    long max_val = 0;
    cout << label << "\n";
    for(int i = 0; num_vertices > i; i++){
        for(int j = 0; num_vertices > j; j++){
            long cell = matrix[i][j];
            if(INF < cell && max_val < cell){
                max_val = matrix[i][j];
            }
        }
    }
    int max_cell_width = len(max(long(num_vertices), max_val));
    cout << ' ';

    for(int j = 0; num_vertices > j; j++){
    cout << setw(max_cell_width + 1) << char(j + 'A');
    }
    cout << endl;
    for(int i = 0; num_vertices > i; i++){
        cout << char(i + 'A');
        for(int j = 0; j < num_vertices; j++){
            cout << " " << setw(max_cell_width);

            if(matrix[i][j] == INF){
                cout << "-";
            }

            else if(use_letters){
                cout << char(matrix[i][j] + 'A');
            }

            else{
                cout << matrix[i][j];
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

void print(long ** const inter, int i, int j){ //Helper function that helps to print out the arrows to help illustrate path in final problem
    if(i == j){
        return;
    }

    else if(INF == inter[i][j]){
        cout << " -> " << char(j + 'A');
        return;
    }

    print(inter, i, inter[i][j]);
    print(inter, inter[i][j], j);
    return;
}

void method(long** const lst){ //This is Floyd's method 
    path_len = new long *[(num_vertices)];
    int a = 0;
    int b = 0;
    while(a < (num_vertices * 2) - num_vertices){
        path_len[a] = new long[(num_vertices * 2) - num_vertices];
        while(b < (num_vertices * 2) - num_vertices){
            path_len[a][b] = lst[a][b];
            b++;
        }
        b = 0;
        a++;
    }

    long** interm = new long*[(2 * num_vertices - (num_vertices))];
    for(; b < 2 * num_vertices - (num_vertices); b++){
        interm[b] = new long[2 * num_vertices - (num_vertices)];
        memset(interm[b], INF, sizeof(long) * ((2 * num_vertices) - num_vertices));
    }

    a = 0;
    b = 0;

    while(num_vertices > a){
        for(; b < num_vertices; b++){
            for(int j = 0; num_vertices > j; j++){
                if((j == b) || (j == a) || (a == b) || (path_len[a][j] == INF) || (path_len[b][a] == INF)){
                    continue;
                }

                if((path_len[b][j] == INF) || (path_len[b][j] > path_len[b][a] + path_len[a][j])){
                    path_len[b][j] = path_len[b][a] + path_len[a][j];
                    interm[b][j] = a;
                }

                else{
                    continue;
                }
            }
        }
        a++;
        b = 0;
    }
    

    display_table(path_len, "Path lengths:"); 
    display_table(interm, "Intermediate vertices:", true);

    for(int a = 0; num_vertices > a;a++){ //Help to print the movement of each path for example A -> B
        for(int b = 0; num_vertices > b; b++){
            cout << char(a + 'A') << " -> " << char(b + 'A') << ", distance: ";
            if(INF != path_len[a][b]){
                cout << path_len[a][b] << ", path: " << char(a + 'A');
            }
            else{
                cout << "infinity, path: none" << "\n"; //If there are an infinite number of paths, then it will show this specific case
                continue;
            }

            if(a == b){ //Lastly if a and b are equal, this means there are no more paths to take, so it ends the line and continues
                cout << endl;
                continue;
            }

            print(interm, a, b); //Uses print function to help illustrate the entire path of the vector
            cout << "\n";
        }
    }

    for(int i = 0; num_vertices > i; i++){

        delete [] interm[i];
        delete [] path_len[i];
    }

    delete[] interm; //Deletes array so function can be used again 
    delete [] path_len; //Deletes array so function can be used again

    
    path_len = NULL;
    interm = NULL;
}

bool verify(string w){
    unsigned int a = 0;
    while(a < w.length()){
        if(!isdigit(w[a])){
            return false;
        }
        a++;
    }
    return true;
}

int main(int argc, const char *argv[]){
    string alpha[] = {"NULL", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"}; //Creates this again so it can be utilized in main()
    if (argc > 2 || argc < 2){ //If 2 arguments are not given, then it displays this error messsage
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    ifstream input_file(argv[1]); //Creates an ifstream object

    if(!input_file){ //Checks to see if input file exists, and if not,  then prints error message.
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }

    input_file.exceptions(ifstream::badbit); //Creates a read error to exception pile the ifstream can handle
    string line;
    try{
        unsigned int lines = 1;
        getline(input_file, line);
        istringstream iss(line);
        if(!(iss >> num_vertices) || (num_vertices > 26 || num_vertices < 1)){
            cerr << "Error: Invalid number of vertices '" << line << "' on line " << lines << "." << endl;
            return 1;
        }
        else{
            iss >> num_vertices;
            lines++;
        }

        long ** matrix = new long*[(num_vertices)];
        int i = 0;
        while(num_vertices > i){
            matrix[i] = new long[num_vertices];
            memset(matrix[i], INF,sizeof(long) * num_vertices);
            i++;
        }
        lines = 1;
        while(getline(input_file, line)){
            lines++;
            string elem;
            istringstream stream(line);
            vector<string> numero;
            while(getline(stream, elem, ' ')){
                if (elem != " "){
                    numero.push_back(elem);
                }
            }

            if(numero.size() != 3){ //Checks to see if all edges are shown and if not, then it will display error
                cerr << "Error: Invalid edge data '" << line << "' on line " << lines << "." << endl;
                return 1;
            }

            if((numero[0][0] > 'A' + num_vertices - 1) || (numero[0][0] < 'A') || (numero[0].length() != 1)){
                cerr << "Error: Starting vertex '" << numero[0] << "' on line " << lines << " is not among valid values A-" << alpha[num_vertices] << "." << endl;
                return 1;
            }

            if ((numero[1][0] > 'A' + num_vertices - 1) || (numero[1][0] < 'A') || (numero[1].length() != 1)){
                cerr << "Error: Ending vertex '" << numero[1] << "' on line " << lines << " is not among valid values A-" << alpha[num_vertices] << "." << endl;
                return 1;
            }

            if(!verify(numero[2]) || stol(numero[2]) < 1){
                cerr << "Error: Invalid edge weight '" << numero[2] << "' on line " << lines << "." << endl;
                return 1;
            }

            matrix[numero[0][0] - 'A'][numero[1][0] - 'A'] = stol(numero[2]);

            numero.clear();
        }

        int b = 0;
        for(int a = 0; a < num_vertices; a++){
            while(num_vertices > b){
                if(a == b){
                    matrix[a][b] = 0;
                }
                b++;
            }
            b = 0;
        }

        display_table(matrix, "Distance matrix:", false);
        method(matrix);

        int a = 0;
        while(a < num_vertices){
            delete [] matrix[a];
            a++;
        }

        delete [] matrix;
        matrix = NULL;

        input_file.close();
    }

    catch(const ifstream::failure &f){
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }
    return 0;
}
