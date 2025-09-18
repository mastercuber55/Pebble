// #define CP_FLOAT_TYPE float
#include <chipmunk/chipmunk.h>
#include <raylib.h>

class PhyObj {
    private:
        cpBody* Body;
        cpShape* Shape;
        Vector2 Size;

    public:

        Color Tint;

        // Basic stuff
        void Init(cpSpace* Space, Rectangle Dest, float Mass);
        ~PhyObj();

        // Physics
        void ApplyForce(cpVect Force);
        void SetAngle(float Rad);
        float GetAngle();

        // Utils
        void Draw();
        operator Vector2() const;
};

#ifdef RAYMUNK_IMPL
#undef RAYMUNK_IMPL

void PhyObj::Init(cpSpace* Space, Rectangle Dest, float Mass) {
    cpFloat Moment =  cpMomentForBox(Mass, Dest.width, Dest.height);
    this->Body = cpSpaceAddBody(Space, cpBodyNew(Mass, Moment));
    cpBodySetPosition(this->Body, { Dest.x, Dest.y });

    this->Shape = cpSpaceAddShape(Space, cpBoxShapeNew(this->Body, Dest.width, Dest.height, 0.0));
    cpShapeSetFriction(this->Shape, 0.7);
    cpShapeSetElasticity(this->Shape, 0.1);
    cpShapeSetUserData(this->Shape, this);

    this->Size.x = Dest.width;
    this->Size.y = Dest.height;
}

PhyObj::~PhyObj() {
    if(this->Shape) cpShapeFree(this->Shape);
    if(this->Body) cpBodyFree(this->Body);
}

void PhyObj::ApplyForce(cpVect Force) {
    cpBodyApplyForceAtLocalPoint(this->Body, Force, {0, 0});
}

void PhyObj::SetAngle(float Rad) {
    cpBodySetAngle(this->Body, Rad);
}

float PhyObj::GetAngle() {
    return cpBodyGetAngle(this->Body);
}

void PhyObj::Draw() {
    cpVect pos = cpBodyGetPosition(this->Body); 
    float Rotation = cpBodyGetAngle(this->Body) * RAD2DEG;
    DrawRectanglePro(
        {(float)pos.x, (float)pos.y, this->Size.x, this->Size.y},
        { this->Size.x / 2, this->Size.y / 2 },
        Rotation, 
        RED
    );
}

PhyObj::operator Vector2() const {
    cpVect pos = cpBodyGetPosition(this->Body); 
    return { (float)pos.x, (float)pos.y };
}
#endif