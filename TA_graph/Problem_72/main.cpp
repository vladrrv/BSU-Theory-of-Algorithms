#include <fstream>
#include <vector>
#include <queue>
using namespace std;
const int MAX_N = 200000;

long long d[MAX_N];
int indices[MAX_N];
bool blocked[MAX_N];
struct vertex {
    int num;
    long long d;
    bool operator<(const vertex& v) const {
        return d > v.d;
    }
};
struct edge {
    int v;
    long long weight;
};
int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int n = 0, m = 0, a, b;
    long long weight;
    infile >> n >> m;
    vector<edge>* lists = new vector<edge>[n];
    for (int i = 0; i < m; ++i) {
        infile >> a >> b >> weight;
        lists[--a].push_back({--b, weight});
        lists[b].push_back({a, weight});
    }
    priority_queue<vertex> h;
    fill_n(d, n, LLONG_MAX);
    fill_n(blocked, n, false);
    d[0] = 0;
    h.push({0,0});
    for (int i = 1; i < n; ++i) {
        h.push({i,LLONG_MAX});
    }
    while (!h.empty()) {
        vertex w = h.top();
        h.pop();
        if (blocked[w.num]) continue;
        blocked[w.num] = true;
        for (const edge& u : lists[w.num]) {
            long long new_w = w.d + u.weight;
            if (indices[u.v] >= 0 && d[u.v] > new_w) {
                d[u.v] = new_w;
                h.push({u.v, new_w});
            }
        }
    }
    long long ans = d[n-1];
    outfile << ans;
    outfile.close();
    return 0;
}
