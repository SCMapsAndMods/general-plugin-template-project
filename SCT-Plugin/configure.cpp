#include "definitions.h"
#include "Plugin.h"
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

//Called when the user hits the configure button in MPQDraft or FireGraft
BOOL WINAPI Plugin::Configure(HWND hParentWnd) {
  /*
   *  You can't really change the version from
   *  here with the current format.  However, you
   *  can check the version and probably set any
   *  globals.
   */

  MessageBox(
    hParentWnd,
    PLUGIN_NAME " (ID: " STR(PLUGIN_ID) ")"
    "\nMade by pastelmind"
    "\nBuilt on " __DATE__ " " __TIME__
    "\n"
    "\n[System Changes]"
    "\n- Dark Swarm reduces range attack damage by 50%"
    "\n- SCVs auto-repair nearby units"
    "\n- Supply Depots can be lowered/raised"
    "\n- Bunker shows no attack graphic for Fiends"
    "\n- Changed cooldown modifiers for Acid Spore / Ensnare / Stim Packs"
    "\n- Changed unit speed modifiers for Ensnare and Stim Packs"
    "\n- Damage types: Venomous - 100%/125%/150%, Concussive - 100%/50%/50%"
    "\n- Recharge Shields: overcharge, +distance, +efficiency"
    "\n- Improved weapon accuracy for Lurkers and Vultures"
    "\n- Improved Interceptor repair"
    "\n- Hydralisk Den can morph into Deep Warren"
    "\n"
    "\n[Tech and Upgrades]"
    "\n- Stim Packs: 20HP cost for Fiends, add effect overlay"
    "\n- Irradiate: Damage non-organic units, SVs are immune"
    "\n- Plague: Deal full damage, always reduce HP to 1 / 256"
    "\n- Consume: Prevent using on Broodlings"
    "\n- First Aid Packs upgrade: Terran infantry HP regen"
    "\n- Energy upgrades provide +25% energy regeneration"
    "\n- Ocular Implants spell: +2 sight range, +1 weapon range"
    "\n   (Exceptions: Arbiter weapon range is unaffected)"
    "\n- Sight range upgrades: Thermal Sensors, Prescience, Antennae, Sensor Array"
    "\n- Particle Filter upgrade: Change Nexus Cannon attack graphic"
    "\n"
    "\n[Interface Features]"
    "\n- Improved unit weapon tooltip text"
    "\n- Display attack range circle for Siege Mode Tanks and Nexuses selected"
    "\n- Display rally points for selected buildings"
    "\n"
    "\nNote: This plugin is compatible with the Smart Rally plugin and the AI Resource Inflater plugin."
    ,
    PLUGIN_NAME,
    MB_TASKMODAL
  );

  return TRUE;
}

