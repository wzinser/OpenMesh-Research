#include <iostream>
#include <string>
#include <cmath>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>
#include <boost/range/numeric.hpp>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/LoopT.hh>
#include <tuple>


typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;


using namespace std;
using namespace boost::numeric::odeint;

const double sigma = 1.1;
const double beta = .4;
const double delta = .1;
const double phi = .4;

typedef boost::array< double , 2 > state_type;




void ODE( const state_type &x , state_type &dxdt , double t )
{
    dxdt[0] = (sigma * x[0]) - (beta*x[0]*x[1]);
    dxdt[1] = (delta * x[0] * x[1]) - (phi * x[1]);
}



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

 

int main()
{
  MyMesh mesh;

  //Create Vertices
  MyMesh::VertexHandle vhandle[3];
  vhandle[0] = mesh.add_vertex(MyMesh::Point( 0, 0 ,  0));
  vhandle[1] = mesh.add_vertex(MyMesh::Point( 1, 0 ,  0));
  vhandle[2] = mesh.add_vertex(MyMesh::Point( 1, 1 ,  0));
  vhandle[3] = mesh.add_vertex(MyMesh::Point( 0, 1 ,  0));
 
  //Create Faces
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
 Loop( 3 );
 Loop.detach();

 // Solve ODE
 state_type x = { 30 , 30}; // initial conditions
 runge_kutta4< state_type > stepper;
 integrate(ODE, x, 0.0, 100.0, 0.01, write_ODE);
  for (int k = 0; k<i; k++)
  { cout << prey_count[k] << endl; }



// integrate_n_steps( stepper , ODE , x , 0.0, 1.0 , 50, write_ODE );


string Output;


OpenMesh::IO::Options wopt;
wopt = OpenMesh::IO::Options::FaceColor;

 mesh.request_face_colors();

for (int count = 0; count<i; count++)
{
	int count_prey = prey_count[count];
	int count_pred = pred_count[count];
	int total_count = count_prey + count_pred;
	int z = 0;
	int y = 0;

	for (MyMesh::FaceIter f_it=mesh.faces_begin();  f_it !=mesh.faces_end(); ++f_it)
	{ 

	
	      	
		if ( z < count_prey)
			{
		    		mesh.set_color(*f_it,MyMesh::Color(0,0,255));
			}
	
		else if ( (y > count_prey) && (y < total_count) )
			{
				mesh.set_color(*f_it,MyMesh::Color(255,0,0));
		
			}
	
		else
			{
		       	        mesh.set_color(*f_it,MyMesh::Color(0,0,0));
			}

			z++;
			y++;
	}




		

Output = std::to_string(count) + "output.off";
OpenMesh::IO::write_mesh(mesh, Output, wopt);

}

return 0;

}













