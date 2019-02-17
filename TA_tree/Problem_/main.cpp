#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct matrix_dim {
    int n;
    int m;
};
int main() {
	ifstream infile("input.txt");
	ofstream outfile("output.txt");
    int count = 0;
    infile >> count;
    int f[count][count];
    matrix_dim dims[count];
	for (int i = 0; i < count; i++) {
        matrix_dim dim{0,0};
        infile >> dim.n;
        infile >> dim.m;
        dims[i] = dim;
        f[i][i] = 0;
	}
    for (int l = 1; l < count; l++)
        for (int i = 0, j = l; j < count; i++, j++) {
            vector<int> g;
            for (int k = i; k < j; k++) {
                g.push_back(f[i][k] + f[k+1][j] + dims[i].n*dims[k].m*dims[j].m);
            }
            f[i][j] = *min_element(g.begin(), g.end());
        }
    outfile << f[0][count-1];
    return 0;
}