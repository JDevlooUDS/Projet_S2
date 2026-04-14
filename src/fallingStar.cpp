#include "../header/fallingStar.h"

FallingStar::FallingStar() {
	animation = ResourceManager::getInstance().getFallingStarAnimation();
}

FallingStar::~FallingStar() {}

void FallingStar::update(double deltaTime, const Inputs& input) {
	if (!active) return;
	manageAnimation(deltaTime);
}

void FallingStar::manageAnimation(double deltaTime) {
	animationTimer += deltaTime;
	if (animationTimer >= ANIMATION_SPEED) {
		animationTimer = 0.0;
		animationIndex++;
	}

	if (animationIndex == animation.size()) {
		deactivate();
		setVisible(false);
		animationIndex = 0;
		return;
	}

	setPixmap(animation[animationIndex]);

}