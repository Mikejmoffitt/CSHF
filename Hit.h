#ifndef HIT_H
#define HIT_H

class Hit
{
public:
	Hit(const int dmg, const int kbx, const int kby, const int htfr,
		const int htst, const bool nj, const bool lau, const int downf, const int snd, const bool low);
	// MAMBOS

	// How many hit points to deduct on contact
	const int DAMAGE;

	// dX for knockback
	const int KNOCKBACKX;

	// dY for knockback
	const int KNOCKBACKY;

	// How many frames to freeze game action
	const int HITFREEZE;

	// How many frames victim is stunned
	const int HITSTUN;

	// Invulnerability until ground
	const bool NOJUGGLE;

	// Whether or not to allow victim to be popped up when standing
	const bool LAUNCH;

	// If >0, enables knockdown for this many frames
	const int DOWNFRAMES;

	// Which sound index to use 
	const int SOUND;

	// Whether it's a high or low attack
	const bool LOW;
};

#endif