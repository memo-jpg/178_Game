#include "_enemyOck.h"

_enemyOck::_enemyOck()
{
    pos.x = 1.25f;
    pos.y = -0.85f;
    pos.z = -8.0f;

    scale.x = 0.245f;
    scale.y = 0.245f;
    scale.z = 1.0f;

    maxHealth = 2;
    health = 2;
    actionTrigger = STAND;
}

_enemyOck::~_enemyOck()
{
    //dtor
}

void _enemyOck::initOck()
{
    const int enemySheetWidth = 437;
    const int enemySheetHeight = 346;
    const int ockRegionWidth = 120;
    const int ockRegionHeight = 60;

    enmsInit(4, 2, "images/enemies_sheet.png");
    setSpriteRegionPixels(
        enemySheetWidth,
        enemySheetHeight,
        0,
        0,
        ockRegionWidth,
        ockRegionHeight
    );
}
