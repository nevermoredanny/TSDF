//
//  TestCamera.cpp
//  KinFu
//
//  Created by Dave on 19/05/2016.
//  Copyright © 2016 Sindesso. All rights reserved.
//

#include <gtest/gtest.h>
#include "../../include/Camera.hpp"
#include "../../include/RenderUtilities.hpp"
#include "../../include/TUMDataLoader.hpp"
#include "../../include/DepthImage.hpp"


#include "TestHelpers.hpp"

#pragma mark - Construction

const float EPS = 1e-6;

#pragma mark - Coordinate Transforms
#pragma mark World to Camera
static Eigen::Vector3f world_coordinates[] = {
    Eigen::Vector3f{ 0, 0, 0 },
    Eigen::Vector3f{ 100, 0, 0 },
    Eigen::Vector3f{ 100, 100, 0 },
    Eigen::Vector3f{ 0, 100, 0 },
    Eigen::Vector3f{ 0, 100, 100 },
    Eigen::Vector3f{ 0, 0, 100 },
    Eigen::Vector3f{ 100, 0, 100 },
    Eigen::Vector3f{ 100, 100, 100 }
};

TEST( Camera, at_0_0_0__facing_0_0_1i) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( world_coordinates[i].x(), camera_coordinate.x(), EPS );
        EXPECT_NEAR( world_coordinates[i].y(), camera_coordinate.y(), EPS );
        EXPECT_NEAR( world_coordinates[i].z(), camera_coordinate.z(), EPS );
    }

    delete cam;
    delete cam;
}

TEST( Camera, at_0_0_0__facing_m1_0_0) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->look_at(-1.0f, 0.0f, 0.0f );

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), world_coordinates[i].z(), EPS );
        EXPECT_NEAR( camera_coordinate.y(), world_coordinates[i].y(), EPS );
        EXPECT_NEAR( camera_coordinate.z(), -world_coordinates[i].x(), EPS );
    }
    delete cam;
    delete cam;
}

TEST( Camera, at_0_0_0__facing_0_m1_0 ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->look_at(0.0f, -1.0f, 0.0f );

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), world_coordinates[i].x(), EPS );
        EXPECT_NEAR( camera_coordinate.y(), world_coordinates[i].z(), EPS );
        EXPECT_NEAR( camera_coordinate.z(), -world_coordinates[i].y(), EPS );
    }
    delete cam;
}

TEST( Camera, at_0_0_0__facing_0_1_0 ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->look_at(0.0f, 1.0f, 0.0f );

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), world_coordinates[i].x(), EPS );
        EXPECT_NEAR( camera_coordinate.y(), -world_coordinates[i].z(), EPS );
        EXPECT_NEAR( camera_coordinate.z(), world_coordinates[i].y(), EPS );
    }
    delete cam;
}

TEST( Camera, at_0_0_0__facing_1_0_0 ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->look_at(1.0f, 0.0f, 0.0f );

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), -world_coordinates[i].z(), EPS );
        EXPECT_NEAR( camera_coordinate.y(), world_coordinates[i].y(), EPS );
        EXPECT_NEAR( camera_coordinate.z(), world_coordinates[i].x(), EPS );
    }
    delete cam;
}

TEST( Camera, at_0_0_0__facing_0_0_m1 ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->look_at(0.0f, 0.0f, -1.0f );

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), -world_coordinates[i].x(), EPS );
        EXPECT_NEAR( camera_coordinate.y(), world_coordinates[i].y(), EPS );
        EXPECT_NEAR( camera_coordinate.z(), -world_coordinates[i].z(), EPS );
    }
    delete cam;
}

#pragma Translation Only
TEST( Camera, at_100_0_0__facing_0_0_0 ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->move_to(100, 0, 0);

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), world_coordinates[i].x() - 100, EPS );
        EXPECT_NEAR( camera_coordinate.y(), world_coordinates[i].y(), EPS );
        EXPECT_NEAR( camera_coordinate.z(), world_coordinates[i].z(), EPS );
    }
    delete cam;
}

TEST( Camera, at_0_100_0__facing_0_0_0 ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->move_to(0, 100, 0);

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), world_coordinates[i].x(), EPS );
        EXPECT_NEAR( camera_coordinate.y(), world_coordinates[i].y() - 100, EPS );
        EXPECT_NEAR( camera_coordinate.z(), world_coordinates[i].z(), EPS );
    }
    delete cam;
}
TEST( Camera, at_0_0_100__facing_0_0_0i__default) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->move_to(0, 0, 100);

    Vector3f camera_coordinate;
    for ( int i = 0; i < 8; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), world_coordinates[i].x(), EPS );
        EXPECT_NEAR( camera_coordinate.y(), world_coordinates[i].y(), EPS );
        EXPECT_NEAR( camera_coordinate.z(), world_coordinates[i].z() - 100, EPS );
    }
    delete cam;
}

#pragma Translation and Rotation through unit cube vertices
TEST( Camera, givenWorldCoordinateWhenCameraOn_m1_m1_m1_ThenCameraCoordinateCorrect ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    cam->move_to(-1, -1, -1);
    cam->look_at(0, 0, 0);

    Vector3f camera_coordinate;
    // Only do origin
    for ( int i = 0; i < 0; i++ ) {
        camera_coordinate = cam->world_to_camera(world_coordinates[i]);

        EXPECT_NEAR( camera_coordinate.x(), 0, EPS );
        EXPECT_NEAR( camera_coordinate.y(), 0, EPS );
        EXPECT_NEAR( camera_coordinate.z(), -M_SQRT2, EPS );
    }
    delete cam;
}





#pragma mark - pixel to image plane

TEST( Camera, givenPointWhenInCentreOfImageThenCamPointIsOrigin ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();

    Vector2i point{ 320, 240 };
    Vector2f cam_point;
    cam_point = cam->pixel_to_image_plane(point);

    EXPECT_NEAR( cam_point.x(), 0.0f, EPS );
    EXPECT_NEAR( cam_point.y(), 0.0f, EPS );

    delete cam;
    delete cam;
}


TEST( Camera, givenPixelAt_0_0_ConvertToCamAndBackIsOK ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;

    Vector2i initial_pixel_coordinate{0, 0};
    Vector2f image_plane_coord;
    Vector2i final_pixel_coordinate;

    image_plane_coord = cam->pixel_to_image_plane(initial_pixel_coordinate);
    EXPECT_NEAR(image_plane_coord.x(), -0.5, 0.1 );
    EXPECT_NEAR(image_plane_coord.y(), -0.5, 0.11 );

    final_pixel_coordinate = cam->image_plane_to_pixel( image_plane_coord);

    EXPECT_EQ(initial_pixel_coordinate.x(), final_pixel_coordinate.x());
    EXPECT_EQ(initial_pixel_coordinate.y(), final_pixel_coordinate.y());
    delete cam;
}

TEST( Camera, givenPixelAt_MaxX_0_ConvertToCamAndBackIsOK ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;

    Vector2i initial_pixel_coordinate{640, 0};
    Vector2f image_plane_coord;
    Vector2i final_pixel_coordinate;

    image_plane_coord = cam->pixel_to_image_plane(initial_pixel_coordinate);
    EXPECT_NEAR(image_plane_coord.x(), 0.5, 0.1 );
    EXPECT_NEAR(image_plane_coord.y(), -0.5, 0.11 );

    final_pixel_coordinate = cam->image_plane_to_pixel( image_plane_coord);
    EXPECT_EQ(initial_pixel_coordinate.x(), final_pixel_coordinate.x());
    EXPECT_EQ(initial_pixel_coordinate.y(), final_pixel_coordinate.y());
    delete cam;
}

TEST( Camera, givenPixelAt_0_MaxY_ConvertToCamAndBackIsOK ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;

    Vector2i initial_pixel_coordinate{0, 480};
    Vector2f image_plane_coord;
    Vector2i final_pixel_coordinate;

    image_plane_coord = cam->pixel_to_image_plane(initial_pixel_coordinate);
    EXPECT_NEAR(image_plane_coord.x(), -0.5, 0.1 );
    EXPECT_NEAR(image_plane_coord.y(), 0.5, 0.11 );

    final_pixel_coordinate = cam->image_plane_to_pixel( image_plane_coord);
    EXPECT_EQ(initial_pixel_coordinate.x(), final_pixel_coordinate.x());
    EXPECT_EQ(initial_pixel_coordinate.y(), final_pixel_coordinate.y());
    delete cam;
}

TEST( Camera, givenPixelAt_MaxX_MaxY_ConvertToCamAndBackIsOK ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;

    Vector2i initial_pixel_coordinate{640, 480};
    Vector2f image_plane_coord;
    Vector2i final_pixel_coordinate;

    image_plane_coord = cam->pixel_to_image_plane(initial_pixel_coordinate);
    EXPECT_NEAR(image_plane_coord.x(), 0.5, 0.1 );
    EXPECT_NEAR(image_plane_coord.y(), 0.5, 0.11 );

    final_pixel_coordinate = cam->image_plane_to_pixel( image_plane_coord);
    EXPECT_EQ(initial_pixel_coordinate.x(), final_pixel_coordinate.x());
    EXPECT_EQ(initial_pixel_coordinate.y(), final_pixel_coordinate.y());
    delete cam;
}


#pragma mark - Pose

TEST( Camera, givenDefaultConstructorThenPoseIsIdentity ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    EXPECT_EQ( cam->pose(), Matrix4f::Identity() );
    delete cam;
}

TEST( Camera, givenMoveToThenPoseUpdatesCorrcetly ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    EXPECT_EQ( cam->pose(), Matrix4f::Identity() );

    cam->move_to(Vector3f { 100.0, 200.0, 300.0 } );

    Matrix4f pose = cam->pose();
    EXPECT_EQ( pose(0, 3), 100 );
    EXPECT_EQ( pose(1, 3), 200 );
    EXPECT_EQ( pose(2, 3), 300 );
    delete cam;
}



TEST( Camera, at_0_0_100__facing_0_0_0__active_look ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    Vector3f camera_postion{ 0, 0, 100 };

    cam->move_to(camera_postion );
    cam->look_at(Vector3f::Zero() );

    Matrix4f pose = cam->pose();

    Matrix4f expected = make_y_axis_rotation(-M_PI, camera_postion);
    for ( int i = 0; i < 16; i++ ) {
        int r = i % 4;
        int c = i / 4;
        EXPECT_NEAR(pose(i), expected(i), EPS ) << "Failed at (" << r << ", " << c << ")" << std::endl;
    }
    delete cam;
}


TEST( Camera, at_100_0_0__facing_0_0_0__active_look) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    Vector3f camera_postion{ 100, 0, 0 };

    cam->move_to(camera_postion );
    cam->look_at(Vector3f::Zero() );
    Matrix4f pose = cam->pose();


    Matrix4f expected = make_y_axis_rotation(-M_PI_2, camera_postion);
    for ( int i = 0; i < 16; i++ ) {
        int r = i % 4;
        int c = i / 4;
        EXPECT_NEAR(pose(i), expected(i), EPS ) << "Failed at (" << r << ", " << c << ")" << std::endl;
    }
    delete cam;
}

TEST( Camera, at_m100_0_0__facing_0_0_0__active_look ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    Vector3f camera_postion{ -100, 0, 0 };

    cam->move_to(camera_postion );
    cam->look_at(Vector3f::Zero() );
    Matrix4f pose = cam->pose();

    Matrix4f expected = make_y_axis_rotation(M_PI_2, camera_postion);
    for ( int i = 0; i < 16; i++ ) {
        int r = i % 4;
        int c = i / 4;
        EXPECT_NEAR(pose(i), expected(i), EPS ) << "Failed at (" << r << ", " << c << ")" << std::endl;
    }
    delete cam;
}

TEST( Camera, at_0_0_m100__facing_0_0_0__active_look) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    Vector3f camera_postion{ 0, 0, -100 };

    cam->move_to(camera_postion );
    cam->look_at(Vector3f::Zero() );
    Matrix4f pose = cam->pose();

    Matrix4f expected = make_y_axis_rotation(0, camera_postion);
    for ( int i = 0; i < 16; i++ ) {
        int r = i % 4;
        int c = i / 4;
        EXPECT_NEAR(pose(i), expected(i), EPS ) << "Failed at (" << r << ", " << c << ")" << std::endl;
    }
    delete cam;
}

TEST( Camera, at_0_100_0__facing_0_0_0__active_look ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    Vector3f camera_postion{ 0, 100, 0 };

    cam->move_to(camera_postion );
    cam->look_at(Vector3f::Zero() );
    Matrix4f pose = cam->pose();

    Matrix4f expected = make_x_axis_rotation(M_PI_2, camera_postion);
    for ( int i = 0; i < 16; i++ ) {
        int r = i / 4;
        int c = i % 4;
        EXPECT_NEAR( pose(i), expected(i), EPS ) << "Incorrect value for index (" << r << ", " << c << ")";
    }
    delete cam;
}

TEST( Camera, givenLookAtOriginWhenBelowThenPoseLooksUp ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;
    Vector3f camera_postion{ 0, -100, 0 };

    cam->move_to(camera_postion );
    cam->look_at(Vector3f::Zero() );
    Matrix4f pose = cam->pose();

    Matrix4f expected = make_x_axis_rotation(-M_PI_2, camera_postion);
    for ( int i = 0; i < 16; i++ ) {
        int r = i / 4;
        int c = i % 4;
        EXPECT_NEAR( pose(i), expected(i), EPS ) << "Incorrect value for index (" << r << ", " << c << ")";
    }
    delete cam;
}



TEST( Camera, givenCameraThenSettingPoseWorks ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;

    Matrix4f new_pose;
    new_pose << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;

    cam->set_pose(new_pose);

    EXPECT_EQ( cam->pose(), new_pose );
    delete cam;
}



TEST( Camera, givenDepthMapThenGenerateNormalMap ) {

    using namespace Eigen;

    Camera *cam = Camera::default_depth_camera();;

    TUMDataLoader tum{ "/mnt/hgfs/PhD/Kinect Raw Data/TUM/rgbd_dataset_freiburg1_xyz"};


    // Load depth image
    Matrix4f pose;
    DepthImage * di = tum.next( pose );

    Matrix<float, 3, Dynamic> vertices;
    Matrix<float, 3, Dynamic> normals;

    cam->depth_image_to_vertices_and_normals(di->data(), di->width(), di->height(), vertices, normals);

    save_normals_as_colour_png("/home/dave/Desktop/cam_test_normals_tum.png", di->width(), di->height(), normals);
    save_rendered_scene_as_png("/home/dave/Desktop/cam_test_render_tum.png", di->width(), di->height(), vertices, normals, *cam, Vector3f{10000, 10000, 1000});

    delete di;
    delete cam;
}

int main( int argc, char *argv[] ) {
    testing::InitGoogleTest(&argc, argv );
    return RUN_ALL_TESTS();
}
