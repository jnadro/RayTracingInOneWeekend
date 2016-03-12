#include <iostream>
#include "ray.h"

bool hitSphere(const vec3& center, float radius, const ray& r) {
	const vec3 oc = r.origin() - center;
	const float a = dot(r.direction(), r.direction());
	const float b = 2.0f * dot(oc, r.direction());
	const float c = dot(oc, oc) - radius * radius;
	const float discriminant = b * b - 4.0f * a * c;
	return discriminant > 0;
}

vec3 color(const ray& r) {
	if (hitSphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, r)) {
		return vec3(1.0f, 0.0f, 0.0f);
	}
	vec3 unitDirection = makeUnitVector(r.direction());
	float t = 0.5f * (unitDirection.y() + 1.0f);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

int main() {
	const int nx = 200;
	const int ny = 100;

	std::cout << "P3\n" 
			  << nx << " " << ny 
			  << "\n255\n";

	const vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
	const vec3 horizontal(4.0f, 0.0f, 0.0f);
	const vec3 vertical(0.0f, 2.0f, 0.0f);
	const vec3 origin(0.0f, 0.0f, 0.0f);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			const float u = float(i) / float(nx);
			const float v = float(j) / float(ny);
			const ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			const vec3 col = color(r);
			const int ir = int(255.99 * col.r());
			const int ig = int(255.99 * col.g());
			const int ib = int(255.99 * col.b());
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}
