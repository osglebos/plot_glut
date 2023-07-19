#include <math.h>

class vec
{
public:
	double x, y, z;
	vec(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c)
	{
	}

	vec operator+(const vec &b)
	{
		return vec(x + b.x, y + b.y, z + b.z);
	}

	vec operator-(const vec &b)
	{
		return vec(x - b.x, y - b.y, z - b.z);
	}

	vec operator+=(const vec &b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		return  *this;
	}

	double operator&(const vec &b)
	{
		return (x * b.x + y * b.y + z * b.z);
	}

	double operator!(void)
	{
		return sqrt(*this&*this);
	}

	vec operator|(vec v)
	{
		return vec(((*this).x + v.x) / 2, ((*this).y + v.y) / 2, ((*this).z + v.z) / 2);
	}

	vec operator*(const double b)
	{
		return vec(b * x, b * y, b * z);
	}

	static vec Rnd(void)
	{
		return vec(rand() * 1.0 / RAND_MAX,
			rand() * 1.0 / RAND_MAX,
			rand() * 1.0 / RAND_MAX);
	}

	static vec Rnd0(void)
	{
		return vec(0, rand() * 3.0 / RAND_MAX + 0.5, 0);
	}

	vec &operator=(const vec v)
	{
		x = v.x, y = v.y, z = v.z;
		return *this;
	}
};
