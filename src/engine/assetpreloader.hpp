#ifndef ASSETPRELOADER_HPP
#define ASSETPRELOADER_HPP

#include <iterator>
#include <vector>

#include "sprite.hpp"
#include "gameobject.hpp"

namespace GBAEngine {

    class SpritePreloader : public Component {
        public:
            SpritePreloader() {}

            template <typename SpriteIter>
            SpritePreloader(SpriteIter start, SpriteIter end) {
                addSprites(start, end);
            }

            template <typename SpriteIter>
            void addSprites(SpriteIter start, SpriteIter end) {
                for (auto iter = start; iter != end; iter++) {
                    toBeAllocated.push_back(*iter);
                }
            }
        
        protected:

            void awake() override;
            void destroy() override;

        private:

            std::vector<const Sprite*> allocations;
            std::vector<const Sprite*> toBeAllocated;
    };

    class AnimationPreloader : public Component {
        public:
            AnimationPreloader() {}

            template <typename AnimIter>
            AnimationPreloader(AnimIter start, AnimIter end) {
                addSprites(start, end);
            }

            template <typename AnimIter>
            void addSprites(AnimIter start, AnimIter end) {
                for (auto iter = start; iter != end; iter++) {
                    toBeAllocated.push_back(*iter);
                }
            }
        
        protected:

            void awake() override;
            void destroy() override;

        private:

            std::vector<const Sprite*> allocations;
            std::vector<const SpriteAnimation*> toBeAllocated;
    };

}

#endif