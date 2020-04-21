#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <vector>
#include <algorithm>

class PolygonCollisions : public olc::PixelGameEngine {
    public:
    
    PolygonCollisions()
    {
        sAppName = "PolygonCollisions";
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
    
    /// Check if the two polygons overlap using SAT (Separated Axis Theorem)
    /// Where be basically project a "shadow" of each polygon onto an axis parallel to the normal of each
    /// edge of each polygon and if any of the "shadows" don't overlap, the shapes don't overlap.
    bool ShapeOverlap_SAT(sPolygon &r1, sPolygon &r2)
    {
        sPolygon *poly1;
        sPolygon *poly2;
        
        for(int shape = 0; shape < 2; shape++)
        {
            if(shape == 0)
            {
                // First test r1 against r2
                poly1 = &r1;
                poly2 = &r2;
            }
            else
            {
                // Then test r2 against r1
                poly1 = &r2;
                poly2 = &r1;
            }
            
            // Process each edge
            for(int a = 0; a < poly1->p.size(); a++)
            {
                // Get the other point for this edge
                int b = (a + 1) % poly1->p.size();
                
                // Create a vector along the normal of this edge (-y, x)
                sVec2d axisProj = { -(poly1->p[b].y - poly1->p[a].y), poly1->p[b].x - poly1->p[a].x};
                
                // Calculate the min and max 1D points for the shadow of r1
                float min_r1 = INFINITY, max_r1 = -INFINITY;
                for(int p = 0; p < poly1->p.size(); p++)
                {
                    // Calculate projection as the dot product of each point and the axis
                    float q = (poly1->p[p].x * axisProj.x + poly1->p[p].y * axisProj.y);
                    
                    // Keep track of the total "shadow" cast on the axis
                    min_r1 = std::min(min_r1, q);
                    max_r1 = std::max(max_r1, q);
                }
                
                // Calculate the min and max 1D points for the shadow r2
                float min_r2 = INFINITY, max_r2 = -INFINITY;
                for(int p = 0; p < poly2->p.size(); p++)
                {
                    // Calculate projection as the dot product of each point and the axis
                    float q = (poly2->p[p].x * axisProj.x + poly2->p[p].y * axisProj.y);
                    
                    // Keep track of the total "shadow" cast on the axis
                    min_r2 = std::min(min_r2, q);
                    max_r2 = std::max(max_r2, q);
                }
                
                if(!(max_r2 >= min_r1 && max_r1 >= min_r2))
                {
                    // We have an edge that the "shadows" don't overlap, so the objects don't overlap
                    return false;
                }

            }

        }
        
        // Every "shadow" overlapped, so the shapes overlap
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
        
        // Check for overlap
        for(int m = 0; m < vecShapes.size(); m++)
            for(int n = m + 1; n < vecShapes.size(); n++)
            {
                vecShapes[m].overlap = ShapeOverlap_SAT(vecShapes[m], vecShapes[n]);
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
