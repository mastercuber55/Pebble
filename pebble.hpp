// #include <memory>
// #include <raylib.h>
// #include <vector>

// namespace Phy {

//     struct World;

//     struct Box {

//         Vector2 vel;
//         Vector2 force;
//         float mass;

//         Box(Rectangle hitbox, std::shared_ptr<World> w);

//         private:
//             std::weak_ptr<World> world;
//     };

//     struct World : public std::enable_shared_from_this<World> {

//         void AddObj(std::shared_ptr<Box> obj);
//         void RemObj(std::shared_ptr<Box> obj);
//         std::shared_ptr<World> getPtr();
//         void step(float dt);

//         private:
//             std::vector<std::unique_ptr<Box>> objs;

//     };
// };

// #ifdef PHY_IMPL
// #endif

#include "Frax.hpp"
#include <any>
#include <chipmunk/chipmunk.h>

namespace Pebble {
struct Box {
  cpBody *Body;
  cpShape *Shape;
  cpSpace *Space;

  Box(cpVect pos, cpVect size, cpFloat mass, cpSpace *space);
  ~Box();

  Box(const Box &) = delete;
  Box &operator=(const Box &) = delete;
  
  operator cpBody *() const;
  operator cpShape *() const;
};

void FraxDraw(Frax::Rect &);
} // namespace Pebble

// #define PEBBLE_IMPL
#ifdef PEBBLE_IMPL
#undef PEBBLE_IMPL

namespace Pebble {

Box::Box(cpVect pos, cpVect size, cpFloat mass, cpSpace *space) {

  this->Space = space;

  cpFloat Moment = cpMomentForBox(mass, size.x, size.y);
  this->Body = cpSpaceAddBody(Space, cpBodyNew(mass, Moment));
  cpBodySetPosition(this->Body, pos);

  this->Shape =
      cpSpaceAddShape(Space, cpBoxShapeNew(this->Body, size.x, size.y, 0.0));
  cpShapeSetUserData(this->Shape, this);
}

Box::~Box() {
  if (cpSpaceContainsShape(Space, Shape)) {
    cpSpaceRemoveShape(Space, Shape);
  }
  if (cpSpaceContainsBody(Space, Body)) {
    cpSpaceRemoveBody(Space, Body);
  }
  if (Shape) {
    cpShapeFree(Shape);
  }
  if (Body) {
    cpBodyFree(Body);
  }
}

Box::operator cpBody *() const { return this->Body; }

Box::operator cpShape *() const { return this->Shape; }

void FraxDraw(Frax::Rect &Rect) {
  auto box = std::any_cast<Box *>(Rect.Data);
  cpVect pos = cpBodyGetPosition(box->Body);

  Rect.SetCenter({pos.x, pos.y});
  Rect.Rotation = cpBodyGetAngle(box->Body) * RAD2DEG;

  Rect.Draw();
}

} // namespace Pebble

#endif