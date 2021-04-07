#ifndef MATH
#define MATH
/* Contains Math Functions And Structures */
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>

namespace Croipe::Math
{
    /* Structures */
    struct Vector3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    struct Triangle {
        Vector3 points[3];
    };

    struct Mesh {
        std::vector<Triangle> triangles;

        bool LoadFromObjectFile(std::string filename)
	    {
            triangles.clear();
            std::fstream f(filename);
            if (!f.is_open())
                return false;

            std::vector<Vector3> verts;
            while (!f.eof())
            {
                char line[128];
                f.getline(line, 128);
                std::stringstream s;
                s << line;
                char junk;
                if (line[0] == 'v')
                {
                    Vector3 v;
                    s >> junk >> v.x >> v.y >> v.z;
                    verts.push_back(v);
                }
                if (line[0] == 'f')
                {
                    int f[3];
                    s >> junk >> f[0] >> f[1] >> f[2];
                    triangles.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
                }
            }
    
            return true;
        }
    };

    struct Matrix4x4 {
        float matrix[4][4] = { 0 };
    };

    /* Advanced Structs */
    struct Wireframe {
        Vector3 rotation, translation, camera;
        Mesh mesh;
    };

    /* Functions */
    void MultiplyMatrixVector(Vector3 &i, Vector3 &o, Matrix4x4 &m) {
            o.x = i.x * m.matrix[0][0] + i.y * m.matrix[1][0] + i.z * m.matrix[2][0] + m.matrix[3][0];
            o.y = i.x * m.matrix[0][1] + i.y * m.matrix[1][1] + i.z * m.matrix[2][1] + m.matrix[3][1];
            o.z = i.x * m.matrix[0][2] + i.y * m.matrix[1][2] + i.z * m.matrix[2][2] + m.matrix[3][2];
            float w = i.x * m.matrix[0][3] + i.y * m.matrix[1][3] + i.z * m.matrix[2][3] + m.matrix[3][3];

            if (w != 0.0f)
            {
                o.x /= w; o.y /= w; o.z /= w;
            }
    }

    Vector3 CalculateNormal(Triangle &triangle) {  
        /* Calculate Lines */
        Vector3 normal, line1, line2;
        line1.x = triangle.points[1].x - triangle.points[0].x;
        line1.y = triangle.points[1].y - triangle.points[0].y;
        line1.z = triangle.points[1].z - triangle.points[0].z;

        /* Calculate Normal */
        line2.x = triangle.points[2].x - triangle.points[0].x;
        line2.y = triangle.points[2].y - triangle.points[0].y;
        line2.z = triangle.points[2].z - triangle.points[0].z;

        normal.x = line1.y * line2.z - line1.z * line2.y;
        normal.y = line1.z * line2.x - line1.x * line2.z;
        normal.z = line1.x * line2.y - line1.y * line2.x;

        /* Normalize */
        float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
        normal.x /= l; normal.y /= l; normal.z /= l;

        return normal;
    }

    Triangle RotateTriangle(Triangle &triangle, Vector3 &rotation) {
        float zAngle = rotation.z;
        float xAngle = rotation.x;
        float yAngle = rotation.y;

        Matrix4x4 rotationMatrixZ, rotationMatrixX, rotationMatrixY;
        Triangle rotatedTriangleZ, rotatedTriangleZX, rotatedTriangleZXY;

        /* Rotation Maxtrix Setup */
        rotationMatrixZ.matrix[0][0] = cosf(zAngle);
        rotationMatrixZ.matrix[0][1] = sinf(zAngle);
        rotationMatrixZ.matrix[1][0] = -sinf(zAngle);
        rotationMatrixZ.matrix[1][1] = cosf(zAngle);
        rotationMatrixZ.matrix[2][2] = 1.0f;
        rotationMatrixZ.matrix[3][3] = 1.0f;

        rotationMatrixX.matrix[0][0] = 1.0f;
        rotationMatrixX.matrix[1][1] = cosf(xAngle);
        rotationMatrixX.matrix[1][2] = sinf(xAngle);
        rotationMatrixX.matrix[2][1] = -sinf(xAngle);
        rotationMatrixX.matrix[2][2] = cosf(xAngle);
        rotationMatrixX.matrix[3][3] = 1.0f;

        rotationMatrixY.matrix[0][0] = cosf(yAngle);
        rotationMatrixY.matrix[0][2] = sinf(yAngle);
        rotationMatrixY.matrix[2][0] = -sinf(yAngle);
        rotationMatrixY.matrix[1][1] = 1.0f;
        rotationMatrixY.matrix[2][2] = cosf(yAngle);
        rotationMatrixY.matrix[3][3] = 1.0f;


        /* Rotate */
        MultiplyMatrixVector(triangle.points[0], rotatedTriangleZ.points[0], rotationMatrixZ);
        MultiplyMatrixVector(triangle.points[1], rotatedTriangleZ.points[1], rotationMatrixZ);
        MultiplyMatrixVector(triangle.points[2], rotatedTriangleZ.points[2], rotationMatrixZ);

        MultiplyMatrixVector(rotatedTriangleZ.points[0], rotatedTriangleZX.points[0], rotationMatrixX);
        MultiplyMatrixVector(rotatedTriangleZ.points[1], rotatedTriangleZX.points[1], rotationMatrixX);
        MultiplyMatrixVector(rotatedTriangleZ.points[2], rotatedTriangleZX.points[2], rotationMatrixX);

        MultiplyMatrixVector(rotatedTriangleZX.points[0], rotatedTriangleZXY.points[0], rotationMatrixY);
        MultiplyMatrixVector(rotatedTriangleZX.points[1], rotatedTriangleZXY.points[1], rotationMatrixY);
        MultiplyMatrixVector(rotatedTriangleZX.points[2], rotatedTriangleZXY.points[2], rotationMatrixY);

        return rotatedTriangleZXY;
    }

    Triangle TranslateTriangle(Triangle &triangle, Vector3 &translation) {
        Triangle translatedTriangle = triangle;

        float translationZ = translation.z;
        float translationX = translation.x;
        float translationY = translation.y;

        /* Translate Triangle */
        translatedTriangle.points[0].z += translationZ;
        translatedTriangle.points[1].z += translationZ;
        translatedTriangle.points[2].z += translationZ;

        translatedTriangle.points[0].x += translationX;
        translatedTriangle.points[1].x += translationX;
        translatedTriangle.points[2].x += translationX;

        translatedTriangle.points[0].y += translationY;
        translatedTriangle.points[1].y += translationY;
        translatedTriangle.points[2].y += translationY;

        return translatedTriangle;
    }

    Triangle ProjectTriangle(Triangle &triangle, int width, int height, float near_=0.1f, float far_=1000.0f, float fov=90.0f) {
        float aspectRatio = (float)height / (float)width;
        float fovRadians = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);
    
        /* Projection Matrix Setup */
        Triangle projectedTriangle;
        Matrix4x4 projectionMatrix;
        projectionMatrix.matrix[0][0] = aspectRatio * fovRadians;
        projectionMatrix.matrix[1][1] = fovRadians;
        projectionMatrix.matrix[2][2] = far_ / (far_ - near_);
        projectionMatrix.matrix[3][2] = (-far_ * near_) / (far_ - near_);
        projectionMatrix.matrix[2][3] = 1.0f;
        projectionMatrix.matrix[3][3] = 0.0f;

        /* Project */
        MultiplyMatrixVector(triangle.points[0], projectedTriangle.points[0], projectionMatrix);
        MultiplyMatrixVector(triangle.points[1], projectedTriangle.points[1], projectionMatrix);
        MultiplyMatrixVector(triangle.points[2], projectedTriangle.points[2], projectionMatrix);

        projectedTriangle.points[0].x += 1.0f; projectedTriangle.points[0].y += 1.0f;
        projectedTriangle.points[1].x += 1.0f; projectedTriangle.points[1].y += 1.0f;
        projectedTriangle.points[2].x += 1.0f; projectedTriangle.points[2].y += 1.0f;

        /* Scale */
        projectedTriangle.points[0].x *= 0.5f * (float)width;
        projectedTriangle.points[0].y *= 0.5f * (float)height;
        projectedTriangle.points[1].x *= 0.5f * (float)width;
        projectedTriangle.points[1].y *= 0.5f * (float)height;
        projectedTriangle.points[2].x *= 0.5f * (float)width;
        projectedTriangle.points[2].y *= 0.5f * (float)height;

        return projectedTriangle;
    }

    Triangle ClipTriangle(Triangle &triangle) {
        Triangle clippedTriangle = triangle;

        if (triangle.points[0].z <= 0.0f) {
            clippedTriangle.points[0].z = 0.5f;
        }

        if (triangle.points[1].z <= 0.0f) {
            clippedTriangle.points[1].z = 0.5f;
        }

        if (triangle.points[2].z <= 0.0f) {
            clippedTriangle.points[2].z = 0.5f;
        }

        return clippedTriangle;
    }
}

#endif