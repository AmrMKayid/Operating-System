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

    public void load_players(Player p) {
        // TODO: adds a player thread to the list of on-board players.

    }

    public void run_ride(Player p) {
        // TODO: updates the state of on-board threads to ride-complete.

    }

    public void end_ride() {
        // TODO: empties the list of on-board players and puts the wheel to sleep until the next ride is run.

    }

    @Override
    public void run() {

        try {
            sleep(maxWaiting);
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
