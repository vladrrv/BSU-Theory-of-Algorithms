#include <fstream>
#include <queue>
#include <list>
using namespace std;

const int MAXR = 100;
const int MAXN = 100;

struct Vertex {
    int number;	//номер остановки
    int sum;
    int prevBus;
    int prevStop;
    int bus;
    explicit Vertex(int a = 0, int b = 0, int c = 0, int bus = 0, int prStop = 0):
            number(a),
            sum(b),
            prevBus(c),
            bus(bus),
            prevStop(prStop) {}
    bool operator<(const Vertex& x) const {
        return sum > x.sum;
    }
};

int main() {
    ifstream infile("in.txt");
    ofstream outfile("out.txt");
    int n, r, a, b;
    infile >> n >> r >> a >> b;
    list<int> busStop[MAXN];	// list of bus from station
    list<int> stops[MAXR][MAXN];  //для конкретного автобуса, список остановок, куда он может попасть с конкретной остановки
    int prev[MAXR][MAXN][2]; //0- bus  1 - stop
    bool was[MAXR][MAXN];
    for (int i = 1; i <= r; ++i) {
        int k;
        infile >> k;
        int stop;
        int prevStop =  -1;
        for (int j = 0; j < k; ++j) {
            infile>>stop;
            busStop[stop].push_back(i);
            /*	if (prevStop != -1)
                    busStop[prevStop].push_back(i);
    */
            if (prevStop != -1)
                stops[i][stop].push_back(prevStop);
            if (prevStop != -1)
                stops[i][prevStop].push_back(stop);
            prevStop = stop;
        }
    }
    priority_queue<Vertex> q;

    for (auto it = busStop[a].begin(); it != busStop[a].end(); ++it) {
        int bus = *it;
        q.push(Vertex( a, 0, -1, bus, -1));
        prev[bus][a][0] = -1;
        prev[bus][a][1] = -1;
        was[bus][a] = true;
    }
    while (!q.empty()) {
        Vertex v = q.top();
        q.pop();
        was[v.bus][v.number] = 1;
        prev[v.bus][v.number][0] = v.prevBus;
        prev[v.bus][v.number][1] = v.prevStop;
        if (v.number == b) {
            outfile<<v.sum<<endl;
            int nowBus = v.bus;
            int nowStop = v.number;
            while (nowBus != -1) {
                outfile << "StopNo " << nowStop << " BusNo " << nowBus << endl;
                int tmpBus = nowBus;
                nowBus = prev[nowBus][nowStop][0];
                nowStop = prev[tmpBus][nowStop][1];
            }
            return 0;
        }
        for (auto it = busStop[v.number].begin(); it != busStop[v.number].end(); ++it) {
            int bus = *it;
            for (auto stopList = stops[bus][v.number].begin(); stopList != stops[bus][v.number].end(); ++stopList) {
                int bonus = 1;
                if (bus != v.bus) bonus = 4;
                int stop = *stopList;
                if (was[bus][stop]) continue;
                Vertex temp = Vertex( stop, v.sum + bonus, v.bus, bus, v.number);
                q.push(temp);
            }
        }
    }
    outfile << "NoWay";
    return 0;
}