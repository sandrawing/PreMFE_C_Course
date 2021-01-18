#include <fstream>
#include <iostream>
#include <random>
#include <ctime>

using namespace std;

//************************************************************************************

class Player {
public:
    const int* chipDistribution;
    Player* left_p;
    Player* right_p;
    int numChips;

    Player() {};
    Player(const int* chipDistribution) {
        chipDistribution = chipDistribution;
    }
    void setLeftPlayer(Player* p) {
        this->left_p = p;

    }
    Player* getLeftPlayer() {
        return this->left_p;
    }

    void setRightPlayer(Player* p) {
        this->right_p = p;
    }

    Player* getRightPlayer() {
        return this->right_p;
    }

    int getNumChips() {
        return numChips;
    }
    void setNumChips(int nc) {
        numChips = nc;
    }
    int addChips(int nc) {
        this->setNumChips(this->getNumChips() + nc);
        return this->getNumChips();
    }
    void single_die(int d, int &centralChips) {
        switch (chipDistribution[d - 1])
        {
        case 1:
            this->addChips(-1);
            this->left_p->addChips(1);
            break;
        case 2:
            this->addChips(-1);
            centralChips += 1;
            break;
        case 3:
            this->addChips(-1);
            this->right_p->addChips(1);
            break;
        default:
            break;
        }
    }
    int play(int d1, int d2, int d3) {
        int centralChips = 0;
        if (numChips >= 3) {
            single_die(d1, centralChips);
            single_die(d2, centralChips);
            single_die(d3, centralChips);
        }
        else if (numChips == 2) {
            single_die(d1, centralChips);
            single_die(d2, centralChips);
        }
        else if (numChips == 1) {
            single_die(d1, centralChips);
        }
        return centralChips;
    };
};


class Game {
public:
    long seed;
    const int* chipDistribution;
    int* chipcount;
    mt19937 mt;
    uniform_int_distribution<int> dis_unif;
    Player** players;
    int numPlayers;
    int first;
    Player first_p;
    int count;

    Game(int numPlayers, const int* chipDistribution, long seed): 
        numPlayers(numPlayers), chipDistribution(chipDistribution) {
        players = new Player * [numPlayers];
        for (int i = 0; i < numPlayers; i++) {
            players[i] = new Player(chipDistribution);
            players[i]->setLeftPlayer(players[(i + 1) % numPlayers]);
            players[i]->setRightPlayer(players[(i - 1) % numPlayers]);
        }
        mt19937 mt_rand(seed);
        mt = mt_rand;
        uniform_int_distribution<int> dis_unif(0, numPlayers - 1);
        first = dis_unif(mt);
    }
    virtual ~Game() {
        for (int i = 1; i < numPlayers; i++) { 
            delete& players[i]; 
        }
        delete[] players;
        delete[] chipcount;
    }
    int countPlayersWithChips() {
        count = 0;
        for (int i = 0; i < numPlayers; i++) {
            if (players[i]->getNumChips() != 0) {
                count += 1;
            }
        }
        return count;
    };
    int playRound(int startingPlayer) {
        int d1, d2, d3;
        int centralValue = 0;
        for (int i = 0; i < numPlayers; i++) {
            int index = (i + startingPlayer) % numPlayers;
            if (countPlayersWithChips() == 1) {
                break;
                cout << "early stopping" << endl;
            }
            d1 = dis_unif(mt);
            d2 = dis_unif(mt);
            d3 = dis_unif(mt);
            centralValue += players[index]->play(d1, d2, d3);
        }
        return centralValue;
    }
    const int* playGame(const int* startingChips, int maxRounds) {
        for (int i = 0; i < numPlayers; i++) {
            players[i]->setNumChips(startingChips[i]);
        }
        chipcount = new int[numPlayers];
        
        int centralValue = 0;
        for (int i = 0; i < maxRounds; i++) {
            centralValue += playRound(first);
        }
        if (countPlayersWithChips() == 1) {
            for (int i = 0; i < numPlayers; i++) {
                if (players[i]->getNumChips() > 0)
                    players[i]->addChips(centralValue);
            }
        }
        for (int i = 0; i < numPlayers; i++) {
            chipcount[i] = players[i]->getNumChips();
        }
        return chipcount;
    }
};


//************************************************************************************

void SimulateGame(const char* desc
	, int numPlayers
	, const int* chipDistribution
	, long seed
	, const int* startingChips
	, int maxRounds
	, ostream& outputStream)
{

	// create the game object
	Game game(numPlayers, chipDistribution, seed);

	// initialize our expected chips array
	double* expectedChips = new double[numPlayers];
	for (int i = 0; i < numPlayers; i++) {
		expectedChips[i] = 0;
	}

	double roundsWithWinner = 0;
	// run 100000 simulations
	for (int s = 0; s < 100000; s++) {

		// play a single game
		const int* playerChips = game.playGame(startingChips, maxRounds);

		// keep track of chips held at end of game
		for (int i = 0; i < numPlayers; i++) {
			expectedChips[i] += playerChips[i];
		}

		// was there a single winner?
		if (game.countPlayersWithChips() == 1) {
			roundsWithWinner += 1;
		}

		// for simulation iterations of 100, 1000, 10000 and 100000, write the output
		if ((s + 1) == 100 || (s + 1) == 1000 || (s + 1) == 10000 || (s + 1) == 100000) {
			cout << desc << ',' << (s + 1) << endl;
			outputStream << desc << ',' << (s + 1) << ',' << (roundsWithWinner / (s + 1));
			for (int i = 0; i < numPlayers; i++) {
				double ev = expectedChips[i] / (s + 1);
				outputStream << ',' << ev;
			}
			outputStream << endl;
		}

	}
}


int main()
{
	ofstream outfile;

	// open the file
	outfile.open("lcr_output.csv");

	// write a header
	outfile << "Game,MaxRounds,RoundsWithWinner";
	for (int i = 0; i < 9; i++) {
		outfile << ",Player" << (i + 1);
	}
	outfile << endl;

	// standard game
	int chipDistribution[] = { 0,0,0,1,2,3 };
	int playerChips[] = { 10, 10, 10, 10, 10, 10, 10, 10, 10 };
	SimulateGame("standard", 9, chipDistribution, (long)time(0), playerChips, 100, outfile);

	// standard game - one player has more
	int playerChipsTilted[] = { 5, 5, 5, 5, 50, 5, 5, 5, 5, 5 };
	SimulateGame("standard-tilted", 9, chipDistribution, (long)time(0), playerChipsTilted, 100, outfile);

	// game with greater chance of passing but no center 
	int chipDistributionNoCenter[] = { 0,0,1,1,3,3 };
	SimulateGame("nocenter", 9, chipDistributionNoCenter, (long)time(0), playerChips, 100, outfile);

	SimulateGame("nocenter-tilted", 9, chipDistributionNoCenter, (long)time(0), playerChipsTilted, 100, outfile);

	// game with greater chance of passing but no 'holding'
	int chipDistributionNoHold[] = { 1,1,2,2,3,3 };
	SimulateGame("nohold", 9, chipDistributionNoHold, (long)time(0), playerChips, 100, outfile);

	SimulateGame("nohold-tilted", 9, chipDistributionNoHold, (long)time(0), playerChipsTilted, 100, outfile);


	// close the file
	outfile.close();

	return 0;
}
