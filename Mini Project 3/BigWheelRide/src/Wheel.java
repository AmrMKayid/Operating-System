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
        System.out.printf("Player %d on board, capacity: %d", p.playerID, playersCount);
    }

    /**
     * updates the state of on-board threads to ride-complete.
     *
     * @param p
     */
    public void run_ride(Player p) {

    }

    /**
     * empties the list of on-board players and puts the wheel to sleep until the next ride is run.
     */
    public void end_ride() {
        playersList.clear();
    }

    @Override
    public void run() {
        System.out.printf("wheel start sleep");
        try {
            sleep(maxWaiting);
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
