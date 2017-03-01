// Pantanow Engine rev3 201703
// Copyleft 2017 by The Mojon Twins

// Bullets!

void bullets_init (void) {

}

void bullets_create (void) {

}

void bullets_do (void) {
	
}

void bullets_shoot_player (signed int x, unsigned int y) {
	// Distance
	d = distance (x, y, px, py);
	
	// Shoot!
	dx = (px - x); dy = (signed int) (py - y);

	bullets_create (x, y, 
		(COCO_SPEED * dx) / d,			// vx
		(COCO_SPEED * dy) / d			// vy
	);
}

