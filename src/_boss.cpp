#include "_boss.h"

_boss::_boss()
{
}

_boss::~_boss()
{
}

void _boss::initBossSprite(int xFrameCount,
                           int yFrameCount,
                           char* fileName,
                           int atlasWidth,
                           int atlasHeight,
                           int left,
                           int top,
                           int width,
                           int height)
{
    enmsInit(xFrameCount, yFrameCount, fileName);
    setSpriteRegionPixels(atlasWidth, atlasHeight, left, top, width, height);
}
