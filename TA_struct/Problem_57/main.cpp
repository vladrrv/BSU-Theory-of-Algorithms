#include <fstream>
using namespace std;

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int n = 0, c = 0, m = 0, x = 0, h = 0;
    infile >> m >> c >> n;
    int* table = new int[m];
    fill_n(table, m, -1);
    for (int i = 0; i < n; ++i) {
        infile >> x;
        h = x % m;
        while (table[h] > -1) {
            h += c;
            h %= m;
            if (table[h] == x) break;
        }
        table[h] = x;
    }
    for (int i = 0; i < m; ++i) {
        outfile << table[i] << ((i+1 < m)? " " : "");
    }
    outfile.close();
    return 0;
}