#include "driver_state.h"
#include <cstring>

driver_state::driver_state()
{
}

driver_state::~driver_state()
{
    delete [] image_color;
    delete [] image_depth;
}

// This function should allocate and initialize the arrays that store color and
// depth.  This is not done during the constructor since the width and height
// are not known when this class is constructed.
void initialize_render(driver_state& state, int width, int height)
{
    state.image_width=width;
    state.image_height=height;
    state.image_color=0;
    state.image_depth=0;
//    std::cout<<"TODO: allocate and initialize state.image_color and state.image_depth."<<std::endl;

    unsigned long total_pixels = width * height;
    state.image_color = new pixel[total_pixels];
    
    for (unsigned int i = 0; i < total_pixels; ++i) {
 		state.image_color[i] = make_pixel(0, 0, 0); //initialize the color black: rgb(0,0,0)
 	}
}

// This function will be called to render the data that has been stored in this class.
// Valid values of type are:
//   render_type::triangle - Each group of three vertices corresponds to a triangle.
//   render_type::indexed -  Each group of three indices in index_data corresponds
//                           to a triangle.  These numbers are indices into vertex_data.
//   render_type::fan -      The vertices are to be interpreted as a triangle fan.
//   render_type::strip -    The vertices are to be interpreted as a triangle strip.
void render(driver_state& state, render_type type)
{
    std::cout<<"TODO: implement rendering."<<std::endl;

    
    /* driver_state:

    float * vertex_data
    int num_vertices
    int floats_per_vertex
	
	*/

    //data_geometry = {vec4 gl_Position, float * data};

    //how to get gl_Position
    vec4 gl_Position = ???;
    float * curr = state->vertex_data;
    unsigned int i;
    //data_geometry idk = {gl_Position, data};
    
    while (curr != nullptr) {
    	data_geometry arr[3];
	    for (i = 0; i < 3; ++i) {
	    	arr[i] = {gl_Position, curr};

	    	if (curr->next != nullptr) {
	    		curr = curr->next;
	    	}
	    	else {
	    		break; //no more triangles can be made?
	    	}
	    }
	    //data_geometry arr[3] = {?, ?, ?};
	    rasterize_triangle(state, arr);
	    i = 0;
	}

}


// This function clips a triangle (defined by the three vertices in the "in" array).
// It will be called recursively, once for each clipping face (face=0, 1, ..., 5) to
// clip against each of the clipping faces in turn.  When face=6, clip_triangle should
// simply pass the call on to rasterize_triangle.
void clip_triangle(driver_state& state, const data_geometry* in[3],int face)
{
    if(face==6)
    {
        rasterize_triangle(state, in);
        return;
    }
    std::cout<<"TODO: implement clipping. (The current code passes the triangle through without clipping them.)"<<std::endl;
    clip_triangle(state,in,face+1);
}

// Rasterize the triangle defined by the three vertices in the "in" array.  This
// function is responsible for rasterization, interpolation of data to
// fragments, calling the fragment shader, and z-buffering.
void rasterize_triangle(driver_state& state, const data_geometry* in[3])
{
    std::cout<<"TODO: implement rasterization"<<std::endl;
}

