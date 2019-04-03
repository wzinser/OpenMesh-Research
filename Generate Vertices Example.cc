#include <iostream>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include <OpenMesh/Tools/Subdivider/Uniform/LoopT.hh>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>



// ----------------------------------------------------------------------------
typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;
// ----------------------------------------------------------------------------
// Build a simple cube and write it to std::cout
  
int main()
{
  MyMesh mesh;
  // generate vertices

  
 // Generate a NxN matrix of size N
  int size = 10;
  int i = 0;
  int j = 0;
  int counter = 0;

  MyMesh::VertexHandle vhandle[100];
  while (j < size)
   {

        if ( i == size)
         {
           j++;
           i = 0;
         }

        if ( j == size)
         {
            break;
        }


    vhandle[counter] = mesh.add_vertex(MyMesh::Point(i,j,0));
    i++;
    counter++;
 
   } 
try
  {
    if ( !OpenMesh::IO::write_mesh(mesh, "output.ply") )
    {
      std::cerr << "Cannot write mesh to file 'output.ply'" << std::endl;
      return 1;
    }
  }
  catch( std::exception& x )
  {
    std::cerr << x.what() << std::endl;
    return 1;
  }
  return 0;
}

