#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <limits>
#include "camera.h"
#include "hitableList.h"
#include "material.h"
#include "sampler.h"
#include "sphere.h"

vec3 color(const Ray& r, Hitable* world, UniformRandomSampler& sampler, int depth) {
	hitRecord rec;
	if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
		Ray scattered;
		vec3 attenuation;

		if (depth < 50 && rec.material->scatter(r, rec, sampler, attenuation, scattered)) {
			return attenuation * color(scattered, world, sampler, depth + 1);
		}
		else {
			return vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else {
		vec3 unitDirection = normalize(r.direction());
		float t = 0.5f * (unitDirection.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

int main() {
	const int nx = 200;
	const int ny = 100;
	const int ns = 100;

	std::cout << "P3\n" 
			  << nx << " " << ny 
			  << "\n255\n";

	static const int sphereCount = 5;
	Hitable* list[sphereCount] = { nullptr };
	list[0] = new Sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(vec3(0.1f, 0.2f, 0.5f)));
	list[1] = new Sphere(vec3(0.0f, -100.5f, -1.0f), 100, new Lambertian(vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new Sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(vec3(0.8f, 0.6f, 0.2f), 0.0f));
	list[3] = new Sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Dielectric(1.5f));
	list[4] = new Sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f, new Dielectric(1.5f));

	Hitable* world = new HitableList(list, sphereCount);

	UniformRandomSampler sampler;

	Camera camera(vec3(-2.0f, 2.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), 90, float(nx) / float(ny));
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0.0f, 0.0f, 0.0f);

			for (int s = 0; s < ns; ++s) {
				const float u = float(i + sampler.getNextSample()) / float(nx);
				const float v = float(j + sampler.getNextSample()) / float(ny);
				const Ray r = camera.getRay(u, v);
				col += color(r, world, sampler, 0);
			}

			col /= float(ns);

			constexpr float gamma = 1.0f / 2.2f;
			col = vec3(std::pow(col.r(), gamma), std::pow(col.g(), gamma), std::pow(col.b(), gamma));

			const int ir = int(255.99 * col.r());
			const int ig = int(255.99 * col.g());
			const int ib = int(255.99 * col.b());
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	delete world;
	delete list[0];
	delete list[1];
	delete list[2];
	delete list[3];
	delete list[4];
}
