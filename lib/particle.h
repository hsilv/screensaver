#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
public:
    float x, y, z, vx, vy, vz, speed;
    uint8_t r, g, b;

    Particle() : x(0), y(0), z(0), vx(0), vy(0), vz(0), speed(0), r(255), g(255), b(255) {}

    Particle(float x, float y, float z, float vx, float vy, float vz, float speed, uint8_t r, uint8_t g, uint8_t b)
        : x(x), y(y), z(z), vx(vx), vy(vy), vz(vz), speed(speed), r(r), g(g), b(b) {}

    void update(float dt, float sigma, float rho, float beta, float globalSpeed, float maxSpeed)
    {
        float dx = sigma * (y - x);
        float dy = x * (rho - z) - y;
        float dz = x * y - beta * z;

        vx += dx * dt;
        vy += dy * dt;
        vz += dz * dt;

        float speed = std::sqrt(vx * vx + vy * vy + vz * vz);
        if (speed > maxSpeed)
        {
            float scale = maxSpeed / speed;
            vx *= scale;
            vy *= scale;
            vz *= scale;
        }

        x += vx * dt * globalSpeed;
        y += vy * dt * globalSpeed;
        z += vz * dt * globalSpeed;

        if (std::isnan(x) || std::isnan(y) || std::isnan(z))
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            vx = 0.0f;
            vy = 0.0f;
            vz = 0.0f;
        }
    }

    void updateRossler(float dt, float a, float b, float c, float globalSpeed, float maxSpeed)
    {
        float dx = -y - z;
        float dy = x + a * y;
        float dz = b + z * (x - c);

        vx += dx * dt;
        vy += dy * dt;
        vz += dz * dt;

        float speed = std::sqrt(vx * vx + vy * vy + vz * vz);
        if (speed > maxSpeed)
        {
            float scale = maxSpeed / speed;
            vx *= scale;
            vy *= scale;
            vz *= scale;
        }

        x += vx * dt * globalSpeed;
        y += vy * dt * globalSpeed;
        z += vz * dt * globalSpeed;

        if (std::isnan(x) || std::isnan(y) || std::isnan(z))
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            vx = 0.0f;
            vy = 0.0f;
            vz = 0.0f;
        }
    }
};
#endif