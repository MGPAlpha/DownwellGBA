#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <functional>
#include "gbamath.hpp"

namespace GBAEngine {
    class EasingFunction {
        public:
            EasingFunction(std::function<fixed32(fixed32)> f);

            // Call the easing function, remaps t to an output 0->0, 1->1, (0,1)->(0,1), (-inf,0)U(1,inf) -> undefined
            fixed32 operator()(fixed32 t) const;
            static const EasingFunction EASE_OUT;
            static const EasingFunction EASE_IN;
            static const EasingFunction LINEAR;
        private:
            std::function<fixed32(fixed32)> func;
    };
}

#endif