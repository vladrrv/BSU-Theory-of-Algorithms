#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

FILE *fin = fopen("input.txt", "rt");
FILE *fout = fopen("output.txt", "wt");
int n, m;
int **field, **bans;
bool first = true;

void readField() {
    field = new int*[n];
    bans = new int*[n];
    for (size_t i = 0; i < n; i += 1) {
        field[i] = new int[m];
        bans[i] = new int[m];
        for (size_t j = 0; j < m; j += 1) {
            int read;
            fscanf(fin, "%d", &read);

            bans[i][j] = read;

            field[i][j] = 0;
        }
    }
}

void writeField() {
    for (size_t i = 0; i < n; i += 1) {
        for (size_t j = 0; j < m; j += 1) {
            fprintf(fout, "%d ", bans[i][j] == -1 ? -1 : field[i][j]);
        }

        if (i < n - 1) { fprintf(fout, "\n"); }
    }
}

void appendVar(int i, int j) {
    field[i][j + 1] = !field[i][j + 1];
    field[i][j + 2] = !field[i][j + 2];

    field[i + 3][j + 1] = !field[i + 3][j + 1];
    field[i + 3][j + 2] = !field[i + 3][j + 2];

    field[i + 1][j + 3] = !field[i + 1][j + 3];
    field[i + 2][j + 3] = !field[i + 2][j + 3];

    field[i + 1][j] = !field[i + 1][j];
    field[i + 2][j] = !field[i + 2][j];
}

bool wrong(int i, int j) {   
    return bans[i][j] * field[i][j] < 0;
}

bool isFieldValid() {
    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < m; j += 1) {
            if (bans[i][j] == -1 && field[i][j] == 1)
                return false;
        }
    }

    return true;
}

const int FIRST_COL = 0;
const int FIRST_ROW = 0;
int LAST_COL;
int LAST_ROW;
bool SMALL_FIELD;

bool validateVar(int i, int j) {  
    if (wrong(i, j + 1)) { return false; }  
    
    if (wrong(i, j + 2)     && (j == LAST_COL)) { return false; }

    if (wrong(i + 1, j)     && ((j == FIRST_COL) || (i == LAST_ROW))) { return false; }

    if (wrong(i + 1, j + 3) && ((j == LAST_COL) || ((i == LAST_ROW) && (j >= m-6)))) { return false; }
    
    if (wrong(i + 2, j + 3) && ((i == LAST_ROW) && (j >= m-6))) { return false; }
    
    if (wrong(i + 2, j)     && (i == LAST_ROW)) { return false; }

    if (wrong(i + 3, j + 1) && ((i == LAST_ROW) || ((i >= n-6) && (j == LAST_COL) && (SMALL_FIELD)))) { return false; }

    if (wrong(i + 3, j + 2) && (((j == LAST_COL) && (i == LAST_ROW)) || ((i >= n-6) && (j == FIRST_ROW) && SMALL_FIELD))) { return false; }

    return true;
}

void perebor(int i, int j) {
    if (i == n - 3) {
        if (isFieldValid()) {
            first ? first = false : fprintf(fout, "\n\n");
            writeField();
        }
    } else {
        if (j == m - 4) {
            perebor(i + 1, 0);
            

            appendVar(i, j);
            if (validateVar(i, j)) { perebor(i + 1, 0); }

            appendVar(i, j);
        } else {
            perebor(i, j + 1);

            appendVar(i, j);
            if (validateVar(i, j)) { perebor(i, j + 1); }
            
            appendVar(i, j);
        }
    }
}

int main()
{
    fscanf(fin, "%d%d", &n, &m);
    LAST_COL = m - 4;
    LAST_ROW = n - 4;
    SMALL_FIELD = m < 6;
    readField();

    if (n < 4 || m < 4) {
        writeField();
        return 0;
    }

    perebor(0, 0);
    return 0;
}
