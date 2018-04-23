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

	
	
	public synchronized int getPlayerCount()
	{
		return playersCount;
	}

	public synchronized void load_players(Player p) {
		playersList.add(p);
		playersCount++;
		System.out.printf("Player %d on board, capacity: %d\n", p.playerID, playersCount);
	}

	
	public synchronized  void run_ride() {
		end_ride();
	}

	public synchronized void  end_ride() {
		System.out.println("Threads in this ride are: ");
		for (Player player : playersList) {
			System.out.print(player.playerID + ", ");
		}
		System.out.println();
		playersList.clear();
		Operator.incMyCounter(playersCount);
		playersCount = 0;
	}

	@Override
	public void run() {
		while(true)
		{
		try {
			System.out.println("wheel start sleep");
			sleep(maxWaiting);
			System.out.println("wheel end sleep");
		} catch (InterruptedException e) {
			System.out.println("Wheel is full, Let's go for a ride");
		}
		
		run_ride();

		}
	}
}
