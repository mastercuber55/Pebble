# Pebble, A Tiny Physics Engine

## Cause of Existence?

Okay so I've been working on my game called Dangan Starfall and a major issue was that using a ancient simple physics engine such as physac works right of the box but running into memory issues with this is so amazingly easy. And when I tried chipmunk2D it was somewhat confusing and hard to maintain for such a game that requires minimal physics. so I created Pebble. A Tiny wrapper for chipmunk that doesn't focus on doing everything but on doing simplest things.

## Usage

I use my Frax Framework with raylib all the time however I've tried to make pebble self reliant and can run without Frax but honestly. There's not much to it. Its a small system to allow simpler usage of chipmunk for me.

```cxx
namespace Pebble {
    struct Box {
        cpBody *Body;
        cpShape *Shape;
        cpSpace *Space;

        Box(cpVect pos, cpVect size, cpFloat mass, cpSpace *space);
        ~Box();

    };

    void FraxDraw(Frax::Rect &);
}
```

As you can see, there's not much going around. All `Pebble::Box` is take details of the hitbox and give you a out of the "Box" thing to use. It may sound small but it comes really handy. Also since its a C++ struct, it can manage the lifetime of the `Shape` and `Body` quite well. I've abstracted away some stuff you'll not use from here but they too fix some memory issues.

As for using it. you can use it as you see fit... or perhaps with Frax.\
If you do use it with Frax, `Frax::Rect` has a property called `Data` with type being `std::any` to store custom data where you can store `Pebble::Box`. Below is an example of how you can do so.

```cxx
Player.Data = new Pebble::Box({4, 4}, {32, 32}, 64, Space);
```

Also in case its something like Player whose physical body you'll need to use often. I'd personally recommend you to create a `Pebble::Box` seperately and then obtain its `.Body` and store it somewhere. well Ig if you're using Frax, you'll be storing it in your current `Frax::Scene`. 

```red
THIS HAS BEEN A INITIAL README WITH JUST BASIC INFO FOR USAGE AND WILL BE MODIED SOON FOR BETTER USAGE EXAMPLES AND STUFF AND WHATEVER.
```