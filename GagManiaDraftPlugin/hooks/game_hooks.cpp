/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../graphics/graphics.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/ExtendSightLimit.h"
#include "psi_field.h"
#include <cstdio>

bool firstRun = true;

namespace hooks {

/// This hook is called every frame; most of your plugin's logic goes here.
bool nextFrame() {
   static u8* unitsdatHumanAI = (u8*)0x00662268;
		static u8* unitsdatComputerAI = (u8*)0x00662EA0;
		static u8* unitsdatReturnToAI = (u8*)0x00664898;
		
		if (!scbw::isGamePaused()) { //If the game is not paused
			graphics::resetAllGraphics();
			hooks::updatePsiFieldProviders();
		if (firstRun) {
			//scbw::printText("Hello, world!");
			//초반 일꾼 세팅
			for(CUnit* setting=*firstVisibleUnit;setting;setting=setting->next)
			{
				
				if(setting->id==106&&setting->mainOrderId!=OrderId::Die)
				{
					scbw::createUnitAtPos(7,setting->playerId,setting->position.x,setting->position.y);
					scbw::createUnitAtPos(7,setting->playerId,setting->position.x,setting->position.y);
				}
				if(setting->id==154&&setting->mainOrderId!=OrderId::Die)
				{
					scbw::createUnitAtPos(64,setting->playerId,setting->position.x,setting->position.y);
					scbw::createUnitAtPos(64,setting->playerId,setting->position.x,setting->position.y);
				}
				if(setting->id==131&&setting->mainOrderId!=OrderId::Die)
				{
					scbw::createUnitAtPos(41,setting->playerId,setting->position.x,setting->position.y);
					scbw::createUnitAtPos(41,setting->playerId,setting->position.x,setting->position.y);
				}
			}
			firstRun = false;
		}

		// Loop through the unit table.
    // Warning: There is no guarantee that the current unit is actually a unit
    // rather than an unused space in memory.
		//for (int i = 0; i < UNIT_ARRAY_LENGTH; ++i) {
		//	CUnit* unit = &unitTable[i];
    //  //Write your code here
		//}

    // Alternative looping method
    // Guarantees that [unit] points to an actual unit.

		{
				//선택된 유닛 경로,사거리 표시
				static	u8* unitsdatAttackUnit = (u8*)0x00663320;
				for (int i = 0; i < *clientSelectionCount; ++i) {
				 CUnit *selUnit = clientSelectionGroup->unit[i];
				 if(selUnit==NULL)continue;
				 if(selUnit->playerId==*LOCAL_HUMAN_ID||*IS_IN_REPLAY){
				 if(selUnit->mainOrderId==25||selUnit->mainOrderId==30||selUnit->mainOrderId==31)
				 {
					graphics::drawFilledCircle(selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y, 6, graphics::WHITE, graphics::ON_MAP);
					 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y,graphics::WHITE,graphics::ON_MAP);
				 }
				 if(selUnit->mainOrderId==OrderId::Follow){
				 graphics::drawFilledCircle(selUnit->orderTarget.unit->position.x, selUnit->orderTarget.unit->position.y, 6, graphics::GREEN, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.unit->position.x,selUnit->orderTarget.unit->position.y,graphics::GREEN,graphics::ON_MAP);
				 }
				 if(selUnit->mainOrderId==OrderId::Patrol){
				 graphics::drawFilledCircle(selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y, 6, graphics::GREEN, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.pt.x,selUnit->orderTarget.pt.y,graphics::GREEN,graphics::ON_MAP);
				 }
				 if(selUnit->mainOrderId==OrderId::Pickup2&&selUnit->orderTarget.unit){
				 graphics::drawFilledCircle(selUnit->orderTarget.unit->position.x, selUnit->orderTarget.unit->position.y, 6, graphics::GREEN, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.unit->position.x,selUnit->orderTarget.unit->position.y,graphics::GREEN,graphics::ON_MAP);
				 }
				 if(selUnit->mainOrderId==OrderId::MoveUnload&&selUnit->orderTarget.pt.x&&selUnit->orderTarget.pt.y){
				 graphics::drawFilledCircle(selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y, 6, graphics::GREEN, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.pt.x,selUnit->orderTarget.pt.y,graphics::GREEN,graphics::ON_MAP);
				 }

				 if(selUnit->mainOrderId==OrderId::Move){
				 graphics::drawFilledCircle(selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y, 6, graphics::GREEN, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.pt.x,selUnit->orderTarget.pt.y,graphics::GREEN,graphics::ON_MAP);
				 }
				 if(selUnit->mainOrderId==OrderId::AttackMove){
					 graphics::drawFilledCircle(selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y, 6, graphics::RED, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.pt.x,selUnit->orderTarget.pt.y,graphics::RED,graphics::ON_MAP);
				 }
				 if(selUnit->mainOrderId==unitsdatAttackUnit[selUnit->id]&&!(selUnit->subunit)&&selUnit->orderTarget.unit&&selUnit->mainOrderId!=OrderId::Nothing2&&selUnit->mainOrderId!=OrderId::Medic&&selUnit->id!=UnitId::spider_mine){
					 graphics::drawFilledCircle(selUnit->orderTarget.unit->position.x, selUnit->orderTarget.unit->position.y, 6, graphics::RED, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.unit->position.x,selUnit->orderTarget.unit->position.y,graphics::RED,graphics::ON_MAP);
				 }
				 if(selUnit->mainOrderId==unitsdatAttackUnit[selUnit->id]&&selUnit->subunit&&selUnit->subunit->orderTarget.unit&&selUnit->mainOrderId!=OrderId::Nothing2&&selUnit->mainOrderId!=OrderId::Medic&&selUnit->id!=UnitId::spider_mine){
					 graphics::drawFilledCircle(selUnit->subunit->orderTarget.unit->position.x, selUnit->subunit->orderTarget.unit->position.y, 6, graphics::RED, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->subunit->orderTarget.unit->position.x,selUnit->subunit->orderTarget.unit->position.y,graphics::RED,graphics::ON_MAP);
				 }
				 if(selUnit->id==UnitId::siege_tank_s)
				 {
					 graphics::drawCircle(selUnit->position.x,selUnit->position.y,410,graphics::TEAL,graphics::ON_MAP);
				 }
				 if(selUnit->id==UnitId::reaver)
				 {
					 graphics::drawCircle(selUnit->position.x,selUnit->position.y,282,graphics::TEAL,graphics::ON_MAP);
				 }
				 if(selUnit->status & UnitStatus::HoldingPosition&&selUnit->id!=UnitId::medic)
				 {
					 if(selUnit->orderTarget.unit)
					 {
						  graphics::drawFilledCircle(selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y, 6, graphics::RED, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.pt.x,selUnit->orderTarget.pt.y,graphics::RED,graphics::ON_MAP);
					 }
				 }
				 if(selUnit->mainOrderId==56||selUnit->mainOrderId==61){
					 graphics::drawFilledCircle(selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y, 6, graphics::RED, graphics::ON_MAP);
				 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.pt.x,selUnit->orderTarget.pt.y,graphics::RED,graphics::ON_MAP);
				 }
				 if(selUnit->mainOrderId==OrderId::BuildingLand){
						graphics::drawFilledCircle(selUnit->orderTarget.pt.x, selUnit->orderTarget.pt.y, 6, graphics::GREEN, graphics::ON_MAP);
						graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->orderTarget.pt.x,selUnit->orderTarget.pt.y,graphics::GREEN,graphics::ON_MAP);
				 }
				 if((selUnit->rally.pt.x&&selUnit->rally.pt.y)&&(selUnit->rally.pt.x!=selUnit->position.x&&selUnit->rally.pt.y!=selUnit->position.y)&&(selUnit->id!=UnitId::pylon&&selUnit->id!=UnitId::shuttle)&&!(selUnit->rally.unit))
				 {
					 graphics::drawFilledCircle(selUnit->rally.pt.x, selUnit->rally.pt.y, 6, graphics::WHITE, graphics::ON_MAP);
					 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->rally.pt.x, selUnit->rally.pt.y,graphics::WHITE,graphics::ON_MAP);
				 }
				 if(selUnit->rally.unit&&selUnit->rally.unit!=selUnit&&(selUnit->rally.pt.x!=selUnit->position.x&&selUnit->rally.pt.y!=selUnit->position.y)&&(selUnit->id!=UnitId::pylon&&selUnit->id!=UnitId::shuttle))
				 {
					 graphics::drawFilledCircle(selUnit->rally.unit->position.x, selUnit->rally.unit->position.y, 6, graphics::WHITE, graphics::ON_MAP);
					 graphics::drawLine(selUnit->position.x,selUnit->position.y,selUnit->rally.unit->position.x, selUnit->rally.unit->position.y,graphics::WHITE,graphics::ON_MAP);
				 }
				}
				}
			}
 
		for (CUnit *unit = *firstVisibleUnit; unit; unit = unit->next) {
			

			
		/*
			if (unit->id == UnitId::shuttle) {
				//sprintf(buffer, "Order ID for Shuttle: %#x", unit->mainOrderId);
				//PrintText(buffer);
				DWORD* volatile UpdatePsiFieldPowerStatus = (DWORD*) 0x0063FF44;
				if (unit->status & 0x1 && !(unit->building.pylon.pylonAura) && (unit->mainOrderId == OrderId::CarrierStop) && !(scbw::hasOverlay(unit,136)))
				{
					unit->mainOrderId = OrderId::InitPsiProvider;
					scbw::createOverlay(unit->sprite,136);
				}
				
				CSprite *aura = unit->building.pylon.pylonAura;
				if (aura) {
					aura->position.x = unit->sprite->position.x;
					aura->position.y = unit->sprite->position.y;
					BYTE temp = aura->visibilityFlags;
					scbw::RefreshSpriteVisibility(aura, 0);
					scbw::RefreshSpriteVisibility(aura, temp);
					*UpdatePsiFieldPowerStatus = 1;
				}

				if((unit->status & 0x1 && (unit->building.pylon.pylonAura) && (unit->mainOrderId == OrderId::Stop)))
				{
					scbw::SpriteDestructor(unit->building.pylon.pylonAura);
					unit->building.pylon.pylonAura = NULL;
					scbw::RemoveFromPsiProviderList(unit);
					scbw::removeOverlays(unit,136);
					
			}
			}*/
		
		
   

			
			/*
			{
				int radius=50;
				int hitpoints=50;
				if(unit->mainOrderId != Die)
				{
      if (unit->id == high_templar) {
         if (unit->energy < 256*hitpoints) {
            for (int j=0;j<1700;j++) {
               CUnit *anyunit;
               anyunit = &unitTable[j];
			   if(anyunit->mainOrderId != Die
				   &&anyunit->status & 0x1
				   &&!(anyunit->status & 0x2)
				   &&unitsdat_special[anyunit->Id] & 0x200000)
			   {
               if (unit->energy < 256*hitpoints && anyunit->energy > 256
                  && abs(anyunit->currentXPos - unit->currentXPos) < radius
                  && abs(anyunit->currentYPos - unit->currentYPos) < radius

                  && anyunit->id != unit->id ) {
                     unit->energy = unit->energy + 32;
                     anyunit->energy = anyunit->energy - 64;
               }
			   else if(anyunit->energy < 256
				   &&anyunit->unitId != unit->unitId
				   &&unit->energy != 256*hitpoints)
			   {
				   unit->energy = unit->energy + 16;
				   anyunit->energy = 0;
			   }
			  
            }
         }
      }
   }
				}
			}
			*/
		/*
			{
				int radius=55;
				 if(unit->unitId == medic)
	  {
		  if(unit->mainOrderId != Die)
		  {
	  if(unit->mainOrderId == MedicHeal1
		  ||unit->mainOrderId == MedicHoldPosition)
	  {
		 if(unit->orderTarget != NULL)
		 {
		 
			  if(abs(unit->orderTarget->currentXPos - unit->currentXPos) < radius
               && abs(unit->orderTarget->currentYPos - unit->currentYPos) < radius)
			  {
					   if(unit->orderTarget->unitId==marine
						   &&unit->orderTarget->shields < 256*unitsdat_maxshield[unit->orderTarget->unitId]
						   &&unit->shields > 256)
					   {	
						   unit->orderTarget->shields = unit->orderTarget->shields + 64;
						   unit->shields = unit->shields - 128;
					   }
					   else if(unit->shields < 256
						   && unit->orderTarget->unitId == marine)
					   {
						   unit->orderTarget->shields = unit->orderTarget->shields + 16;
						   unit->shields = 0;
					   }
					   if(unit->orderTarget->unitId==firebat
						   &&unit->orderTarget->shields < 256*unitsdat_maxshield[unit->orderTarget->unitId]
						   &&unit->shields > 256)
					   {
						   unit->orderTarget->shields = unit->orderTarget->shields + 64;
						   unit->shields = unit->shields - 128;
					   }
					   else if(unit->shields < 256
						   && unit->orderTarget->unitId == firebat)
					   {
						   unit->orderTarget->shields = unit->orderTarget->shields + 16;
						   unit->shields = 0;
					   }
					   
			  }
		  }
		  
		  
	  }
	  }
	  }
			}
			*/
			/*
			{
			if(unit->unitId == hydralisk
			   &&unit->mainOrderId == Burrowed)
		   {
			   
			   if(unit->shields < 256*unitsdat_maxshield[unit->unitId])
			   {
					unit->shields=unit->shields + 32;
			   }
			   if(unit->healthPoints < 256*unitsdat_maxhp[unit->unitId])
			   {
				   unit->healthPoints = unit->healthPoints + 32;
			   }
		   }
			}
			*/
			{
				/*
				if(unit->id == UnitId::vulture
			   &&unit->vulture.spiderMineCount < 5
			   &&unit->vulture.spiderMineCount >= 1
			   &&unit->mainOrderId!=OrderId::Die)
			{
			 if(unit->buildQueueSlot ==100 )
			   {
			   unit->vulture.spiderMineCount = unit->vulture.spiderMineCount + 1;
			  unit->buildQueueSlot = 0;
			   }
			 else 
			 {unit->buildQueueSlot = unit->buildQueueSlot + 1;}
			}
			 */
				//벌쳐 마인 장전
				if(unit->id == UnitId::vulture && unit->mainOrderId!=OrderId::Die)
				{
					if(unit->vulture.spiderMineCount<5&&unit->vulture.spiderMineCount)
					{
						if(unit->unusedTimer==0)
						{
							unit->unusedTimer=30;
						}
					}
				}
			}
			
			{
			//벙커 회수
			if(unit->id == UnitId::bunker
			   &&unit->mainOrderId != OrderId::Die)
		   {
			   if(unit->mainOrderId == OrderId::CarrierStop)
			   {
				   if(Unit::MineralCost[unit->id])resources->minerals[unit->playerId] +=Unit::MineralCost[unit->id]*0.75;
				   if(Unit::GasCost[unit->id])resources->gas[unit->playerId] +=Unit::GasCost[unit->id]*0.75;
				   unit->mainOrderId = OrderId::Die;
			   }
		   }
			}
			{
				//파일런 치료 범위
				int raidus=250;
				CUnit* pylonsearching;
				if(unit->id==UnitId::pylon
					&&unit->mainOrderId!=OrderId::Die
			   &&unit->remainingBuildTime==0
			   &&unit->sprite->mainGraphic->animation!=0x15)
		   {
			   for(pylonsearching=*firstVisibleUnit;pylonsearching;pylonsearching=pylonsearching->next)
			   {

			   if(abs(pylonsearching->position.x-unit->position.x) < raidus
				   &&abs(pylonsearching->position.y-unit->position.y) < raidus
				   &&pylonsearching->playerId==unit->playerId)
			   {
				   if(pylonsearching->id == UnitId::zealot
					   &&pylonsearching->hitPoints < Unit::MaxHitPoints[pylonsearching->id])
				   {
					   pylonsearching->setHp(pylonsearching->hitPoints+16);
				   }
				   if(pylonsearching->id == UnitId::dragoon
					   &&pylonsearching->hitPoints < Unit::MaxHitPoints[pylonsearching->id])
				   {
					  pylonsearching->setHp(pylonsearching->hitPoints+16);
				   }
				   if(pylonsearching->id == UnitId::high_templar
					   &&pylonsearching->hitPoints < Unit::MaxHitPoints[pylonsearching->id])
				   {
					  pylonsearching->setHp(pylonsearching->hitPoints+16);
				   }
				   if(pylonsearching->id == UnitId::dark_templar
					   &&pylonsearching->hitPoints < Unit::MaxHitPoints[pylonsearching->id])
				   {
					  pylonsearching->setHp(pylonsearching->hitPoints+16);
				   }
			   }
			   }
		   }
			}
			{//드라군 과부화
				DWORD imgId = 136;

      
				if (unit->id == UnitId::dragoon && unit->mainOrderId!=OrderId::Die){//if unit its a dragoon and its alive    
				if (unit->mainOrderId == OrderId::CarrierStop){ //*1 (See comments after the code)      
					unit->stimTimer=75; //*2 (See comments after the code)   
					
					unit->sprite->createOverlay(imgId);
					unit->playIscriptAnim(IscriptAnimation::Unused1);
					unit->mainOrderId=OrderId::HoldPosition2;      unit->currentButtonSet=UnitId::zealot;//*3 (See comments after the code)      
					unit->status= unit->status | 0x1000;//canot receive orders    
				}    
					if (unit->stimTimer>0){      
						if (unit->stimTimer>31){ //swap between black and original color during the overload duration        
							if (unit->stimTimer % 2 == 0) // if stimTimer is an even number turns to original color   
								unit->sprite->playerId=unit->playerId;    
							else unit->sprite->playerId=160;                   // if stimTimer is an odd number turns to black color          
								
							
								
							
						}
						else{        
							if (unit->stimTimer==31)         
								unit->mainOrderId=OrderId::Stop;//set to stop so it wont attack any more until enabled      
							if (unit->stimTimer==30){          
								unit->status= unit->status | 0x8;//disabled  
								unit->removeOverlay(136);
								unit->playIscriptAnim(IscriptAnimation::Unused2);
							unit->sprite->playerId=160; //black color        
							}     
							if (unit->stimTimer==1){         
							unit->status= unit->status & ~0x8;//enabled       
							unit->status= unit->status & ~0x1000;//can receive orders again        
							unit->sprite->playerId=unit->playerId; //original color        
							unit->rankIncrease=200;//*4 (See comments after the code)    
							}    
							}  
							}    
							if (unit->rankIncrease>0){//cooldown      
								unit->rankIncrease--;      
								if (unit->rankIncrease==0)        
									unit->currentButtonSet=unit->id;//backs to the original buttonSet    
							}  
				}
			}	
			
			{
				//감염된 테란 자폭
				if(unit->id==UnitId::infested_terran && unit->mainOrderId!=OrderId::Die && unit->orderSignal!=1 && unit->status & 0x1)
		   {
			   if(unit->mainOrderId!=OrderId::Burrow && unit->buildQueue[0]==40)
			   {
				   unit->mainOrderId=OrderId::SapLocation;
				   unit->orderTarget.pt.x=unit->position.x;
				   unit->orderTarget.pt.y=unit->position.y;
				   unit->current_speed1=0;
				   unit->current_speed2=0;
			   }
			   /*
			   if(unit->mainOrderId==Burrowed&&unit->orderSignal!=1)
			   {
				   if(unit->status & 0x1000)unit->status=unit->status & ~0x1000;
			   }
			   */
		   }
			}
			{
				//넥서스 시증 시전
				if (unit->mainOrderId!=OrderId::Die){
					if (unit->id==UnitId::nexus && unit->mainOrderId== OrderId::PlaceScanner){
                if(unit->orderTarget.unit!=NULL && unit->orderTarget.unit->status & 0x2 && unit->orderTarget.unit->playerId ==unit->playerId && unit->orderTarget.unit->remainingBuildTime==0 &&  //taget its a building and its completed
					!(unit->orderTarget.unit->id==UnitId::photon_cannon || unit->orderTarget.unit->id==UnitId::pylon ||unit->orderTarget.unit->id==UnitId::shield_battery ||unit->orderTarget.unit->id==UnitId::assimilator ) && (unit->orderTarget.unit->id>=154 && unit->orderTarget.unit->id <= 172)&&(unit->sprite->mainGraphic->animation!=0x15)){ //buildings that cannot be affected by the ability
                    //unit->orderTarget.unit->sprite->playerID=15; //black color
                    unit->orderTarget.unit->unusedTimer=100; //this is used as a timer for the duration (protoss buildings cant move so it wont affect anytihng)
					unit->mainOrderId=OrderId::Nothing2;
                    unit->orderTarget.unit=NULL;
					if(!scbw::isCheatEnabled(CheatFlags::TheGathering))unit->energy-=Tech::EnergyCost[0x04]*256;  //remove 50 energy
                }else{ //if the target is invalid cast feedback on self to trhow a error msj
					unit->mainOrderId=OrderId::CastFeedback;
                    unit->orderTarget.unit=unit;
                }
            }

			}
			}
			{
			//시증 적용된 상태
				if(unit->mainOrderId!=OrderId::Die&&(unit->id>=154 && unit->id <= 172)&&unit->unusedTimer)
		{
			if(!(unit->getOverlay(379)))unit->sprite->createOverlay(379);
			if(unit->building.upgradeResearchTime>0&&unit->cycleCounter%2!=0&&!(unit->status & 0x400))unit->building.upgradeResearchTime--;
			if(unit->currentBuildUnit&&unit->currentBuildUnit->remainingBuildTime>1&&unit->cycleCounter%2!=0&&!(unit->status & 0x400)){
				unit->currentBuildUnit->remainingBuildTime--;
				unit->currentBuildUnit->hitPoints=102400;
			}
		}
			}
			/*
			{
				if(unit->mainOrderId==OrderId::CastMindControl)
				{
			   if(unit->orderTarget.unit!=NULL){
				   if(unit->orderTarget.unit->id==UnitId::scv
				   ||unit->orderTarget.unit->id==UnitId::probe
				   ||unit->orderTarget.unit->id==UnitId::drone)
			   {
				   unit->mainOrderId=OrderId::Guard;
			   }
			   }
				}
			}
			
			{
				if(unit->unitId==wraith
				&&unit->mainOrderId!=Die)
			{
				if(upgradelevelPlayer[unit->playerId][0x12]>0)
				{
					if(unit->groundWeaponCooldown > 15)
					{
						unit->groundWeaponCooldown=15;
					}
				}
			}
			}
			*/
			{
 				//자동수리
				int x=0,y=0;
				int raidus=200;
				if (unit->id==UnitId::scv && unit->mainOrderId!=OrderId::Die){
            if(unit->mainOrderId==OrderId::HoldPosition2)
                unit->rankIncrease=1;//rank increase dosnt have any real use, so here its used as an indicator when the command is active
            if(unit->mainOrderId==OrderId::Stop)
                unit->rankIncrease=0; //unactive the command
			if (unit->mainOrderId==3 && unit->rankIncrease==1) //if idle and the comand its active it set to hold position state
                unit->mainOrderId=OrderId::HoldPosition2;
            if((unit->mainOrderId==OrderId::HoldPosition2 || unit->mainOrderId==OrderId::AttackMove) && unit->rankIncrease){
                
                for (CUnit* target=*firstVisibleUnit;target;target=target->next) { 

                    if (target!=unit&&target->status & 0x1 && target->playerId == unit->playerId && target->mainOrderId!=OrderId::Die){ //i will explain this part later (1)
                        if ((abs(target->position.x-unit->position.x)<raidus&&abs(target->position.y-unit->position.y)<raidus))
							if (target->hitPoints < Unit::MaxHitPoints[target->id]&&((Unit::BaseProperty[target->id]&UnitProperty::Mechanical && Unit::GroupFlags[target->id].isTerran))){ // the unit its damaged
									unit->mainOrderId=OrderId::Repair1;
                                unit->orderTarget.unit=target;
                            }
                    }
                }
            }
        }
			}
			/*
			{
				if(unit->connectedUnit!=NULL){
		   if(unit->connectedUnit->id==91
			   &&(unit->id==UnitId::vulture || unit->id==UnitId::siege_tank || unit->id==UnitId::goliath))
		   {
			   if(unit->hitPoints < Unit::MaxHitPoints[unit->id])
			   {
				   unit->hitPoints=unit->hitPoints+16;
			   }
		   }
			}
			}
			/*
			{
				if (unit->unitId == scourge && unit->orderTarget
            && unit->airWeaponCooldown != 0
            && unit->mainOrderId != Die)
         {
            if (unit->orderTarget->healthPoints > 0) {
               const BYTE scourgeWeapon = unitsdat_airweapon[unit->unitId];
               const DWORD damage = unitsdat_maxhits[unitsdat_airweapon[unit->unitId]] << 8;
               DoWeaponDamage(
                  damage,
                  unit->orderTarget,
                  unit, unit->playerId,
                  scourgeWeapon,
                  unit->sprite->image->direction,
                  1
               );
               unit->mainOrderId=Die;
            }
            else
               unit->airWeaponCooldown = 0;
         }
			}
			
			{
				if(unit->id==UnitId::battlecruiser
					&&unit->mainOrderId!=OrderId::Die)
		   {
			   if(scbw::getUpgradeLevel(unit->playerId,0x2D))
			   {
				  
				  if(unit->ensnareTimer==0)unit->flingyTopSpeed=1043;
				  else unit->flingyTopSpeed=Flingy::TopSpeed[unit->flingyId];
				  if(unit->ensnareTimer==0)unit->flingyAcceleration=(Flingy::Acceleration[unit->flingyId]*2);
				 else unit->flingyAcceleration=Flingy::Acceleration[unit->flingyId];
				 if(unit->ensnareTimer==0)unit->flingyTurnSpeed=(Flingy::TurnSpeed[unit->flingyId]*2);
				 else unit->flingyTurnSpeed=Flingy::TurnSpeed[unit->flingyId];
				  
				   
			   }
			}
			}
			/*
			{
				if(unit->unitId==high_templar
			   &&unit->mainOrderId!=Die)
		   {
			   if(unit->mainOrderId==CarrierStop)
			   {
				   unit->orderTarget=unit;
				   unit->mainOrderId=Hallucianation1;
				   
			   }
		   }
			}
			
			{
				if(unit->id==UnitId::arbiter
					&&unit->mainOrderId!=OrderId::Die)
		   {
			   if(scbw::getUpgradeLevel(unit->playerId,UpgradeId::UnusedUpgrade46))
			   {
				    unit->status=unit->status | 0x20000000;
			   }
			}
			}
			
			{
				if(unit->id==UnitId::mutalisk&&unit->mainOrderId!=OrderId::Die)
		   {
			   if(unit->rankIncrease==1)
			   {
				   unit->id=55;
				   unit->shields=Unit::MaxShieldPoints[55];
				   unit->hitPoints=unit->buildQueue[1]*256;
				   unit->buildQueue[1]=228;
				   unit->rankIncrease=0;
			   }
		   }
		   
				if(unit->id==55&&unit->mainOrderId!=OrderId::Die&&unit->status & 0x1)
		   {
			   if(unit->buildQueue[0]==56)
			   {
				   unit->buildQueue[1]=unit->hitPoints/256;
			   }
		   }
				if((unit->id==59)&&unit->mainOrderId!=OrderId::Die)
		   {
			   if(unit->buildQueue[0]==56)
			   {
				   unit->rankIncrease=1;
			   }
		   }
			}
			/*
			{
				if(unitsdatHumanAI[unit->unitId]==MoveToMinerals)
		   {
			   if(unit->mainOrderId==unitsdatHumanAI[unit->unitId])
			   {
				  
				   
				   
					   UNIT* search;
				   for(int j=0;j<1700;j++)
				   {
					   search=&unitTable[j];
					   
				
				if(search->unitId==command_center&&search->mainOrderId!=Die&&search->status & 0x1)
				{
				 createUnitAtPos(7,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(7,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(7,search->playerId,search->currentXPos,search->currentYPos);
			    }
			   if(search->unitId==hatchery&&search->mainOrderId!=Die&&search->status & 0x1)
				{
				 //createUnitAtPos(41,unit->playerId,unit->currentXPos,unit->currentYPos);
			     createUnitAtPos(41,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(41,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(41,search->playerId,search->currentXPos,search->currentYPos);
			    }
			   if(search->unitId==nexus&&search->mainOrderId!=Die&&search->status & 0x1)
				{
				 //createUnitAtPos(64,unit->playerId,unit->currentXPos,unit->currentYPos);
			     createUnitAtPos(64,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(64,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(64,search->playerId,search->currentXPos,search->currentYPos);
			    }
				   
			   }
				unitsdatHumanAI[7]=Guard;
				   unitsdatHumanAI[41]=Guard;
				   unitsdatHumanAI[64]=Guard;   
				   unitsdatComputerAI[7]=Guard;
				   unitsdatComputerAI[41]=Guard;
				   unitsdatComputerAI[64]=Guard;
				    
			   /*{   
		for(int j=0;j<1700;i++)
		{
			UNIT* search;
		 
			search=&unitTable[j];
			
				if(search->unitId==command_center||search->unitId==hatchery||search->unitId==nexus)
				{
			   if(search->unitId==command_center)
				{
				 createUnitAtPos(7,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(7,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(7,search->playerId,search->currentXPos,search->currentYPos);
			    }
			   if(search->unitId==hatchery)
				{
				 //createUnitAtPos(41,unit->playerId,unit->currentXPos,unit->currentYPos);
			     createUnitAtPos(41,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(41,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(41,search->playerId,search->currentXPos,search->currentYPos);
			    }
			   if(search->unitId==nexus)
				{
				 //createUnitAtPos(64,unit->playerId,unit->currentXPos,unit->currentYPos);
			     createUnitAtPos(64,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(64,search->playerId,search->currentXPos,search->currentYPos);
			     createUnitAtPos(64,search->playerId,search->currentXPos,search->currentYPos);
			    }
		       
				}
		}
			   }

			 
			   }
		   }
			}
			*/
			{//카고쉽 자원 운반,채취시 에너지소모 완전 소모시 일정시간동안 정지
				if(unit->id==91&&unit->mainOrderId!=OrderId::Die)
		   {
			   if((unit->mainOrderId==OrderId::MoveToMinerals||unit->mainOrderId==OrderId::MiningMinerals||unit->mainOrderId==OrderId::ReturnMinerals)&&(unit->energy>0))
			   {
				   unit->energy=unit->energy-16;
			   }
			   if(unit->energy==0)
			   {
				   unit->mainOrderId=OrderId::Stop;
				   unit->status=unit->status | 0x8;
				   unit->status=unit->status | 0x1000;
			   }
			   if(unit->energy>=2560)
			   {
				   if(unit->status & 0x8){
				   unit->status=unit->status & ~0x8;
				   unit->status=unit->status & ~0x1000;
				   }
			   }
			   if(!(unit->status & 0x1))
			   {
				   unit->energy=51200;
			   }
			   if(unit->mainOrderId==OrderId::Stop&&unit->status & 0x800000)unit->mainOrderId=OrderId::ResetCollision2;
		   }
			}
			/*
			{
				if(unit->unitId==dark_templar&&unit->mainOrderId!=Die&&unit->status & 0x1)
		   {
			  if(unit->mainOrderId==CarrierStop)
			  {
				  
				  if(unit->stimTimer==0)
				  {
					  unit->secondaryOrderId=Decloak;
					  unit->stimTimer=60;
					  unit->currentButtonSet=zealot;
				  }
			  }
			  if(unit->stimTimer==0&&unit->secondaryOrderId!=Cloak)
			  {
				  unit->secondaryOrderId=Cloak;
				  unit->currentButtonSet=unit->unitId;
			  }
			  if(unit->stimTimer!=0&&unit->secondaryOrderId==Cloak)
			  {
				  
				  unit->currentButtonSet=unit->unitId;
				  unit->secondaryOrderId=Decloak;
			  }
			  
		   }
			}
			*/
			
			{//광전사 돌진
				if(unit->id==UnitId::zealot&&unit->mainOrderId!=OrderId::Die&&unit->status & 0x1)
				{
					if(unit->status & 0x10000000)
					{
						
						if(unit->sprite->mainGraphic->animation==0x0B&&unit->ensnareTimer==0)
						{
							unit->playIscriptAnim(IscriptAnimation::Unused2);
						}
						if(unit->sprite->mainGraphic->animation==0x0A&&unit->ensnareTimer)
						{
							unit->playIscriptAnim(IscriptAnimation::Walking);
						}
						
						if(unit->orderTarget.unit&&(abs(unit->orderTarget.pt.x-unit->position.x)<192&&abs(unit->orderTarget.pt.y-unit->position.y)<192)&&unit->mainOrderId==OrderId::AttackUnit&&unit->sprite->mainGraphic->animation==0x0A&&unit->spellCooldown==0)
						{
							unit->playIscriptAnim(IscriptAnimation::Unused1);
							unit->spellCooldown=45;
						}
					}
				}
			}
			/*
			{
		   if(unit->unitId==14&&unit->mainOrderId!=Die)
		   {
			   unit->maelstromCounter=0;
			   unit->ensnareTimer=0;
			   unit->isUnderStorm=0;
			    RemoveOverlays(unit, 384, 386);
				RemoveOverlays(unit, 365, 367);
				RemoveOverlays(unit, 982, 984);
			   if(unit->statisTimer)
			   {
					   unit->statisTimer=0;
					   unit->status = unit->status & ~0x4000000;
			   }
			   if(unit->secondaryOrderId==109)
			   {
				   if(unit->status & 0x800)unit->status=unit->status & ~0x800;
				   unit->secondaryOrderId=23;
			   }
		   }
		   if(unit->mainOrderId==MagnaPulse&&unit->orderTarget!=NULL)
		   {
			   if(unit->orderTarget->unitId==14)
			   {
				   unit->mainOrderId=Guard;
			   }
		   }
		   if(unit->mainOrderId==Hallucianation1&&unit->orderTarget!=NULL)
		   {
			   if(unit->orderTarget->unitId==14)
			   {
				   unit->mainOrderId=Guard;
			   }
		   }
		   if(unit->mainOrderId==CastMindControl&&unit->orderTarget!=NULL)
		   {
			   if(unit->orderTarget->unitId==14)
			   {
				   unit->mainOrderId=Guard;
			   }
		   }
	   
			}
			
			{
				CUnit* search;
				int raidus=450;
				if(unit->id==158&&unit->mainOrderId!=OrderId::Die)
		   {
			   if(unit->currentBuildUnit!=NULL)
			   {
				   unit->currentBuildUnit->rankIncrease=1;
			   }
		   }
				if(unit->id==64&&unit->mainOrderId!=OrderId::Die&&unit->status & 0x1)
		   {
			   if(unit->rankIncrease!=0){
				   for(search=*firstVisibleUnit;search;search=search->next)
				   {

					   if(abs(search->position.x-unit->position.x)<raidus&&abs(search->position.y-unit->position.y)<raidus)
					   {
						   
						   if(search->id>=176&&search->id<=178)
						   {
							   unit->mainOrderId=OrderId::Harvest1;
							   unit->orderTarget.unit=search;
							   unit->rankIncrease=0;
						   }
						   
					   }
				   }
			   if(unit->orderTarget.unit!=NULL)
			   {
				   if(unit->mainOrderId==OrderId::Follow&&unit->orderTarget.unit->id==156)
				   {unit->mainOrderId=OrderId::Guard;
				   unit->rankIncrease=0;
				   }
			   }
			   }
		   }
			}
			*/
			{//카고쉽 유닛에 따라 팔로우,수송 설정
				if(unit->id==91&&unit->mainOrderId!=OrderId::Die)
		   {
			   if(unit->mainOrderId==OrderId::Follow&&unit->orderTarget.unit!=NULL&&unit->sprite->mainGraphic->animation==0x0B)
			   {
				   if(unit->orderTarget.unit->playerId==unit->playerId&&!(unit->orderTarget.unit->status & 0x4)&&!(unit->orderTarget.unit->status & 0x2))
				   {
					   unit->mainOrderId=OrderId::Pickup2;
						   unit->rankIncrease=1;
				   
						   if(unit->rankIncrease!=0&&unit->mainOrderId!=OrderId::Pickup2)
				   {
					   unit->mainOrderId=OrderId::Follow;
					   unit->rankIncrease=0;
				   }
				   }
				   unit->rankIncrease=0;
			   }
		   }
			}
			{
				if(unit->id==153&&unit->mainOrderId!=OrderId::Die)
		   {
			   if(unit->mainOrderId==unitsdatHumanAI[unit->id]||unit->mainOrderId==unitsdatComputerAI[unit->id])
			   {
				   if(unit->secondaryOrderId!=0x4E)unit->secondaryOrderId=0x4E;
			   }
		   }
			}
			/*
			{
				if((unit->unitId==45||unit->unitId==49)&&unit->mainOrderId!=Die)
		   {
			   if(unit->mainOrderId==AttackUnit&&unit->orderTarget!=NULL)
			   {
				   if(unit->orderTarget->healthPoints/256<unitsdat_maxhp[barracks]/2&&unit->orderTarget->unitId==barracks)
				   {
					   if(!(unit->sprite->image->iscriptCurrentAnimation==0x02&&unit->sprite->image->iscriptCurrentAnimation==0x03&&unit->sprite->image->iscriptCurrentAnimation==0x05&&unit->sprite->image->iscriptCurrentAnimation==0x06))unit->mainOrderId=InfestMine2;
					   if(unit->sprite->image->iscriptCurrentAnimation==0x02||unit->sprite->image->iscriptCurrentAnimation==0x03||unit->sprite->image->iscriptCurrentAnimation==0x05||unit->sprite->image->iscriptCurrentAnimation==0x06)
					   {
						   if(abs(unit->orderTargetX-unit->currentXPos)<100&&abs(unit->orderTargetY-unit->currentYPos)<100)unit->mainOrderId=InfestMine2;
						   else unit->mainOrderId=Follow;
					   }
				   }
				   if(unit->orderTarget->unitId!=barracks)
				   {
					   unit->mainOrderId=InfestMine2;
				   }
			   }
			   if(unit->mainOrderId==Follow&&unit->orderTarget!=NULL)
			   {
				   if((unit->orderTarget->healthPoints/256<unitsdat_maxhp[barracks]/2)&&(unit->orderTarget->unitId==barracks))
				   {
					   if(abs(unit->orderTargetX-unit->currentXPos)<100&&abs(unit->orderTargetY-unit->currentYPos)<100)unit->mainOrderId=InfestMine2;
					   else if(unit->sprite->image->iscriptCurrentAnimation==0x0B)unit->mainOrderId=InfestMine2;
				   }
			   }
			   if(unit->mainOrderId==InfestMine2&&unit->orderTarget!=NULL)
			   {
				   if(unit->orderTarget->healthPoints/256>unitsdat_maxhp[barracks]/2)
				   {
					   unit->orderTarget=unit;
				   }
			   }
		   }
		   if(unit->mainOrderId==Irradiate&&unit->sprite->image->iscriptCurrentAnimation==0x07)
		   {
			   unit->spellCooldown=0;
		   }
			}
			*/
			/*
			{
				if(unitsdat_maxshield[5]==unitsdat_maxshield[30])
				{
				if(unit->unitId==5&&unit->mainOrderId!=Die&&unit->status & 0x1)
				{
					unit->buildQueue[0]=unit->shields/256;
					if(unit->buildQueue[1]!=0)
					{
						unit->shields=unit->buildQueue[1]*256;
						unit->buildQueue[1]=0;
					}
				}
				if(unit->unitId==30&&unit->mainOrderId!=Die&&unit->status & 0x1)
				{
					unit->buildQueue[1]=unit->shields/256;
					if(unit->buildQueue[0]!=0)
					{
						unit->shields=unit->buildQueue[0]*256;
						unit->buildQueue[0]=0;
					}
				}
			}
			}
			*/
			/*
			{
				if(unit->mainOrderId==ZergBuildSelf)
	   {
		   if(unit->shields < 256*unitsdat_maxshield[unit->unitId])
		   {
			   unit->shields+=unit->buildRepairHPGain;
		   }
	   }
					if(unit->mainOrderId==BuildAddon)
	   {
		   if(unit->building.addon&&unit->building.addon->shields<256*unitsdat_maxshield[unit->building.addon->unitId])
		   {
			   unit->building.addon->shields+=unit->building.addon->buildRepairHPGain;
		   }
	   }
				
			if(unit->connectedUnit!=NULL
		   &&unit->mainOrderId==BuildSelf1){
	   if(unit->rankIncrease==0
		   &&unit->connectedUnit->mainOrderId==ConstructingBuilding){
	   
		   if(unit->shields < 256*unitsdat_maxshield[unit->unitId])
		   {
			   unit->shields+=unit->buildRepairHPGain;
		   }
	   
	   }
	   if(unit->connectedUnit->mainOrderId!=ConstructingBuilding
		   &&unit->rankIncrease==0){
			   unit->rankIncrease=1;
	   }
	   if(unit->connectedUnit->mainOrderId==ConstructingBuilding
		   &&(unit->connectedUnit->currentXPos==unit->currentXPos && unit->connectedUnit->currentYPos==unit->currentYPos)
		   &&unit->rankIncrease==1)
	   {
		   unit->rankIncrease=0;
	   }

   }
			
			}
			*/
			/*
			{
				if(unit->id==UnitId::medic&&unit->mainOrderId==OrderId::MedicHeal1||unit->mainOrderId==OrderId::MedicHoldPosition)
			{
				if(unit->orderTarget.unit){
				if(unit->sprite->mainGraphic->animation!=0x0B)
				{
					unit->orderTarget.unit->isBeingHealed=1;
				}
				}
			}
			
			}
			*/
			{
				//유닛 밀리기
				int raidus=37;
				if(unit->mainOrderId!=OrderId::Die&&!(Unit::BaseProperty[unit->id] & UnitProperty::Worker)&&!(Unit::BaseProperty[unit->id] & UnitProperty::Flyer)&&(unit->mainOrderId!=66&&unit->mainOrderId!=67&&unit->mainOrderId!=OrderId::WarpingArchon&&unit->mainOrderId!=OrderId::Burrow&&unit->mainOrderId!=OrderId::Unburrow&&unit->mainOrderId!=OrderId::WarpingDarkArchon&&unit->mainOrderId!=OrderId::CompletingArchonSummon&&unit->mainOrderId!=OrderId::ConstructingBuilding&&unit->mainOrderId!=OrderId::AttackMove&&unit->mainOrderId!=OrderId::Follow&&unit->mainOrderId!=OrderId::Move&&unit->mainOrderId!=OrderId::EnterTransport&&unit->mainOrderId!=OrderId::Repair1)&&(unit->id!=35&&unit->id!=13)&&!(unit->status & 0x800000)&&!(unit->status & 0x200000)&&unit->status & 0x1&&!(unit->status & 0x2)&&!(unit->status & 0x4)&&unit->status & 0x20000&&(unit->lockdownTimer==0&&unit->stasisTimer==0)&&!(unit->mainOrderId==OrderId::Consume&&unit->mainOrderId==OrderId::MedicHeal1)&&!(unit->status & 0x8000000)&&!(unit->status & 0x10)&&!(unit->status & 0x20)&&!(unit->status & 0x40)&&!(unit->sprite->mainGraphic->animation==0x02||unit->sprite->mainGraphic->animation==0x03||unit->sprite->mainGraphic->animation==0x05||unit->sprite->mainGraphic->animation==0x06||unit->sprite->mainGraphic->animation==0x04||unit->sprite->mainGraphic->animation==0x07||unit->sprite->mainGraphic->animation==0x0D||unit->sprite->mainGraphic->animation==0x0B||unit->sprite->mainGraphic->animation==0x0F))
				{
					CUnit* search;
					for(search=*firstVisibleUnit;search;search=search->next)
					{
						
						if(search->mainOrderId!=OrderId::Die
							&&search->id!=35
							&&search->playerId==unit->playerId
							&&!(unit->status & 0x200000)
							&&!(Unit::BaseProperty[search->id] & 0x10)
							&&search->status & 0x1
							&&(unit->mainOrderId==OrderId::PlayerGuard||unit->mainOrderId==OrderId::Medic||(unit->mainOrderId==unitsdatHumanAI[unit->id]||unit->mainOrderId==unitsdatComputerAI[unit->id]))
							&&!(search->status & 0x4)
							&&search->sprite->mainGraphic->animation==0x0B)
						{
							if(abs(search->position.x-unit->position.x)<raidus&&abs(search->position.y-unit->position.y)<raidus)
							{
								if(search->position.x>unit->position.x)
								{
									if(unit->id!=UnitId::medic)unit->mainOrderId=OrderId::Move;
									if(unit->id==UnitId::medic){unit->mainOrderId=OrderId::HealMove;if(unit->status & 0x1000)unit->status=unit->status & ~0x1000;}
									unit->orderTarget.pt.x=unit->position.x-10;
								}
								if(search->position.x<unit->position.x)
								{
									if(unit->id!=UnitId::medic)unit->mainOrderId=OrderId::Move;
									if(unit->id==UnitId::medic){unit->mainOrderId=OrderId::HealMove;if(unit->status & 0x1000)unit->status=unit->status & ~0x1000;}
									unit->orderTarget.pt.x=unit->position.x+10;
								}
								if(search->position.y>unit->position.y)
								{
									if(unit->id!=UnitId::medic)unit->mainOrderId=OrderId::Move;
									if(unit->id==UnitId::medic){unit->mainOrderId=OrderId::HealMove;if(unit->status & 0x1000)unit->status=unit->status & ~0x1000;}
									unit->orderTarget.pt.y=unit->position.y-10;
								}
								if(search->position.y<unit->position.y)
								{
									if(unit->id!=UnitId::medic)unit->mainOrderId=OrderId::Move;
									if(unit->id==UnitId::medic){unit->mainOrderId=OrderId::HealMove;if(unit->status & 0x1000)unit->status=unit->status & ~0x1000;}
									unit->orderTarget.pt.y=unit->position.y+10;
								}
								if((unit->mainOrderId==OrderId::Move||unit->mainOrderId==OrderId::HealMove)&&(unit->orderTarget.pt.x==0))unit->mainOrderId=unitsdatReturnToAI[unit->id];
								if((unit->mainOrderId==OrderId::Move||unit->mainOrderId==OrderId::HealMove)&&(unit->orderTarget.pt.y==0))unit->mainOrderId=unitsdatReturnToAI[unit->id];
							}
						}
					}
					
				}
			}
			
			{//테란 건물 그을림 구현
				if(unit->id==UnitId::command_center&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,6);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==6)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::barracks&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,9);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==9)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::factory&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,7);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==7)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::starport&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,6);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==6)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::science_facility&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,6);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==6)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::engineering_bay&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,6);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==6)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::supply_depot&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,2);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==2)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::armory&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,2);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==2)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::academy&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,2);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==2)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::nuclear_silo&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,2);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==2)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::comsat_station&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,2);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==2)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::covert_ops&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,2);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==2)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::machine_shop&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,5);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==5)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
				if(unit->id==UnitId::physics_lab&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==0)scbw::playFrame(unit->sprite->mainGraphic,7);
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==7)scbw::playFrame(unit->sprite->mainGraphic,0);
				}
			
				
				if(unit->id==UnitId::missile_turret&&unit->mainOrderId!=OrderId::Die&&unit->remainingBuildTime==0)
				{
					
					if(unit->buildingOverlayState<1&&unit->sprite->mainGraphic->frameSet==2){
						scbw::playFrame(unit->sprite->mainGraphic,3);
						//if(hasOverlay(unit,297))
						for(CImage* turret=unit->sprite->imageHead;turret;turret=turret->link.next){
							if(turret->id==297)scbw::playFrame(turret,17);
						}
						
					}
					if(unit->buildingOverlayState>=1&&unit->sprite->mainGraphic->frameSet==3){
						scbw::playFrame(unit->sprite->mainGraphic,2);
						for(CImage* turret2=unit->sprite->imageHead;turret2;turret2=turret2->link.next){
							if(turret2->id==297)scbw::playFrame(turret2,0);
						}
					}
				}
			}
			
	
		    {//자가 시전 제한
				if(unit->mainOrderId!=OrderId::Die&&unit->orderTarget.unit&&unit->orderTarget.unit==unit)
				{
					if(unit->subunit&&unit->subunit->orderTarget.unit==unit)unitsdatReturnToAI[unit->id];
					if((unit->mainOrderId!=OrderId::DarkSwarm&&unit->mainOrderId!=OrderId::EmpShockwave&&unit->mainOrderId!=OrderId::PlaceScanner&&unit->mainOrderId!=OrderId::DefensiveMatrix&&unit->mainOrderId!=OrderId::PsiStorm&&unit->mainOrderId!=OrderId::Irradiate&&unit->mainOrderId!=OrderId::Plague&&unit->mainOrderId!=OrderId::Ensnare&&unit->mainOrderId!=OrderId::StasisField&&unit->mainOrderId!=OrderId::Hallucianation1&&unit->mainOrderId!=OrderId::PlaceMine&&unit->mainOrderId!=OrderId::CastMaelstrom&&unit->mainOrderId!=OrderId::CastDisruptionWeb&&unit->mainOrderId!=OrderId::MoveUnload&&unit->mainOrderId!=OrderId::Unload)){
					unit->mainOrderId=unitsdatReturnToAI[unit->id];
					if(unit->subunit&&unit->subunit->orderTarget.unit==unit)unitsdatReturnToAI[unit->id];
					unit->orderTarget.unit=NULL;
					}
				}
			}
					
				
			
			
			/*
			{
				int raidus=100;
				if(unit->unitId==56&&unit->mainOrderId!=Die&&unit->status & 0x1)
				{
					for(int j=0;j<1700;j++)
					{
						UNIT* search;
						search=&unitTable[j];
						if(search!=unit
							&&search->mainOrderId!=Die
							&&search->status & 0x1)
						{
							if(abs(search->currentXPos-unit->currentXPos)<raidus&&abs(search->currentYPos-unit->currentYPos)<raidus)
							{
								if(PlayerAlliances[search->playerId][unit->playerId]==0x0)
								{
									search->mainOrderId=Die;
								}
							}
						}
					}
				}
			}
			
			{
				if(unit->mainOrderId!=Die&&unit->remainingBuildTime==0&&unitsdat_groupflags[unit->unitId] & 0x1&&unitsdat_special[unit->unitId]&0x1)
				{
					if(unit->mainOrderId==Die)
					{
						createUnitAtPos(40,unit->playerId,unit->currentXPos,unit->currentYPos);
						createUnitAtPos(40,unit->playerId,unit->currentXPos,unit->currentYPos);
						createUnitAtPos(40,unit->playerId,unit->currentXPos,unit->currentYPos);
						createUnitAtPos(40,unit->playerId,unit->currentXPos,unit->currentYPos);
						createUnitAtPos(40,unit->playerId,unit->currentXPos,unit->currentYPos);
						createUnitAtPos(40,unit->playerId,unit->currentXPos,unit->currentYPos);
					}
				}
			}
			{
				if((unit->unitId==131||unit->unitId==132||unit->unitId==133)&&unit->mainOrderId!=Die&&unit->remainingBuildTime==0)
				{
					if(unit->mainOrderId==CarrierStop)
					{
						unit->mainOrderId=unitsdatReturnToAI[unit->unitId];
						if(unit->unusedTimer==0)
						{
							CreateOverlay(unit->sprite,21);
							unit->unusedTimer=40;
						}
						
					}
					
				}
			}
			{
				if(unit->mainOrderId!=Die&&unit->orderTarget&&unit->orderTarget==unit)
				{
					if((unit->mainOrderId!=DarkSwarm&&unit->mainOrderId!=EmpShockwave&&unit->mainOrderId!=PlaceScanner&&unit->mainOrderId!=DefensiveMatrix&&unit->mainOrderId!=PsiStorm&&unit->mainOrderId!=Irradiate&&unit->mainOrderId!=Plague&&unit->mainOrderId!=Ensnare&&unit->mainOrderId!=StasisField&&unit->mainOrderId!=Hallucianation1&&unit->mainOrderId!=PlaceMine&&unit->mainOrderId!=CastMaelstrom&&unit->mainOrderId!=CastDisruptionWeb)){
					unit->mainOrderId=unitsdatReturnToAI[unit->unitId];
					unit->orderTarget=NULL;
					}
				}
			}
			{
				if(unit->unitId==121&&unit->mainOrderId!=Die)
				{
					if(unit->mainOrderId==Repair2)unit->mainOrderId=Stop;
					if(unit->mainOrderId==Repair1&&unit->orderTarget)
					{
						if(!(unit->orderTarget->status & 0x4))unit->mainOrderId=Stop;
						if((unit->orderTarget->status & 0x2))unit->mainOrderId=Stop;
						if(unit->orderTarget->playerId!=unit->playerId)unit->mainOrderId=Stop;
						if((!(unit->orderTarget->currentXPos==unit->orderTarget->currentXPos&&unit->orderTarget->currentYPos==unit->currentYPos))&&unit->orderTarget->playerId==unit->playerId)
						{
							if(unit->orderTarget->mainOrderId==PlayerGuard||unitsdatReturnToAI[unit->orderTarget->unitId]){
								if(abs(unit->orderTarget->currentXPos-unit->currentXPos)<227&&abs(unit->orderTarget->currentYPos-unit->currentYPos)<227){
							unit->orderTarget->mainOrderId=Follow;
							unit->orderTarget->orderTarget=unit;
								}
								else {unit->mainOrderId=Stop;}
							}
							else {unit->mainOrderId=Stop;}
						}
					}
					{
				
			if(unit->isBeingHealed&&(!hasOverlay(unit,970)&&!hasOverlay(unit,971)&&!hasOverlay(unit,972)))
			{
				DWORD protoFlags = UnitsDat_PrototypeFlags[unit->unitId];
				DWORD imgId = 970;
				if (protoFlags & UPF_LargeUnit) imgId += 1;
				if (protoFlags & UPF_HugeUnit) imgId += 2;

				CreateOverlay(unit->sprite, imgId);
				unit->isBeingHealed=0;
			}

			if(unit->unitId==medic&&(unit->mainOrderId==MedicHeal1||unit->mainOrderId==MedicHoldPosition)&&unit->orderTarget)
			{
				if(unit->sprite->image->iscriptCurrentAnimation==0x0D)
				{
					unit->orderTarget->isBeingHealed=1;
				}
			}
			
			
			
			if(unit->isBeingHealed==0&&(hasOverlay(unit,970)||hasOverlay(unit,971)||hasOverlay(unit,972)))
			{
				DWORD protoFlags = UnitsDat_PrototypeFlags[unit->unitId];
				DWORD imgId = 970;
				if (protoFlags & UPF_LargeUnit) imgId += 1;
				if (protoFlags & UPF_HugeUnit) imgId += 2;

				RemoveOverlays(unit, imgId);
			}
					}
					{if(unit->mainOrderId==HoldPosition3)unit->mainOrderId=HoldPosition2;}
					/*
					if(unit->mainOrderId==Repair1||unit->mainOrderId==Repair2)unit->mainOrderId=WatchTarget;
					if(unit->orderTarget&&unit->mainOrderId==WatchTarget){
					if(abs(unit->orderTarget->currentXPos-unit->currentXPos)<224&&abs(unit->orderTarget->currentYPos-unit->currentYPos)<224){
					if(unitsdat_special[unit->orderTarget->unitId] & 0x40000000)
					{
						if(unit->energy>0){
						if(unit->healthPoints<unitsdat_maxhp[unit->orderTarget->unitId]*256)
						{
							unit->orderTarget->healthPoints+=unit->orderTarget->buildRepairHPGain;
							unit->energy-=32;
						}
						else {unit->mainOrderId=unitsdatReturnToAI[unit->unitId];}
						}
					}
					else {unit->mainOrderId=unitsdatReturnToAI[unit->unitId];}
					}
					else {unit->mainOrderId=unitsdatReturnToAI[unit->unitId];}
					}
					*/
				{
					//핵 자폭
					if(unit->id==UnitId::nuclear_missile&&unit->mainOrderId!=OrderId::Die)
					{
						if(unit->sprite->mainGraphic->animation==IscriptAnimation::SpecialState1)
						{
							if(unit->mainOrderId==Unit::ReturnToIdleOrder[unit->id])
							{
								unit->userActionFlags |= 0x4;
								unit->remove();
							}
						}
					}
					/*
					if(unit->id==UnitId::cocoon&&unit->mainOrderId!=OrderId::Die)
					{
						if(unit->buildQueue[0]!=228)
						{
							if(Unit::BaseProperty[unit->buildQueue[0]] & UnitProperty::Hero)
							{
								unit->unused_0x106;
							}
						}
					}
					*/
				}
				{//특수 라바 버튼 지정
				if(unit->id==UnitId::larva&&unit->mainOrderId!=OrderId::Die)
				{
					if(unit->connectedUnit&&unit->connectedUnit->id==UnitId::UnusedZerg2)
					{
						unit->currentButtonSet=UnitId::Special_RightPitDoor;
					}
				}
			    }
				{
					//파벳 파란 불꽃지정
					if(unit->id==UnitId::firebat&&unit->mainOrderId!=OrderId::Die&&scbw::getUpgradeLevel(unit->playerId,UpgradeId::UnusedUpgrade59))
					{
						
							//CImage* flame=unit->getOverlay(421);
							/*for(CImage* flame=unit->sprite->imageHead;flame;flame=flame->link.next){
								if(flame&&flame->id==ImageId::FlameThrower&&flame->coloringData!=colorShift[ColorRemapping::BExpl].data)flame->setRemapping(ColorRemapping::BExpl);
							}*/
						CImage* flame=unit->getOverlay(ImageId::FlameThrower);
						if(flame){
							if(flame->coloringData!=colorShift[ColorRemapping::BExpl].data)flame->setRemapping(ColorRemapping::BExpl);
						}
						
					}
				}
			}
			
		
		
		
{
			CBullet* bullet;
			for(bullet=*firstBullet;bullet;bullet=bullet->next)
			{
				/*if(bullet->sourceUnit&&bullet->sourceUnit->id==UnitId::missile_turret)
				{
					if(scbw::getUpgradeLevel(bullet->sourceUnit->playerId,UpgradeId::UnusedUpgrade59))
					{
						if(bullet->cyclicMissileIndex==0)
						{
							bullet->cyclicMissileIndex=4;
						}
					}
				}*/
				//러커 촉수 관련 코드
				if(bullet->weaponType==109&&bullet->time_remaining)
				{
					if(bullet->sourceUnit)
					{
						if((bullet->sourceUnit->mainOrderId==OrderId::Stop||bullet->sourceUnit->mainOrderId==OrderId::Unburrow)){
						bullet->time_remaining=0;
						bullet->hitFlags=0x02;
						}
						//if(!(abs(bullet->position.x-bullet->sourceUnit->position.x)<192&&abs(bullet->position.y-bullet->sourceUnit->position.y)<192)&&!(scbw::getUpgradeLevel(bullet->sourceUnit->playerId,UpgradeId::UnusedUpgrade55)))bullet->time_remaining=0;
					}
					else
					{
						bullet->time_remaining=0;
						bullet->hitFlags=0x02;
					}
				}
			}
		}
		
				/*
				if(bullet->weaponType==29&&bullet->time_remaining>0)
					{
						if(bullet->targetUnit!=NULL)
						{
							if(bullet->flingyAcceleration==flingydatAccelerator[weapondatGraphic[bullet->weaponType]])bullet->flingyAcceleration=100;
							if(bullet->flingyTopSpeed==flingydatMaxSpeed[weapondatGraphic[bullet->weaponType]])bullet->flingyTopSpeed=25600;
							if((bullet->sourceUnit->orderTarget&&bullet->sourceUnit->orderTarget==bullet->targetUnit)&&(bullet->sourceUnit->mainOrderId!=Die))
							{
								if(bullet->targetUnit->unitId==battlecruiser||bullet->targetUnit->unitId==carrier||bullet->targetUnit->unitId==56)bullet->sourceUnit->stimTimer=1;
							}
						}
					}
				*/
			
      //Write your code here
    

		// Loop through the bullet table.
    // Warning: There is no guarantee that the current bullet is actually a
    // bullet rather than an unused space in memory
		//for (int i = 0; i < BULLET_ARRAY_LENGTH; ++i) {
		//	BULLET* bullet = &bulletTable[i];
    //  //Write your code here
		//}

    // Alternative looping method
    // Guarantees that [bullet] points to an actual bullet.
    //for (BULLET* bullet = *firstBullet; bullet; bullet = bullet->next) {
    //  //Write your code here
    //}
  }
  return true;
}

bool gameOn() {
  setMaxSightRange<255>();
  firstRun = true;
  return true;
}

bool gameEnd() {
  return true;
}

} //hooks
