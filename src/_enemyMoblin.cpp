#include "_enemyMoblin.h"
#include <limits>
#include <queue>

namespace
{
    struct GridNode
    {
        int x;
        int y;
    };

    struct QueueNode
    {
        float cost;
        GridNode node;
    };

    struct QueueNodeCompare
    {
        bool operator()(const QueueNode& a, const QueueNode& b) const
        {
            return a.cost > b.cost;
        }
    };

    int flattenedIndex(int x, int y, int diameter)
    {
        return (y * diameter) + x;
    }
}

_enemyMoblin::_enemyMoblin()
{
    pos.x = -1.25f;
    pos.y = -0.85f;
    pos.z = -8.0f;

    scale.x = 0.245f;
    scale.y = 0.245f;
    scale.z = 1.0f;

    maxHealth = 3;
    health = 3;
    actionTrigger = STAND;
    moveInterval = 2.0f;
    moveDuration = 1.0f;
}

_enemyMoblin::~_enemyMoblin()
{
    //dtor
}

void _enemyMoblin::initMoblin()
{
    const int enemySheetWidth = 437;
    const int enemySheetHeight = 346;
    const int moblinRegionWidth = 120;
    const int moblinRegionHeight = 60;

    enmsInit(4, 2, "images/enemies_sheet.png");
    setSpriteRegionPixels(
        enemySheetWidth,
        enemySheetHeight,
        0,
        60,
        moblinRegionWidth,
        moblinRegionHeight
    );
}

bool _enemyMoblin::chooseNextDirection(FacingDirection& nextDirection, const vec3* playerPos, const _enemyNavigation* navigation) const
{
    if (playerPos == NULL || navigation == NULL || tileStepDistance <= 0.0f)
    {
        return false;
    }

    const int maxSearchRadius = 14;
    const int diameter = maxSearchRadius * 2 + 1;
    const int totalNodes = diameter * diameter;
    const float tileSize = tileStepDistance;
    const float infinity = std::numeric_limits<float>::max();

    std::vector<float> distance(totalNodes, infinity);
    std::vector<int> previous(totalNodes, -1);
    std::vector<bool> visited(totalNodes, false);
    std::vector<bool> walkable(totalNodes, false);

    const int startGridX = maxSearchRadius;
    const int startGridY = maxSearchRadius;
    const int startIndex = flattenedIndex(startGridX, startGridY, diameter);

    int goalGridX = (int)round((((float)playerPos->x) - (float)pos.x) / tileSize) + maxSearchRadius;
    int goalGridY = (int)round((((float)playerPos->y) - (float)pos.y) / tileSize) + maxSearchRadius;

    goalGridX = std::max(0, std::min(diameter - 1, goalGridX));
    goalGridY = std::max(0, std::min(diameter - 1, goalGridY));

    for (int gridY = 0; gridY < diameter; gridY++)
    {
        for (int gridX = 0; gridX < diameter; gridX++)
        {
            const int nodeIndex = flattenedIndex(gridX, gridY, diameter);
            vec3 candidatePos = pos;
            candidatePos.x += (float)(gridX - startGridX) * tileSize;
            candidatePos.y += (float)(gridY - startGridY) * tileSize;

            if (nodeIndex == startIndex)
            {
                walkable[nodeIndex] = true;
            }
            else
            {
                walkable[nodeIndex] = navigation->isEnemyPositionWalkable(this, candidatePos);
            }
        }
    }

    std::priority_queue<QueueNode, std::vector<QueueNode>, QueueNodeCompare> frontier;
    distance[startIndex] = 0.0f;
    frontier.push(QueueNode{0.0f, GridNode{startGridX, startGridY}});

    const GridNode offsets[4] = {
        GridNode{-1, 0},
        GridNode{1, 0},
        GridNode{0, -1},
        GridNode{0, 1}
    };

    int bestIndex = startIndex;
    float bestPlayerDistance = fabs((float)playerPos->x - (float)pos.x) + fabs((float)playerPos->y - (float)pos.y);

    while (!frontier.empty())
    {
        const QueueNode current = frontier.top();
        frontier.pop();

        const int currentIndex = flattenedIndex(current.node.x, current.node.y, diameter);
        if (visited[currentIndex])
        {
            continue;
        }

        visited[currentIndex] = true;

        vec3 currentWorldPos = pos;
        currentWorldPos.x += (float)(current.node.x - startGridX) * tileSize;
        currentWorldPos.y += (float)(current.node.y - startGridY) * tileSize;
        const float currentPlayerDistance =
            fabs((float)playerPos->x - (float)currentWorldPos.x) +
            fabs((float)playerPos->y - (float)currentWorldPos.y);

        if (currentPlayerDistance < bestPlayerDistance)
        {
            bestPlayerDistance = currentPlayerDistance;
            bestIndex = currentIndex;
        }

        if (current.node.x == goalGridX && current.node.y == goalGridY)
        {
            bestIndex = currentIndex;
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            const int nextGridX = current.node.x + offsets[i].x;
            const int nextGridY = current.node.y + offsets[i].y;

            if (nextGridX < 0 || nextGridY < 0 || nextGridX >= diameter || nextGridY >= diameter)
            {
                continue;
            }

            const int nextIndex = flattenedIndex(nextGridX, nextGridY, diameter);
            if (!walkable[nextIndex])
            {
                continue;
            }

            const float nextCost = distance[currentIndex] + 1.0f;
            if (nextCost >= distance[nextIndex])
            {
                continue;
            }

            distance[nextIndex] = nextCost;
            previous[nextIndex] = currentIndex;
            frontier.push(QueueNode{nextCost, GridNode{nextGridX, nextGridY}});
        }
    }

    if (bestIndex == startIndex)
    {
        return false;
    }

    int stepIndex = bestIndex;
    while (previous[stepIndex] != startIndex && previous[stepIndex] != -1)
    {
        stepIndex = previous[stepIndex];
    }

    if (previous[stepIndex] == -1)
    {
        return false;
    }

    const int stepGridX = stepIndex % diameter;
    const int stepGridY = stepIndex / diameter;
    const int deltaX = stepGridX - startGridX;
    const int deltaY = stepGridY - startGridY;

    if (deltaX < 0)
    {
        nextDirection = FACE_LEFT;
        return true;
    }

    if (deltaX > 0)
    {
        nextDirection = FACE_RIGHT;
        return true;
    }

    if (deltaY < 0)
    {
        nextDirection = FACE_DOWN;
        return true;
    }

    if (deltaY > 0)
    {
        nextDirection = FACE_UP;
        return true;
    }

    return false;
}
