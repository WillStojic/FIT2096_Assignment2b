#include "Battle.h"
#include "MathsHelper.h"

Battle::Battle(GameObject* aggressor, GameObject* defender)
{
	m_aggressorWin == false;

	Duel(aggressor, defender);
}

void Battle::Duel(GameObject* aggressor, GameObject* defender)
{
	// a battle takes place within a single frame.

	// we keep fighting until someone dies
	while (aggressor->GetHealth() > 0 && defender->GetHealth() > 0)
	{
		//participants can only generate an attack value if they have more the 0 ammo
		int aggressorAttackValue = 0;

		if (aggressor->GetAmmo() > 0)
			aggressorAttackValue = Attack(aggressor);

		int defenderAttackValue = 0;

		if (defender->GetAmmo() > 0)
			defenderAttackValue = Attack(defender);


		if (aggressorAttackValue > defenderAttackValue)
		{
			// aggressor wins the round - the monster will receive some damage
			BeHit(defender, aggressorAttackValue - defenderAttackValue);
		}
		else
		{
			// defender wins round - the player will receive some damage
			BeHit(aggressor, defenderAttackValue - aggressorAttackValue);
		}
	}

	if (aggressor->GetHealth() > 0 && defender->GetHealth() <= 0)
		m_aggressorWin = true;
	else
		m_aggressorWin = false;
}

int Battle::Attack(GameObject* attacker)
{
	return MathsHelper::RandomRange(0, attacker->GetSkill());
}

void Battle::BeHit(GameObject* victim, int amount)
{
	victim->SetHealth(victim->GetHealth() - amount);
}