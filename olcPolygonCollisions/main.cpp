#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <vector>
#include <algorithm>

class PolygonCollisions : public olc::PixelGameEngine {
    public:
        PolygonCollisions() {
            sAppName = "Polygon Collisions";
        }

    
    /// 2d Vector
    struct sVec2d
    {
        float x;
        float y;
    };
    
    
    /// Polygon
    struct sPolygon
    {
        std::vector<sVec2d> o;      // "Model" of shape
        sVec2d pos;                 // Position of shape
        float angle;                // Direction of shape
        std::vector<sVec2d> p;      // Transformed Points
        bool overlap = false;       // Collision has occured
    };
    
    /// Shapes to perform collisions with
    std::vector<sPolygon> vecShapes;
    
    int nMode = 0;
    
    public:
        bool OnUserCreate() override
        {
            // Create Pentagon
            sPolygon s1;
            float fTheta = 3.14159f * 2.0f / 5.0f;
            s1.pos = { 100, 100 };
            s1.angle = 0.0f;
            for(int i = 0; i < 5; i++)
            {
                s1.o.push_back({30.0f * cosf(fTheta * i), 30.0f * sinf(fTheta * i)});
                s1.p.push_back({30.0f * cosf(fTheta * i), 30.0f * sinf(fTheta * i)});
            }
 
            // Create Triangle
             sPolygon s2;
             fTheta = 3.14159f * 2.0f / 3.0f;
             s2.pos = { 200, 150 };
             s2.angle = 0.0f;
             for(int i = 0; i < 3; i++)
             {
                 s2.o.push_back({20.0f * cosf(fTheta * i), 20.0f * sinf(fTheta * i)});
                 s2.p.push_back({20.0f * cosf(fTheta * i), 20.0f * sinf(fTheta * i)});
             }

            // Create Quad
             sPolygon s3;
             s3.pos = { 50, 200 };
             s3.angle = 0.0f;
             for(int i = 0; i < 3; i++)
             {
                 s3.o.push_back({-30,-30});
                 s3.o.push_back({-30,+30});
                 s3.o.push_back({+30,+30});
                 s3.o.push_back({+30,-30});
                 s3.p.resize(4);
             }

            // Add shapes
            vecShapes.push_back(s1);
            vecShapes.push_back(s2);
            vecShapes.push_back(s3);

            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override
        {
            // Shape 1
            if(GetKey(olc::Key::LEFT).bHeld) vecShapes[0].angle -= 2.0f * fElapsedTime;
            if(GetKey(olc::Key::RIGHT).bHeld) vecShapes[0].angle += 2.0f * fElapsedTime;
            if(GetKey(olc::Key::UP).bHeld)
            {
                vecShapes[0].pos.x += cosf(vecShapes[0].angle) * 60.0f * fElapsedTime;
                vecShapes[0].pos.y += sinf(vecShapes[0].angle) * 60.0f * fElapsedTime;
            }
            if(GetKey(olc::Key::DOWN).bHeld)
            {
                vecShapes[0].pos.x -= cosf(vecShapes[0].angle) * 60.0f * fElapsedTime;
                vecShapes[0].pos.y -= sinf(vecShapes[0].angle) * 60.0f * fElapsedTime;
            }

            // Shape 2
            if(GetKey(olc::Key::A).bHeld) vecShapes[1].angle -= 2.0f * fElapsedTime;
            if(GetKey(olc::Key::D).bHeld) vecShapes[1].angle += 2.0f * fElapsedTime;
            if(GetKey(olc::Key::W).bHeld)
            {
                vecShapes[1].pos.x += cosf(vecShapes[1].angle) * 60.0f * fElapsedTime;
                vecShapes[1].pos.y += sinf(vecShapes[1].angle) * 60.0f * fElapsedTime;
            }
            if(GetKey(olc::Key::S).bHeld)
            {
                vecShapes[1].pos.x -= cosf(vecShapes[1].angle) * 60.0f * fElapsedTime;
                vecShapes[1].pos.y -= sinf(vecShapes[1].angle) * 60.0f * fElapsedTime;
            }

            // Update shapes and reset flags
            for(auto &r : vecShapes)
            {
                for(int i = 0; i < r.o.size(); i++)
                {
                    r.p[i] =
                    {
                        (r.o[i].x * cosf(r.angle)) - (r.o[i].y * sinf(r.angle)) + r.pos.x,
                        (r.o[i].x * sinf(r.angle)) + (r.o[i].y * cosf(r.angle)) + r.pos.y,
                    };
                    
                    r.overlap = false;
                }
            }
            
            // ====== Render ======= //
            Clear(olc::BLUE);
            
            // Draw the shapes
            for(auto &r : vecShapes)
            {
                // Draw Boundary
                for(int i = 0; i < r.p.size(); i++)
                    DrawLine(r.p[i].x, r.p[i].y, r.p[(i +1) % r.p.size()].x, r.p[(i +1) % r.p.size()].y);
                
                // Draw Direction
                DrawLine(r.p[0].x, r.p[0].y, r.pos.x, r.pos.y, (r.overlap ? olc::RED : olc::WHITE));
                
            }
            
            return true;
        }
};


int main(int argc, char const *argv[]) {
	PolygonCollisions demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
