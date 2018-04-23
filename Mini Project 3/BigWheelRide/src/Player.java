
public class Player extends Thread {

    Operator op;

    int playerID;
    int waitingTime;
    boolean onBoard;
    boolean rideComplete;

    public Player(int playerID, int waitingTime, Operator op) {
        this.playerID = playerID;
        this.waitingTime = waitingTime;
        this.op = op;
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

        op.passPlayer(this);

        try {
            synchronized (this) {
                this.wait();
            }
        } catch (Exception e) {
            System.out.println(e);
        }

    }

}
