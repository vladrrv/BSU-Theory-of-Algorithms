#include <fstream>
#include <vector>
#include <queue>
using namespace std;

struct edge {
    int v, w;
    int adj(int x) {
        return x != v? v : w;
    }
};

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int r, c, a, b;
    infile >> r >> c;
    edge*edges = new edge[c];
    int*ans = new int[c];
    vector<int>* lists = new vector<int>[r];
    int*path = new int[r];
    int*e_path = new int[r];
    bool*visit = new bool[r];
    fill_n(path, r, -1);
    fill_n(e_path, r, -1);
    fill_n(visit, r, false);
    for (int i = 0; i < c; ++i) {
        infile >> a >> b;
        edges[i] = {--a,--b};
        lists[a].push_back(i);
        lists[b].push_back(i);
    }
    queue<int> q;
    for (int i = 0; i < r; ++i) {
        if (!visit[i]) {
            bool detected = false;
            q.push(i);
            while (!q.empty()) {
                int v = q.front(); q.pop();
                if (visit[v]) continue;
                visit[v] = true;
                for (const int&j : lists[v]) {
                    int w = edges[j].adj(v);
                    if (j == e_path[v]) continue;
                    int old_w = w;
                    if (visit[w] && !detected) {
                        detected = true;
                        while (path[w] != -1) {
                            ans[e_path[w]] = path[w];
                            w = path[w];
                        }
                    }
                    q.push(w);
                    w = old_w;
                    path[w] = v;
                    e_path[w] = j;
                    ans[j] = w;
                }
            }
            if (!detected) {
                outfile << "No";
                return 0;
            }
        }
    }
    for (int i = 0; i < c; ++i) {
        outfile << (ans[i]+1) << " ";
    }
    outfile.close();
    return 0;
}
