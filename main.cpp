#include <Novice.h>
#include "Enemy.h"
#include <vector>
#include <string>

const char kWindowTitle[] = "LC1A16シミズグチ＿ハル";

struct Player {
    float x, y;
};

struct Bullet {
    float x, y;
    bool isActive;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Novice::Initialize(kWindowTitle, 1280, 720);

    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    Player player = { 640.0f, 600.0f };
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies = {
        Enemy(100.0f, 300.0f, 7.0f),
        Enemy(300.0f, 500.0f, -7.0f)
    };

    bool allEnemiesDefeated = false;

    while (Novice::ProcessMessage() == 0) {
        Novice::BeginFrame();
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        if (keys[DIK_W]) player.y -= 5.0f;
        if (keys[DIK_S]) player.y += 5.0f;
        if (keys[DIK_A]) player.x -= 5.0f;
        if (keys[DIK_D]) player.x += 5.0f;

        if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
            bullets.push_back({ player.x, player.y - 16, true });
        }

        for (auto& bullet : bullets) {
            if (bullet.isActive) {
                bullet.y -= 10.0f;
                if (bullet.y < 0) {
                    bullet.isActive = false;
                }
            }
        }

        if (!allEnemiesDefeated) {
            for (auto& enemy : enemies) {
                enemy.Update();
            }

            bool enemyHit = false;

            for (auto& bullet : bullets) {
                if (bullet.isActive) {
                    for (auto& enemy : enemies) {
                        if (enemy.CheckCollision(bullet.x, bullet.y)) {
                            enemyHit = true;
                            bullet.isActive = false;
                        }
                    }
                }
            }

            // どちらかの敵が当たったら両方の敵を消す
            if (enemyHit) {
                for (auto& enemy : enemies) {
                    enemy.isAlive = false;
                }
            }

            allEnemiesDefeated = true;
            for (auto& enemy : enemies) {
                if (enemy.isAlive) {
                    allEnemiesDefeated = false;
                    break;
                }
            }
        }

        if (preKeys[DIK_R] == 0 && keys[DIK_R] != 0) {
            for (auto& enemy : enemies) {
                enemy.isAlive = true;
            }
            allEnemiesDefeated = false;
        }

        Novice::DrawEllipse(static_cast<int>(player.x), static_cast<int>(player.y), 32, 32, 0.0f, 0x00FF00FF, kFillModeSolid);

        for (auto& bullet : bullets) {
            if (bullet.isActive) {
                Novice::DrawEllipse(static_cast<int>(bullet.x), static_cast<int>(bullet.y), 16, 16, 0.0f, 0xFFFFFFFF, kFillModeSolid);
            }
        }

        for (auto& enemy : enemies) {
            enemy.Draw();
        }

        std::string enemyStatusA = "enemyA isAlive = " + std::to_string(enemies[0].isAlive);
        std::string enemyStatusB = "enemyB isAlive = " + std::to_string(enemies[1].isAlive);
        Novice::ScreenPrintf(10, 10, enemyStatusA.c_str(), 0xFFFFFFFF);
        Novice::ScreenPrintf(10, 30, enemyStatusB.c_str(), 0xFFFFFFFF);

        Novice::ScreenPrintf(10, 50, "WASD : player Move", 0xFFFFFFFF);
        Novice::ScreenPrintf(10, 70, "shot : SPACE", 0xFFFFFFFF);
        Novice::ScreenPrintf(10, 90, "enemy respawn : R", 0xFFFFFFFF);

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    Novice::Finalize();
    return 0;
}