#include <iostream>
#include <typeinfo>
// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <map>
#include <iomanip>
#include <cmath>
using namespace std;

// ----------------------------------------------------------------------------
typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;
// ----------------------------------------------------------------------------

 
int main()
{
  MyMesh mesh;
 
default_random_engine generator;
exponential_distribution<float> distribution(3.5);

const float nrolls = 1000000;
const float nstars = 1000;
const int nintervals = 163;

int p[nintervals] = {};

for (int ii=0; ii<nrolls; ++ii) {
	float number = distribution(generator);

      if (number<1.0) ++p[int(number*nintervals)]; 
}

//for (int ii=0; ii<nintervals; ++ii) {
//	cout << p[ii]*nstars/nrolls << endl; }

cout << p[150]*nstars/nrolls << endl;


 OpenMesh::FPropHandleT<float> fprop_float;
 mesh.add_property(fprop_float, "fprop_float");
 mesh.property(fprop_float).set_persistent(true);

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

  // Add faces
  int a = 11;
  int b = 10;
  int c = 0;
  int d = 0;
  int e = 1;
  int f = 11;
  int count = 0;

  while (count < 81)  
  {
  mesh.add_face(vhandle[a], vhandle[b], vhandle[c]);
  mesh.add_face(vhandle[d], vhandle[e], vhandle[f]);
  count++;
 if (count % 9 == 0)
  {
 
	 a = a + 2;
	 b = b + 2;
	 c = c + 2;
	 d = d + 2;
	 e = e + 2;
	 f = f + 2;
  }

 else 
  {
	 a = a + 1;
	 b = b + 1;
	 c = c + 1;
	 d = d + 1;
	 e = e + 1;
	 f = f + 1;
  }

 }

for (int xx = 0; xx < nintervals; ++xx)
{}

int xx = 0;
mesh.request_face_colors();
 for (MyMesh::FaceIter f_it=mesh.faces_begin();  f_it !=mesh.faces_end(); ++f_it)
 {
   xx = xx + 1;	
  mesh.property(fprop_float, *f_it) = p[xx]*nstars/nrolls;
  cout << "The value of face(" << *f_it << ") is: " << mesh.property(fprop_float, *f_it) << endl; 
 
	if(xx <= 16)
        	{ mesh.set_color(*f_it,MyMesh::Color(255,0,0));}
	if(xx > 16 && xx <= 32)
		{ mesh.set_color(*f_it,MyMesh::Color(227,0,28));}
	if(xx >32 && xx <= 48)
		{ mesh.set_color(*f_it,MyMesh::Color(198,0,57));}
	if(xx > 48 && xx <= 64)
		{ mesh.set_color(*f_it,MyMesh::Color(170,0,85));}
	if(xx > 64 && xx <= 80)
		{ mesh.set_color(*f_it,MyMesh::Color(142,0,113));}
	if(xx > 80 && xx <= 96)
		{ mesh.set_color(*f_it,MyMesh::Color(113,0,142));}
	if(xx > 96 && xx <= 112)
		{ mesh.set_color(*f_it,MyMesh::Color(85,0,170));}
	if(xx > 112 && xx <= 128)
		{ mesh.set_color(*f_it,MyMesh::Color(57,0,198));}
	if(xx > 128 && xx <= 144)
		{ mesh.set_color(*f_it,MyMesh::Color(28,0,227));}
	if(xx >144)
		{mesh.set_color(*f_it,MyMesh::Color(0, 0 , 255));}



	// if (mesh.property(fprop_float, *f_it) > 2)
	  // {
 	//		 mesh.set_color(*f_it, MyMesh::Color(255,0,0));
	//   }
  }

if (!mesh.get_property_handle(fprop_float, "fprop_float") )
{ cout << "ERROR" << endl;
	
}


cout << "There are " << mesh.n_faces() << " faces in the mesh." << endl;
cout << "There are " << mesh.n_vertices() << " vertices in the mesh." << endl;

OpenMesh::IO::Options wopt;
wopt = OpenMesh::IO::Options::FaceColor;

  try
  {
    if ( !OpenMesh::IO::write_mesh(mesh, "output.off", wopt) )
    {
      std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
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
