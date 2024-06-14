#include "assetpreloader.hpp"

namespace GBAEngine {

    void SpritePreloader::awake() {
        for (const Sprite* sp : toBeAllocated) {
            auto allocated = SpriteAllocator::checkoutSprite(sp);
            if (allocated) {
                allocations.push_back(sp);
            }
        }
    }

    void SpritePreloader::destroy() {
        for (const Sprite* sp : allocations) {
            SpriteAllocator::returnSprite(sp);
        }
    }

    void AnimationPreloader::awake() {
        for (const SpriteAnimation* anim : toBeAllocated) {
            for (int i = 0; i < anim->frameCount; i++) {
                const Sprite* frame = anim->frames + i;
                auto allocated = SpriteAllocator::checkoutSprite(frame);
                if (allocated) {
                    allocations.push_back(frame);
                }
            }
        }
    }

    void AnimationPreloader::destroy() {
        for (const Sprite* sp : allocations) {
            SpriteAllocator::returnSprite(sp);
        }
    }

}