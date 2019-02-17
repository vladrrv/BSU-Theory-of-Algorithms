#include <fstream>
#include <queue>
using namespace std;

class black_box {
    priority_queue< long, vector<long>, less<> > queue1; //maximum
    priority_queue< long, vector<long>, greater<> > queue2; //minimum
    size_t i;
    size_t sz;
public:
    black_box() {
        i = 0;
        sz = 0;
    }
    void add(long el) {
        sz++;
        long min_top_el = el;
        if (!queue1.empty()) min_top_el = queue1.top();
        if (queue1.size() > i && min_top_el > el) {
            queue1.pop();
            queue1.push(el);
            queue2.push(min_top_el);
        }
        else if (queue1.size() > i) queue2.push(el);
        else queue1.push(el);
    }
    long get() {
        i++;
        long min_top_el = queue1.top();
        if (!queue2.empty()) {
            long max_top_el = queue2.top();
            queue2.pop();
            queue1.push(max_top_el);
        }
        return min_top_el;
    }
    size_t size() {
        return sz;
    }
};

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    black_box box;
    size_t n = 0, m = 0, k = 0;
    infile >> m >> n;
    long* a = new long[m];
    for (size_t i = 0; i < m; i++) {
        long ai = 0;
        infile >> ai;
        a[i] = ai;
    }
    for (size_t i = 0; i < n; i++) {
        size_t ui = 0;
        infile >> ui;
        while (k < ui) {
            box.add(a[k++]);
        }
        long res = box.get();
        outfile << res << ((i+1 < n)? " ": "");
    }
    outfile.close();
    return 0;
}
