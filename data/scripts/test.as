#include "common.as"
#ifndef DEBUG
#define DEBUG
#endif

#define DEBUG

weakref<Texture> textureRef;

enum State
{
    None      = 0,
    Idle      = (1 << 0),
    WalkNorth = (1 << 1),
    WalkSouth = (1 << 2),
    WalkEast  = (1 << 3),
    WalkWest  = (1 << 4)
}

bool isIdle(Object@ self)
{
    return (self.state() & State::Idle != 0);
}

void onCreate(Object@ self)
{
#ifdef DEBUG
    orConsole.print(Console::Warning, "DEBUG MODE");
#endif

    CVar@ test = orCVarManager.cvar("player1.moveLeft");
    if (test is null)
        @test = orCreateBinding("player1.moveLeft", "Moves player1 left", CVar::Archive | CVar::System);

    {
        CVarUnlocker unlock(test);
    }
    test.joyValid(0);

    orConsole.print(Console::Info, "fs_basepath=" + test.toLiteral());
    Transformable@ comp = cast<Transformable>(self.addComponent("Transformable", "transformable"));
    if (comp is null)
        orConsole.print(Console::Warning, "Failed to add Transformable");

    Texture @tex = textureRef.get();
    if (tex is null)
    {
        @tex = orLoadTexture("test/test.png");
        if (tex !is null)
            textureRef = weakref<Texture>(@tex);
    }
}
    
void onThink(Object@ self, float delta)
{
    if (isIdle(self))
        return;
}

void onUpdate(Object@ self, float delta)
{
    if (self is null)
        return;

    Vector2f move;
    move.x = 94.f*orJoystickManager.axisPosition(0, 0)*delta;
    move.y = 94.f*orJoystickManager.axisPosition(0, 1)*delta;
    if (move.x == 0 && move.y == 0)
    {
        self.setState(State::Idle);
        return;
    }
    int state = 0;
    if (move.x > 0)
    {
        state &= ~State::WalkEast;
        state |= State::WalkWest;
    }
    else if (move.x < 0)
    {
        state &= ~State::WalkWest; 
        state |= State::WalkEast;
    }

    if (move.y > 0)
    {
        state &= ~State::WalkNorth;
        state |= State::WalkSouth;
    }
    else if (move.y < 0)
    {
        state &= ~State::WalkSouth;
        state |= State::WalkNorth;
    }
    self.setState(state);
    self.move(move);
}

void onDraw(Object@ self)
{
    Texture @tex = textureRef.get();
    Vector2f origin(tex.width()/2, tex.height()/2);
    Rectanglef subrect(0.0f, 0.0f, tex.width(), tex.height());

    if (tex !is null)
        tex.draw(0.f, 0.f, subrect, origin, false, false, -interpolate(orJoystickManager.axisPosition(0, 2),orJoystickManager.axisPosition(0, 3), 181.f));
}

void onDestroyed(Object@ self)
{
    orConsole.print(Console::Message, "I'm dying, you stupid shit!!!");
    orJoystickManager.motorOff(0);
}

