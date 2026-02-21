#pragma once

#include "Vector2.h"
#include "Math.h"
#include "Entity.h"
#include <vector>
#include <memory>

namespace Paper {

/**
 * Rigidbody - Physics body component
 */
class Rigidbody : public Component {
public:
    enum class BodyType {
        Static,     // Doesn't move (walls, floor)
        Kinematic,  // Moves but not affected by forces (moving platforms)
        Dynamic     // Fully simulated (player, objects)
    };

    Rigidbody(BodyType type = BodyType::Dynamic);

    void update(float deltaTime) override;

    // Physics properties
    Vector2 velocity{0, 0};
    Vector2 acceleration{0, 0};
    float mass = 1.0f;
    float drag = 0.0f; // Air resistance
    float gravityScale = 1.0f;
    float restitution = 0.5f; // Bounciness (0 = no bounce, 1 = perfect bounce)
    
    BodyType bodyType = BodyType::Dynamic;
    bool useGravity = true;

    // Force application
    void addForce(const Vector2& force);
    void addImpulse(const Vector2& impulse);
    void setVelocity(const Vector2& vel) { velocity = vel; }

    // Constraints
    bool freezeRotation = false;
    bool freezeX = false;
    bool freezeY = false;

private:
    Vector2 forceAccumulator{0, 0};
};

/**
 * Collider - Base collision shape component
 */
class Collider : public Component {
public:
    virtual ~Collider() = default;

    bool isTrigger = false; // Triggers don't physically collide, just detect overlaps
    Vector2 offset{0, 0};
    
    virtual bool intersects(const Collider* other) const = 0;
    virtual Rectangle getBounds() const = 0;

protected:
    Vector2 getWorldPosition() const;
};

/**
 * BoxCollider - Rectangular collision shape
 */
class BoxCollider : public Collider {
public:
    BoxCollider(float width = 1.0f, float height = 1.0f);

    Vector2 size{1, 1};

    bool intersects(const Collider* other) const override;
    Rectangle getBounds() const override;
};

/**
 * CircleCollider - Circular collision shape
 */
class CircleCollider : public Collider {
public:
    CircleCollider(float radius = 0.5f);

    float radius = 0.5f;

    bool intersects(const Collider* other) const override;
    Rectangle getBounds() const override;
};

/**
 * Collision - Information about a collision
 */
struct Collision {
    Entity* other;
    Vector2 point;
    Vector2 normal;
    float penetration;
};

/**
 * PhysicsWorld - Manages physics simulation
 */
class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void update(float deltaTime);

    // Global physics settings
    Vector2 gravity{0, 980.0f}; // Pixels per second squared (default = downward)
    int velocityIterations = 8;
    int positionIterations = 3;

    // Collision detection
    void checkCollisions();
    
    // Raycasting
    struct RaycastHit {
        Entity* entity;
        Vector2 point;
        Vector2 normal;
        float distance;
    };
    
    bool raycast(const Vector2& origin, const Vector2& direction, float maxDistance, RaycastHit& hit);
    std::vector<RaycastHit> raycastAll(const Vector2& origin, const Vector2& direction, float maxDistance);

    // Query the world
    std::vector<Entity*> overlapCircle(const Vector2& center, float radius);
    std::vector<Entity*> overlapRectangle(const Rectangle& rect);

private:
    void integrateVelocity(float deltaTime);
    void solveCollisions();
    void integratePosition(float deltaTime);

    bool checkCollision(Collider* a, Collider* b, Collision& collision);
    void resolveCollision(Rigidbody* a, Rigidbody* b, const Collision& collision);
};

} // namespace Paper
