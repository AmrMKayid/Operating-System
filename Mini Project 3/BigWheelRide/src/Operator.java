import java.util.*;
import java.io.*;

public class Operator {

    static Wheel wheel;
    static ArrayList<Player> players;
    static int max_wait_time;

    public static void readCSV(String inputFile) {
        try {
            File file = new File(inputFile);
            FileReader fileReader = new FileReader(file);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            StringBuffer stringBuffer = new StringBuffer();
            String line;

            max_wait_time = Integer.parseInt(bufferedReader.readLine());
            players = new ArrayList<>(Integer.parseInt(bufferedReader.readLine()));

            while ((line = bufferedReader.readLine()) != null) {
                if (line.length() > 0) {
                    String[] playerInfo = line.split(",");
                    int id = Integer.parseInt(playerInfo[0]);
                    int waitingTime = Integer.parseInt(playerInfo[1]);
                    players.add(new Player(id, waitingTime));
                }
            }
            fileReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public static void main(String[] args) {
        // TODO Auto-generated method stub
        readCSV("resources/input-1.txt");

        wheel = new Wheel(max_wait_time);

        for (Player p :
                players) {
            p.start();
        }


    }

}
