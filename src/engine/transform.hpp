#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "gameobject.hpp"

namespace GBAEngine {

    class Transform : public Component {

        public:

            Transform();
            Transform(Vector2);
            Transform(fixed32 x, fixed32 y);

            Vector2 position;

    };

}

#endif