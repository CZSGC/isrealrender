
#include "bitmap.h"
#include "cmath"
#include "render.h"
#include "texture.h"
#include <unordered_map>

const float FOUR = (float)4 / 3;
const float SIXTEEN = (float)16 / 9;
const float TWENTY = (float)21 / 9;
const float ONE = (float)1 / 1;

const int WIDTH = 800;
const int HEIGHT = 800;
const float PI = 3.1415926;

double* zbuffer = new double[(WIDTH + 1) * (HEIGHT+1)];

Vec4f eye{ 1,1,3 };
Vec4f center{ 0,0,0 };
Vec4f up{ 0,1,0 };
Vec4f light(0, 0, 3);

float Iambient = 0.3;

Matrix4f cameraMatrix = cameraTransformation(eye, center, up);

Matrix4f modelMatrix = modelTransformation(Vec3f(0, 0, 0));

Matrix4f projectMatrix = perspectiveTransformation(106, ONE, 0.1, 100);

Matrix4f viewportMatrix = viewportTransformation(0, 0, 800, 800, 99.9);

Matrix3f normalMatrix = modelMatrix.toMatrix3f().Inverse().Transpose();

Model model("E:/tinyrender/tinyrenderer/obj/african_head/african_head.obj");
std::vector<Vec3f> vertexVector = model.vertexVector;
std::vector<Vec3f> normalVertexVector = model.normalVertexVector;
std::vector<Vec3f> textureVertexVector = model.textureVertexVector;
TGAImage image;
TGAImage normalImage;


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

void setTriangle(Vec3f a, Vec3f b, Vec3f c, uint32_t color, graphics_buffers* buffers) {
    int screenWidth = buffers->width();
    int screenHeight = buffers->height();

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

    left = left < 0 ? 0 : left;
    right = right > screenWidth ? screenWidth : right;
    top = top > screenHeight ? screenHeight : top;
    bottom = bottom < 0 ? 0 : bottom;


    for (int i = left; i <= right; i++) 
    {
        for (int j = bottom; j <= top; j++) 
        {
            if (isInside(a, b, c, Vec3f(i, j))) 
            {
                float z = 0.f;
                Vec3f v = Vec3f::barycentric(a, b, c, Vec3f(i, j));
                z = a.z * v[0] + b.z * v[1] + c.z * v[2];
                if (z >= 0 && z < (screenWidth) * (screenHeight)) {
                    if (z > zbuffer[j * (screenWidth) + i])
                    {
                        zbuffer[j * (screenWidth) + i] = z;
                        setPixel(Vec3f(i, j), color, buffers);
                    }
                }
            }
        }
    }


}

void getbox(Vec3f& a,Vec3f& b,Vec3f& c,int& left, int& right, int& bottom, int& top,int screenWidth, int screenHeight) {


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

    left = left < 0 ? 0 : left;
    right = right > screenWidth ? screenWidth : right;
    top = top > screenHeight ? screenHeight : top;
    bottom = bottom < 0 ? 0 : bottom;
}

void setTriangle(std::vector<Vec3f> v, double zbuffer[], graphics_buffers* buffers) {
    int screenWidth = buffers->width();
    int screenHeight = buffers->height();
    int left, right, bottom, top;

    Vec4f pos1(vertexVector[v[0][0]]);
    Vec4f pos2(vertexVector[v[0][1]]);
    Vec4f pos3(vertexVector[v[0][2]]);

    Vec3f a = setTriangleparameters(pos1);
    Vec3f b = setTriangleparameters(pos2);
    Vec3f c = setTriangleparameters(pos3);

    getbox(a, b, c, left, right, bottom, top, screenWidth, screenHeight);


    Vec3f UVb = textureVertexVector[v[2][1]];
    Vec3f UVc = textureVertexVector[v[2][2]];
    Vec3f UVa = textureVertexVector[v[2][0]];


    float du1 = UVb.x - UVa.x;
    float dv1 = UVb.y - UVa.y;
    float du2 = UVc.x - UVa.x;
    float dv2 = UVc.y - UVa.y;


    Vec3f e1 = (pos2-pos1).toVec3();
    Vec3f e2 = (pos3-pos1).toVec3();


    float determinant = 1.f / (du1 * dv2 - du2 * dv1);
    
    Vec3f T = Vec3f(determinant * (dv2 * e1.x - dv1 * e2.x), determinant * (dv2 * e1.y - dv1 * e2.y), determinant * (dv2 * e1.z - dv1 * e2.z)).normalize();

    Vec3f normala = normalVertexVector[v[1][0]];
    Vec3f normalb = normalVertexVector[v[1][1]];
    Vec3f normalc = normalVertexVector[v[1][2]];


    for (int i = left; i <= right; i++)
    {
        for (int j = bottom; j <= top; j++)
        {
            if (isInside(a, b, c, Vec3f(i, j)))
            {

                double z = 0;
                Vec3f barycentric = Vec3f::barycentric(a, b, c, Vec3f(i, j));
                z = a.z * barycentric[0] + b.z * barycentric[1] + c.z * barycentric[2];
                if (z < zbuffer[j * (screenWidth)+i])
                {
                    Matrix3f m;
                    Vec3f pos = Vec3f(pos1.x * barycentric[0] + pos2.x * barycentric[1] + pos3.x * barycentric[2], pos1.y * barycentric[0] + pos2.y * barycentric[1] + pos3.y * barycentric[2], pos1.z * barycentric[0] + pos2.z * barycentric[1] + pos3.z * barycentric[2]);

                    Vec3f normal= Vec3f(normala.x * barycentric[0] + normalb.x * barycentric[1] + normalc.x * barycentric[2], normala.y * barycentric[0] + normalb.y * barycentric[1] + normalc.y * barycentric[2], normala.z * barycentric[0] + normalb.z * barycentric[1] + normalc.z * barycentric[2]);
                    
                    
                    normal = normal.normalize();


                    Vec3f B = Vec3f::crossProduct(T, normal);

                    for (int i = 0; i < 3; i++) {
                        m[i][0] = T[i];
                        m[i][1] = B[i];
                        m[i][2] = normal[i];
                    }

                    int x = (UVa[0] * barycentric[0] + UVb[0] * barycentric[1] + UVc[0] * barycentric[2]) * image.width();
                    int y = ((UVa[1] * barycentric[0] + UVb[1] * barycentric[1] + UVc[1] * barycentric[2])) * image.height();

                    TGAColor color = image.get(x, y);
                    TGAColor normarColor = normalImage.get(x, y);

                    Vec3f normalpos(normarColor.bgra[2] *2.f / 256.f - 1.f, normarColor.bgra[1] * 2.f / 256.f - 1.f, normarColor.bgra[0] * 2.f / 256 - 1.f);
                    normalpos = normalpos.normalize();


                    Vec3f pointToLight = (light.toVec3() - pos).normalize();
                    float Idiffuse = ((normalMatrix * m  * normalpos).normalize()) * (pointToLight) > 0 ? ((normalMatrix * m * normalpos).normalize()) * (pointToLight) : 0;
                    float c = normal * pointToLight;

                    //Vec3f view = (eye.toVec3() - pos).normalize();
                    //float Ispecular = pointToLight * view > 0 ? pointToLight * view : 0;

                    zbuffer[j * (screenWidth)+i] = z;
                    setPixel(Vec3f(i, j), color.changecolor(Idiffuse + Iambient), buffers);
                }
            }
        }
    }


}



Matrix4f viewportTransformation(int x, int y, int w, int h,int depth) {
    Matrix4f m1,m2;
    m2[0][3] = x + w / 2.f;
    m2[1][3] = y + h / 2.f;
    m1[0][0] = w / 2.f;
    m1[1][1] = h / 2.f;

    return m2*m1;
}


Matrix4f modelTransformation(Vec3f modelposition)
{

    Matrix4f scale;
    Matrix4f translationMatrix;
    translationMatrix[0][3] = modelposition.x;
    translationMatrix[1][3] = modelposition.y;
    translationMatrix[2][3] = modelposition.z;
    scale[0][0] = 1;
    scale[1][1] = 1;
    scale[2][2] = 1;
    return scale * translationMatrix;
}

Matrix4f perspectiveTransformation(float fov, float aspectratio,float n,float f)
{
    n = std::abs(n);
    f = std::abs(f);
    float h = std::abs(std::tan(fov / 180 * PI/2) * n);
    float w = std::abs(h * aspectratio);
    Matrix4f mat;
    mat[0][0] = 2 * n / w;
    mat[1][1] = 2 * n / h;
    mat[2][2] = (n + f) / (n - f);
    mat[2][3] = 2 * f * n / (n - f);
    mat[3][3] = 0;
    mat[3][2] = -1;
    return mat;
}




//void drawModelLine(graphics_buffers* buffs) 
//{
//    Model model("E:/tinyrender/tinyrenderer/obj/african_head/african_head.obj");
//    for (int i = 0; i < model.faceVertexIndexVector.size(); i++) 
//    {
//
//        Vec3f v = model.faceVertexIndexVector[i];
//        for (int j = 0; j < 3; j++) 
//        {
//            Vec3f v0 = model.vertexVector[v[j]];
//            Vec3f v1 = model.vertexVector[v[(j + 1) % 3]];
//            float x0 = (v0.x + 1.) * buffs->width() / 2.;
//            float y0 = (v0.y + 1.) * buffs->height() / 2.;
//            float x1 = (v1.x + 1.) * buffs->width() / 2.;
//            float y1 = (v1.y + 1.) * buffs->height() / 2.;
//            setLine(Vec3f(x0, y0), Vec3f(x1, y1), 0xffffffff, buffs);
//        }
//    }
//}

Matrix4f cameraTransformation(Vec4f eye, Vec4f center, Vec4f up) {
    Vec4f z = (eye - center).normalize();
    Vec4f x = Vec4f::crossProduct(up, z).normalize();
    Vec4f y = Vec4f::crossProduct(z, x).normalize();
    Matrix4f Minv;
    Matrix4f Tr;
    for (int i = 0; i < 3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -eye[i];
    }
    return Minv * Tr;
}

void perspectiveDivision(Vec4f &v) {
    v.x = v.x / v.w;
    v.y = v.y / v.w;
    v.z = v.z / v.w;
    v.w = v.w / v.w;
}

Vec3f setTriangleparameters(Vec4f v)
{
    Matrix4f mat;
    mat = projectMatrix * cameraMatrix *modelMatrix * mat;
    
    v = mat * v; 
    perspectiveDivision(v);
    v = viewportMatrix * v;
    return v.toVec3();
}

void test(graphics_buffers* buffs)
{
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        zbuffer[i] = FLT_MAX;
    }

    image.read_tga_file("E:/tinyrender/tinyrenderer/obj/african_head/african_head_diffuse (1).tga");
    image.flip_vertically();

    normalImage.read_tga_file("E:/tinyrender/tinyrenderer/obj/african_head/african_head_nm_tangent.tga");
    normalImage.flip_vertically();

    for (int i = 0; i < model.faceIndexVector.size(); i++)
    {
        std::vector<Vec3f> v = model.faceIndexVector[i];

        setTriangle(v, zbuffer, buffs);
    }
}

float getIntensity(Vec4f normalVector, Vec4f light)
{
    normalVector = normalVector.normalize();
    return normalVector * light;
}