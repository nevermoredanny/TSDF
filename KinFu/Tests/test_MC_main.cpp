#include "../TSDFVolume.hpp"
#include "../GPU/GPUMarchingCubes.hpp"

#include "vector_types.h"

#include <vector>
#include <fstream>

#include <Eigen/Dense>

phd::TSDFVolume * make_sphere_tsdf( const  int vx, const  int vy, const  int vz, const float px, const float py, const float pz ) {
	using namespace Eigen;


	phd::TSDFVolume *volume = phd::TSDFVolume::make_volume( phd::TSDFVolume::GPU, Vector3i{ vx, vy, vz }, Vector3f{px, py, pz} );

	// Now make some data
	float * data = new float[ vx * vy * vz];

	float radius = fminf( px, fminf( py, pz ) ) / 2.5f;

	Vector3f vsize{ px / vx, py / vy, pz / vz };

	size_t idx = 0;
	for ( int z = 0; z < vz; z++ ) {
		float dz = ( (z + 0.5f) * vsize.z() ) - ( pz / 2.0f );
		for ( int y = 0; y < vy; y++ ) {
			float dy = ( (y + 0.5f) * vsize.y() ) - ( py / 2.0f );
			for ( int x = 0; x < vx; x++ ) {
				float dx = ( (x + 0.5f) * vsize.x() ) - ( px / 2.0f );

				float dist = sqrt(dx * dx + dy * dy + dz * dz);

				data[idx++] = dist - radius;
			}
		}
	}


	// Set the data
	volume->set_distance_data( data );

	delete [] data;

	return volume;
}

void write_to_ply( const std::string& file_name, const std::vector<float3>& vertices, const std::vector<int3>& triangles ) {
	std::ofstream f { file_name };
	if( f.is_open() ) {
		f << "ply" << std::endl;
		f << "format ascii 1.0" << std::endl;

		f << "element vertex " << vertices.size() << std::endl;
		f << "property float x" << std::endl;
		f << "property float y" << std::endl;
		f << "property float z" << std::endl;

		f << "element face " << triangles.size() << std::endl;
		f << "property list uchar int vertex_indices" << std::endl;
		f << "end_header" << std::endl;

		for ( int v = 0; v < vertices.size(); v++ ) {
			f << vertices[v].x << " " << vertices[v].y << " " << vertices[v].z << std::endl;
		}
		for ( int t = 0; t < triangles.size(); t++ ) {
			f << "3 " << triangles[t].x << " " << triangles[t].y << " " << triangles[t].z << std::endl;
		}
	} else {
		std::cout << "Problem opening file for write " << file_name << std::endl;
	}	
}


/**
 * Test the Marching Cubes code
 */
int main( int argc, const char * argv[] ) {
	int retval = 0;

	phd::TSDFVolume::volume_type type = phd::TSDFVolume::GPU;
	phd::TSDFVolume *volume = make_sphere_tsdf( 256, 256, 256, 1024.0f, 1024.0f, 1024.0f );


	if ( volume ) {
		phd::Camera camera { 585.6f, 585.6f, 316.0f, 247.6f  };

		std::vector<int3> triangles;
		std::vector<float3> vertices;
		extract_surface( volume, vertices, triangles);

		// Save to PLY file
		write_to_ply( "/home/dave/Desktop/sphere.ply", vertices, triangles);


	} else {
		std::cout << "Couldn't make TSDF volume spehere " << std::endl;
		retval = -1;
	}

	return retval;

}