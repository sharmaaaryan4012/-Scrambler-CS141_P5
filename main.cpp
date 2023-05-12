// Project 5: Scrambler word
// Developed by Aaryan Sharma (UIN - 661307552)
// Summary:

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

// created class scrambler with private data members and public functions
class Scrambler {

private:
    vector<string> board;
    vector<string> board_copy;
    vector<string> history;
    vector<string> board_history;
    string path;
    int size=0;
    vector<string> all_words;
    vector<string> sol;
    vector<string> initial_board;

public:

    Scrambler(const string& path, const int size); // Creates a new board for the game that init the board with words and jumble it
    void scrambler_init(); // inputs enough words in the board based on the size of the board (unscrambled)
    void jumble_board(); // jumbles the board using the provided algorithm

    string str1() const;

    void try_move(const string& cmd); // function try_move is created to shift the characters on the board
    void move(const char& rc, const int& idx, const char& dir); // moves the characters inside the board as required

    string str() const;
    string get_path(); // returns the private path data member
    void set_path(string path); // sets the path inputted by user to private member

    bool is_over() const; // function is over is created to check whether all the words were unscrambled or not
    string display_solution();
    vector<string> get_words() const; //class The scrambler requires a member function called get_words that returns a vector of strings containing every word on the scrambled board.

    void set_board();
    void get_board();
};

// no parameters returns string
string Scrambler::get_path() {
    return path;
}

// no parameters returns string
void Scrambler::get_board()
{
    string str;
    for (int i=0; i<size; i++) {
        str = board.at(i);
    }
}

// parameter is a string input for file name return nothing
void Scrambler::set_path(string path) {
    this->path = path;
}

string Scrambler::str1() const {
    string str = " ";
    int idx;
    int count = 1;

    for(int i = 0; i<size; i++){
        str +=  "   " + to_string(count) ;
        count++;
    }


    str += "\n   ";
    string dashes(size*4,'-');
    str += dashes + "\n";

    count = 1;
    for (int i=0; i<board_copy.size();i++) {
        str += to_string(count) + " | ";
        for (int j=0; j<board_copy.size()-1;j++) {
            str.push_back(board_copy.at(i).at(j));
            str +=  " | ";
            idx = j;
        }
        str.push_back(board_copy.at(i).at(idx+1));
        str +=  " |";

        str += "\n   ";
        str +=  dashes + "\n";
        count++;


    }
    str.pop_back();//removing the last new line character
    return str;
}


// parameters are the user cmd in broken format
void Scrambler::move(const char& rc, const int& idx, const char& dir) {

    if (tolower(rc) == 'r') {                                                                      // Moving the elements within a row(located at index "idx").
        if (tolower(dir) == 'l') {
            char last = (board.at(idx))[0];

            for (int i=0; i<size-1; i++) {
                (board.at(idx))[i] = (board.at(idx))[i+1];
            }
            (board.at(idx))[size-1] = last;
        }

        else if (tolower(dir) == 'r') {
            char first = (board.at(idx))[size-1];

            for (int i=size-1; i>0; i--){
                (board.at(idx))[i] = (board.at(idx))[i-1];
            }
            (board.at(idx))[0] = first;
        }
    }

    else if (tolower(rc) == 'c') {                                                                 // Moving the elements within a column(located at index "idx").
        if (tolower(dir) == 'u') {
            char bottom = (board.at(0))[idx];
            for (int i=0; i<size-1; i++) {
                (board.at(i))[idx] = (board.at(i+1))[idx];
            }
            board[size-1][idx] = bottom;
        }

        else if (tolower(dir) == 'd') {
            char top = (board.at(size-1))[idx];

            for (int i=size-1; i>0; i--){
                (board.at(i))[idx] = (board.at(i-1))[idx];
            }
            (board.at(0))[idx] = top;
        }
    }
    // updates the vector that maintains the log of moves used
    board_history.push_back(str());
}

// no parameter and no return type, changes private member for board
void Scrambler::scrambler_init() {
    for (int i = 0; i < size; i++) {
        int count = size;
        int word_count = 0;
        string rand_str;
        string temp_str;

        while (count > 2) {
            rand_str = all_words.at(rand() % all_words.size());

            if (rand_str.size() <= count && (count - rand_str.size()) != 3) {
                word_count++;
                if (word_count > 1) {
                    initial_board.push_back(rand_str);
                    temp_str += " " + rand_str;
                }

                else {
                    initial_board.push_back(rand_str);
                    temp_str += rand_str;
                    count--;
                }

                count -= temp_str.size();
            }
        }
        if (temp_str.size() < size) {
            for (int i=(temp_str.size()); i<size; i++) {
                temp_str.push_back(' ');
            }
        }
        board.push_back(temp_str);
    }
    // jumbles the board after putting words in
    jumble_board();
    // saving a copy of current state in case user wants to restart
    board_copy = board;
}

// no parameters and no return, updates the private data member for board
void Scrambler::jumble_board() {
    int nmoves = (rand() % 8) + 3;

    for (int i=0; i<nmoves; i++) {
        string history_str;
        int indx;
        char rc;
        char rev_dir;

        int idx = (rand() % size);
        indx = idx;
        int half_chance = (rand() % 2);

        if (i%2 != 0) {
            if (half_chance == 0) {
                rc = 'r';
                rev_dir = 'l';

                move('r', idx, 'r');
            }

            else if (half_chance == 1) {
                rc = 'r';
                rev_dir = 'r';

                move('r', idx, 'l');
            }
            history_str += rc + to_string(indx+1) + rev_dir;
            sol.push_back(history_str);
        }

        else {
            if (half_chance == 0) {
                rc = 'c';
                rev_dir = 'd';

                move('c', idx, 'u');
            }

            else if (half_chance == 1) {
                rc = 'c';
                rev_dir = 'u';

                move('c', idx, 'd');
            }
            history_str += rc + to_string(indx+1) + rev_dir;
            sol.push_back(history_str);
        }
    }
}

// Parameterized constructor: called when creating a object
Scrambler::Scrambler(const string& path, const int size) {

    if (size < 3 || size > 9) {
        cout << "Grid size must be between 3 and 9" << endl;
        exit(1);
    }
    else {
        this->size = size;
        this->path = path;
        string myline;
        ifstream myfile(path);

        // Creating a vector containing all words of the file(path)
        while (getline(myfile, myline)) {
            all_words.push_back(myline);
        }
        scrambler_init();
    }
}

// returns a string formatted board in an organized manner
string Scrambler::str() const {

    string str = " ";
    int idx;
    int count = 1;

    for(int i = 0; i<size; i++){
        str +=  "   " + to_string(count) ;
        count++;
    }

    str += "\n   ";
    string dashes(size*4,'-');
    str += dashes + "\n";
    count = 1;

    for (int i=0; i<board.size();i++) {
        str += to_string(count) + " | ";
        for (int j=0; j<board.size()-1;j++) {

            str.push_back(board.at(i).at(j));
            str +=  " | ";
            idx = j;
        }
        str.push_back(board.at(i).at(idx+1));
        str +=  " |";

        str += "\n   ";
        str +=  dashes + "\n";
        count++;
    }

    //removing the last new line character
    str.pop_back();
    return str;
}

// user command is the parameter which determines how the characters will move
void Scrambler::try_move(const string& cmd) {

    // if user chooses to restart the game
    if (cmd == "R" || cmd == "r") {
        board = board_copy;
    }
    else {
        int idx = (cmd[1]-'0')-1;
        move(cmd[0], idx, cmd[2]);
    }
}

// no parameters, returns true of game is over else returns false, check using the vector storing correct words.
bool Scrambler :: is_over () const{

    bool over = false;
    // setting variables
    int numofwords = get_words().size();
    int numofwordsfound = 0;

    // checking to see if words of both and equal
    for(int i = 0; i < numofwords; i++){
        for(int j = 0; j < board.size(); j++){
            if(board.at(j).find(get_words().at(i)) != string::npos){
                numofwordsfound += 1;
            }
        }
    }
    if(numofwords == numofwordsfound) {
        over = true;
    }
    return over;
}


// returns the vector containing correct words
vector<string> Scrambler::get_words() const {
    return initial_board;
}

// displays a series of boards showing how to solve the board from the stage where the game was just created and  jumbled
string Scrambler::display_solution() {

    vector<string>curr = board_copy;

    string return_str;
    int count = 1;
    return_str += str();
    return_str += "\n";

    for (int j=sol.size() - 1; j>=0; j--) {

        return_str += "*** Move " + to_string(count) + " (";
        return_str += sol[j][0];
        return_str += ",";
        return_str += sol[j][1];
        return_str += ",";
        return_str += sol[j][2];
        return_str += ")\n";

        try_move(sol.at(j));
        return_str += str();
        return_str += "\n";
        count ++;
    }

    board = curr;
    return return_str;
}

// main to be replaced by auto grader
int main() {

    srand(4);
    Scrambler x("dictionary.txt", 3);
    cout << x.display_solution();

    return 0;
}