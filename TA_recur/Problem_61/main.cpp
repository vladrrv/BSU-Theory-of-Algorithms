#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct point {
    int x;
    int y;
    bool operator < (const point& p) const {
        return  (x==p.x)? y < p.y: x < p.x;
    }
    bool operator == (const point& p) const {
        return y == p.y && x == p.x;
    }
};
int binary_search(vector<point>& arr, int l, int r, int x, int y) {
    if (r >= l) {
        int
                mid = l + (r - l)/2,
                xc = arr[mid].x,
                yc = arr[mid].y;
        if (xc == x && yc == y)
            return mid;
        if (x < xc || (x == xc && y < yc))
            return binary_search(arr, l, mid-1, x, y);
        return binary_search(arr, mid+1, r, x, y);
    }
    return -1;
}

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    int
            r = 0,
            c = 0,
            n = 0;
    infile >> r >> c >> n;
    /*
    short** field = new short*[r];
    for (int i = 0; i < r; i++) {
        short* row = new short[c];
        fill(row, row+c, -1);
        field[i] = row;
    }*/
    point temp{0,0};
    vector<point> points;
    for (int i = 0; i < n; i++) {
        infile >> temp.x >> temp.y;
        points.push_back(temp);
    }
    sort(points.begin(), points.end());/*
    for (short i = 0; i < n; i++) {
        field[points[i].x-1][points[i].y-1] = i;
    }*/
    int** table = new int*[n];
    for (int i = 0; i < n; i++) {
        int* row = new int[n];
        fill_n(row, n, -1);
        table[i] = row;
    }
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            int prev_x = 2*points[i].x - points[j].x;
            int prev_y = 2*points[i].y - points[j].y;
            if (prev_x < 1 || prev_y < 1 || prev_y > c) {
                table[i][j] = 2;
                continue;
            }
            int k = binary_search(points, 0, i-1, prev_x, prev_y);
            //int k = field[prev.x-1][prev.y-1];
            if (k >= 0) {
                int index_i = (i < k)? i: k;
                int index_j = (i < k)? k: i;
                int t = table[index_i][index_j];
                if (t > 0) {
                    table[i][j] = t + 1;
                }
            }
        }
    }
    for (int i = n-1; i >= 0; i--) {
        for (int j = i-1; j >= 0; j--) {
            int prev_x = 2*points[i].x - points[j].x;
            int prev_y = 2*points[i].y - points[j].y;
            if (prev_x > r || prev_y > c || prev_y < 1) {
                table[i][j] = 2;
                continue;
            }
            int k = binary_search(points, i+1, n-1, prev_x, prev_y);
            //int k = field[prev.x-1][prev.y-1];
            if (k >= 0) {
                int index_i = (i > k)? i: k;
                int index_j = (i > k)? k: i;
                int t = table[index_i][index_j];
                if (t > 0) {
                    table[i][j] = t + 1;
                }
            }
        }
    }
    int max_len = 2;
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            int len_up = table[i][j], len_low = table[j][i], len = len_up+len_low-2;
            if (len_up > 0 && len_low > 0 && len > max_len) {
                max_len = len;
            }
        }
    }
    outfile << ((max_len > 2)? max_len : 0);
    outfile.close();
    return 0;
}