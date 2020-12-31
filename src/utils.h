#include <GL/glew.h>
#include <string>

using namespace std;

class Utils
{
public:
    // GOLD material - ambient, diffuse, specular, and shininess
    static float* goldAmbient();
    static float* goldDiffuse();
    static float* goldSpecular();
    static float goldShininess();
    // SILVER material - ambient, diffuse, specular, and shininess
    static float* silverAmbient();
    static float* silverDiffuse();
    static float* silverSpecular();
    static float silverShininess();
    // BRONZE material - ambient, diffuse, specular, and shininess
    static float* bronzeAmbient();
    static float* bronzeDiffuse();
    static float* bronzeSpecular();
    static float bronzeShininess();
};
