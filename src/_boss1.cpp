#include "_boss1.h"

_boss1::_boss1()
{
    pos.x = 1.25f;
    pos.y = -0.85f;
    pos.z = -8.0f;

    scale.x = 0.3675f;
    scale.y = 0.3675f;
    scale.z = 1.0f;

    maxHealth = 10;
    health = 10;
    actionTrigger = STAND;
}

_boss1::~_boss1()
{
}

void _boss1::initBoss1()
{
    const int enemySheetWidth = 437;
    const int enemySheetHeight = 346;
    const int ockRegionWidth = 120;
    const int ockRegionHeight = 60;

    initBossSprite(
        4,
        2,
        "images/enemies_sheet.png",
        enemySheetWidth,
        enemySheetHeight,
        0,
        0,
        ockRegionWidth,
        ockRegionHeight
    );
}
