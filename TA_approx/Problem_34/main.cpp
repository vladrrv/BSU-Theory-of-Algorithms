#include <fstream>
#include <cmath>
#include <queue>
using namespace std;

struct point {
    int x, y;
};

void preorder(int n, int*path, int*ans, int&sz, int u) {
    ans[sz++] = u;
    for (int v = 0; v < n; ++v) {
        if (path[v] == u) {
            preorder(n, path, ans, sz, v);
        }
    }
}

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int n, cost = 0;
    infile >> n;
    point*pts = new point[n];
    int**arr = new int*[n];
    for (int i = 0; i < n; ++i) {
        arr[i] = new int[n];
        fill_n(arr[i], n, 0);
        int x, y;
        infile >> x >> y;
        pts[i] = {x, y};
    }
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            int
                    x = abs(pts[i].x-pts[j].x),
                    y = abs(pts[i].y-pts[j].y),
                    d = x+y;
            arr[i][j] = d;
            arr[j][i] = d;
        }
    }
    int*keys = new int[n];
    int*path = new int[n];
    bool*visit = new bool[n];
    fill_n(keys, n, -1);
    fill_n(path, n, -1);
    fill_n(visit, n, false);
    keys[0] = 0;
    for (int i = 0; i < n; ++i) {
        int min_key = -1, u = -1;
        for (int j = 0; j < n; ++j) {
            if (!visit[j] && keys[j] >= 0 && (keys[j] < min_key || min_key < 0)) {
                min_key = keys[j];
                u = j;
            }
        }
        visit[u] = true;
        for (int v = 0; v < n; ++v) {
            if (!visit[v] && arr[u][v] > 0 && (arr[u][v] < keys[v] || keys[v] < 0)) {
                path[v] = u;
                keys[v] = arr[u][v];
            }
        }
    }
    int*ans = keys, sz = 0;
    preorder(n, path, ans, sz, 0);
    for (int i = 0; i < n; ++i) {
        cost += arr[ans[i]][ans[(i+1)%n]];
    }
    outfile << cost << endl;
    for (int i = 0; i <= n; ++i) {
        outfile << (ans[i%n]+1) << " ";
    }
    return 0;
}
