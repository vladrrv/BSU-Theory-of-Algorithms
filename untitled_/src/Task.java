import java.util.Arrays;
import java.util.Random;

class Solver1 {
    String solve(String xs, String ys) {
        int n = 0;
        int m = 0;
        int i = 0;
        while (xs.charAt(i) == '0')
            i++;
        String x = xs.substring(i);
        n = x.length();
        i = 0;
        while (ys.charAt(i) == '0')
            i++;
        String y = ys.substring(i);
        m = y.length();
        int[][] arr = new int[n + 1][m + 1];
        for (i = 0; i < n + 1; i++)
            arr[i][0] = 0;
        for (i = 0; i < m + 1; i++)
            arr[0][i] = 0;
        String[] prev = new String[m + 1];
        String[] ans = new String[m + 1];
        Arrays.fill(ans, "");
        Arrays.fill(prev, "");
        for (i = 1; i < n + 1; i++) {
            for (int j = 1; j < m + 1; j++) {
                if (x.charAt(i - 1) == y.charAt(j - 1)) {
                    arr[i][j] = arr[i - 1][j - 1] + 1;
                    ans[j] = prev[j - 1] + x.charAt(i - 1);
                } else if (arr[i - 1][j] > arr[i][j -1]) {
                    arr[i][j] = arr[i - 1][j];
                    ans[j] = prev[j];
                }
                else if (arr[i - 1][j] < arr[i][j -1]) {
                    arr[i][j] = arr[i][j - 1];
                    ans[j] = ans[j - 1];
                }
                else {
                    arr[i][j] = arr[i - 1][j];
                    if (prev[j].compareTo(ans[j - 1]) > 0)
                        ans[j] = prev[j];
                    else
                        ans[j] = ans[j - 1];
                }
            }
            if (i != n) {
                for (int j = 0; j < m + 1; j++)
                    prev[j] = ans[j];
                Arrays.fill(ans, "");
            }
        }
        StringBuilder sb = new StringBuilder();
        sb.append(String.valueOf(arr[n][m]));
        sb.append(" ");
        sb.append(ans[m]);
        return sb.toString();
    }
}

class Solver2 {
    String solve(String xs, String ys) {
        int n = 0;
        int m = 0;
        int i = 0;
        while (xs.charAt(i) == '0')
            i++;
        String x = xs.substring(i);
        n = x.length();
        i = 0;
        while (ys.charAt(i) == '0')
            i++;
        String y = ys.substring(i);
        m = y.length();
        int[][] arr = new int[n + 1][m + 1];
        String[][] str = new String[n + 1][m + 1];
        for (i = 0; i < n; i++)
            str[i][0] = "";
        for (i = 0; i < m; i++)
            str[0][i] = "";
        for (i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) {
                if (x.charAt(i - 1) == y.charAt(j - 1)) {
                    arr[i][j] = 1 + arr[i - 1][j - 1];
                    str[i][j] = str[i - 1][j - 1] + x.charAt(i - 1);
                } else if (arr[i][j - 1] < arr[i - 1][j]) {
                    arr[i][j] = arr[i - 1][j];
                    str[i][j] = str[i - 1][j];
                } else if (arr[i][j - 1] > arr[i - 1][j]) {
                    arr[i][j] = arr[i][j - 1];
                    str[i][j] = str[i][j - 1];
                } else {
                    arr[i][j] = arr[i][j - 1];
                    int c = str[i][j - 1].compareTo(str[i - 1][j]);
                    if (c > 0)
                        str[i][j] = str[i][j - 1];
                    else
                        str[i][j] = str[i - 1][j];
                }
            }
        StringBuilder sb = new StringBuilder();
        sb.append(String.valueOf(arr[n][m]));
        sb.append(" ");
        sb.append(str[n][m]);
        return sb.toString();
    }
}

public class Task {
    public static void main(String[] args) {
        try {
            Solver1 solver1 = new Solver1();
            Solver2 solver2 = new Solver2();
            Random random = new Random();
            while (true) {
                String x = Integer.toBinaryString(random.nextInt(10000)+1);
                String y = Integer.toBinaryString(random.nextInt(10000)+1);
                String sol1 = solver1.solve(x, y);
                String sol2 = solver2.solve(x, y);
                if (!sol1.equals(sol2)) throw new Exception("x: "+x+"\n"+"y: "+y+"\n"+sol1+"\n"+sol2);
            }
        }
        catch (Exception e){
            System.out.println(e.getMessage());
        }
    }
}
