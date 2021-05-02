/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			Outcome: Practice of the use of Linked Lists, Queues, Stacks and need of templates.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef SNAKES_H
#define SNAKES_H

#include <string>
#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
#include<cstdlib>
#include<Windows.h>
using namespace std;

//Smaller data types used as building blocks within the data structures---------------------------------------------
struct Tile {
	string val_tile;
	Tile* next, * previous, * up, * down;
	bool snake = false;
	bool ladder = false;

	Tile() {
		next = previous = up = down = NULL;		//Null in the beginning
		snake = ladder = false;					//only true for specified cases otherwise false for all
	}
};
template<typename T>
struct Player {
	Tile* player_pos;							//each player will point to the tile it is currently at(so we dont have to traverse from beginning each time)
	T player_id;								//can have unique numbers/characters/strings for each player 
	int colour;
};

//Data Structures----------------------------------------------------------------------------------------------------
template <class T>
class Queue {
	Player<T>* player;										//dynamic array of players
	int head, tail;											//will store index of head and tail of queue
	int total_players;										//total number of players in game i.e total capacity of queue
	void MovePlayer(Player<T>& p, int move_tiles) {			//to move player according to dice 
		for (int i = 0; i < move_tiles; i++) {
			if (p.player_pos->val_tile != "100") {
				if (p.player_pos->val_tile == "10" || p.player_pos->val_tile == "20" || p.player_pos->val_tile == "30" ||
					p.player_pos->val_tile == "40" || p.player_pos->val_tile == "50" || p.player_pos->val_tile == "60" ||
					p.player_pos->val_tile == "70" || p.player_pos->val_tile == "80" || p.player_pos->val_tile == "90")
					p.player_pos = p.player_pos->up;
				else
					p.player_pos = p.player_pos->next;
			}
		}
		//check for snakes and ladders
		//check snakes
		if (p.player_pos->snake) {
			if (p.player_pos->down == NULL) {
				if (p.player_pos->next->down != NULL) p.player_pos = p.player_pos->next;
				else if (p.player_pos->previous->down != NULL) p.player_pos = p.player_pos->previous;
			}
			while (p.player_pos->down != NULL) {
				p.player_pos = p.player_pos->down;
				if (p.player_pos->next->down != NULL) p.player_pos = p.player_pos->next;
				else if (p.player_pos->previous->down != NULL) p.player_pos = p.player_pos->previous;
			}
		}
		//check ladders
		else if (p.player_pos->ladder) {
			if (p.player_pos->up == NULL) {
				if (p.player_pos->next->up != NULL) p.player_pos = p.player_pos->next;
				else if (p.player_pos->previous->up != NULL) p.player_pos = p.player_pos->previous;
			}
			int edgecheck = 1;
			while (p.player_pos->up != NULL && edgecheck % 10 != 0) {
				p.player_pos = p.player_pos->up;
				if (p.player_pos->next->up != NULL) p.player_pos = p.player_pos->next;
				else if (p.player_pos->previous->up != NULL) p.player_pos = p.player_pos->previous;
				if (p.player_pos->val_tile != "L1" && p.player_pos->val_tile != "L2")	edgecheck = stoi(p.player_pos->val_tile);
				if (edgecheck % 10 == 0) {
					p.player_pos = p.player_pos->previous;
				}
			}
		}
	}

public:
	//initialization functions
	Queue() {
		head = tail = 0;
		player = NULL;
		total_players = 0;
	}
	void Initialize_Queue(int numPlayers) {
		player = new Player<T>[numPlayers];
		int c = 237;
		for (int i = 1, add = 0; i <= numPlayers; i++) {
			player[i - 1].player_id = i;
			player[i - 1].player_pos = NULL;			//players will not be on board until first 6 or 1 on dice
			player[i - 1].colour = c - add;				//to give diff colours to all the players
			add += 20;
		}
		head = 0;
		tail = numPlayers - 1;
		total_players = numPlayers;
	}
	void InitializePlayers(Tile* start) {
		if (player[head].player_pos == NULL)
		{
			player[head].player_pos = start;
		}
		else cout << "Player already initialized my dude 0.0\n";
	}

	//getter functions
	T front() const {
		return player[head].player_id;
	}
	T back() const {
		return player[tail].player_id;
	}
	int getHead() const {
		return head;
	}
	int getTail()const {
		return tail;
	}
	int getPlayerPosition() const {						//return current number of the tile player is on (function for player whose turn it is)
		if (player[head].player_pos != NULL) {
			string temp = player[head].player_pos->val_tile;		//use string to convert board tile values to integers to return
			int pos;
			if (temp == "S1" || temp == "S2" || temp == "L1" || temp == "L2") {		//incase player is on snake tail/ladder head then compute number of tile
				if (player[head].player_pos->next != NULL)temp = player[head].player_pos->next->val_tile;
				else temp = player[head].player_pos->up->val_tile;
				pos = stoi(temp);
				pos -= 1;
			}
			else pos = stoi(temp);
			return pos;
		}
		else return -1;											//if a player is not yet on board(no 6/1 on dice) then display -1 as position
	}
	int getPlayerPosition(int num) const {						//return current number of the tile player is on (for any player)
		if (player[num].player_pos != NULL) {
			string temp = player[num].player_pos->val_tile;		//use string to convert board tile values to integers to return
			int pos;
			if (temp == "S1" || temp == "S2" || temp == "L1" || temp == "L2") {		//incase player is on snake tail/ladder head then compute number of tile
				if (player[num].player_pos->next != NULL)temp = player[num].player_pos->next->val_tile;
				else temp = player[num].player_pos->up->val_tile;
				pos = stoi(temp);
				pos -= 1;
			}
			else pos = stoi(temp);
			return pos;
		}
		else return -1;											//if a player is not yet on board(no 6/1 on dice) then display -1 as position
	}
	Tile* returnPosition(int num) const {
		return player[num].player_pos;
	}
	T getPlayerNumber(int num) const {
		return player[num].player_id;							//show unique id of any player
	}
	T getPlayerNumber() const {
		return player[head].player_id;							//show unique id of player whos turn it is
	}
	int returnTotalPlayers()const {
		return total_players;
	}
	int getPlayerColour(int num)const {
		return player[num].colour;
	}


	//game control functions
	void Move(int numtomove) {
		MovePlayer(player[head], numtomove);					//movement should only be done at head of Queue as it will be turn of that player
	}
	bool is_empty() const {
		if (head == tail && head == -1) return true;			//if head and tail both -1 meaning nothing in queue
		else return false;
	}
	bool OnlyOnePlayerLeft() {						//will be used to end game as its min 2 player game
		if (head == tail)return true;
		else return false;
	}
	void AfterMoveRearrangeQueue() {
		head = (head + 1) % total_players;			//move head and tail instead of completely dequeing and enqueueing
		tail = (tail + 1) % total_players;			//entire elements of Queue as this would be simpler and more efficient
	}
	void ReverseQueue() {
		if (is_empty())	return;						//base case
		Player<T> storeTemp = dequeue();			//store each player information during recursive call
		ReverseQueue();
		enqueue(storeTemp);							//store player info back in queue during backtracking
	}
	void movetotestcaseposition(int move_tiles) {				//simply to pass traverse board test case
		for (int i = 0; i < move_tiles; i++) {
			if (player[head].player_pos->val_tile == "10" || player[head].player_pos->val_tile == "20" || player[head].player_pos->val_tile == "30" ||
				player[head].player_pos->val_tile == "40" || player[head].player_pos->val_tile == "50" || player[head].player_pos->val_tile == "60" ||
				player[head].player_pos->val_tile == "70" || player[head].player_pos->val_tile == "80" || player[head].player_pos->val_tile == "90")
				player[head].player_pos = player[head].player_pos->up;			//edge tile check																									
			else
				player[head].player_pos = player[head].player_pos->next;		//normal movement
		}
	}

	//enqueue or dequeue functions
	void enqueue(Player<T> p) {
		if (head == tail && tail == -1) {
			head = 0;
			tail = 0;
			player[tail] = p;
		}
		else if ((tail + 1) % total_players != head) {		//to make sure queue has free space before adding into queue
			tail += 1;
			player[tail] = p;
		}
		else cout << "ERROR:::Queue is already full,  please dequeue first.\n";
	}
	Player<T> dequeue() {								//simple dequeue function, does not alter size of queue array
		if (head == tail) {								//in case only one element in queue and need to completely empty queue
			Player<T> toReturn = player[head];
			head = tail = -1;
			return toReturn;
		}
		Player<T> toReturn = player[head];				//therwise simple head manipulation 
		head = (head + 1) % total_players;				//imp to note altho pointers used they are not dynamically allocated 
		return toReturn;								//so no need to delete manually, memory is still static
	}
	void DeletePlayer() {				//different from dequeue as it completely deletes a player and also decreases size of queue
		Queue<T> temp;
		temp.player = new Player<T>[total_players - 1];
		dequeue();
		for (int i = 0; i < total_players - 1; i++) {
			temp.player[i] = dequeue();
		}
		total_players -= 1;
		for (int i = 0; i < total_players; i++) {
			enqueue(temp.player[i]);
		}
		delete temp.player;
	}
	void decPlayers() {
		total_players -= 1;
	}

};

struct dice {
	int dice_num;
	dice* next;

	dice() {
		next = NULL;
		dice_num = 0;
	}
};
class RollStack {
	dice* dice_top;							//linked list implementation as alot more efficient than arrays
public:
	RollStack() {
		dice_top = NULL;
	}

	void push_roll(int value) {
		if (dice_top == NULL) {				//in case of empty stack
			dice_top = new dice;
			dice_top->dice_num = value;
		}
		else if (dice_top != NULL) {		//in case of non-empty stack
			dice* temp = new dice;
			temp->next = dice_top;
			dice_top = temp;
			dice_top->dice_num = value;
		}
	}
	int top() {
		if (dice_top != NULL) {
			return dice_top->dice_num;
		}
		else return 0;
	}
	void pop() {
		if (dice_top != NULL) {
			dice* toDelete = dice_top;
			dice_top = dice_top->next;
			delete toDelete;
		}
	}

	bool is_empty() {
		if (dice_top == NULL) return true;
		else return false;
	}
	void empty() {
		while (!is_empty()) {
			pop();						//if not empty keep poppin until empty :)
		}
	}

	~RollStack() {
		empty();						//if stack isnt empty, empty it
	}
};

//Main Game Function ------------------------------------------------------------------------------------------------
class Game {
	Queue<int> turnQueue;
	Tile* start;
	Tile* end;

	void initializeLaddersAndSnakes() {
		Tile* tempprevladder = start;
		Tile* tempnextladder = NULL;
		Tile* tempprevsnake = start;
		Tile* tempnextsnake = NULL;
		int altr = 1;																							//to deal with the zig zag movement of board

		//ladders initialization
		while (tempprevladder != NULL) {
			if (tempprevladder->val_tile == "L1" || tempprevladder->val_tile == "L2") {						//if any tile has ladder in it 
				for (int i = 0; i < 3; i++) {																	//all ladders are of 4 tiles(-1 as it is start tile so only 3 connections between them)
					if (i == 0) tempprevladder->ladder = true;												    //so only bottom of ladder is considered and when player reach the specific tile they move up
					if (tempprevladder->next->next != NULL) tempnextladder = tempprevladder->next->next;
					else tempnextladder = tempprevladder->next;
					while (tempnextladder->val_tile != "L1" && tempnextladder->val_tile != "L2") {				//move tempnextladder to the next ladder to be able to join the two
						if (tempnextladder->next != NULL) tempnextladder = tempnextladder->next;
						else tempnextladder = tempnextladder->up;
					}
					tempprevladder->up = tempnextladder;
					if (altr) {
						tempprevladder = tempprevladder->up->next;												//since ladders are not vertical but horizontal
						altr = 0;																				//furthermore, board is in zig-zag
					}
					else {																						//therefore need to alternate the direction of movement
						tempprevladder = tempprevladder->up->previous;											//when joining ladder tiles from two diff rows which is literally ever ladder tile
						altr = 1;
					}
				}
				if (tempprevladder->next != NULL) tempprevladder = tempprevladder->next;							//when reaching top of L1 we end up at its prev which would be problematic
			}
			if (tempprevladder->val_tile == "10" || tempprevladder->val_tile == "20" || tempprevladder->val_tile == "30" || tempprevladder->val_tile == "40" || tempprevladder->val_tile == "50"
				|| tempprevladder->val_tile == "60" || tempprevladder->val_tile == "70" || tempprevladder->val_tile == "80" || tempprevladder->val_tile == "90")
				tempprevladder = tempprevladder->up;
			else tempprevladder = tempprevladder->next;
		}

		//snakes initialization
		altr = 0;
		int snake1 = 0;
		while (tempprevsnake != NULL) {
			if (tempprevsnake->val_tile == "S1" || tempprevsnake->val_tile == "S2") {
				if (tempprevsnake->val_tile == "S1") snake1 = 1;
				for (int i = 0; i < 3; i++) {
					if (tempprevsnake->next->next != NULL) tempnextsnake = tempprevsnake->next->next;
					else tempnextsnake = tempprevsnake->next;
					while (tempnextsnake->val_tile != "S1" && tempnextsnake->val_tile != "S2") {
						if (tempnextsnake->next != NULL) tempnextsnake = tempnextsnake->next;
						else tempnextsnake = tempnextsnake->up;
					}
					if (i == 2) tempnextsnake->snake = true;
					if (altr) {
						if (!snake1) {
							tempnextsnake->previous->down = tempprevsnake;
						}
						else {
							tempnextsnake->down = tempprevsnake;
							snake1 = 0;
						}
						tempprevsnake = tempnextsnake;
						altr = 0;
					}
					else {
						tempnextsnake->next->down = tempprevsnake;
						tempprevsnake = tempnextsnake;
						if (tempprevsnake->val_tile == "S1") snake1 = 1;
						altr = 1;
					}
				}
				if (tempprevsnake->next != NULL) tempprevsnake = tempprevsnake->next;							//in case we end up at right behind a snake head/tail so to prevent any errors skip a tile if it isnt NULL
			}
			if (tempprevsnake->val_tile == "10" || tempprevsnake->val_tile == "20" || tempprevsnake->val_tile == "30" || tempprevsnake->val_tile == "40" || tempprevsnake->val_tile == "50"
				|| tempprevsnake->val_tile == "60" || tempprevsnake->val_tile == "70" || tempprevsnake->val_tile == "80" || tempprevsnake->val_tile == "90")
				tempprevsnake = tempprevsnake->up;
			else tempprevsnake = tempprevsnake->next;
		}
	}

public:
	Game(const std::string& filename, int num_players) {
		if (num_players >= 2) {
			//Initialize Board
			int count = 1;
			int line = 1;
			fstream mapfile;
			char checkchar[4];
			mapfile.open("map.txt", ios::out || ios::in);
			if (mapfile.is_open()) {
				Tile* temp = NULL;
				long tempnum = 0;
				while (true) {
					if (line == 2) {						//to alternate each line for zigzag formation
						tempnum -= 3;						//all board indexes written in 3 spaces in .txt file
						mapfile.seekg(tempnum);				//change position of file pointer
					}
					mapfile >> checkchar;					//store value of board from .txt file

					//creation of nodes
					if (temp == NULL) {
						temp = new Tile;					//when board hasn't been initialized
						temp->val_tile = checkchar;
						end = temp;							//a pointer pointing to 100, will be used for display
					}
					else {
						if ((count - 1) % 10 != 0) {				//for non edge tiles
							temp->previous = new Tile;				//using pevious since creating board from top according to given map.txt file
							temp->previous->next = temp;			//making sure both tiles point to each other
							temp = temp->previous;					//moving temp to new position to create chain
							temp->val_tile = checkchar;

						}
						else if ((count - 1) % 10 == 0) {
							temp->down = new Tile;					//edge files need to be vertically connected
							temp->down->up = temp;
							temp = temp->down;
							temp->val_tile = checkchar;
						}
					}

					if (count % 10 == 0) {					//after each line manipulate file pointer 
						tempnum = mapfile.tellg();
						if (line == 1) {					//left to right traversal
							tempnum += 32;
							line = 2;
						}
						else if (line == 2) {				//right to left traversal
							line = 1;
							tempnum += 29;
						}
						mapfile.seekg(tempnum);				//place file pointer on desired position
					}
					count++;
					if (count == 101) {
						start = temp;						//after board initialization make sure start is pointing to logically first position
						break;
					}
				}
				mapfile.close();
			}
			initializeLaddersAndSnakes();

			//Initialize Players (will be in Queue)
			turnQueue.Initialize_Queue(num_players);
			for (int i = 0; i < num_players; i++) {
				cout << " P" << turnQueue.getPlayerNumber(i);
				cout << "-->" << turnQueue.getPlayerPosition(i);
			}
			cout << endl;
			//Initialize random number generator through seed for DICE
			srand(time(NULL));
		}
		else cout << "ERROR::This is a minimum two player game, can not initialize queue.\nPersonal Attack:: Please make some friends.\n";
	}

	//getter functions + snake ladder check
	Queue<int>& GetTurnQueue() {
		return turnQueue;
	}
	Tile* GetTile(int pos) {
		Tile* temp = start;
		int count = 1;
		while (temp != NULL) {
			if (pos == count) return temp;	//if position equal to count return the tile that temp is pointing to
			if (count % 10 == 0) {			//since count is same as tile number, but tiles are stored as characters
				temp = temp->up;
			}
			else temp = temp->next;
			count++;
		}
		return NULL;						//if position is an invalid number that is not within the board
	}
	bool IsLadder(int pos) {
		Tile* temp = start;
		int count = 1;
		while (temp != NULL) {
			if (pos == count) {
				if (temp->ladder) return true;			//once position reached check if ladder tail exists
				else return false;						//if not no need to traverse any further stop 
			}
			if (count % 10 == 0) {
				cout << endl;
				temp = temp->up;
			}
			else temp = temp->next;
			count++;
		}
		return false;								//if position an invalid number that is not within the board
	}
	bool IsSnake(int pos) {
		Tile* temp = start;
		int count = 1;
		while (temp != NULL) {
			if (pos == count) {
				if (temp->snake) return true;			//once position reached check if snake head exists
				else return false;						//if not no need to traverse any further stop 
			}
			if (count % 10 == 0) {
				cout << endl;
				temp = temp->up;
			}
			else temp = temp->next;
			count++;
		}
		return NULL;
		return false;								//if position an invalid number that is not within the board
		return false;
	}

	//Game control functions
	int TraverseBoard(int current_pos, RollStack& stack) {
		//this entire part is solely due to test cases it shouldn't be allowed in reality since
		//it is against the rules of the game to be able to move the player yourself or a func
		//of this sort should be private which takes current position as argument
		while (turnQueue.getPlayerPosition() == -1) {
			turnQueue.InitializePlayers(start);
		}
		if (turnQueue.getPlayerPosition() != current_pos && turnQueue.getPlayerPosition() < current_pos) {
			int bringtoCurrent = current_pos - turnQueue.getPlayerPosition();
			turnQueue.movetotestcaseposition(bringtoCurrent);
		}

		if (!stack.is_empty()) {
			int sumofstack = 0;
			while (!stack.is_empty()) {
				sumofstack += stack.top();
				turnQueue.Move(stack.top());
				stack.pop();
			}
			int toReturn = turnQueue.getPlayerPosition();
			RollDice(sumofstack);
			return toReturn;
		}
		else return -1;
	}
	int TraverseBoard(RollStack& stack) {
		if (!stack.is_empty()) {
			int sumofstack = 0;
			while (!stack.is_empty()) {
				if (stack.top() > (100 - turnQueue.getPlayerPosition())) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
					if (turnQueue.getPlayerPosition() != 100)	cout << "\n   ----Unfortunately you cannot move according to this number-----  \n";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					break;
				}
				sumofstack += stack.top();
				turnQueue.Move(stack.top());
				stack.pop();
			}
			int toReturn = turnQueue.getPlayerPosition();
			if (toReturn == 100) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 236);
				cout << "\n   ****** CONGRATULATIONS ==> Player " << turnQueue.getPlayerNumber() << " has won game  *******\n\n";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				turnQueue.DeletePlayer();
				return toReturn;
			}
			RollDice(sumofstack);
			return toReturn;
		}
		else return -1;
	}
	void RollDice(int value) {
		if (value != 0) {
			if (turnQueue.getPlayerPosition() == -1) {
				if (value == 6 || value == 1) {
					turnQueue.InitializePlayers(start);
					value = 0;		//no movement when beginning just bring on board
				}
				turnQueue.AfterMoveRearrangeQueue();
				return;
			}
			else {
				if (value % 3 == 0) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
					cout << "\nYou just pulled an UNO REVERSE equivalent of snake game ;)\n--->NOTE:Player turns will reverse now<---\n\n";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					turnQueue.ReverseQueue();							//Rule e, i.e if divisible by 3 reverse
				}
				else turnQueue.AfterMoveRearrangeQueue();				//otherwise normal movement of queue
			}
		}
	}
	void ApplyTurnRules(int sixcheck, RollStack& stack) {
		if (sixcheck == 3) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			cout << "P" << turnQueue.getPlayerNumber() << " got 3 sixes thus " << "P" << turnQueue.getPlayerNumber() << "'s turn has been skipped\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			turnQueue.AfterMoveRearrangeQueue();
			stack.empty();
		}
		if (sixcheck == 4) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			cout << "You got 4 consec sixes thus next players turn has been skipped.\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			turnQueue.AfterMoveRearrangeQueue();
			stack.empty();
		}
	}

	//FINAL PLAY FUNCTION
	void Play() {
		int random_dice_num;
		char enter;
		RollStack stack;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
		cout << "P" << turnQueue.getPlayerNumber() << " press enter to roll dice and play your turn\n";
		cin.get(enter);						//to slow game down and see whats going on + to interact with players
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

		while (!turnQueue.OnlyOnePlayerLeft()) {
			system("CLS");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 78);
			cout << " ------------------------------GAME HAS BEGUN---------------------------------\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			cout << "                           Made By: Aleezeh Usman\n";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

			int count = 0;
			for (int i = 0; i < 1; i++) {
				random_dice_num = rand() % 6 + 1;
				if (random_dice_num > (100 - turnQueue.getPlayerPosition()) && random_dice_num != 6) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
					cout << "P" << turnQueue.getPlayerNumber() << "  got: " << random_dice_num << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
					cout << "\n  ----Unfortunately you cannot move according to this number-----  \n";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					turnQueue.AfterMoveRearrangeQueue();				//player loses their turn :(
					break;												//if player at end and cant move the number it got break and move to next player
				}
				//			cin >> random_dice_num;	//control input while checking or demos
				if (turnQueue.getPlayerPosition() == -1) {				//check if player on board yet or not
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
					cout << "P" << turnQueue.getPlayerNumber() << "  got: " << random_dice_num << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					RollDice(random_dice_num);							//initialize player and bring on board on first 6 or 1
					//following code commented due to rolldice implementation given in test cases, otherwise it is allowed
					//for player to get another turn after just entering the board
					/*if (random_dice_num == 1 || random_dice_num == 6) {
						turnQueue.InitializePlayers(start);
						random_dice_num = rand() % 6 + 1;
						cout << "P" << turnQueue.getPlayerNumber() << " got: " << random_dice_num << endl;
						stack.push_roll(random_dice_num);
						while (random_dice_num == 6) {
							random_dice_num = rand() % 6 + 1;
							cout << "P" << turnQueue.getPlayerNumber() << "  got: " << random_dice_num << endl;
							stack.push_roll(random_dice_num);
						}
					}*/
				}
				else {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
					cout << "P" << turnQueue.getPlayerNumber() << "  got: " << random_dice_num << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					stack.push_roll(random_dice_num);
					while (random_dice_num == 6) {														//if player gets a 6, another turn is awarded
						random_dice_num = rand() % 6 + 1;
						//					cin >> random_dice_num;	//to check 3 sixes, or 4 sixes rule implementation cz very rare chance of it coming itself lol
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
						cout << "P" << turnQueue.getPlayerNumber() << "  got: " << random_dice_num << endl;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						stack.push_roll(random_dice_num);
						count += 1;
					}
					if (count == 3) {							//For Rule c implementation
						ApplyTurnRules(count, stack);
						break;									//to skip traversal
					}
					int check = TraverseBoard(stack);
					if (count == 4) {					//after traversal so reverse queue option can be implemented and then whoever is next player 
						ApplyTurnRules(count, stack);	//their turn can be skipped 	
					}
				}
				//if (check == -1) turnQueue.AfterMoveRearrangeQueue();
			}

			DisplayBoard();
			cout << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
			cout << "P" << turnQueue.getPlayerNumber() << " press enter to roll dice and play your turn\n";
			cin.get(enter);						//to slow game down and see whats going on + to interact with players
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 78);
		cout << " ------------------------------GAME HAS ENDED---------------------------------\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}

	//display functions
	void checkQueueMovement(int num_players) {		//only shows player movement
		for (int i = turnQueue.getHead(), check = 0; check < num_players; check++) {
			cout << " P" << turnQueue.getPlayerNumber(i);
			cout << "-->" << turnQueue.getPlayerPosition(i);
			i = (i + 1) % num_players;
		}

	}
	void DisplayBoard() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		cout << "   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		Tile* temp = end;
		int count = 99;
		int TravBoard = 91;
		bool direction = 1;
		while (temp != NULL) {
			cout << "  ";
			int check = true;																	//to keep a check on player positions
			string player;
			player = "P";
			int playernum;																		//to store player number for unique colour display
			for (int i = turnQueue.getHead(), j = 0; j < turnQueue.returnTotalPlayers(); j++) {
				if (turnQueue.returnPosition(i) == temp) {
					check = false;																//control the check, if player on that position
					player.append(to_string(turnQueue.getPlayerNumber(i)));
					playernum = i;
				}
				i = (i + 1) % turnQueue.returnTotalPlayers();									//to remain within queue range
			}
			if (count >= 1 && count < 10 && count != 6 && count != 4 && check) cout << "   ";	//add spaces for single digits + checks for Ladders/Snakes/Players in first row
			else if (count != 99) cout << "  ";													//all other spaces
			else cout << " ";																	//for first number i.e 100

			if (check)	cout << temp->val_tile;													//either display tile value 
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), turnQueue.getPlayerColour(playernum));
				cout << player;																	//or display which player is on the board tile
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}

			if (count % 10 == 0) {																//for edge tiles
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				cout << endl << endl;;													//pretty formatting
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				if (direction) {																//to deal with zigzag formation
					TravBoard = TravBoard - 10;
					temp = GetTile(TravBoard);
					direction = 0;
				}
				else {
					temp = GetTile(TravBoard);
					TravBoard = TravBoard - 10;
					direction = 1;
					temp = temp->down;
				}
			}
			else if (direction)temp = temp->previous;											//to deal with zigzag formation
			else if (!direction) temp = temp->next;
			count--;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		cout << "   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}

	//destructor
	~Game() {
		while (start != NULL) {
			Tile* toDelete = start;
			if (start->next != NULL) start = start->next;
			else if (start->up != NULL) start = start->up;		//check edges while moving through the board
			if (start->val_tile == "100") start = NULL;
			delete toDelete;
		}
		while (!turnQueue.is_empty())turnQueue.DeletePlayer();
	}
};

#endif
