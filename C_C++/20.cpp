
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

struct Vec3 {
    double x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double d) const { return Vec3(x * d, y * d, z * d); }
    Vec3 operator/(double d) const { return Vec3(x / d, y / d, z / d); }

    Vec3 normalize() const {
        double mag = sqrt(x * x + y * y + z * z);
        return Vec3(x / mag, y / mag, z / mag);
    }
    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
};

struct Ray {
    Vec3 origin, direction;
    Ray(Vec3 o, Vec3 d) : origin(o), direction(d.normalize()) {}
};

struct Sphere {
    Vec3 center;
    double radius;
    Vec3 color;

    Sphere(Vec3 c, double r, Vec3 col) : center(c), radius(r), color(col) {}

    bool intersect(const Ray& ray, double& t) const {
        Vec3 oc = ray.origin - center;
        double b = 2.0 * oc.dot(ray.direction);
        double c = oc.dot(oc) - radius * radius;
        double discriminant = b * b - 4 * c;
        if (discriminant < 0) return false;
        t = (-b - sqrt(discriminant)) / 2.0;
        return (t > 0);
    }
};

Vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres) {
    double min_t = 1e30;
    const Sphere* hit_sphere = nullptr;

    for (const auto& sphere : spheres) {
        double t;
        if (sphere.intersect(ray, t) && t < min_t) {
            min_t = t;
            hit_sphere = &sphere;
        }
    }

    if (hit_sphere) return hit_sphere->color;
    return Vec3(0, 0, 0); // Background color
}

void render(const std::vector<Sphere>& spheres) {
    const int width = 400, height = 300;
    std::ofstream image("raytracer.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double u = (x - width / 2.0) / width;
            double v = (y - height / 2.0) / height;
            Ray ray(Vec3(0, 0, 0), Vec3(u, v, -1));
            Vec3 color = trace(ray, spheres);

            int r = std::min(255, (int)(color.x * 255));
            int g = std::min(255, (int)(color.y * 255));
            int b = std::min(255, (int)(color.z * 255));
            image << r << " " << g << " " << b << "\n";
        }
    }

    image.close();
}

int main() {
    std::vector<Sphere> spheres = {
        Sphere(Vec3(0, 0, -5), 1, Vec3(1, 0, 0)), // Red sphere
        Sphere(Vec3(2, 0, -6), 1, Vec3(0, 1, 0)), // Green sphere
        Sphere(Vec3(-2, 0, -6), 1, Vec3(0, 0, 1)) // Blue sphere
    };

    render(spheres);
    std::cout << "Rendering complete. Output saved as raytracer.ppm\n";
    return 0;
}
