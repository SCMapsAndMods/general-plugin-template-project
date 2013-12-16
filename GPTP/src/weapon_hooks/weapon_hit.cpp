#include "lockdown.h"
#include <SCBW/api.h>

namespace weapon_hooks {

//Controls how each weapon type works.
void __stdcall CBullet_Damage(const CBullet *bullet) {
  CUnit *target = bullet->attackTarget.unit;

  switch (Weapon::ExplosionType[bullet->weaponType]) {
    //No effect
    case WeaponEffect::None:
      break;

    //Normal hit
    case WeaponEffect::NormalHit:
      //TODO: Add this
      break;

    //Regular splash stuff
    case WeaponEffect::SplashRadial:
    case WeaponEffect::SplashEnemy:
    case WeaponEffect::NuclearMissile:
      //TODO: Add this
      break;

    case WeaponEffect::Lockdown:
      if (target && !target->isDead())
        lockdownHook(target, 131);
      break;
      
    case WeaponEffect::Parasite:
      if (target && !target->isDead()) {
        scbw::playSound(921, target);
        target->parasiteFlags |= 1 << bullet->sourcePlayer;
        scbw::refreshConsole();
        if (bullet->sourceUnit)
          AI_WeaponHit(target, bullet->sourceUnit, false);
      }
      break;

    case WeaponEffect::Broodlings:
      if (bullet->sourceUnit && target && !target->isDead()) {
        AI_WeaponHit(target, bullet->sourceUnit, true);
      }
      break;

    case WeaponEffect::EmpShockwave:
      //TODO: Add this
      break;

    case WeaponEffect::Irradiate:
      //TODO: Add this
      break;

    case WeaponEffect::Ensnare:
      //TODO: Add this
      break;

    case WeaponEffect::Plague:
      //TODO: Add this
      break;

    case WeaponEffect::StasisField:
      //TODO: Add this
      break;

    case WeaponEffect::DarkSwarm:
      //TODO: Add this
      break;

    case WeaponEffect::Consume:
      //TODO: Add this
      break;

    case WeaponEffect::YamatoGun:
      //TODO: Add this
      break;

    case WeaponEffect::Restoration:
      //TODO: Add this
      break;

    case WeaponEffect::DisruptionWeb:
      //TODO: Add this
      break;

    case WeaponEffect::CorrosiveAcid:
      //TODO: Add this
      break;

    case WeaponEffect::OpticalFlare:
      //TODO: Add this
      break;

    case WeaponEffect::Maelstrom:
      //TODO: Add this
      break;

    case WeaponEffect::SplashAir:
      //TODO: Add this
      break;

    //Note: Mind Control and Feedback are handled in their own respective orders
    //rather than being treated as regular weapons. Thus, using them in
    //weapons.dat will crash the game.
    default:
      //TODO: Add this
      break;
  }
}

} //hooks
