#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "gameobject.hpp"
#include "transform.hpp"
#include "events.hpp"
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
            uint32_t layer = Layer::L_0;
            uint32_t mask = Layer::L_0;
            int priority = 0;

            struct Comparator {
                bool operator()(const Collider* lhs, const Collider* rhs) const {
                    // This is important. we need to ensure that it isnt just comparing priorities, otherwise all of same priority are equal
                    if (lhs->priority == rhs->priority) {
                        return lhs < rhs;
                    }
                    return lhs->priority < rhs->priority;
                }
            };

            bool isTriggerOnly = false;

            GameEvent<Collider*> onEnter;
            GameEvent<Collider*> onExit;
            GameEvent<Collider*> onStay;

            bool needsIntersectionCheck();
        protected:
            Transform* transform;

            std::set<Collider*> trackedIntersections;

            void awake() override;
            void destroy() override;
        private:
            std::set<Collider*> lastFrameIntersections;
            std::set<Collider*> currentFrameIntersections;
            void registerIntersection(Collider*);
            void registerNonintersection(Collider*);
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

    template <typename _Ret, typename... _Args>
    class ColliderFunctionTable {
        private:
            typedef std::pair<const std::type_info*, const std::type_info*> TypePair;
            typedef std::function<_Ret(Collider*, Collider*, _Args...)> CollisionFunc;
            std::map<TypePair, CollisionFunc> handlers;
            std::function<void(_Args&...)> argReverser;
            bool hasReverser = false;
        public:
            ColliderFunctionTable() {}
            ColliderFunctionTable(std::function<void(_Args&...)> reverser) {
                this->argReverser = reverser;
                hasReverser = true;
            }

            template <class T1, class T2>
            void addHandler(std::function<_Ret(T1*, T2*, _Args...)> handler) {
                handlers[std::make_pair(&typeid(T1), &typeid(T2))] = [handler](Collider* col1, Collider* col2, _Args... args) -> _Ret {
                    return handler((T1*)col1, (T2*)col2, args...);   
                };
                handlers[std::make_pair(&typeid(T2), &typeid(T1))] = [this, handler](Collider* col1, Collider* col2, _Args... args) -> _Ret {
                    if (hasReverser) this->argReverser(args...);
                    return handler((T1*)col2, (T2*)col1, args...);
                };
            }
            _Ret check(Collider* a, Collider* b, _Args... args) {
                TypePair key = std::make_pair(&typeid(*a), &typeid(*b));
                if (handlers.count(key) > 0) {
                    CollisionFunc handler = handlers[key];
                    if (handler) {
                        return handler(a, b, args...);
                    }
                }
                return false;
            }

    };

    class Physics {
        friend class Collider;
       
        public:
            static ColliderFunctionTable<bool> intersectionHandlers;
            static ColliderFunctionTable<Collision, Vector2> collisionHandlers;

            static void updateIntersections();
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
                // mgba_printf("trying to add handler");
                Physics::intersectionHandlers.addHandler<RectCollider, RectCollider>(collideRect);
                mgba_printf("handler added");
            END_STATIC_CTOR
    };

}

#endif