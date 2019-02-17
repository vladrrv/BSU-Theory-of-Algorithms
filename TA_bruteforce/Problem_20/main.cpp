#include <fstream>
using namespace std;

int *field;
bool *mask;
int m, n, nm;
ofstream outfile("output.txt");

bool check(int i) {
    int row = i / m, col = i % m;
    int j = i+1;
    if (mask[j] && field[j] < 0)
        return false;
    ++j;
    if ((col == m-4) && mask[j] && field[j] < 0)
        return false;
    j = i + m;
    if ((col == 0 || row == n-4) && mask[j] && field[j] < 0)
        return false;
    j += 3;
    if ((col == m-4 || row == n-4 && col > m-7) && mask[j] && field[j] < 0)
        return false;
    j = i+2*m;
    if ((row == n-4) && mask[j] && field[j] < 0)
        return false;
    j += 3;
    if ((row == n-4 && col > m-7) && mask[j] && field[j] < 0)
        return false;
    j = i+3*m+1;
    if ((row == n-4 || row > n-7 && col > m-5 && m<6) && mask[j] && field[j] < 0)
        return false;
    ++j;
    if ((row == n-4 && col == m-4 || row > n-7 && col == 0 && m<6) && mask[j] && field[j] < 0)
        return false;
    return true;
}
void place(int i) {
    int j = i+1;
    mask[j] = !mask[j];
    ++j;
    mask[j] = !mask[j];
    j = i+m;
    mask[j] = !mask[j];
    j += 3;
    mask[j] = !mask[j];
    j = i+2*m;
    mask[j] = !mask[j];
    j += 3;
    mask[j] = !mask[j];
    j = i+3*m+1;
    mask[j] = !mask[j];
    ++j;
    mask[j] = !mask[j];
}
void print_ans() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int x = field[i*m+j];
            bool y = mask[i*m+j];
            outfile << (y? 1: x) << " ";
        }
        outfile << endl;
    }
    outfile << endl;
}
void bruteforce(int i) {
    if (i == (n-3)*(m-3)) {
        print_ans();
        return;
    }
    int row = i/(m-3), col = i%(m-3), j = row*m+col;
    if (check(j))
        bruteforce(i + 1);
    place(j);
    if (check(j))
        bruteforce(i + 1);
    place(j);
}

int main() {
    ifstream infile("input.txt");
    infile >> n >> m; nm = n*m;
    field = new int[nm];
    mask = new bool[nm];
    fill_n(field, nm, 0);
    fill_n(mask, nm, false);
    for (int i = 0; i < nm; ++i) {
        infile >> field[i];
    }
    if (m > 3 && n > 3)
        bruteforce(0);
    else
        print_ans();
    return 0;
}
