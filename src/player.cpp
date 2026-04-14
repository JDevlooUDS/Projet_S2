#include "../header/player.h"


Player::Player() {
	idleAnimation = ResourceManager::getInstance().getIdleAnimation();
	jumpAnimation = ResourceManager::getInstance().getJumpAnimation();
	dashAnimation = ResourceManager::getInstance().getDashAnimation();
	runAnimation = ResourceManager::getInstance().getRunAnimation();

	for (int i = 0; i < AFTER_IMAGE_MAX; i++) {
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
		item->setVisible(false);
		afterImages.push_back(item);
	}



	setFlag(QGraphicsItem::ItemIsFocusable);
	setFocus();
	hp = 3;
	speed = BASE_SPEED;
	reverseSpeed = REVERSE_SPEED;
	dashXVelocity = 0;
	dashYVelocity = 0;
	setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

Player::~Player() {}


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
	manageCollision();
	if (inputs.isAccelerated) accelBonus = ACCEL_BONUS;
	else accelBonus = 1.0f;
	if (playerState == NORMAL) {
		manageNormalState(deltaTime, inputs);
	}
	else if (playerState == DASH) {
		manageDashState(deltaTime, inputs);
	}

	manageAnimation(deltaTime);
	if (isGrounded) lastGroundPosition = pos();
	if (xVelocity < 0 || dashXVelocity < 0) {
		setTransform(QTransform());
		setTransform(QTransform().translate(24,0).scale(-1, 1).translate(-24,0));
	}
	if (xVelocity > 0 || dashXVelocity > 0) {
		setTransform(QTransform());
	}

	if (invinsibilityTimer >= INVINSIBILITY_LIMIT) {
		invinsible = false;
		setVisible(true);
	}
	if (invinsible && flickerTimer >= FLICKER_LIMIT) {
		setVisible(!isVisible());
		flickerTimer = 0.0;
	}
	flickerTimer += deltaTime;
	invinsibilityTimer += deltaTime;

}

void Player::manageDashState(double deltaTime, const Inputs& inputs) {
	if ((inputs.isSpacePressed)) {
		jumpBufferTimer = JUMP_BUFFER_LIMIT;
	}
	jumpBufferTimer -= deltaTime;
	coyoteTimer -= deltaTime;
	if (!inBoost) inBoostBuffer += deltaTime;
	else inBoostBuffer = 0.0;
	if ((jumpBufferTimer > 0 && (isGrounded || coyoteTimer > 0)) && jumpEnabled) {
		jump();
		xVelocity = dashXVelocity;
		dashXVelocity = 0;
		dashCount = 1;
		if (inBoost || inBoostBuffer < BOOST_BUFFER_LIMIT) isJumpingFromDash = true;
		coyoteTimer = 0;
		jumpBufferTimer = 0;
	}

	if (dashTimer >= DASH_LIMIT) {
		dashing = false;
		playerState = NORMAL;
		dashXVelocity = 0;
		if (dashDirection == DOWN || dashDirection == DOWN_LEFT || dashDirection == DOWN_RIGHT) {
			fallVelocity = BASE_FALL_VELOCITY; // vitesse max vers le bas
		}
		else if (dashDirection == UP || dashDirection == UP_LEFT || dashDirection == UP_RIGHT) {
			fallVelocity = -UP_DASH_VELOCITY_RETAIN; // conserve un elan vers le haut
		}
		else {
			fallVelocity = 0; // repart de zero pour tout autre dash
		}
	}
	dashTimer += deltaTime;
	
	afterImageTimer += deltaTime;
	if (afterImageTimer >= AFTER_IMAGE_SPEED) {
		afterImageTimer = 0.0f;
		if (afterImageIndex >= afterImages.size()) afterImageIndex = 0;
		QGraphicsPixmapItem* item = afterImages[afterImageIndex];
		item->setPixmap(pixmap());
		item->setPos(pos());
		item->setOpacity(0.7);
		item->setVisible(true);
		afterImageIndex++;
	}

	for (QGraphicsPixmapItem* ghost : afterImages) {
		if (ghost->isVisible()) {
			ghost->setOpacity(ghost->opacity() - (1.0f*deltaTime));
			if (ghost->opacity() <= 0) ghost->setVisible(false);
			ghost->setTransform(transform());
		}
	}
	

	moveDash(deltaTime);
}

void Player::manageNormalState(double deltaTime, const Inputs& inputs) {
	for (QGraphicsPixmapItem* ghost : afterImages) {
		ghost->setVisible(false);
	}
	yVelocity = fallVelocity;
	
	if (isJumpingFromDash) {
		xVelocity *= 1.0f - (0.1f * deltaTime);
		if (abs(xVelocity) < BASE_SPEED) {
			xVelocity = (xVelocity > 0) ? BASE_SPEED : -BASE_SPEED;
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
	if (!dashing) updateGravity(deltaTime);

	if (xVelocity != 0 && !dashing && !jumping) setAnimation(RUN);
	if (xVelocity == 0 && !dashing && !jumping) setAnimation(IDLE);

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
	float multiplier = accelBonus * accelerationMultiplier * speedMultiplier;
	moveBy(multiplier * xVelocity * deltaTime,0);

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
	float multiplier = accelBonus * accelerationMultiplier * speedMultiplier;
	moveBy(multiplier * dashXVelocity * deltaTime, 0);
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
	setAnimation(JUMP);
	AudioManager::getInstance().playJumpSFX();
}

bool Player::isJumping() {
	return jumping;
}

bool Player::isOnGround() {
	return isGrounded;
}

void Player::updateGravity(double deltaTime) {
	fallVelocity += GRAVITY * deltaTime;
	if (fallVelocity > BASE_FALL_VELOCITY * fallSpeedMultiplier) {
		fallVelocity = BASE_FALL_VELOCITY * fallSpeedMultiplier;
	}
}

void Player::setFallSpeedMultiplier(float multiplier) {
	fallSpeedMultiplier = multiplier;
}

void Player::disableJump() {
	jumpEnabled = false; 
}

void Player::enableJump() {
	jumpEnabled = true; 
}

void Player::ground() {//
	if (!jumping && !dashing) isJumpingFromDash = false;
	
	isGrounded = true;
	jumping = false;
	if (!dashing) dashCount = 1;
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
	dashing = true;
	manageDashDirection(inputs);
 	QVector2D direction = getFixedVelocity();
	float targetSpeed;
	if (abs(xVelocity) <= REVERSE_SPEED) {
		targetSpeed = BASE_SPEED * DASH_MULTIPLIER;
	}
	else {
		targetSpeed = abs(xVelocity) * WAVE_DASH_MULTIPLIER;
	}
 	dashXVelocity = direction.x() * targetSpeed;
	dashYVelocity = direction.y() * BASE_SPEED * DASH_MULTIPLIER;
	jumping = false;
	dashCount--;
	setAnimation(DASH_ANIM);
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

void Player::resolveCollisionX() {
	QList<QGraphicsItem*> items = collidingItems();

	for (QGraphicsItem* item : items) {
		int type = item->type();
		if (type == Wall::Type) {
			setX(lastPosition.x());
			break;
		}
	}
}

void Player::resolveCollisionY() {
	bool resolved = false;

	QList<QGraphicsItem*> items = collidingItems();

	for (QGraphicsItem* item : items) {
		int type = item->type();
		if (type == Wall::Type) {
			if (yVelocity > 0) {
				setY(lastPosition.y());
				resolved = true;
				ground();
			}
			else if (yVelocity < 0) {
				setY(lastPosition.y());
			}
			break; 
		}
	}
	isGrounded = resolved;
	if (wasGroundedLastFrame && dashing) ground();
	if (!isGrounded && wasGroundedLastFrame) {
		coyoteTimer = COYOTE_TIME_LIMIT;
		fallVelocity = 0;
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
		accelerationMultiplier -= ACCELERATION_DELTA;
		if (accelerationMultiplier < MIN_ACCELERATION) accelerationMultiplier = MIN_ACCELERATION;
	}

	lastFrameXVelocity = xVelocity;
}

void Player::resetAcceleration() {
	accelerationMultiplier = MIN_ACCELERATION;
}

float Player::getXVelocity() {
	return xVelocity;
}

void Player::setAnimation(const AnimationState state) {
	if (animationState == state) return;
	animationState = state;
	animationIndex = 0;
	animationTimer = 0.0f;
}

void Player::manageAnimation(double deltaTime) {
	animationTimer += deltaTime;

	if (animationTimer >= ANIMATION_SPEED) {
		animationIndex++;
		animationTimer = 0.0f;
	}

	switch (animationState) {
	case IDLE:
		if (animationIndex >= idleAnimation.size()) animationIndex = 0;
		setPixmap(idleAnimation[animationIndex]);
		break;

	case JUMP:
		if (animationIndex >= jumpAnimation.size()) animationIndex = jumpAnimation.size() - 1;
		setPixmap(jumpAnimation[animationIndex]);
		break;

	case RUN:
		if (animationIndex >= runAnimation.size()) animationIndex = 0;
		setPixmap(runAnimation[animationIndex]);
		break;

	case DASH_ANIM:
		if (animationIndex >= dashAnimation.size()) animationIndex = dashAnimation.size() - 1;
		setPixmap(dashAnimation[animationIndex]);
		break;
	}
}

QRectF Player::boundingRect() const {
	return QRectF(0,0,48,48);
}

QPainterPath Player::shape() const {
	QPainterPath path;
	path.addRect(14,0,20,46);
	return path;
}

vector<QGraphicsPixmapItem*> Player::getAfterImages() {
	return afterImages;
}

void Player::damage() {
	if (invinsible) return;
	hp--;
	AudioManager::getInstance().playLoseLifeSFX();
	if (hp <= 0) {
		isDead = true;
		//AudioManager::getInstance().playDeathSFX();
	}
	resetAcceleration();	
}

void Player::replace() {
	if (pos().x() > lastGroundPosition.x()) {
		setPos(lastGroundPosition.x() - 64, lastGroundPosition.y());

	}
	else {
		setPos(lastGroundPosition.x() + 64, lastGroundPosition.y());
	}
	xVelocity = 0;
}

bool Player::isAlive() {
	return !isDead;
}

int Player::getAccelerationMapped() {
	int map = round((accelerationMultiplier - 1) * 100);
	return map;
}

void Player::setInvinsible() {
	if (invinsible) return;
	invinsible = true;
	invinsibilityTimer = 0.0;
}

void Player::resetDash() {
	dashCount = 1;
}

void Player::setInBoost(bool inBoost) {
	this->inBoost = inBoost;
}

void Player::manageCollision() {
	QList<QGraphicsItem*> collidingItems = this->collidingItems();
	bool touchingBoost = false;
	bool touchingTrap = false;

	foreach(QGraphicsItem * item, collidingItems) {
		int type = item->type();
		if (type == Trap::Type) {
			Trap* trap = qgraphicsitem_cast<Trap*>(item);
			trap->applyEffect(this);
			resetAcceleration();
			resetDash();
			touchingTrap = true;
		}
		else if (type == Spike::Type) {
			damage();
			if (Jon::getInstance().isConnected()) {
				if (hp == 2) {
					Jon::getInstance().SendToSerial(false, false, true, true, true, false);
				}
				if (hp == 1) {
					Jon::getInstance().SendToSerial(false, true, true, true, false, false);
				}
				if (hp == 0) {
					Jon::getInstance().SendToSerial(true, true, true, false, false, false);
				}
			}
			resetAcceleration();
			setInvinsible();
		}
		else if (type == Boost::Type) {
			Boost* boost = qgraphicsitem_cast<Boost*>(item);
			boost->applyEffect(this);
			touchingBoost = true;
		}
		else if (type == Hole::Type) {
			damage();
			replace();
			if (Jon::getInstance().isConnected()) {
				if (hp == 2) {
					Jon::getInstance().SendToSerial(false, false, true, true, true, false);
				}
				if (hp == 1) {
					Jon::getInstance().SendToSerial(false, true, true, true, false, false);
				}
				if (hp == 0) {
					Jon::getInstance().SendToSerial(true, true, true, false, false, false);
				}
			}
		}
	}

	setInBoost(touchingBoost);

	if (!touchingTrap && !touchingBoost) {
		setSpeedMultiplier(1.0f);
		enableJump();
	}
	if (!touchingTrap) {
		setFallSpeedMultiplier(1.0f);
	}
}


/*

TODO:
	Mettre le son global
	Ajouter le background
	Ajouter les menus
	Ajouter un classement
	Mettre les voix de Alexis si je les recois

BUGFIX:
	Le personnage passe parfois au sol quand waveDash dans boost

AUTRE:
	trouver une facons d'enlever le scroll avec la souris dans la scene de jeu
	manque de son
*/