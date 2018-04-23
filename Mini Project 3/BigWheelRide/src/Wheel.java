import java.util.ArrayList;

public class Wheel extends Thread {

    int capacity;
    int playersCount;
    ArrayList<Player> playersList;
    int maxWaiting;
    boolean isRunning;

    public Wheel(int maxWaiting) {
        this.capacity = 5;
        this.playersCount = 0;
        playersList = new ArrayList<>();
        this.maxWaiting = maxWaiting;
        this.isRunning = true;
    }

    /**
     * adds a player thread to the list of on-board players.
     *
     * @param p
     */
    public synchronized void load_players(Player p) {
        playersList.add(p);
        p.onBoard = true;
        playersCount++;
        System.out.printf("Player %d on board, capacity: %d\n", p.playerID, this.playersCount);
    }

    /**
     * empties the list of on-board players and puts the wheel to sleep until the next ride is run.
     */
    public void end_ride() {
        if (playersCount == 5) {
            System.out.println("Wheel is full, Let's go for a ride");
        }

        System.out.println("Threads in this ride are: ");

        for (Player player :
                playersList) {
            try {
                synchronized (player) {
                    System.out.print(player.playerID + ", ");
                    player.notify();
                }
            } catch (Exception e) {
                System.out.println(e);
            }


        }
        System.out.println();
        playersList.clear();
        playersCount = 0;
        System.out.println("wheel end sleep\n");
    }

    @Override
    public void run() {
        while (isRunning) {
            System.out.println("wheel start sleep");
            try {
                sleep(maxWaiting);
            } catch (Exception e) {
            }
            end_ride();
        }
    }
}
