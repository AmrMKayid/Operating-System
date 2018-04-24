
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

        try {
            sleep(waitingTime);
            Operator.addToTheQueue(this);
        } catch (Exception e) {
            System.out.println(e);
        }


    }

}
