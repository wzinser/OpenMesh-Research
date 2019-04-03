#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/Sqrt3T.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/LoopT.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <random>

using namespace std;
typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;

int main()
{
	MyMesh mesh;
mesh.request_face_colors();

// Add vertices and faces
MyMesh::VertexHandle vhandle[4];
vhandle[0] = mesh.add_vertex(MyMesh::Point(0,0,0));
vhandle[1] = mesh.add_vertex(MyMesh::Point(0,1,0));
vhandle[2] = mesh.add_vertex(MyMesh::Point(1,1,0));
//vhandle[3] = mesh.add_vertex(MyMesh::Point(1,0,0));

mesh.add_face(vhandle[2], vhandle[1], vhandle[0]);
//mesh.add_face(vhandle[2], vhandle[0], vhandle[3]);

// Subdivision Tool
OpenMesh::Subdivider::Uniform::LoopT<MyMesh> loop;
loop.attach(mesh);
loop(4);
loop.detach();

OpenMesh::Subdivider::Uniform::CatmullClarkT<MyMesh> cat;
cat.attach(mesh);
cat(2);
cat.detach();

 // Add float data type to faces and vertices
 OpenMesh::FPropHandleT<float> fprop_float;
 mesh.add_property(fprop_float, "fprop_float");
 mesh.property(fprop_float).set_persistent(true);

 OpenMesh::VPropHandleT<float> vprop_float;
 mesh.add_property(vprop_float, "vprop_float");
 mesh.property(vprop_float).set_persistent(true);


// Generate random number between 1-10
random_device device;
mt19937 rng(device());
uniform_int_distribution<std::mt19937::result_type> dist6(1,10);

//Iterate over each face and place random number in the vertice
	for (MyMesh::VertexIter v_it=mesh.vertices_begin(); v_it !=mesh.vertices_end(); ++v_it)
{

	mesh.property(vprop_float, *v_it) = dist6(rng);
//	cout << "The value of vertice(" << *v_it << ") is: " << mesh.property(vprop_float, *v_it) << endl; 


}




for (MyMesh::FaceIter f_it=mesh.faces_begin(); f_it !=mesh.faces_end(); ++f_it)
{
	cout << endl;
	cout << "The vertices that surround face " <<  *f_it << " are: " << endl;   
 	MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it);
	float x = 0;
for(; fv_it.is_valid(); ++fv_it)
  {
    
    x = x + mesh.property(vprop_float, *fv_it); 	
 cout << *fv_it << " ---- value: " << mesh.property(vprop_float, *fv_it) <<  endl;
  }


			if (x > 15)
				{
			        mesh.property(fprop_float, *f_it) = 1;	
				mesh.set_color(*f_it, MyMesh::Color(255,0,0));
				}
			if (x <=15)
			{
			        mesh.property(fprop_float, *f_it) = 0;	
				mesh.set_color(*f_it, MyMesh::Color(0,0,255));
			}
 cout << "The sum of this face's vertices is: " << x << endl; 


cout << "Face " << *f_it << " has a value of: " << mesh.property(fprop_float, *f_it) << endl;

}


















OpenMesh::IO::Options wopt;
wopt = OpenMesh::IO::Options::FaceColor;








 // Write mesh to output.off file
try
{
	if (!OpenMesh::IO::write_mesh(mesh, "output.off", wopt))
	{
		std::cerr << "Cannot write to file " << std::endl;
		return 1;
	}

}

catch (std::exception& x)
{
	std::cerr <<x.what() << std::endl;
	return 1;
}




}
