#include <iostream>
#include <climits>
#include <iterator>
#include <set>
#include <stack>
#include <algorithm> //random_shuffle
#include <vector>
#include <queue> //queue
#include <map> //map
#include <fstream> //file processing
#include <sstream> //formatted string processing
#include <cstdlib> //atof and atoi
#include <utility> //pair and make_pair
#include <ctime>
#include <string> //string and to_string
using namespace std;
typedef vector<int> vecint;

struct State {
	int board[4][4];
	int x, y; // location of blank
};

int myrandom(int);
vecint randPuzzle();
int findBlank(vecint);
int numInversion(vecint);
bool isSolvable(vecint);


// -----------------------------------------------------------------------
// display the puzzle
// -----------------------------------------------------------------------

void display(const State& s) {
	cout << "The state of " << 15 << "-puzzle: " << endl;
	cout << "--------------------------------------------" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (s.board[i][j] == 0) cout << " * ";
			else {
				if (s.board[i][j] < 10) cout << " ";
				cout << s.board[i][j] << " ";
			}
		}
		cout << endl;
	}
	cout << "--------------------------------------------" << endl;
}

// -----------------------------------------------------------------------
// load the 663 pattern database
// -----------------------------------------------------------------------

map<string, int> database_234; // 2, 3, 4
map<string, int> database_156; // 1, 5, 6, 9, 10, 13
map<string, int> database_78; // 7, 8, 11, 12, 14, 15

void load_pattern_database_663() {
	ifstream myfile1("database234.txt");
	if (myfile1.is_open())
	{
		string line;
		while (getline(myfile1, line)) {
			istringstream ss(line);
			int distance;
			string position, dis;
			getline(ss, position, ' ');
			getline(ss, dis, ' ');
			distance = atoi(dis.c_str());
			database_234[position] = distance;
		}
		myfile1.close();
	}
	else cout << "Unable to open file1\n";

	ifstream myfile2("database15691013.txt");
	if (myfile2.is_open())
	{
		string line;
		while (getline(myfile2, line)) {
			istringstream ss(line);
			int distance;
			string position, dis;
			getline(ss, position, ' ');
			getline(ss, dis, ' ');
			distance = atoi(dis.c_str());
			database_156[position] = distance;
		}
		myfile2.close();
	}
	else cout << "Unable to open file2\n";

	ifstream myfile3("database7811121415.txt");
	if (myfile3.is_open())
	{
		string line;
		while (getline(myfile3, line)) {
			istringstream ss(line);
			int distance;
			string position, dis;
			getline(ss, position, ' ');
			getline(ss, dis, ' ');
			distance = atoi(dis.c_str());
			database_78[position] = distance;
		}
		myfile3.close();
	}
	else cout << "Unable to open file3\n";
}

// -----------------------------------------------------------------------

bool operator==(const State& lhs, const State& rhs) {
	if (!(lhs.x == rhs.x && lhs.y == rhs.y)) return false;
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) if (lhs.board[i][j] != rhs.board[i][j]) return false;
	return true;
}

bool operator<(const State& lhs, const State& rhs) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (lhs.board[i][j] < rhs.board[i][j]) return true;
			else if (lhs.board[i][j] > rhs.board[i][j]) return false;
		}
	}
	if (lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y)) return true;
	return false;
}

int h(const State& state) {
	string num2, num3, num4;
	string num1, num5, num6, num9, num10, num13;
	string num7, num8, num11, num12, num14, num15;
	string position1, position2, position3;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (state.board[i][j] == 2) num2 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 3) num3 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 4) num4 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 1) num1 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 5) num5 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 6) num6 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 9) num9 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 10) num10 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 13) num13 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 7) num7 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 8) num8 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 11) num11 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 12) num12 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 14) num14 = to_string(i) + to_string(j);
			else if (state.board[i][j] == 15) num15 = to_string(i) + to_string(j);
		}
	}
	position1 = num2 + num3 + num4;
	position2 = num1 + num5 + num6 + num9 + num10 + num13;
	position3 = num7 + num8 + num11 + num12 + num14 + num15;
	return database_234[position1] + database_156[position2] + database_78[position3];
}

// int h(State &s) {
//     int ret = 0;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             if (s.x == i && s.y == j) continue;
//             int v = s.board[i][j];
//             int x = (v - 1)/4, y = (v - 1)%4;
//             ret += abs(x-i) + abs(y-j);
//         }    
//     }
//     return ret;
// }



// idastar search

State goal, nongoal;
set<State> nongoals;
map<pair<int, int>, string> moves;
bool solvable;
string moved;
int exceed, costbound;
set<State> visited;
State start;
State node;

bool dfs(int depth) {
	int currcost = depth + h(node);
	if (currcost > costbound) {
		exceed = min(exceed, currcost);
		return false;
	}
	if (node == goal || nongoals.count(node) > 0) {
		solvable = (node == goal);
		return true;
	}

	visited.insert(node);

	for (auto it = moves.begin(); it != moves.end(); it++) {
		int dx = it->first.first, dy = it->first.second;
		if (0 <= node.x + dx && node.x + dx < 4 && 0 <= node.y + dy && node.y + dy < 4) {
			node.board[node.x][node.y] = node.board[node.x + dx][node.y + dy];
			node.board[node.x + dx][node.y + dy] = 0;
			node.x += dx; node.y += dy;
			if (visited.count(node) == 0) {
				moved += it->second;
				if (dfs(depth + 1)) return true;
				else moved.pop_back();
			}
			node.x -= dx; node.y -= dy;
			node.board[node.x + dx][node.y + dy] = node.board[node.x][node.y];  
			node.board[node.x][node.y] = 0;
		}
	}
	visited.erase(node);
	return false;
}

void solve_idastar() {
	costbound = h(start);
	bool success;
	do {
		exceed = INT_MAX;
		moved.clear();
		visited.clear();
		node = start;
		success = dfs(0);
		costbound = exceed;
	} while (!success);
	if (solvable) cout << moved << endl;
	else cout << "This puzzle is not solvable." << endl;
}


// -----------------------------------------------------------------------
// create a random and solvable 15 puzzle
// -----------------------------------------------------------------------

int myrandom(int i) { return rand() % i; }

vecint randPuzzle() {
	srand(unsigned(time(0)));
	vecint state;
	for (int i = 0; i < 16; i++) state.push_back(i);
	random_shuffle(state.begin(), state.end(), myrandom);
	return state;
	while (true) {
		random_shuffle(state.begin(), state.end(), myrandom);
		if (isSolvable(state)) return state;
	}
}

bool isSolvable(vecint state) { // 15 puzzle
	int line = 0; // line number from bottom
	for (int i = 0; i < state.size(); i++) {
		if (state[i] == 0) {
			line = 4 - i / 4;
			break;
		}
	}
	/* if n is even, puzzle instance is solvable if
	 * 1. the blank is on an even row counting from the bottom and number of inversions is odd.
	 * 2. the blank is on an odd row counting from the bottom and number of inversions is even.
	 */
	if (line % 2 == 0 && numInversion(state) % 2 == 1) return true;
	else if (line % 2 == 1 && numInversion(state) % 2 == 0) return true;
	return false;
}

int numInversion(vecint state) {
	int size = state.size();
	int num = 0;
	for (int i = 0; i < size - 1; i++) {
		if (state[i] == 0) continue;
		for (int j = i + 1; j < size; j++) {
			if (state[j] == 0) continue;
			if (state[i] > state[j]) num++;
		}
	}
	return num;
}

State rand_state() {
	vecint puzzle = randPuzzle();
	State state;
	for (int i = 0; i < puzzle.size(); i++) {
		int row = i / 4, column = i % 4;
		if (puzzle[i] == 0) {
			state.x = row;
			state.y = column;
		}
		state.board[row][column] = puzzle[i];
	}
	return state;
}

int main() {
	const clock_t begin_time_load = clock();
	load_pattern_database_663();
	cout << "Running time(seconds) of loading pattern database: " << float(clock() - begin_time_load) / CLOCKS_PER_SEC << endl;

	moves[make_pair(0, -1)] = "L"; moves[make_pair(0, 1)] = "R";
	moves[make_pair(-1, 0)] = "U"; moves[make_pair(1, 0)] = "D";

	const clock_t begin_time_rand_state = clock();
	start = rand_state();
	display(start);
	cout << "Running time(seconds) of creating random state: " << float(clock() - begin_time_rand_state) / CLOCKS_PER_SEC << endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			goal.board[i][j] = 4 * i + j + 1;
			nongoal.board[i][j] = 4 * i + j + 1;
		}
	}

	goal.board[3][3] = 0;
	nongoal.board[3][3] = 0;
	goal.x = 3;
	nongoal.x = 3;
	goal.y = 3;
	nongoal.y = 3;

	swap(nongoal.board[3][2], nongoal.board[3][1]);
	nongoals.insert(nongoal);
	swap(nongoal.board[3][2], nongoal.board[3][1]);

	swap(nongoal.board[0][0], nongoal.board[0][1]);
	nongoals.insert(nongoal);
	swap(nongoal.board[0][0], nongoal.board[0][1]);

	const clock_t begin_time_solve = clock();
	solve_idastar();
	cout << "Running time(seconds) of solving puzzle: " << float(clock() - begin_time_solve) / CLOCKS_PER_SEC << endl;


	return 0;
}