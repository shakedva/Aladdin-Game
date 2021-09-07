//----------------------------------include section-----------------------------------
#include "CollisionHandling.h"
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "Direction.h"
#include "macros.h"
#include "Aladdin.h"
#include "Apple.h"
#include "Ruby.h"
#include "Genie.h"
#include "LivesGift.h"
#include "Floor.h"
#include "Wall.h"
#include "Rope.h"
#include "Weapon.h"
#include "Enemy.h"
#include "NearEnemy.h"
#include "DistantEnemy.h"
#include "JugglingEnemy.h"
#include "AppleWeapon.h"
#include "KnifeWeapon.h"
#include "EndLevel.h"

//----------------------------------const section-----------------------------------
const int ALA_OFFSET = 10;

//----------------------------------namespace section-----------------------------------
namespace // anonymous namespace — the standard way to make function "static"
{
    // primary collision-processing functions
    void aladdinApple(GameObject& aladdin, GameObject& apple)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        Apple& app = dynamic_cast<Apple&>(apple);

        Media::instance().playSound(Media::s_collectApp);
        ala.updateInfo(SCORE, INC);
        ala.updateInfo(WEAPONS, INC);
        app.setDisposed();
    }
    void appleAladdin(GameObject& apple, GameObject& aladdin) 
    {
        aladdinApple(aladdin, apple);
    }

    void aladdinRuby(GameObject& aladdin, GameObject& ruby)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        Ruby& rub = dynamic_cast<Ruby&>(ruby);

        Media::instance().playSound(Media::s_collectRuby);
        ala.updateInfo(SCORE, INC_RUBY);
        rub.setDisposed();
    }
    void rubyAladdin(GameObject& ruby, GameObject& aladdin)
    {
        aladdinRuby(aladdin, ruby);
    }

    void aladdinLivesGift(GameObject& aladdin, GameObject& livesGift)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        LivesGift& lg = dynamic_cast<LivesGift&>(livesGift);

        Media::instance().playSound(Media::s_extraHealth);
        ala.updateInfo(LIVES, INC);
        lg.setDisposed();
    }
    void livesGiftAladdin(GameObject& livesGift, GameObject& aladdin)
    {
        aladdinLivesGift(aladdin, livesGift);
    }

    void aladdinWall(GameObject& aladdin, GameObject& wall)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        ala.hitAwall();
        ala.setIdleAction();
    }
    void wallAladdin(GameObject& wall, GameObject& aladdin)
    {
        aladdinWall(aladdin, wall);
    }

    void aladdinFloor(GameObject& aladdin, GameObject& floor)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);

        if (ala.getPosition().y < (floor.getPosition().y - (floor.getGlobalBounds().height / 2) + 1))
            ala.setOnGround(true);
    }
    void floorAladdin(GameObject& floor, GameObject& aladdin)
    {
        aladdinFloor(aladdin, floor);
    }

    void aladdinRope(GameObject& aladdin, GameObject& rope)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        Rope& ro = dynamic_cast<Rope&>(rope);

        if (ro.getGlobalBounds().contains(ala.getPosition()))
        {
            ala.setOnRope(true);

            if (ro.getGlobalBounds().top - ALA_OFFSET < ala.getGlobalBounds().top)
                ala.centerOnRope(ro.getPosition());
            else
                ala.reachedEndOfRope();
        }
    }
    void ropeAladdin(GameObject& aladdin, GameObject& rope)
    {
        aladdinRope(rope, aladdin);
    }

    void aladdinKnifeWeapon(GameObject& aladdin, GameObject& weapon)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        KnifeWeapon& we = dynamic_cast<KnifeWeapon&>(weapon);
        if (ala.getAction() != Aladdin::PlayerAction::Attack)
        {
            ala.updateInfo(LIVES, DEC);
            Media::instance().playSound(Media::s_aliRestart);
            ala.setAladdinToDefaultPosition();
        }
        we.setDisposed();
    }
    void knifeWeaponAladdin(GameObject& weapon, GameObject& aladdin)
    {
        aladdinKnifeWeapon(aladdin, weapon);
    }

    void aladdinEnemy(GameObject& aladdin, GameObject& enemy)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        Enemy& en = dynamic_cast<Enemy&>(enemy);

        if (ala.getAction() == Aladdin::PlayerAction::Attack)
        {
            Media::instance().playSound(Media::s_enemyDis);
            en.setDisposed();
        }
        else
        {
            Media::instance().playSound(Media::s_aliRestart);
            ala.setAladdinToDefaultPosition();
            ala.updateInfo(LIVES, DEC);
        }

    }
    void enemyAladdin(GameObject& enemy, GameObject& aladdin)
    {
        aladdinEnemy(aladdin, enemy);
    }

    void aladdinGenie(GameObject& aladdin, GameObject& genie)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        Genie& ge = dynamic_cast<Genie&>(genie);

        Media::instance().playSound(Media::s_collectGenie);
        ge.setDisposed();
        ala.setDefaultPosition(ala.getAladdinPos());
    }
    void genieAladdin(GameObject& genie, GameObject& aladdin)
    {
        aladdinGenie(aladdin, genie);
    }

    void aladdinEndLevel(GameObject& aladdin, GameObject& endLevel)
    {
        Aladdin& ala = dynamic_cast<Aladdin&>(aladdin);
        EndLevel& endl = dynamic_cast<EndLevel&>(endLevel);
        Media::instance().playSound(Media::s_levelComplete);
        //endl.setAnimation(true);
        //std::cout << "col ala end level\n ";
        endl.setDisposed();
    }
    void endLevelAladdin(GameObject& aladdin, GameObject& endLevel)
    {
        aladdinEndLevel(endLevel, aladdin);
    }

    void enemyAppleWeapon(GameObject& enemy, GameObject& weapon)
    {
        Enemy& en = dynamic_cast<Enemy&>(enemy);
        AppleWeapon& we = dynamic_cast<AppleWeapon&>(weapon);
        Media::instance().playSound(Media::s_enemyDis);
        en.setDisposed();
        we.setDirection(Direction::Stay);
    }
    void appleWeaponEnemy(GameObject& weapon, GameObject& enemy)
    {
        enemyAppleWeapon(enemy, weapon);
    }

    void weaponWall(GameObject& weapon, GameObject& wall)
    {
        Weapon& we = dynamic_cast<Weapon&>(weapon);
        we.setDirection(Direction::Stay);
        //we.setDisposed();
    }
    void wallWeapon(GameObject& wall, GameObject& weapon)
    {
        weaponWall(weapon, wall);
    }

    void weaponFloor(GameObject& weapon, GameObject& floor)
    {
        weaponWall(weapon, floor);
    }
    void floorWeapon(GameObject& floor, GameObject& weapon)
    {
        weaponWall(weapon, floor);
    }

    void weaponWeapon(GameObject& weapon1, GameObject& weapon2)
    {
        Weapon& we1 = dynamic_cast<Weapon&>(weapon1);
        Weapon& we2 = dynamic_cast<Weapon&>(weapon2);
        we1.setDisposed();
        we2.setDisposed();
    }

    using HitFunctionPtr = void (*)(GameObject&, GameObject&);
    using Key = std::pair<std::type_index, std::type_index>;
    using HitMap = std::map<Key, HitFunctionPtr>;

    //all possible collisions
    HitMap initializeCollisionMap()
    {
        HitMap phm;
        phm[Key(typeid(Aladdin), typeid(Apple))] = &aladdinApple;
        phm[Key(typeid(Apple), typeid(Aladdin))] = &appleAladdin;

        phm[Key(typeid(Aladdin), typeid(Ruby))] = &aladdinRuby;
        phm[Key(typeid(Ruby), typeid(Aladdin))] = &rubyAladdin;

        phm[Key(typeid(Aladdin), typeid(LivesGift))] = &aladdinLivesGift;
        phm[Key(typeid(LivesGift), typeid(Aladdin))] = &livesGiftAladdin;

        phm[Key(typeid(Aladdin), typeid(Wall))] = &aladdinWall;
        phm[Key(typeid(Wall), typeid(Aladdin))] = &wallAladdin;

        phm[Key(typeid(Aladdin), typeid(Floor))] = &aladdinFloor;
        phm[Key(typeid(Floor), typeid(Aladdin))] = &floorAladdin;

        phm[Key(typeid(Aladdin), typeid(Rope))] = &aladdinRope;
        phm[Key(typeid(Rope), typeid(Aladdin))] = &ropeAladdin;

        phm[Key(typeid(Aladdin), typeid(KnifeWeapon))] = &aladdinKnifeWeapon;
        phm[Key(typeid(KnifeWeapon), typeid(Aladdin))] = &knifeWeaponAladdin;

        phm[Key(typeid(Aladdin), typeid(Genie))] = &aladdinGenie;
        phm[Key(typeid(Genie), typeid(Aladdin))] = &genieAladdin;

        phm[Key(typeid(Aladdin), typeid(Enemy))] = &aladdinEnemy;
        phm[Key(typeid(Enemy), typeid(Aladdin))] = &enemyAladdin;

        phm[Key(typeid(Aladdin), typeid(EndLevel))] = &aladdinEndLevel;
        phm[Key(typeid(EndLevel), typeid(Aladdin))] = &endLevelAladdin;

        phm[Key(typeid(Enemy), typeid(AppleWeapon))] = &enemyAppleWeapon;
        phm[Key(typeid(AppleWeapon), typeid(Enemy))] = &appleWeaponEnemy;

        phm[Key(typeid(Aladdin), typeid(NearEnemy))] = &aladdinEnemy;
        phm[Key(typeid(NearEnemy), typeid(Aladdin))] = &enemyAladdin;
        phm[Key(typeid(NearEnemy), typeid(AppleWeapon))] = &enemyAppleWeapon;
        phm[Key(typeid(AppleWeapon), typeid(NearEnemy))] = &appleWeaponEnemy;

        phm[Key(typeid(Aladdin), typeid(DistantEnemy))] = &aladdinEnemy;
        phm[Key(typeid(DistantEnemy), typeid(Aladdin))] = &enemyAladdin;
        phm[Key(typeid(DistantEnemy), typeid(AppleWeapon))] = &enemyAppleWeapon;
        phm[Key(typeid(AppleWeapon), typeid(DistantEnemy))] = &appleWeaponEnemy;

        phm[Key(typeid(Aladdin), typeid(JugglingEnemy))] = &aladdinEnemy;
        phm[Key(typeid(JugglingEnemy), typeid(Aladdin))] = &enemyAladdin;
        phm[Key(typeid(JugglingEnemy), typeid(AppleWeapon))] = &enemyAppleWeapon;
        phm[Key(typeid(AppleWeapon), typeid(JugglingEnemy))] = &appleWeaponEnemy;

        phm[Key(typeid(Weapon), typeid(Wall))] = &weaponWall;
        phm[Key(typeid(Wall), typeid(Weapon))] = &wallWeapon;

        phm[Key(typeid(AppleWeapon), typeid(Wall))] = &weaponWall;
        phm[Key(typeid(Wall), typeid(AppleWeapon))] = &wallWeapon;
        phm[Key(typeid(KnifeWeapon), typeid(Wall))] = &weaponWall;
        phm[Key(typeid(Wall), typeid(KnifeWeapon))] = &wallWeapon;
        phm[Key(typeid(AppleWeapon), typeid(Floor))] = &weaponFloor;
        phm[Key(typeid(Floor), typeid(AppleWeapon))] = &floorWeapon;
        phm[Key(typeid(KnifeWeapon), typeid(Floor))] = &weaponFloor;
        phm[Key(typeid(Floor), typeid(KnifeWeapon))] = &floorWeapon;

        phm[Key(typeid(Weapon), typeid(Weapon))] = &weaponWeapon;
        phm[Key(typeid(AppleWeapon), typeid(KnifeWeapon))] = &weaponWeapon;
        phm[Key(typeid(KnifeWeapon), typeid(AppleWeapon))] = &weaponWeapon;

        return phm;
    }

    //find the given collision in collision map
    HitFunctionPtr lookup(const std::type_index& class1, const std::type_index& class2)
    {
        static HitMap collisionMap = initializeCollisionMap();
        auto mapEntry = collisionMap.find(std::make_pair(class1, class2));
        if (mapEntry == collisionMap.end())
        {
            return nullptr;
        }
        return mapEntry->second;
    }
}// end namespace

//----------------------------------function section-----------------------------------

//
void processCollision(GameObject& object1, GameObject& object2)
{
    auto phf = lookup(typeid(object1), typeid(object2));
    if (!phf)
        return;
    phf(object1, object2);
}