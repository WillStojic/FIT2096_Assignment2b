#ifndef BATTLE_H
#define BATTLE_H

#include "GameObject.h"

class Battle
{
private:
	int Attack(GameObject* attacker);
	void BeHit(GameObject* victim, int amount);

	bool m_aggressorWin;
public:
	//battles are generalised to an aggressor (the one who starts it) and a defender.
	//since the player it the only one who can move and begin battles, they are always the aggressor.
	Battle(GameObject* aggressor, GameObject* defender);

	void Duel(GameObject* aggressor, GameObject* defender);
	bool CheckWinner() { return m_aggressorWin; }
};

#endif