#include <fstream>
#include <vector>
#include <queue>
using namespace std;

int *in;
int *out;
struct my {
    int x, c;
    bool operator<(const my &o) const {
        int da = out[x] - in[x], db = out[o.x] - in[o.x];
        return da < db;
    }
};

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int n = 0, m = 0, k = 0;
    infile >> n >> m;
    vector<int> *lists_out = new vector<int>[n];
    vector<int> *lists_in = new vector<int>[n];
    int *ans = new int[n];
    vector<int> s1;
    vector<int> s2;
    in = new int[n];
    out = new int[n];
    bool *used = new bool[n];
    fill_n(in, n, 0);
    fill_n(out, n, 0);
    fill_n(used, n, false);
    for (int i = 0; i < m; ++i) {
        int x, y, t;
        infile >> x >> y >> t; --x; --y;
        if (t == 2) {
            lists_out[x].push_back(y);
            ++out[x];
            lists_in[y].push_back(x);
            ++in[y];
        } else {
            lists_out[y].push_back(x);
            ++out[y];
            lists_in[x].push_back(y);
            ++in[x];
        }
    }
    queue<int> q_src, q_snk;
    priority_queue<my> q;
    int *count = new int[n];
    fill_n(count, n, 0);
    for (int i = 0; i < n; ++i) {
        q.push({i,0});
        if (out[i] == 0) {
            q_snk.push(i);
        }
        if (in[i] == 0) {
            q_src.push(i);
        }
    }
    int rem = n, thrown = 0;
    while (rem > 0) {
        while (!q_snk.empty()) {
            int p = q_snk.front(); q_snk.pop();
            if (used[p]) continue;
            used[p] = true;
            s2.push_back(p);
            --rem;
            for (int v : lists_in[p]) {
                if (!used[v]) {
                    --out[v];
                    if (out[v] == 0)
                        q_snk.push(v);
                    else {
                        ++count[v];
                        q.push({v, count[v]});
                    }
                }
            }
        }
        while (!q_src.empty()) {
            int p = q_src.front(); q_src.pop();
            if (used[p]) continue;
            used[p] = true;
            s1.push_back(p);
            --rem;
            for (int v : lists_out[p]) {
                if (!used[v]) {
                    --in[v];
                    if (in[v] == 0)
                        q_src.push(v);
                    else {
                        ++count[v];
                        q.push({v, count[v]});
                    }
                }
            }
        }
        while (!q.empty() && (used[q.top().x] || count[q.top().x] > q.top().c))
            q.pop();
        if (!q.empty()) {
            int p = q.top().x;
            q.pop();
            thrown += in[p];
            used[p] = true;
            s1.push_back(p);
            --rem;
            for (int v : lists_in[p]) {
                if (!used[v]) {
                    --out[v];
                    if (out[v] == 0)
                        q_snk.push(v);
                    else {
                        ++count[v];
                        q.push({v, count[v]});
                    }
                }
            }
            for (int v : lists_out[p]) {
                if (!used[v]) {
                    --in[v];
                    if (in[v] == 0)
                        q_src.push(v);
                    else {
                        ++count[v];
                        q.push({v, count[v]});
                    }
                }
            }
        }
    }
    int z = 0;
    for (int v : s2) {
        ans[z++] = v + 1;
    }
    for (auto it = s1.rbegin(); it != s1.rend(); ++it) {
        ans[z++] = (*it) + 1;
    }
    outfile << (m-thrown) << endl << (ans[0]);
    for (int i = 1; i < n; ++i) {
        outfile << " " << (ans[i]);
    }
    return 0;
}