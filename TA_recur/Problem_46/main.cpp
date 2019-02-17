#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const int MaxN = 100002;
vector<pair<int, int>> text_A_indexed;
vector<int> text_B, buf(MaxN, INT32_MAX);
int N, M, Out;

int who(int k) {
    int left = 0, right = N-1;
    while (right - left > 1) {
        if (text_A_indexed[(left + right) >> 1].first > k) {
            right = (left + right) >> 1;
        }
        else {
            left = (left + right) >> 1;
        }
    }
    if (text_A_indexed[left].first == k) return text_A_indexed[left].second;
    else if (text_A_indexed[right].first == k) return text_A_indexed[right].second;
    else return -1;
}

void find_len(int k) {
    int left = 0, right = M, len;
    while (right - left > 1) {
        if (buf[(left + right) >> 1] > k) {
            right = (left + right) >> 1;
        }
        else {
            left = (left + right) >> 1;
        }
    }
    if (buf[left] > k) {
        len = left;
    }
    else {
        len = left + 1;
    }
    buf[len] = k;
    if (len > Out) {
        Out = len;
    }
}
void proc() {
    sort(text_A_indexed.begin(), text_A_indexed.end(), [](auto a, auto b) { return a.first < b.first;});
    for (int i = 0; i < M; i++) {
        text_B[i] = who(text_B[i]);
    }
    buf[0] = 0;
    for (int i = 0; i < M; i++) {
        if (text_B[i] > -1) find_len(text_B[i]);
    }
}


int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int n = 0, m = 0;
    infile >> n >> m;
    int temp;
    for (int i = 0; i < n; i++) {
        infile >> temp;
        text_A_indexed.emplace_back(temp, i);
    }
    for (int i = 0; i < m; i++) {
        infile >> temp;
        text_B.push_back(temp);
    }
    M = m;
    N = n;

    proc();
    outfile << Out;
    outfile.close();
    return 0;
}