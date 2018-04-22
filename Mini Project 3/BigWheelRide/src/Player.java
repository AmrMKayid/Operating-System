
public class Player extends Thread {

    int playerID;
    int waitingTime;
    boolean onBoard;
    boolean rideComplete;

    public Player(int playerID, int waitingTime) {
        this.playerID = playerID;
        this.waitingTime = waitingTime;
        this.onBoard = false;
        this.rideComplete = false;
    }

    @Override
    public void run() {
        // TODO Auto-generated method stub

        try {
            sleep(waitingTime);
        } catch (Exception e) {
            System.out.println(e);
        }

        System.out.printf("player wakes up: %d\n", playerID);

        System.out.printf("passing player: %d to the operator\n", playerID);


    }

}
