#include <fstream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

struct stop {
    short r, s;
};
struct mp {
    int v;
    short prev_r, prev_s;
};
int main() {
    ifstream infile("in.txt");
    ofstream outfile("out.txt");
    int n, start_stop, end_stop, m, x, time = 0;
    short r;
    infile >> n >> r >> end_stop >> start_stop;
    if (start_stop == end_stop) {
        outfile << 0;
        return 0;
    }
    vector<mp>*routes = new vector<mp>[2*r];
    vector<stop>*lists = new vector<stop>[n];
    queue<pair<stop,stop>> q; queue<char> d; //cur-prev
    for (short i = 0; i < r; ++i) {
        infile >> m;
        int temp;
        infile >> temp;
        short s = 0;
        if (temp == start_stop) {
            q.push({ {i,  s}, {-1, -1} });
            d.push(0);
            q.push({ {r + i, s}, {-1, -1} });
            d.push(0);
            routes[i].push_back({ temp, -1, -1 });
            routes[r+i].push_back({ temp, -1, -1 });
        } else {
            routes[i].push_back({ temp, -2, -2 });
            routes[r+i].push_back({ temp, -2, -2 });
        }
        ++s;
        for (int j = 1; j < m; ++j) {
            infile >> x;
            if (x == start_stop) {
                q.push({ {i,  s}, {-1, -1} });
                d.push(0);
                q.push({ {r + i, s}, {-1, -1} });
                d.push(0);
                routes[i].push_back({ x, -1, -1 });
                routes[r+i].push_back({ x, -1, -1 });
            } else {
                routes[i].push_back({ x, -2, -2 });
                routes[r+i].push_back({ x, -2, -2 });
            }
            ++s;
            lists[temp - 1].push_back({i, s});
            lists[x - 1].push_back({r + i, (short) (s - 1)});
            temp = x;
        }
    }
    while (!q.empty()) {
        int sz = (int)q.size();
        for (int i = 0; i < sz; ++i) {
            stop t = q.front().first, prev = q.front().second; q.pop();
            char del = d.front(); d.pop();
            short k = t.r, l = t.s;
            if (del == 0) {
                int v = routes[k][l].v;
                if (v == end_stop) {
                    outfile << time;
                    routes[k][l].prev_r = prev.r;
                    routes[k][l].prev_s = prev.s;
                    stack<stop> st;
                    while (t.r >= 0) {
                        st.push(t);
                        t = {routes[t.r][t.s].prev_r, routes[t.r][t.s].prev_s};
                    }
                    while (!st.empty()) {
                        t = st.top(); st.pop();
                        int rt = (st.empty()? t.r: st.top().r)%r+1;
                        outfile << endl << (routes[t.r][t.s].v) << " " << rt;
                    }
                    return 0;
                }
                if (t.r < r && l + 1 < routes[k].size() && routes[k][l+1].prev_r == -2) {
                    q.push({ {k, (short) (l + 1)}, t });
                    d.push(0);
                    routes[k][l+1].prev_r = t.r;
                    routes[k][l+1].prev_s = t.s;
                } else if (t.r >= r && l - 1 >= 0 && routes[k][l-1].prev_r == -2) {
                    q.push({ {k, (short) (l - 1)}, t });
                    d.push(0);
                    routes[k][l-1].prev_r = t.r;
                    routes[k][l-1].prev_s = t.s;
                }
                for (stop &u : lists[v-1]) {
                    if ((u.r == t.r && abs(u.s-l) == 1) || routes[u.r][u.s].prev_r > -2) continue;
                    q.push({ {u.r, u.s}, t });
                    d.push(3);
                }
            } else if (routes[k][l].prev_r == -2) {
                q.push({t, prev});
                d.push(--del);
                if (del == 0) {
                    routes[k][l].prev_r = prev.r;
                    routes[k][l].prev_s = prev.s;
                }
            }
        }
        ++time;
    }

    outfile << "NoWay";
    return 0;
}
