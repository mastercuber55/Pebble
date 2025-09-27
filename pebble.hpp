// #define CP_USE_DOUBLES 0
#include <chipmunk/chipmunk.h>

namespace Pebble {
struct Obj {
  cpBody *Body;
  cpShape *Shape;
  cpSpace *Space;

  Obj(cpSpace *space, cpVect pos, cpVect size, cpFloat mass);
  Obj(cpSpace *space, cpVect pos, cpFloat radius, cpFloat mass);
  ~Obj();

  Obj(const Obj &) = delete;
  Obj &operator=(const Obj &) = delete;

  operator cpBody *() const;
  operator cpShape *() const;

  void setAngle(cpFloat);
  cpFloat getAngle();

  cpVect getPosition();

  void setVelocity(cpVect);
  void setAngularVelocity(cpFloat);
  void applyForce(cpVect);
  void applyImpulse(cpVect);
  void setCollisionType(cpCollisionType);
};

#ifdef FRAX_FRAMEWORK
void Draw(Frax::Rect *);
#endif

} // namespace Pebble

// Inline function definations
namespace Pebble {

inline cpVect Obj::getPosition() {
  return cpBodyGetPosition(Body);
}

inline void Obj::applyForce(cpVect force) {
  return cpBodyApplyForceAtLocalPoint(Body, force, cpvzero);
}

inline void Obj::applyImpulse(cpVect impulse) {
  return cpBodyApplyImpulseAtLocalPoint(Body, impulse, cpvzero);
}

inline void Obj::setAngle(cpFloat radians) {
  return cpBodySetAngle(Body, radians);
}

inline cpFloat Obj::getAngle() { return cpBodyGetAngle(Body); }

inline void Obj::setVelocity(cpVect vel) {
  return cpBodySetVelocity(Body, vel);
}

inline void Obj::setAngularVelocity(cpFloat vel) {
  return cpBodySetAngularVelocity(Body, vel);
}

inline void Obj::setCollisionType(cpCollisionType type) {
  return cpShapeSetCollisionType(Shape, type);
}
} // namespace Pebble

// #define PEBBLE_IMPL
#ifdef PEBBLE_IMPL
#undef PEBBLE_IMPL

namespace Pebble {

#ifdef FRAX_FRAMEWORK
void Draw(Frax::Rect *Rect) {
  auto box = std::any_cast<Obj *>(Rect->Data);
  cpVect pos = cpBodyGetPosition(box->Body);

  Rect->SetCenter({static_cast<float>(pos.x), static_cast<float>(pos.y)});
  Rect->Rotation = cpBodyGetAngle(box->Body) * RAD2DEG;

  Rect->Draw();
}
#endif
Obj::Obj(cpSpace *space, cpVect pos, cpVect size, cpFloat mass) {

  this->Space = space;

  cpFloat Moment = cpMomentForBox(mass, size.x, size.y);
  this->Body = cpSpaceAddBody(Space, cpBodyNew(mass, Moment));
  cpBodySetPosition(this->Body, pos);

  this->Shape =
      cpSpaceAddShape(Space, cpBoxShapeNew(this->Body, size.x, size.y, 0.0));
  cpShapeSetUserData(this->Shape, this);

  cpShapeSetFriction(this->Shape, 0.7f);
  cpShapeSetElasticity(this->Shape, 0.3f);
}

Obj::Obj(cpSpace *space, cpVect pos, cpFloat radius, cpFloat mass) {

  this->Space = space;

  cpFloat Moment = cpMomentForCircle(mass, 0, radius, cpvzero);
  this->Body = cpSpaceAddBody(Space, cpBodyNew(mass, Moment));
  cpBodySetPosition(this->Body, pos);

  this->Shape =
      cpSpaceAddShape(Space, cpCircleShapeNew(this->Body, radius, cpvzero));
  cpShapeSetUserData(this->Shape, this);

  cpShapeSetFriction(this->Shape, 0.7f);
  cpShapeSetElasticity(this->Shape, 0.3f);
}

Obj::~Obj() {
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

Obj::operator cpBody *() const { return this->Body; }

Obj::operator cpShape *() const { return this->Shape; }

} // namespace Pebble

#endif