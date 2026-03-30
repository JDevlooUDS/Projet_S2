#include "../header/player.h"

Player::Player() {
	setPixmap(ResourceManager::getInstance().getPlayerSprite());
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFocus();
	hp = 3;
	speed = BASE_SPEED;
}

Player::~Player() {

}

void Player::update(double deltaTime, Inputs& inputs) {
	yVelocity = fallVelocity * 1;
	xVelocity = 0;
	if (inputs.isLeftPressed) xVelocity = -1 * speed;
	if (inputs.isRightPressed) xVelocity = speed;

	if ((inputs.isSpacePressed)) {
		jumpBufferTimer = JUMP_BUFFER_LIMIT;
		
	}
	jumpBufferTimer -= deltaTime;
	coyoteTimer -= deltaTime;
	if ((jumpBufferTimer > 0 && (isGrounded || coyoteTimer > 0)) && jumpEnabled) {
		jump();
		coyoteTimer = 0;
		jumpBufferTimer = 0;
	}

	manageDashDirection(inputs);
	

	if (isDashing()) {
		updateDash(deltaTime);
		QVector2D playerVelocity = getFixedVelocity();
		xVelocity = playerVelocity.x() * speed;
		yVelocity = playerVelocity.y() * fallVelocity;
	}
	else {
		updateGravity(deltaTime);
	}

	move(deltaTime);
	wasGroundedLastFrame = isGrounded;
}

void Player::move(double deltaTime) {
	if (!active) return;
	moveX(deltaTime);
	resolveCollisionX();
	moveY(deltaTime);
	resolveCollisionY();
}

void Player::moveX(double deltaTime) {
	if (!active) return;
	lastPosition = pos();
	moveBy(speedMultiplier * xVelocity * deltaTime,0);
}

void Player::moveY(double deltaTime) {
	if (!active) return;
	lastPosition = pos();
	moveBy(0, yVelocity * deltaTime);
}

void Player::jump() {
 	fallVelocity = JUMP_VELOCITY;
	isGrounded = false;
	jumping = true;
}

bool Player::isJumping() {
	return jumping;
}

void Player::updateGravity(double deltaTime) {
	fallVelocity += GRAVITY * deltaTime;
	if (fallVelocity > BASE_FALL_VELOCITY) fallVelocity = BASE_FALL_VELOCITY;
}

void Player::disableJump() {
	jumpEnabled = false; 
}

void Player::enableJump() {
	jumpEnabled = true; 
}

void Player::ground() {
	isGrounded = true;
	jumping = false;
	dashCount = 1;
}

void Player::setSpeedMultiplier(float multiplier) {
	speedMultiplier = multiplier;
}

float Player::getSpeedMultiplier() {
	return speedMultiplier;
}

void Player::dash() {
	if (dashCount == 0) return;
	speed = DASH_SPEED;
	fallVelocity = DASH_SPEED;
	dashing = true;
	dashCount--;
}

bool Player::isDashing() {
	return dashing;
}

void Player::updateDash(double deltaTime) {
	if (dashTimer >= DASH_LIMIT) {
		dashing = false;
		dashTimer = 0.0;
		speed = BASE_SPEED;
		fallVelocity = 0;
		dashDirection = NONE;
		return;
	}
	dashTimer += deltaTime;
}

void Player::setDashDirection(DashDirection dashDirection) {
	this->dashDirection = dashDirection;
}

QVector2D Player::getFixedVelocity() {
	switch (dashDirection) {
		case UP:
			return QVector2D(0.0,-1.0);
		case UP_RIGHT:
			return QVector2D(1.0, -1.0);
		case RIGHT:
			return QVector2D(1.0,0.0);
		case DOWN_RIGHT:
			return QVector2D(1.0,1.0);
		case DOWN:
			return QVector2D(0.0,1.0);
		case DOWN_LEFT:
			return QVector2D(-1.0,1.0);
		case LEFT:
			return QVector2D(-1.0,0.0);
		case UP_LEFT:
			return QVector2D(-1.0,-1.0);
		default:
			return QVector2D(0,0);
	}
}

void Player::setWalls(vector<GameObject*> walls) {
	this->walls = walls;
}

void Player::resolveCollisionX() {
	for (GameObject* wall : walls) {
		if (collidesWithItem(wall)) {
			if (xVelocity > 0) {
				setX(lastPosition.x());
			}
			else if (xVelocity < 0) {
				setX(lastPosition.x());
			}
		}
	}
}

void Player::resolveCollisionY() {
	bool resolved = false;
	for (GameObject* wall : walls) {
		if (collidesWithItem(wall)) {
			if (yVelocity > 0) {
				setY(lastPosition.y());
				fallVelocity = 0;
				resolved = true;
				ground();
			}
			else if (yVelocity < 0) {		
				setY(lastPosition.y());
				fallVelocity = 0;
			}
		}
	}
	isGrounded = resolved;
	if (!isGrounded && wasGroundedLastFrame) {
		coyoteTimer = COYOTE_TIME_LIMIT;
	}
}

void Player::manageDashDirection(Inputs& inputs) {
	if (inputs.isDashPressed) {
		dash();
		if (inputs.isRightPressed && !inputs.isDownPressed && !inputs.isLeftPressed && !inputs.isUpPressed) {
			setDashDirection(RIGHT);
		}
		else if (!inputs.isRightPressed && inputs.isDownPressed && !inputs.isLeftPressed && !inputs.isUpPressed) {
			setDashDirection(DOWN);
		}
		else if (!inputs.isRightPressed && !inputs.isDownPressed && inputs.isLeftPressed && !inputs.isUpPressed) {
			setDashDirection(LEFT);
		}
		else if (!inputs.isRightPressed && !inputs.isDownPressed && !inputs.isLeftPressed && inputs.isUpPressed) {
			setDashDirection(UP);
		}
		else if (inputs.isRightPressed && !inputs.isDownPressed && !inputs.isLeftPressed && inputs.isUpPressed) {
			setDashDirection(UP_RIGHT);
		}
		else if (inputs.isRightPressed && inputs.isDownPressed && !inputs.isLeftPressed && !inputs.isUpPressed) {
			setDashDirection(DOWN_RIGHT);
		}
		else if (!inputs.isRightPressed && inputs.isDownPressed && inputs.isLeftPressed && !inputs.isUpPressed) {
			setDashDirection(DOWN_LEFT);
		}
		else if (!inputs.isRightPressed && !inputs.isDownPressed && inputs.isLeftPressed && inputs.isUpPressed) {
			setDashDirection(UP_LEFT);
		}
		else {
			setDashDirection(RIGHT);
		}
	}
}