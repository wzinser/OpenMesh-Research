# Modeling with OpenMesh

This program provides an example of how to create a mesh and use it to visualize data. A simple first order ordinary differential equation is solved using the ODEint C++ library and displayed on a user defined mesh created from OpenMesh C++ libary. The purpose of this is to serve as a starting point to use OpenMesh as a data structure for more complex projects and further research.   

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Operating System

Ubuntu 18.04 LTS (Virtual Machine)

### Prerequisites

- GNU G++ Compiler
```
$ sudo apt install g++
```
- C++ Boost Libaries
```
sudo apt-get install libboost-all-dev
```
### Installing

Run OpenMesh download script to download all libraries and build the most current version. Note: running the script after installation will prompt to uninstall.

```
./OpenMesh_Download
```
Download ODEint libary

```
$ git clone git://github.com/headmyshoulder/odeint-v2
```

## Running
Once the libraries have been compiled the next step is to create your working directory that will hold your project. 

- Go to OpenMeshRoot/src/OpenMesh/Apps and create a new directory, say "MyOwnProject"
- Now create a new file called "CMakeLists.txt" containing the following lines:
```
include (ACGCommon)
include_directories (
  ../../..
  ${CMAKE_CURRENT_SOURCE_DIR}
)
set (targetName MyOwnProject)
# collect all header and source files
acg_append_files (headers "*.hh" .)
acg_append_files (sources "*.cc" .)
acg_add_executable (${targetName} ${headers} ${sources})
target_link_libraries (${targetName}
  OpenMeshCore
  OpenMeshTools
)

```
- Add sourcefiles to new project directory
- Add the following lines to OpenMeshRoot/src/OpenMesh/Apps/CMakeLists.txt

```
add_subdirectory (MyOwnProject) 
```

- Navigate to the new directory and run the following commands

```
cmake ..
make
```

- Your project will now be built in your binaries folder and can be ran using

```
./MyOwnProject
```

- The default output wil be a object file containg information on points,faces,etc. This file can also be viewed in many 3rd party mesh editing software such as Meshlab.

## Examples

### Building a Mesh

This example builds a simple square mesh and subdivides it using the LoopT subdivision tool

```
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
```
![snapshot00](https://user-images.githubusercontent.com/47840209/55462561-ecac6380-55ab-11e9-86d4-930d30bb18ae.png)



### Iterating over mesh
Iteration over faces,vertices,edges,etc. is possible using the following loop.
```
 for (MyMesh::FaceIter f_it=mesh.faces_begin();  f_it !=mesh.faces_end(); ++f_it)
 {
   // do something
 }
```

### Adding Data to Mesh
Properties can be added to mesh elements (faces,vertices, edges, halfedges) such as boolean,strings, standard c++ datatypes. This following examples adds a random floating number into each face of the mesh using iterators.
```
OpenMesh::FPropHandleT<float> fprop_float;
mesh.add_property(fprop_float, "fprop_float");
mesh.property(fprop_float).set_persistent(true);

// Generate random number between 1-10
random_device device;
mt19937 rng(device());
uniform_int_distribution<std::mt19937::result_type> dist6(1,10);

for (MyMesh::FaceIter f_it=mesh.faces_begin();  f_it !=mesh.faces_end(); ++f_it)
 {
  mesh.property(fprop_float, *f_it) = dist6(rng);
 }
```

### ODE Solver

The ODE solver uses the boost ODEint library. A simple example is below solving the Lokta-Voltera equations (predator/prey). More examples and different integration methods can be found in their documentation.
```
//Declare Constants
const double sigma = 1.1;
const double beta = 0.4;
const double delta = 0.1;
const double phi = 0.4;
typedef boost::array< double , 2 > state_type;

//Setup Equation
void ODE( const state_type &x , state_type &dxdt , double t )
{
    dxdt[0] = (sigma * x[0]) - (beta*x[0]*x[1]);
    dxdt[1] = (delta * x[0] * x[1]) - (phi * x[1]);
}

//Pull results into an array during iteration
double prey_count[100];
double pred_count[100];
int i = 0;
tuple<double, double> write_ODE( const state_type &x , const double t )
{ 
    prey_count[i] = x[0];
    pred_count[i] = x[1];
    cout << t << '\t' << x[0] << '\t' << x[1] << '\t' << prey_count[i] << endl;
    i++;
    return std::make_tuple(pred_count[i], prey_count[i]);
}

//Solve ODE
state_type x = { 30 , 30}; // initial conditions
integrate(ODE, x, 0.0, 100.0, 0.01, write_ODE);
```
## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Resources

https://www.openmesh.org/
http://headmyshoulder.github.io/odeint-v2/index.html
http://www.meshlab.net/
