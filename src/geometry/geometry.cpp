#include "geometry.hpp"

// ##############################################
// ### Cube3d ###################################
// ##############################################

Cube3d::Cube3d(const Vector3d &_center, const double size) : Solid3d() {
    Vector3d points[8];

    // front
    points[0] = Vector3d(-size / 2, -size / 2, -size / 2);
    points[1] = points[0] + Vector3d(size, 0   , 0);
    points[2] = points[0] + Vector3d(size, size, 0);
    points[3] = points[0] + Vector3d(0   , size, 0);

    // back
    for (int i = 4; i < 8; ++i)
        points[i] = points [i - 4] + Vector3d(0, 0, size);

    for (int i = 0; i < 8; ++i)
        points[i].set_color(get_random_colour());

    for (int i = 0; i < 4; ++i) {
        add_segment(Segment3d(points[i], points[(i + 1) % 4]));         // front face
        add_segment(Segment3d(points[i + 4], points[(i + 1) % 4 + 4])); // back face
        add_segment(Segment3d(points[i], points[i + 4]));               // links
    }

    *this += _center;
}


// ##############################################
// ### Sphere3d #################################
// ##############################################

Sphere3d::Sphere3d(const Vector3d &_center, 
                   const double size,
                   const unsigned number_of_circles,
                   const unsigned number_of_points_per_circle) : Solid3d() {

    std::vector<Vector3d> points;

    for (unsigned i = 0; i < number_of_circles; ++i) {
        double theta = map(i, 0, number_of_circles - 1, -90, 90);

        Vector3d first_circle_point;

        for (unsigned j = 0; j < number_of_points_per_circle; ++j) {
            double phi = map(j, 0, number_of_points_per_circle, -180, 180);

            points.push_back(Vector3d(size * cos(as_radians(theta)) * cos(as_radians(phi)),
                                      size * cos(as_radians(theta)) * sin(as_radians(phi)),
                                      size * sin(as_radians(theta))));

            if (j == 0)
                first_circle_point = points.back();

            if (j != 0)
                add_segment(Segment3d(points.end()[-1], points.end()[-2]));
            // no need to repeat this point
            if (theta <= -90 || theta >= 90)
                break;
        }
        add_segment(Segment3d(first_circle_point, points.back()));

    }

    //for (size_t i = 0; i < points.size(); ++i)
     //   add_segment(Segment3d(points[i], points[(i + 1) % number_of_circles]));

    // if (sphere_design == 0)
    //         for (size_t i = 0; i < points.size(); ++i)
    //             add_segment(Segment3d(points[i], points[i] + Vector3d(0.5, 0, 0)));

    //     else if (sphere_design == 1)
    //         for (size_t i = 0; i < points.size(); ++i)
    //             add_segment(Segment3d(points[i], points[(i + number_of_points_per_circle) % points.size()]));
        
    //     else if (sphere_design == 2)
    //         for (size_t i = 0; i < points.size(); ++i)
    //             add_segment(Segment3d(points[i], points[(i + 10) % points.size()]));

    *this += _center;
}


// ##############################################
// ### Asteroid3d ###############################
// ##############################################

Asteroid3d::Asteroid3d(const Vector3d &_center, const double size) : Solid3d() {

    //Sphere3d base_sphere(_center, size, 30, 50);

    //*this += base_sphere;

    // for (int i = 0; i < 30; ++i) {

    //     size_t idx = rand(0, edges.size());

    //     edges[idx].get_a() += Vector3d(rand(0.0, 20.0), rand(0.0, 20.0), rand(0.0, 20.0));
    // }

    std::vector<Vector3d> points;
    unsigned number_of_circles = 20;
    unsigned number_of_points_per_circle = 50;

    // set points
    for (unsigned i = 0; i < number_of_circles; ++i) {
        double theta = map(i, 0, number_of_circles - 1, -85, 85);

        for (unsigned j = 0; j < number_of_points_per_circle; ++j) {
            double phi = map(j, 0, number_of_points_per_circle, -180, 180);

            points.push_back(Vector3d(size * cos(as_radians(theta)) * cos(as_radians(phi)),
                                      size * cos(as_radians(theta)) * sin(as_radians(phi)),
                                      size * sin(as_radians(theta))));
        }
    }

    // move points
    for (unsigned i = 0; i < number_of_circles; ++i) {

        for (unsigned j = 0; j < number_of_points_per_circle; ++j) {
            size_t current_point_idx = i * number_of_points_per_circle + j;

            if (i > 3 && i < number_of_circles - 4 &&
                j > 3 && j < number_of_points_per_circle - 4 &&
                rand(0, 60) == 0) {

                const double factor = rand(1.2, 1.5);
                //#points.end()[-2] *= factor * 0.8;
                //points.end()[-2].set_color(sf::Color(124, 0, 0));
                //points.end()[- number_of_points_per_circle - 1] *= factor * 0.96;
                points[current_point_idx - number_of_points_per_circle] *= (factor + 1) / 2;
                points[current_point_idx + number_of_points_per_circle] *= (factor + 1) / 2;

                points[current_point_idx - 1] *= (factor + 1) / 2;
                points[current_point_idx + 1] *= (factor + 1) / 2;
                points[current_point_idx] *= factor;
                points[current_point_idx].set_color(sf::Color::Red);
            }
        }
    }

    // set links
    for (unsigned i = 0; i < number_of_circles; ++i) {
        Vector3d first_circle_point;

        for (unsigned j = 0; j < number_of_points_per_circle; ++j) {
            size_t current_point_idx = i * number_of_points_per_circle + j;

            if (j > 0)
                add_segment(Segment3d(points[current_point_idx], points[current_point_idx - 1]));
            else
                add_segment(Segment3d(points[current_point_idx], points[current_point_idx + number_of_points_per_circle - 1]));

            if (i > 0) {
                add_segment(Segment3d(points[current_point_idx],
                                      points[current_point_idx - number_of_points_per_circle]));
            }
        }
    }
}

