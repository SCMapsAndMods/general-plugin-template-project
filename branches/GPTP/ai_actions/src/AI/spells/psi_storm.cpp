#include "psi_storm.h"

namespace AI {

class PsiStormTargetFinderProc: public SpellTargetFinderProc {
  private:
    const CUnit *caster;
    bool isUnderAttack;
  public:
    PsiStormTargetFinderProc(const CUnit *caster, bool isUnderAttack)
      : caster(caster), isUnderAttack(isUnderAttack) {}

    bool operator()(const CUnit *target) const {
      if (!isTargetWorthHitting(target, caster))
        return false;

      if (!scbw::canWeaponTargetUnit(WeaponId::PsiStorm, target, caster))
        return false;

      const int totalEnemyLife = getTotalEnemyLifeInArea(target->getX(), target->getY(), 96, caster, WeaponId::PsiStorm);
      if (!isUnderAttack && totalEnemyLife < 250)
        return false;

      const int totalAllyLife = getTotalAllyLifeInArea(target->getX(), target->getY(), 96, caster, WeaponId::PsiStorm);
      if (totalAllyLife * 2 >= totalEnemyLife)
        return false;

      return true;
    }
};

const CUnit* findBestPsiStormTarget(const CUnit *caster, bool isUnderAttack) {
  scbw::UnitFinder spellTargetFinder;
  return NULL;
}

} //AI
