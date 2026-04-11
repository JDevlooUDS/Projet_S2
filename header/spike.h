#include "gameObject.h"

class Spike : public GameObject {
public:
	Spike();
	~Spike();
	void update(double deltaTime, const Inputs& inputs) override;

private:
};