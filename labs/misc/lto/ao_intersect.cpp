#include "ao.h"

void
ray_sphere_intersect(Isect *isect, const Ray *ray, const Sphere *sphere)
{
    vec rs;

    // Calculate the vector 'rs' from the sphere center to the ray origin
    rs.x = ray->org.x - sphere->center.x;
    rs.y = ray->org.y - sphere->center.y;
    rs.z = ray->org.z - sphere->center.z;

    // Calculate coefficients C and B
    // rs**2 - B**2 is the length of the perpendicular line
    // sqrt(r**2 - (rs**2 - B**2)) is the length of the chord.
    //
    // Here it writes C = rs**2 - r**2, D = B**2 - C = B**2 - (rs**2 - r**2) = r**2 - (rs**2 - B**2)
    // B**2 is the square of the projected rs on ray direction.
    // If D is larger than 0, it is ray intersect with the sphere
    // D is used to determine whether the ray cast intersect with the sphere.
    double B = vdot(rs, ray->dir);
    double C = vdot(rs, rs) - sphere->radius * sphere->radius;
    double D = B * B - C;

    // Check if there is an intersection
    if (D > 0.0) {
        // Calculate the first intersection point
        double t = -B - sqrt(D);
        
        // Check if the intersection point is within the positive ray direction
        if ((t > 0.0) && (t < isect->t)) {
            // Update the intersection data in the 'isect' structure
            isect->t = t;
            isect->hit = 1;
            
            // Calculate the intersection point coordinates
            isect->p.x = ray->org.x + ray->dir.x * t;
            isect->p.y = ray->org.y + ray->dir.y * t;
            isect->p.z = ray->org.z + ray->dir.z * t;
            
            // Calculate the surface normal at the intersection point
            isect->n.x = isect->p.x - sphere->center.x;
            isect->n.y = isect->p.y - sphere->center.y;
            isect->n.z = isect->p.z - sphere->center.z;

            // Normalize the surface normal vector
            vnormalize(&(isect->n));
        }
    }
}

void
ray_plane_intersect(Isect *isect, const Ray *ray, const Plane *plane)
{
    double d = -vdot(plane->p, plane->n);
    double v = vdot(ray->dir, plane->n);

    if (fabs(v) < 1.0e-17) return;

    double t = -(vdot(ray->org, plane->n) + d) / v;

    if ((t > 0.0) && (t < isect->t)) {
        isect->t = t;
        isect->hit = 1;
        
        isect->p.x = ray->org.x + ray->dir.x * t;
        isect->p.y = ray->org.y + ray->dir.y * t;
        isect->p.z = ray->org.z + ray->dir.z * t;

        isect->n = plane->n;
    }
}
