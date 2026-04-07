#include "../header/player.h"


Player::Player() {
	setPixmap(ResourceManager::getInstance().getPlayerSprite());
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFocus();
	hp = 3;
	speed = BASE_SPEED;
	reverseSpeed = REVERSE_SPEED;
}

Player::~Player() {

}


void Player::updateFlip(double deltaTime) {
	if (!isGrounded || xVelocity == 0) return;  // accumule seulement, ne reset PAS si x==0

	bool pressingOpposite = (xVelocity > 0 && !facingRight) || (xVelocity < 0 && facingRight);

	if (pressingOpposite) {
		flipHoldTime += deltaTime;
		if (flipHoldTime >= FLIP_CONST) {
			facingRight = !facingRight;
			setPixmap(pixmap().transformed(QTransform().scale(-1, 1)));
			flipHoldTime = 0.0f;
		}
	}
	else {
		flipHoldTime = 0.0f;  // reset seulement si on va dans la bonne direction
	}
}

void Player::update(double deltaTime, const Inputs& inputs) {
	if (!active) return;
	if (playerState == NORMAL) {
		manageNormalState(deltaTime, inputs);
	}
	else if (playerState == DASH) {
		manageDashState(deltaTime, inputs);
	}
}

void Player::manageDashState(double deltaTime, const Inputs& inputs) {
	if ((inputs.isSpacePressed)) {
		jumpBufferTimer = JUMP_BUFFER_LIMIT;
	}
	jumpBufferTimer -= deltaTime;
	coyoteTimer -= deltaTime;
	if ((jumpBufferTimer > 0 && (isGrounded || coyoteTimer > 0)) && jumpEnabled) {
		jump();
		xVelocity = dashXVelocity;
		dashCount = 1;
		isJumpingFromDash = true;
		coyoteTimer = 0;
		jumpBufferTimer = 0;
	}

	if (dashTimer >= DASH_LIMIT) {
		dashing = false;
		playerState = NORMAL;
	}
	dashTimer += deltaTime;

	moveDash(deltaTime);
}

void Player::manageNormalState(double deltaTime, const Inputs& inputs) {
	updateFlip(deltaTime);
	bool pressingOpposite = (xVelocity > 0 && !facingRight) || (xVelocity < 0 && facingRight);
	if (pressingOpposite) speed = REVERSE_SPEED;
	else speed = BASE_SPEED;
	yVelocity = fallVelocity * 1;
	
	if (isJumpingFromDash) {
		xVelocity *= 1.0f - (0.1f * deltaTime);
		if (xVelocity < BASE_SPEED) {
			xVelocity = BASE_SPEED;
			isJumpingFromDash = false;
		}
		if (inputs.isLeftPressed) xVelocity = -1 * abs(xVelocity);
		if (inputs.isRightPressed) xVelocity = abs(xVelocity);
	}
	else {
		xVelocity = 0;
		if (inputs.isLeftPressed) xVelocity = -1 * speed;
		if (inputs.isRightPressed) xVelocity = speed;
	}

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

	if (inputs.isDashPressed && dashCount > 0) {
		dash(inputs);
	}
	updateGravity(deltaTime);

	manageAcceleration(deltaTime);
	updateFlip(deltaTime);
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
	lastPosition = pos();	
	moveBy(accelerationMultiplier * speedMultiplier * xVelocity * deltaTime,0);

}

void Player::moveY(double deltaTime) {
	lastPosition = pos();
	moveBy(0, yVelocity * deltaTime);
}

void Player::moveDash(double deltaTime) {
	moveDashX(deltaTime);
	resolveCollisionX();
	moveDashY(deltaTime);
	resolveCollisionY();
}

void Player::moveDashX(double deltaTime) {
	lastPosition = pos();
	moveBy(accelerationMultiplier * dashXVelocity * speedMultiplier * deltaTime, 0);
}

void Player::moveDashY(double deltaTime) {
	lastPosition = pos();
	moveBy(0, dashYVelocity * deltaTime);
}

void Player::jump() {
 	fallVelocity = JUMP_VELOCITY;
	isGrounded = false;
	jumping = true;
	playerState = NORMAL;
	dashing = false;
}

bool Player::isJumping() {
	return jumping;
}

bool Player::isOnGround() {
	return isGrounded;
}

void Player::updateGravity(double deltaTime) {
	fallVelocity += GRAVITY * deltaTime;
	if (fallVelocity > BASE_FALL_VELOCITY) {
		fallVelocity = BASE_FALL_VELOCITY;
	}
}

void Player::disableJump() {
	jumpEnabled = false; 
}

void Player::enableJump() {
	jumpEnabled = true; 
}

void Player::ground() {
	if (!jumping && !dashing) isJumpingFromDash = false;
	
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

void Player::dash(const Inputs& inputs) {
	if (dashCount == 0) return;
	playerState = DASH;
	manageDashDirection(inputs);
 	QVector2D direction = getFixedVelocity();
 	dashXVelocity = direction.x() * BASE_SPEED * DASH_MULTIPLIER;
	dashYVelocity = direction.y() * BASE_SPEED * DASH_MULTIPLIER;
	jumping = false;
	dashCount--;
	dashTimer = 0.0;
}

bool Player::isDashing() {
	return dashing;
}

void Player::updateDash(double deltaTime) {
	if (dashTimer >= DASH_LIMIT) {
		dashing = false;
		dashTimer = 0.0;
		fallVelocity = 0;
		dashDirection = NONE;
		if (!jumping) speed = BASE_SPEED;
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
			return QVector2D(0.0,-1.0).normalized();
		case UP_RIGHT:
			return QVector2D(1.0, -1.0).normalized();
		case RIGHT:
			return QVector2D(1.0,0.0).normalized();
		case DOWN_RIGHT:
			return QVector2D(1.0,1.0).normalized();
		case DOWN:
			return QVector2D(0.0,1.0).normalized();
		case DOWN_LEFT:
			return QVector2D(-1.0,1.0).normalized();
		case LEFT:
			return QVector2D(-1.0,0.0).normalized();
		case UP_LEFT:
			return QVector2D(-1.0,-1.0).normalized();
		default:
			return QVector2D(0,0).normalized();
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
				resolved = true;
				ground();
			}
			else if (yVelocity < 0) {		
				setY(lastPosition.y());
			}
		}
	}
	isGrounded = resolved;
	if (wasGroundedLastFrame && yVelocity == 0) ground();
	if (!isGrounded && wasGroundedLastFrame) {
		coyoteTimer = COYOTE_TIME_LIMIT;
	}
}

void Player::manageDashDirection(const Inputs& inputs) {
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

void Player::manageAcceleration(double deltaTime) {
	if (xVelocity == 0) {
		accelerationMultiplier -= ACCELERATION_DELTA * deltaTime;
		if (accelerationMultiplier < MIN_ACCELERATION) accelerationMultiplier = MIN_ACCELERATION;
	}
	else if ((lastFrameXVelocity > 0 && xVelocity > 0) && isGrounded) {
		accelerationMultiplier += ACCELERATION_DELTA * deltaTime;
		if (accelerationMultiplier > MAX_ACCELERATION) accelerationMultiplier = MAX_ACCELERATION;
	}
	else if ((lastFrameXVelocity < 0 && xVelocity < 0) && isGrounded) {
		accelerationMultiplier += ACCELERATION_DELTA * deltaTime;
		if (accelerationMultiplier > MAX_ACCELERATION) accelerationMultiplier = MAX_ACCELERATION;
	}
	else {
		accelerationMultiplier -= ACCELERATION_DELTA * deltaTime;
		if (accelerationMultiplier < MIN_ACCELERATION) accelerationMultiplier = MIN_ACCELERATION;
	}

	lastFrameXVelocity = xVelocity;
}

void Player::resetAcceleration() {
	accelerationMultiplier = MIN_ACCELERATION;
}