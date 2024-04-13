#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "gameobject.hpp"
#include "transform.hpp"
#include "../collision.hpp"

#include <set>
#include <typeinfo>
#include <map>

#define BEGIN_STATIC_CTOR struct constructor{\
        constructor(){
#define END_STATIC_CTOR }};\
        static constructor cons;

namespace GBAEngine {

    enum Layer {
        L_0 = 1<<0,
        L_1 = 1<<1,
        L_2 = 1<<2,
        L_3 = 1<<3,
        L_4 = 1<<4,
        L_5 = 1<<5,
        L_6 = 1<<6,
        L_7 = 1<<7,
        L_8 = 1<<8,
        L_9 = 1<<9,
        L_10 = 1<<10,
        L_11 = 1<<11,
        L_12 = 1<<12,
        L_13 = 1<<13,
        L_14 = 1<<14,
        L_15 = 1<<15,
        L_16 = 1<<16,
        L_17 = 1<<17,
        L_18 = 1<<18,
        L_19 = 1<<19,
        L_20 = 1<<20,
        L_21 = 1<<21,
        L_22 = 1<<22,
        L_23 = 1<<23,
        L_24 = 1<<24,
        L_25 = 1<<25,
        L_26 = 1<<26,
        L_27 = 1<<27,
        L_28 = 1<<28,
        L_29 = 1<<29,
        L_30 = 1<<30,
        L_31 = 1<<31
    };

    class Collider : public Component {
        friend class Physics;
        public:
            Layer layer = Layer::L_0;
            Layer mask = Layer::L_0;
            int priority = 0;

            struct Comparator {
                bool operator()(const Collider* lhs, const Collider* rhs) const {
                    return lhs->priority < rhs->priority;
                }
            };
        protected:
            Transform* transform;

            void awake() override;
            void destroy() override;
    };

    template<class ColliderTypeA, class ColliderTypeB>
    class CollisionChecker {
        virtual bool intersect(ColliderTypeA, ColliderTypeB) = 0;
        bool intersectReverse(ColliderTypeB b, ColliderTypeA a) {
            return intersect(a,b);
        }
        virtual Collision collide(ColliderTypeA, ColliderTypeB) = 0;
        Collision collideReverse(ColliderTypeB b, ColliderTypeA a) {
            Collision c = collide(a, b);
            c.push = -c.push;
            return c;
        }
    };

    class Physics {
        friend class Collider;
        
        static std::map<std::pair<const std::type_info*, const std::type_info*>, std::function<bool(Collider*, Collider*)>> intersectionHandlers;
        
        public:
            template <class T1, class T2>
            static void addIntersectionHandler(std::function<bool(T1*, T2*)> handler) {
                intersectionHandlers[std::make_pair(&typeid(T1), &typeid(T2))] = [handler](Collider* col1, Collider* col2){
                    return handler((T1*)col1, (T2*)col2);   
                };
                intersectionHandlers[std::make_pair(&typeid(T2), &typeid(T1))] = [handler](Collider* col1, Collider* col2){
                    return handler((T1*)col2, (T2*)col1); 
                };
            }
            static bool checkIntersection(Collider* a, Collider* b);
        private:



            static std::set<Collider*, Collider::Comparator> activeColliders;
    };

    class RectCollider : public Collider {
        public:

            enum PivotMode {
                CENTER,
                TOP_LEFT,
                TOP_RIGHT,
                BOTTOM_LEFT,
                BOTTOM_RIGHT,
            };

            RectCollider(Vector2 size, Vector2 offset, PivotMode pivot);
            RectCollider(Vector2 size, Vector2 offset);
            RectCollider(Vector2 size);
            RectCollider(Vector2 size, PivotMode pivot);
            RectCollider(PivotMode pivot);
            RectCollider();

            static bool collideRect(RectCollider* a, RectCollider* b);

            Vector2 size;
            Vector2 offset;

            PivotMode pivot;

            Rect getRect() const;

        private:
            BEGIN_STATIC_CTOR
                mgba_open();
                Physics::addIntersectionHandler<RectCollider, RectCollider>(collideRect);
                mgba_printf("handler added");
            END_STATIC_CTOR
    };

}

#endif