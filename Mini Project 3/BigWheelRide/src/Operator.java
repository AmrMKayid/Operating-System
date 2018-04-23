import java.util.*;
import java.io.*;

public class Operator {

	public static Wheel wheel;
	static ArrayList<Player> players;
	static int max_wait_time;
	static int maxCount = 0;
	static int myCounter = 0;
	public static ArrayList<Player> queue = new ArrayList<Player>();

	
	public static void readCSV(String inputFile) {
		try {
			File file = new File(inputFile);
			FileReader fileReader = new FileReader(file);
			BufferedReader bufferedReader = new BufferedReader(fileReader);
			String line;

			max_wait_time = Integer.parseInt(bufferedReader.readLine());
			maxCount = Integer.parseInt(bufferedReader.readLine());
			players = new ArrayList<>();

			while ((line = bufferedReader.readLine()) != null) {
				if (line.length() > 0) {
					String[] playerInfo = line.split(",");
					int id = Integer.parseInt(playerInfo[0]);
					int waitingTime = Integer.parseInt(playerInfo[1]);
					players.add(new Player(id, waitingTime));
				}
			}
			fileReader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {

		readCSV("resources/input-1.txt");

		wheel = new Wheel(max_wait_time);

		wheel.start();

		for (Player p : players) {
			p.start();
		}

		while (true) {
			
			
			if (!queue.isEmpty()) {
				addPlayerToTheWheel();
			}

			if (wheel.getPlayerCount() == 5) {
				wheel.interrupt();
			}
			

			if (myCounter == maxCount)
				System.exit(0);
		}

	}

	public synchronized static void addToTheQueue(Player player) 
	{
		System.out.printf("player wakes up: %d\n", player.playerID);
		System.out.printf("passing player: %d to the operator\n", player.playerID);
		queue.add(player);
	}
	
	public synchronized static void incMyCounter(int NumberOfPlayersOnTheWeel) 
	{
		myCounter += NumberOfPlayersOnTheWeel;
	}

	public synchronized static void addPlayerToTheWheel() 
	{
		wheel.load_players(queue.remove(0));
	}


}
