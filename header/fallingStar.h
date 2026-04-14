#include "gameObject.h"
#include "resourceManager.h"

class FallingStar : public GameObject {
public:
	FallingStar();
	~FallingStar();
	void update(double deltaTime, const Inputs& inputs) override;

private:
	void manageAnimation(double deltaTime);

	vector<QPixmap> animation;
	int animationIndex = 0;
	double animationTimer = 0.0;
	const double ANIMATION_SPEED = 0.1;

};