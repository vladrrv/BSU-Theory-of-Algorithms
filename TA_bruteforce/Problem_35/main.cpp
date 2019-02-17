#include <fstream>
#include <cmath>
using namespace std;

int n;
double cost = 0;

double**remove_row_col(double **arr_src, int m, int row, int col) {
    double **arr = new double*[m-1];
    for (int i = 0, k = 0; i < m; ++i, ++k) {
        if (i == row) {
            --k;
            continue;
        }
        arr[k] = new double[m-1];
        for (int j = 0, l = 0; j < m; ++j, ++l) {
            if (j == col) {
                --l;
                continue;
            }
            arr[k][l] = arr_src[i][j];
        }
    }
    for (int i = 0; i < m-1; ++i) {
        bool fr = true, fc = true;
        for (int j = 0; j < m-1; ++j) {
            if (fr && arr[i][j] == -1) {
                fr = false;
            }
        }
        if (fr) {
            row = i;
        }
        for (int j = 0; j < m-1; ++j) {
            if (fc && arr[j][i] == -1) {
                fc = false;
            }
        }
        if (fc) {
            col = i;
        }
    }
    arr[row][col] = -1;
    return arr;
}

double min_row_col(double**arr, int m, int row, int col) {
    double r_min = -1, c_min = -1;
    for (int i = 0; i < m; ++i) {
        if ((col != i) && (arr[row][i] >= 0) && (arr[row][i] < r_min || r_min < 0)) {
            r_min = arr[row][i];
        }
        if ((row != i) && (arr[i][col] >= 0) && (arr[i][col] < c_min || c_min < 0)) {
            c_min = arr[i][col];
        }
    }
    return r_min + c_min;
}

void tsp(double**arr, int m, double lower_bound) {
    for (int i = 0; i < m; ++i) {
        double min_row = -1;
        for (int j = 0; j < m; ++j) {
            if (arr[i][j] >= 0 && (arr[i][j] < min_row || min_row < 0)) {
                min_row = arr[i][j];
            }
        }
        if (min_row < 0) continue;
        lower_bound += min_row;
        for (int j = 0; j < m; ++j) {
            if (arr[i][j] >= 0) {
                arr[i][j] -= min_row;
            }
        }
    }
    for (int j = 0; j < m; ++j) {
        double min_col = -1;
        for (int i = 0; i < m; ++i) {
            if (arr[i][j] >= 0 && (arr[i][j] < min_col || min_col < 0)) {
                min_col = arr[i][j];
            }
        }
        if (min_col < 0) continue;
        lower_bound += min_col;
        for (int i = 0; i < m; ++i) {
            if (arr[i][j] >= 0) {
                arr[i][j] -= min_col;
            }
        }
    }
    if (cost > 0 && lower_bound >= cost) return;
    if (m == 2) {
        cost = lower_bound;
        return;
    }
    double max_coef = -1; int c = -1, r = -1;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            if (arr[i][j] == 0) {
                double cur = min_row_col(arr, m, i, j);
                if (cur > max_coef) {
                    max_coef = cur;
                    r = i; c = j;
                }
            }
        }
    }
    tsp(remove_row_col(arr, m, r, c), m-1, lower_bound);
    arr[r][c] = -2;
    if (m == n) arr[c][r] = -2;
    tsp(arr, m, lower_bound);
}

struct point {
    double x, y;
};

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");
    infile >> n;
    point*pts = new point[n];
    double**arr = new double*[n];
    for (int i = 0; i < n; ++i) {
        arr[i] = new double[n];
        fill_n(arr[i], n, 0);
        double x, y;
        infile >> x >> y;
        pts[i] = {x, y};
    }
    for (int i = 0; i < n; ++i) {
        arr[i][i] = -1;
        for (int j = i+1; j < n; ++j) {
            double
                    x = pts[i].x-pts[j].x,
                    y = pts[i].y-pts[j].y,
                    d = sqrt(x*x+y*y);
            arr[i][j] = d;
            arr[j][i] = d;
        }
    }
    tsp(arr, n, 0);
    outfile << fixed << cost;
    return 0;
}
