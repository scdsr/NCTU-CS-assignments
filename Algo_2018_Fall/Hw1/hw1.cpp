#include <limits.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

//#define DEBUG_BUILDTABLE
//#define FINISH_JOB

// Use INT_MAX to represent INFINITE
#define INF INT_MAX

void ReadFile();
void OutputResult(fstream&, int, vector<vector<int>>&, vector<int>&, int);
int ExtractMin(vector<vector<int>>&);
void Insert(vector<vector<int>>&, const int);
void ReplaceX(string&, const string&, const string&);

int main() {
    ReadFile();
    return 0;
}

void ReadFile() {
    int table_count = 0;
    fstream infile("input.txt", infile.in);
    fstream outfile("output.txt", outfile.out);
    string row_content;
    string insert;
    int used_method;  // 1 for insert, 2 for extract-min
    int minimum = 0;  // If use extract-min method, output minimum

    // Total iteration count
    infile >> table_count;

#ifdef DEBUG_BUILDTABLE
    cout << "Table count: " << table_count << "\n";
#endif
    for (int i = 0; i < table_count; i++) {
        vector<vector<int>> YoungsTable;
        int row_count = 0;
        int col_count = 0;

#ifdef DEBUG_BUILDTABLE
        cout << "Iter: " << i + 1 << "\n";
#endif
        // ========== If method = 1, Build Insert array =========
        vector<int> insert_arr;
        infile >> used_method;
        // Eat useless newlines
        infile >> ws >> ws;
        // Record insert numbers
        if (used_method == 1) {
            getline(infile, insert);
            stringstream stream(insert);
            int n;
            while (stream >> n) {
                insert_arr.push_back(n);
            }
        }
        // =======================================================
#ifdef DEBUG_BUILDTABLE
        cout << "Use method: " << used_method << "\n";
        cout << "Insert arr: ";
        for (int i = 0; i < insert_arr.size(); i++)
            cout << insert_arr[i] << " ";
        cout << "\n";
#endif

        while (getline(infile, row_content)) {
            row_count++;
            // ============== Convert x to INF ===========
            stringstream ss;
            ss.clear();
            ss << INF;
            string STR_INF = ss.str();
            ReplaceX(row_content, "x", STR_INF);
            // ===============================================
            if (row_content.length() == 0) break;
            vector<int> v;
            stringstream stream(row_content);
            int n;
            while (stream >> n) {
                v.push_back(n);
            }
            YoungsTable.push_back(v);
            col_count = v.size();
        }
        row_count -= 1;

#ifdef DEBUG_BUILDTABLE
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < col_count; j++) {
                cout << YoungsTable[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
#endif

        if (used_method == 1) {
            // Insert mode
            for (int i = 0; i < insert_arr.size(); i++)
                Insert(YoungsTable, insert_arr[i]);
        } else if (used_method == 2) {
            // Extract mode
            minimum = ExtractMin(YoungsTable);
        }
        OutputResult(outfile, used_method, YoungsTable, insert_arr, minimum);
        if (i != table_count - 1) outfile << "\n";
#ifdef FINISH_JOB
        if (used_method == 1) {
            cout << "Insert ";
            for (int i = 0; i < insert_arr.size(); i++)
                cout << insert_arr[i] << " ";
            cout << "\n";
        } else if (used_method == 2) {
            cout << "Extract-min " << minimum << "\n";
        }
        for (int i = 0; i < row_count; i++) {
            for (int j = 0; j < col_count; j++) {
                if (YoungsTable[i][j] == INF)
                    cout << "x";
                else
                    cout << YoungsTable[i][j];
                cout << " ";
            }
            cout << "\n";
        }
        cout << "\n";
#endif
    }
}

void OutputResult(fstream& outfile, int method, vector<vector<int>>& young,
                  vector<int>& insert_arr, int min) {
    int row_count = young.size();
    int col_count = young[0].size();

    // =========== Write to output.txt ==============
    if (method == 1) {
        // ======= Use: Insert mode ========
        outfile << "Insert ";
        for (int i = 0; i < insert_arr.size(); i++) {
            outfile << insert_arr[i];
            if (i != insert_arr.size() - 1) outfile << " ";
        }
        outfile << "\n";
        // =================================
    } else if (method == 2) {
        // ======= Use: Extract mode =======
        outfile << "Extract-min " << min << "\n";
        // =================================
    }

    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            if (young[i][j] == INF)
                outfile << "x";
            else
                outfile << young[i][j];
            if (j != col_count - 1) outfile << " ";
        }
        outfile << "\n";
    }
}

int ExtractMin(vector<vector<int>>& young) {
    int minimum = young[0][0];
    int row_count = young.size();
    int col_count = young[0].size();
    int row = 0;
    int col = 0;

    while (row < row_count && col < col_count) {
        int cur = young[row][col];
        int original_row = row;
        int original_col = col;
        int right = INF;
        int down = INF;

        if (row < row_count - 1) down = young[row + 1][col];
        if (col < col_count - 1) right = young[row][col + 1];

        if (right == INF && down == INF) {
            young[row][col] = INF;
            break;
        } else if (down <= right)
            row++;
        else
            col++;

        swap(young[row][col], young[original_row][original_col]);
    }
    return minimum;
}

void Insert(vector<vector<int>>& young, const int insert) {
    // Compare with left/up elem, if larger than insert,
    // Exchange insert with the elem position
    int row = young.size() - 1;
    int col = young[0].size() - 1;
    young[row][col] = insert;

    while (true) {
        if (row > 0 && col > 0) {
            if (young[row - 1][col] > young[row][col - 1] &&
                young[row - 1][col] > insert) {
                swap(young[row][col], young[row - 1][col]);
                row -= 1;
            } else if (young[row][col - 1] > young[row - 1][col] &&
                       young[row][col - 1] > insert) {
                swap(young[row][col], young[row][col - 1]);
                col -= 1;
            } else if (young[row - 1][col] == young[row][col - 1] &&
                       young[row - 1][col] > insert) {
                swap(young[row][col], young[row - 1][col]);
                row -= 1;
            } else
                break;
        } else if (row == 0 && col > 0) {
            if (young[row][col - 1] > insert) {
                swap(young[row][col], young[row][col - 1]);
                col -= 1;
            } else
                break;
        } else if (col == 0 && row > 0) {
            if (young[row - 1][col] > insert) {
                swap(young[row][col], young[row - 1][col]);
                row -= 1;
            } else
                break;
        } else {
            break;
        }
    }
}

void ReplaceX(string& str, const string& replace, const string& to) {
    // Replace x in input file
    // To INF
    if (replace.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(replace, start_pos)) != string::npos) {
        str.replace(start_pos, replace.length(), to);
        start_pos += to.length();
    }
}
