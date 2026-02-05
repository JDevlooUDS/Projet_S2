class Game {
public:
	Game();
	~Game();
	void run();
private:
	void getInputs();
	void update();
	void draw();

	bool isRunning = false;
	bool isFinished = false;
};