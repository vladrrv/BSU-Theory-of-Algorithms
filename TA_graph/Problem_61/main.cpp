#include <fstream>
#include <vector>
#include <queue>
using namespace std;

const long long MODULUS = 1000000007;
struct edge {
    int v, w, r;
    int adj(int x) {
        return x != v? v : w;
    }
};

int modpow(int exp) {
    long long result = 1;
    long long base = 2;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
            result %= MODULUS;
        }
        base *= base;
        base %= MODULUS;
        exp >>= 1;
    }
    return (int)result;
}
int main() {
    ifstream infile("labels.in");
    ofstream outfile("labels.out");
    int n, k, a, b, r, ans = 0;
    infile >> n;
    for (int i = 1; i < n; ++i) {
        infile >> a >> b;
    }
    infile >> k;
    edge*edges = new edge[k];
    vector<int>*lists = new vector<int>[n];
    for (int i = 0; i < k; ++i) {
        infile >> a >> b >> r;
        edges[i] = {--a,--b,r};
        lists[a].push_back(i);
        lists[b].push_back(i);
    }
    int*lbl = new int[n];
    int*e_path = new int[n];
    bool*visit = new bool[n];
    fill_n(lbl, n, -1);
    fill_n(e_path, n, -1);
    fill_n(visit, n, false);

    queue<int> q;
    int num_t = 0;
    for (int i = 0; i < n; ++i) {
        if (lbl[i] < 0) {
            ++num_t;
            q.push(i);
            lbl[i] = 0;
            while (!q.empty()) {
                int v = q.front(); q.pop();
                if (visit[v]) continue;
                visit[v] = true;
                for (const int&j : lists[v]) {
                    int w = edges[j].adj(v);
                    if (j == e_path[v]) continue;
                    int w_lbl = (lbl[v] + edges[j].r) % 2;
                    if (visit[w]) {
                        if (w_lbl != lbl[w]) {
                            outfile << 0;
                            return 0;
                        }
                    } else {
                        lbl[w] = w_lbl;
                        e_path[w] = j;
                        q.push(w);
                    }
                }
            }
        }
    }
    ans = modpow(num_t-1);
    outfile << ans;
    outfile.close();
    return 0;
}
