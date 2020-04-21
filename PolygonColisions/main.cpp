#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class PolygonCollisions : public olc::PixelGameEngine {
    public:
        PolygonCollisions() {
            sAppName = "Polygon Collisions";
        }

    public:
        bool OnUserCreate() override {
            // Called once at the start, so create things here
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override {
            Clear(olc::BLACK);
            return true;
        }
};


int main(int argc, char const *argv[]) {
	PolygonCollisions demo;
	if (demo.Construct(15, 10, 32, 32))
		demo.Start();

	return 0;
}
