import java.util.ArrayList;

public class Wheel extends Thread {

    int capacity;
    int playersCount;
    ArrayList<Player> playersList;
    int maxWaiting;

    public Wheel(int maxWaiting) {
        this.capacity = 5;
        this.playersCount = 0;
        playersList = new ArrayList<>();
        this.maxWaiting = maxWaiting;
    }

    /**
     * adds a player thread to the list of on-board players.
     *
     * @param p
     */
    public void load_players(Player p) {
        playersList.add(p);
        p.onBoard = true;
        playersCount++;
        System.out.printf("Player %d on board, capacity: %d\n", p.playerID, playersCount);
    }

    /**
     * updates the state of on-board threads to ride-complete.
     *
     * @param p
     */
    public void run_ride(Player p) {
        p.onBoard = false;
        p.rideComplete = true;
    }

    /**
     * empties the list of on-board players and puts the wheel to sleep until the next ride is run.
     */
    public void end_ride() {
        System.out.println("Wheel is full, Let's go for a ride");
        System.out.println("Threads in this ride are: ");
        for (Player player :
                playersList) {
            System.out.print(player.playerID + ", ");
        }
        System.out.println();
        playersList.clear();
    }

    @Override
    public void run() {
        System.out.println("wheel start sleep");
        try {
            sleep(maxWaiting);
            end_ride();
            System.out.println("wheel end sleep");
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
