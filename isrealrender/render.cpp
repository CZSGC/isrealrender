
#include "bitmap.h"
#include "cmath"
void setPixel(Vec3f v, uint32_t pix, graphics_buffers* buffers) 
{
    int x = v.x, y = v.y;
    if (x >= 0 && x < buffers->width() && y >= 0 && y < buffers->height())
    {
        buffers->back_.data[(y)*buffers->width() + x] = pix;
    }
}

void setLine(Vec3f v0, Vec3f v1, uint32_t pex, graphics_buffers* buffers)
{
    int x0 = v0.x, y0 = v0.y, x1 = v1.x, y1 = v1.y;
    bool steep = false;
    if (std::abs(y1 - y0) > std::abs(x1 - x0)) 
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) 
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = 2 * std::abs(dy);
    int error = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++) 
    {
        if (steep) 
        {
            setPixel(Vec3f(y, x), 0xffffffff, buffers);
        }
        else 
        {
            setPixel(Vec3f(x, y), 0xffffffff, buffers);
        }
        error += derror;
        if (error > dx) 
        {
            y += (y1 > y0) ? 1 : -1;
            error -= 2 * dx;
        }
    }

}

void setTriangle(Vec3f a, Vec3f b, Vec3f c, std::vector<float> zbuffer, uint32_t color, graphics_buffers* buffers) {


    int left, right, bottom, top;
    if (a.x < b.x)
        left = a.x < c.x ? a.x : c.x;
    else
        left = b.x < c.x ? b.x : c.x;


    if (a.y < b.y)
        bottom = a.y < c.y ? a.y : c.y;
    else
        bottom = b.y < c.y ? b.y : c.y;


    if (a.x > b.x)
        right = a.x > c.x ? a.x : c.x;
    else
        right = b.x > c.x ? b.x : c.x;


    if (a.y > b.y)
        top = a.y > c.y ? a.y : c.y;
    else
        top = b.y > c.y ? b.y : c.y;



    for (int i = left; i <= right; i++) 
    {
        for (int j = bottom; j <= top; j++) 
        {
            if (isInside(a, b, c, Vec3f(i, j))) 
            {
                float z = 0.f;
                Vec3f v = Vec3f::barycentric(a, b, c, Vec3f(i, j));
                z = a.z * v[0] + b.z * v[1] + c.z * v[2];
                if (z > zbuffer[j * (buffers->width()) + i]) 
                {
                    zbuffer[j * buffers->width() + i] = z;
                    setPixel(Vec3f(i, j), color,buffers);
                }
            }
        }
    }


}

Matrix4f cameraTransformation(Vec4f e, Vec4f u, Vec4f g) 
{

    Vec4f d = g - e;
    Matrix4f translationMatrix;
    Matrix4f rotationMatrix;
    Vec4f vecX = Vec4f::crossProduct(u, d);

    for (int i = 0; i < 3; i++) 
    {
        rotationMatrix[i][1] = u[i];
        rotationMatrix[i][2] = -d[i];
        rotationMatrix[i][0] = vecX[i];

        translationMatrix[i][3] = -e[i];
    }
    return (rotationMatrix.inverse())*translationMatrix;
}

Matrix4f orthographicTransformation(float n, float f, float t, float b, float l, float r) 
{
    Matrix4f translationMatrix;
    Matrix4f scaleMatrix;
    translationMatrix[0][3] = -((r + l) / 2);
    translationMatrix[1][3] = -((t + b) / 2);
    translationMatrix[2][3] = -((n + f) / 2);

    scaleMatrix[0][0] = 2 / (r - l);
    scaleMatrix[1][1] = 2 / (t - b);
    scaleMatrix[2][2] = 2 / (n - f);

    return scaleMatrix * translationMatrix;
}

Matrix4f ModelTransformation(Vec3f modelposition)
{


    Matrix4f translationMatrix;
    translationMatrix[0][3] = modelposition.x;
    translationMatrix[1][3] = modelposition.y;
    translationMatrix[2][3] = modelposition.z;

    return translationMatrix;
}

Matrix4f PerspectiveTransformation(float n,float f) 
{
    Matrix4f mat;
    mat[0][0] = n;
    mat[1][1] = n;
    mat[3][3] = 0;
    mat[3][2] = 1;
    mat[2][2] = n + f;
    mat[2][3] = -n * f;
    return mat;
}

Matrix4f PerspectiveTransformation( float f)
{
    Matrix4f mat;
    mat[1][1] = -1;
    mat[3][3] = 0;
    mat[3][2] = -(1 / f);
    return mat;
}



void drawModelLine(graphics_buffers* buffs) 
{
    Model model("E:/tinyrender/tinyrenderer/obj/african_head/african_head.obj");
    for (int i = 0; i < model.faceVertexIndexVector.size(); i++) 
    {

        Vec3f v = model.faceVertexIndexVector[i];
        for (int j = 0; j < 3; j++) 
        {
            Vec3f v0 = model.vertexVector[v[j]];
            Vec3f v1 = model.vertexVector[v[(j + 1) % 3]];
            float x0 = (v0.x + 1.) * buffs->width() / 2.;
            float y0 = (v0.y + 1.) * buffs->height() / 2.;
            float x1 = (v1.x + 1.) * buffs->width() / 2.;
            float y1 = (v1.y + 1.) * buffs->height() / 2.;
            setLine(Vec3f(x0, y0), Vec3f(x1, y1), 0xffffffff, buffs);
        }
    }
}



void test(graphics_buffers* buffs) 
{
    Model model("E:/tinyrender/tinyrenderer/obj/african_head/test.obj");
    Vec4f eye{ 1,1,3 };
    Vec4f center{ 0,0,0 };
    Vec4f up{ 0,1,0 };
    Vec4f t = eye - center;
    Matrix4f cameraMatrix = cameraTransformation(eye,up,center);
    Matrix4f modelMatrix = ModelTransformation(Vec3f(0,0,0));
    Matrix4f projectMatrix = PerspectiveTransformation(std::sqrt(t.x * t.x + t.y * t.y + t.z * t.z));
    Matrix4f viewportMatrix= orthographicTransformation

}