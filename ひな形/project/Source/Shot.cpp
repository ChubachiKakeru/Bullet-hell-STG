#include "Shot.h"

Shot::Shot(float startX, float startY)
    : x(startX), y(startY), isActive(true)
{
    std::cout << "Shot created at (" << x << ", " << y << ")\n";
}

void Shot::Update() {
    if (isActive) {
        y -= 5; // �e��������Ɉړ�
        if (y < 0) isActive = false; // ��ʊO�Ŗ�����
    }
}

void Shot::Draw() {
    if (isActive) {
        std::cout << "Drawing shot at (" << x << ", " << y << ")\n";
    }
}
