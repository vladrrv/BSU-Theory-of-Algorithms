#include <fstream>
#include <queue>
using namespace std;

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
    for (int l = 0; l < n; ++l) {
        if (arr[l] > 0) continue;
        queue<int> q;
        q.push(0);
        arr[l] = ++k;
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] && arr[j] == 0) {
                    arr[j] = ++k;
                    q.push(j);
                }
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        outfile << arr[i] << " ";
    }
    outfile.close();
    return 0;
}