import java.util.*;
import java.io.*;

public class Operator {

    Wheel wheel;
    ArrayList<Player> players;
    int max_wait_time;
    int finished = 0;

    public Operator() {

        readCSV("resources/input-1.txt");

        wheel.start();
        for (Player p :
                players) {
            p.start();
        }
    }

    public void readCSV(String inputFile) {
        try {
            File file = new File(inputFile);
            FileReader fileReader = new FileReader(file);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            StringBuffer stringBuffer = new StringBuffer();
            String line;

            max_wait_time = Integer.parseInt(bufferedReader.readLine());
            wheel = new Wheel(max_wait_time);
            players = new ArrayList<>(Integer.parseInt(bufferedReader.readLine()));

            while ((line = bufferedReader.readLine()) != null) {
                if (line.length() > 0) {
                    String[] playerInfo = line.split(",");
                    int id = Integer.parseInt(playerInfo[0]);
                    int waitingTime = Integer.parseInt(playerInfo[1]);
                    players.add(new Player(id, waitingTime, this));
                }
            }
            fileReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public synchronized void passPlayer(Player p) {

        while (wheel.playersCount == 5);

        System.out.printf("passing player: %d to the operator\n", p.playerID);
        wheel.load_players(p);

        if (wheel.playersCount == 5)
            this.wheel.interrupt();

        finished++;
        System.err.println(finished);
//        System.err.printf("Finished: %d, Size: %d\n", finished, players.size());
        if (finished == players.size()) {
            System.err.println("Finisheddddddd");
            this.wheel.isRunning = false;
        }

    }

    public static void main(String[] args) {
        // TODO Auto-generated method stub

        Operator op = new Operator();

    }

}
