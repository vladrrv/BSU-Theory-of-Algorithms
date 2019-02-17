#include <fstream>
using namespace std;

void dfs(int n, bool** matrix, int* arr, int i, int& k) {
    if (arr[i] > 0) return;
    arr[i] = ++k;
    for (int j = 0; j < n; ++j) {
        if (matrix[i][j]) {
            dfs(n, matrix, arr, j, k);
        }
    }
}

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int n = 0;
    infile >> n;
    int* arr = new int[n];
    bool** matrix = new bool*[n];
    fill_n(arr, n, 0);

    for (int i = 0; i < n; ++i) {
        bool* row = new bool[n];
        for (int j = 0; j < n; ++j) {
            infile >> row[j];
        }
        matrix[i] = row;
    }
    int k = 0;
    for (int i = 0; i < n; ++i) {
        dfs(n, matrix, arr, i, k);
    }
    for (int i = 0; i < n; ++i) {
        outfile << arr[i] << " ";
    }
    outfile.close();
    return 0;
}