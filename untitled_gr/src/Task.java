import java.io.*;
import java.util.Arrays;
import java.util.Comparator;

class Work implements Comparable<Work> {
    long s;
    long p;
    int num;

    public Work(long s, long p, int num) {
        this.s = s;
        this.p = p;
        this.num = num;
    }

    public long getS() {
        return s;
    }

    public long getP() {
        return p;
    }

    public int getNum() {
        return num;
    }

    @Override
    public int compareTo(Work o) {
        if (this.getP() != o.getP())
            return (int)(o.getP() - this.getP());
        else
            return (int)(o.getS() - this.getS());
    }
}

public class Task {
    public static void main(String[] args) {
        try {
            StreamTokenizer st = new StreamTokenizer(new BufferedReader(new FileReader("input.txt")));
            st.nextToken();
            int n = (int) st.nval;
            Work[] works = new Work[n];
            int s, p;
            for (int i = 0; i < n; i++) {
                st.nextToken();
                s = (int)st.nval;
                st.nextToken();
                p = (int)st.nval;
                works[i] = new Work(s, p, i + 1);
            }
            Arrays.sort(works);
            boolean processor1 = true, processor2 = true;
            long time1 = 0, time2 = 0;
            long time = 0;
            for (int i = 0; i < n - 1; i++) {
                if (processor1 == true) {
                    time2 = Math.max(time1 + works[i].getS(), time2);
                    time1 += works[i].getS() + works[i].getP();
                }
                else if (processor2 == true) {
                    time1 = Math.max(time2 + works[i].getS(), time1);
                    time2 += works[i].getS() + works[i].getP();
                }
                time = Math.min(time1, time2);
                if (time1 > time) {
                    processor1 = false;
                    processor2 = true;
                }
                else if (time2 > time) {
                    processor2 = false;
                    processor1 = true;
                }
                else {
                    processor2 = true;
                    processor1 = true;
                }
            }
            if (processor1 == true)
                time1 += works[n - 1].getS() + works[n - 1].getP();
            else if (processor2 == true)
                time2 += works[n - 1].getS() + works[n - 1].getP();
            time = Math.max(time1, time2);
            PrintWriter writer = new PrintWriter("output.txt");
            writer.println(time);
            writer.print(works[0].getNum());
            for (int i = 1; i < n; i++) {
                writer.print(" ");
                writer.print(works[i].getNum());
            }

            writer.close();
        }
        catch (IOException e) {
            System.out.println("exc " + e);
        }
    }
}
