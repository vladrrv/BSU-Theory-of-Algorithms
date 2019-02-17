#include <fstream>
#include <list>
using namespace std;

int main() {
    ifstream infile("input.in");
    ofstream outfile("output.out");
    int x, a, n, k;
    infile >> n >> k >> x; --x;
    bool**matrix = new bool*[n];
    list<int> ans;
    for (int i = 0; i < n; ++i) {
        matrix[i] = new bool[n];
        fill_n(matrix[i], n, false);
        for (int j = 0; j < k; ++j) {
            infile >> a;
            matrix[i][--a] = true;
        }
    }
    ans.push_back(x);
    bool*visit = new bool[n];
    fill_n(visit, n, false);
    visit[x] = true;
    while (ans.size() < n) {
        int f = ans.back(), i;
        for (i = 0; i < n; ++i) {
            if (matrix[f][i] && !visit[i]) {
                ans.push_back(i);
                visit[i] = true;
                break;
            }
        }
        if (i == n) {
            auto v_it = ans.begin(), w_it = ++ans.begin();
            while (w_it != ans.end()) {
                if (matrix[*v_it][f] && matrix[*w_it][f]) {
                    ans.pop_back();
                    ans.insert(w_it, f);
                    break;
                }
                ++v_it; ++w_it;
            }
        }
    }
    int last = ans.back();
    auto v_it = ans.begin(), w_it = ++ans.begin();
    while (!(matrix[*v_it][last] && matrix[*w_it][x])) {
        ++v_it; ++w_it;
    }
    outfile << (x+1);
    while (w_it != ans.end()) {
        outfile << " " << (*w_it + 1);
        ++w_it;
    }
    do {
        outfile << " " << (*v_it + 1);
    } while (v_it-- != ans.begin());
    return 0;
}
