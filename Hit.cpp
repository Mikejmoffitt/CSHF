#include "Hit.h"

Hit::Hit(const int dmg, const int kbx, const int kby, const int htfr,
		const int htst, const bool nj, const bool lau, const int downf, const int snd, const bool low)
		: DAMAGE(dmg), KNOCKBACKX(kbx), KNOCKBACKY(kby), HITFREEZE(htfr), HITSTUN(htst),
		NOJUGGLE(nj), LAUNCH(lau), DOWNFRAMES(downf), SOUND(snd), LOW(low)
{
}