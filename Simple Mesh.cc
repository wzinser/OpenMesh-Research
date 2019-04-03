#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/LoopT.hh>



typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;


using namespace std;
 

int main()
{
  MyMesh mesh;

  // Create Vertices
  MyMesh::VertexHandle vhandle[3];
  vhandle[0] = mesh.add_vertex(MyMesh::Point( 0, 0 ,  0));
  vhandle[1] = mesh.add_vertex(MyMesh::Point( 1, 0 ,  0));
  vhandle[2] = mesh.add_vertex(MyMesh::Point( 1, 1 ,  0));
  vhandle[3] = mesh.add_vertex(MyMesh::Point( 0, 1 ,  0));
 
  // Create Faces
  std::vector<MyMesh::VertexHandle>  face_vhandles;
  face_vhandles.clear();
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  mesh.add_face(face_vhandles);


 // Initialize Subdivider Tool
 OpenMesh::Subdivider::Uniform::LoopT<MyMesh> Loop;


 // Execute 5 subdivision steps
 Loop.attach(mesh);
 Loop( 5 );
 Loop.detach();


		


OpenMesh::IO::write_mesh(mesh, "output.off");



return 0;

}













