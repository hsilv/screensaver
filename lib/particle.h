#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
public:
    float x, y, z, vx, vy, vz, speed;

    Particle(float x, float y, float z, float vx, float vy, float vz, float speed)
        : x(x), y(y), z(z), vx(vx), vy(vy), vz(vz), speed(speed) {}

    void update(float dt, float sigma, float rho, float beta)
    {
        float dx = sigma * (y - x);
        float dy = x * (rho - z) - y;
        float dz = x * y - beta * z;

        vx += dx * dt;
        vy += dy * dt;
        vz += dz * dt;

        x += vx * dt * speed;
        y += vy * dt * speed;
        z += vz * dt * speed;

        // Verify if each coordinate is NaN
        if (x != x || y != y || z != z)
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
    }
};

#endif // PARTICLE_H