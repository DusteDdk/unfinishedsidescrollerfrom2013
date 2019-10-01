#ifndef PLAYERSHIP_HPP_INCLUDED
#define PLAYERSHIP_HPP_INCLUDED

#include "../ents.hpp"
#include "../playerprofile.hpp"
#include "weapon.hpp"
enum slots { slotCargo1, slotCargo2, slotCargo3, slotCargo4, slotCargo5, slotEngine, slotTop, slotFront, slotBottom };

class playerShip : public gameObject
{
  private:
    spriteClass *engineSprite;
    weaponClass *weapTop, *weapFront, *weapBot;
    playerInfo *pi;

  public:
    playerShip();
    void setup(spriteManagerClass* sprMan, playerInfo* info); //This puts on the correct weapons/engine/hull
    void draw();

    //Player control
    inputClass* inp;
    void sim();

    //Specs
    float shield; //0=no shield, 1=shield takes as much power as it's damaged, 2= half damage, etc
    int battery; //General purpose power supply, for now only used for shield
    int healthMax;
    int batteryMax;
    GLfloat healthPercentage, batteryPercentage;
    GLfloat shownHealthPercentage, shownBatteryPercentage;

    void setHealthPoints(int points);
    void setBatteryPoints(int points);

    void damage( int points );

};


#endif // PLAYERSHIP_HPP_INCLUDED
