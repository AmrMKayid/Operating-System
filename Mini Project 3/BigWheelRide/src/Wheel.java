import java.util.ArrayList;

public class Wheel extends Thread {

    int capacity;
    public int playersCount;
    ArrayList<Player> playersList;
    int maxWaiting;

    public Wheel(int maxWaiting) {
        this.capacity = 5;
        this.playersCount = 0;
        playersList = new ArrayList<>();
        this.maxWaiting = maxWaiting;
    }


    public synchronized int getPlayerCount() {
        return playersCount;
    }

    public synchronized void load_players(Player p) {
        playersList.add(p);
        System.out.printf("Player %d on board, capacity: %d\n", p.playerID, ++playersCount);
    }

    public synchronized void end_ride() {

        if (playersCount == 5)
            System.err.println("Wheel is full, Let's go for a ride");

        System.err.println("Threads in this ride are: ");

        for (Player player : playersList) {
            System.out.print(player.playerID + ", ");
        }

        System.out.println("\n");
        playersList.clear();
        Operator.incMyCounter(playersCount);
        playersCount = 0;

        System.out.println("wheel end sleep\n");

    }

    @Override
    public void run() {
        while (true) {
            try {
                System.out.println("wheel start sleep");
                sleep(maxWaiting);
            } catch (InterruptedException e) {
            }

            end_ride();
        }
    }
}
