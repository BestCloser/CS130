#include "driver_state.h"
#include <cstring>
#include <cfloat>

using namespace std;

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
    state.image_width = width;
    state.image_height = height;
    state.image_color = 0;
    state.image_depth = 0;

    //std::cout<<"TODO: allocate and initialize state.image_color and state.image_depth."<<std::endl;
    
    state.image_color = new pixel[width * height];    
    state.image_depth = new float[width * height];

    for (int i = 0; i < (width * height); i++) {
      state.image_color[i] = make_pixel(0, 0, 0);
      state.image_depth[i] = 1;
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
    switch(type) {
        case render_type::triangle:
        {
            for (int i = 0; i < state.num_vertices; i += 3) { 
                data_geometry** triangle = new data_geometry*[3];
                for (int j = 0; j < 3; j++) { 
                    triangle[j] = new data_geometry;
                    data_vertex v;
                    v.data = new float[MAX_FLOATS_PER_VERTEX];
                    triangle[j]->data = new float[MAX_FLOATS_PER_VERTEX];
                    for (int k = 0; k < state.floats_per_vertex; k++) {
                        v.data[k] = state.vertex_data[k + state.floats_per_vertex*(i+j)];
                        triangle[j]->data[k] = v.data[k];
                    }
                    state.vertex_shader((const data_vertex)v, *triangle[j], state.uniform_data);
                }

                clip_triangle(state, (const data_geometry**)triangle, 0);
            }
        }
        break;

        case render_type::indexed: 
        {
            const data_geometry *out[3];
            data_geometry triangle[3];
            data_vertex v[3];

            for (int i = 0; i < state.num_triangles * 3; i += 3) {
                for(int j = 0; j < 3; j++) {
                    v[j].data = &state.vertex_data[state.index_data[i + j] * state.floats_per_vertex];
                    triangle[j].data = v[j].data;
                    state.vertex_shader(v[j], triangle[j], state.uniform_data);
                    out[j] = &triangle[j];
                }
                clip_triangle(state, out, 0);
            }
        }
        break;

        case render_type::fan: 
        {
            const data_geometry *out[3];
            data_geometry triangle[3];
            data_vertex v[3];
            int flag;

            for (int i = 0; i < state.num_vertices; i++) {
                for (int j = 0; j < 3; j++) {
                    flag = i + j;
                    if (j == 0) { 
                        flag = 0; 
                    }

                    v[j].data = &state.vertex_data[flag * state.floats_per_vertex];
                    triangle[j].data = v[j].data;
                    state.vertex_shader(v[j], triangle[j], state.uniform_data);
                    out[j] = &triangle[j];
                }
                clip_triangle(state, out, 0);
            }
        }
        break;

        case render_type::strip: 
        {
            const data_geometry *out[3];
            data_geometry triangle[3];
            data_vertex v[3];

            for (int i = 0; i < state.num_vertices - 2; i++) {
                for (int j = 0; j < 3; j++) {
                    v[j].data = &state.vertex_data[(i + j) * state.floats_per_vertex];
                    triangle[j].data = v[j].data;
                    state.vertex_shader(v[j], triangle[j], state.uniform_data);
                    out[j] = &triangle[j];
                }
                clip_triangle(state, out, 0);
            }
        }
        break;

        default: 
        break;
    }
}


// This function clips a triangle (defined by the three vertices in the "in" array).
// It will be called recursively, once for each clipping face (face=0, 1, ..., 5) to
// clip against each of the clipping faces in turn.  When face=6, clip_triangle should
// simply pass the call on to rasterize_triangle.
void clip_triangle(driver_state& state, const data_geometry* in[3],int face)
{

    if (face == 1) {
        rasterize_triangle(state, in);
        return;
    }

    // std::cout << "TODO: implement clipping. (The current code passes the triangle through without clipping them.)" << std::endl;
    vec4 a = in[0]->gl_Position; 
    vec4 b = in[1]->gl_Position; 
    vec4 c = in[2]->gl_Position;

    const data_geometry *in1[3] = {in[0], in[1], in[2]};
    data_geometry d1[3], d2[3];
    float a_1, b_1, b_2;
    vec4 p_1, p_2;
 
    if ((a[2] < -a[3]) && (b[2] < -b[3]) && (c[2] < -c[3])) {
        return;
    }

    else {
        if ((a[2] < -a[3]) && (b[2] >= -b[3]) && (c[2] >= -c[3])) {
            b_1 = (-b[3] - b[2]) / (a[2] + a[3] - b[3] - b[2]);
            b_2 = (-a[3] - a[2]) / (c[2] + c[3] - a[3] - a[2]);

            p_1 = b_1 * a + (1 - b_1) * b;
            p_2 = b_2 * c + (1 - b_2) * a;

            d1[0].data = new float[state.floats_per_vertex];
            d1[1] = *in[1];
            d1[2] = *in[2];

            for (int i = 0; i < state.floats_per_vertex; ++i) {
                switch (state.interp_rules[i]) {
                    case interp_type::flat:
                        d1[0].data[i] = in[0]->data[i];
                    break;

                    case interp_type::smooth:
                        d1[0].data[i] = b_2 * in[2]->data[i] + (1 - b_2) * in[0]->data[i];
                    break;

                    case interp_type::noperspective:
                        a_1 = b_2 * in[2]->gl_Position[3] / (b_2 * in[2]->gl_Position[3] + (1 - b_2) * in[0]->gl_Position[3]);
                        d1[0].data[i] = a_1 * in[2]->data[i] + (1 - a_1) * in[0]->data[i];
                    break;

                    default:
                    break;
                }
            }
            d1[0].gl_Position = p_2;
            in1[0] = &d1[0];
            in1[1] = &d1[1];
            in1[2] = &d1[2];
            
            clip_triangle(state, in1, face + 1);

            d2[0].data = new float[state.floats_per_vertex];
            d2[2] = *in[2];

            for (int i = 0; i < state.floats_per_vertex; ++i) {
                switch (state.interp_rules[i]) {
                    case interp_type::flat:
                        d2[0].data[i] = in[0]->data[i];
                    break;

                    case interp_type::smooth:
                        d2[0].data[i] = b_1 * in[0]->data[i] + (1 - b_1) * in[1]->data[i];
                    break;

                    case interp_type::noperspective:
                        a_1 = b_1 * in[0]->gl_Position[3] / (b_1 * in[0]->gl_Position[3] + (1 - b_1) * in[1]->gl_Position[3]);
                        d2[0].data[i] = a_1 * in[0]->data[i] + (1 - a_1) * in[1]->data[i];
                    break;

                    default:
                    break;
                }
            }
            d2[0].gl_Position = p_1;
            in1[0] = &d2[0];
            in1[1] = &d1[1];
            in1[2] = &d1[0];
        }

    clip_triangle(state, in1, face + 1);
    }
}

// Rasterize the triangle defined by the three vertices in the "in" array.  This
// function is responsible for rasterization, interpolation of data to
// fragments, calling the fragment shader, and z-buffering.
void rasterize_triangle(driver_state& state, const data_geometry* in[3])
{
    //std::cout<<"TODO: implement rasterization"<<std::endl;

    int image_index;
    float ax, ay, bx, by, cx, cy;
    float area_abc, area_pbc, area_apc, area_abp;
    float alpha, beta, gamma;
    float min_x, min_y, max_x, max_y;
    float alpha_p, beta_p, gamma_p;

    
    ax = (state.image_width/2.0) * (in[0]->gl_Position[0]/in[0]->gl_Position[3]) + (state.image_width/2.0) - (0.5);
    ay = (state.image_height/2.0) * (in[0]->gl_Position[1]/in[0]->gl_Position[3]) + (state.image_height/2.0) - (0.5);

    bx = (state.image_width/2.0) * (in[1]->gl_Position[0]/in[1]->gl_Position[3]) + (state.image_width/2.0) - (0.5);
    by = (state.image_height/2.0) * (in[1]->gl_Position[1]/in[1]->gl_Position[3]) + (state.image_height/2.0) - (0.5);

    cx = (state.image_width/2.0) * (in[2]->gl_Position[0]/in[2]->gl_Position[3]) + (state.image_width/2.0) - (0.5);
    cy = (state.image_height/2.0) * (in[2]->gl_Position[1]/in[2]->gl_Position[3]) + (state.image_height/2.0) - (0.5);

    min_x = std::min(ax, std::min(bx, cx));
    min_y = std::min(ay, std::min(by, cy));
    max_x = std::max(ax, std::max(bx, cx));
    max_y = std::max(ay, std::max(by, cy));

    area_abc = 0.5 * ((bx * cy - cx * by) - (ax * cy - cx * ay) + (ax * by - bx * ay));
   
    if (min_x < 0) {
        min_x = 0;
    }

    if (min_y < 0) {
        min_y = 0;
    }

    if (max_x > state.image_width) {
        max_x = state.image_width;
    }

    if (max_y > state.image_height) {
        max_y = state.image_height;
    }

    for (int j = min_y; j < max_y; j++) {
        for (int i = min_x; i < max_x; i++) {
            image_index = i + j * state.image_width;
            area_pbc = 0.5 * ((bx * cy - cx * by) - (i * cy - j * cx) + (i * by - j * bx));
            area_apc = 0.5 * ((i * cy - j * cx) - (ax * cy - cx * ay) + (j * ax - i * ay));
            area_abp = 0.5 * ((j * bx - i * by) - (j * ax - i * ay) + (ax * by - bx * ay));      

            alpha = area_pbc/area_abc;
            beta = area_apc/area_abc;
            gamma = area_abp/area_abc;

            if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                data_fragment frag;
                frag.data = new float[MAX_FLOATS_PER_VERTEX];
                data_output output;
                //z buffering
                float depth1 = alpha * in[0]->gl_Position[2]/in[0]->gl_Position[3] + beta * in[1]->gl_Position[2]/in[1]->gl_Position[3] + gamma * in[2]->gl_Position[2]/in[2]->gl_Position[3];

                if (state.image_depth[image_index] > depth1) {
                    for (int k = 0; k < state.floats_per_vertex; k++) {
                        float k_gour;
                        switch (state.interp_rules[k]) {
                            case interp_type::flat:
                                frag.data[k] = in[0]->data[k];
                            break;

                            case interp_type::smooth:
                                k_gour = (alpha/in[0]->gl_Position[3] + beta/in[1]->gl_Position[3] + gamma/in[2]->gl_Position[3]);

                                alpha_p = alpha/k_gour/(in[0]->gl_Position[3]);
                                beta_p = beta/k_gour/(in[1]->gl_Position[3]);
                                gamma_p = gamma/k_gour/(in[2]->gl_Position[3]);

                                frag.data[k] = alpha_p * in[0]->data[k] + beta_p * in[1]->data[k] + gamma_p * in[2]-> data[k];
                            break;

                            case interp_type::noperspective:
                                frag.data[k] = alpha * in[0]->data[k] + beta * in[1]->data[k] + gamma * in[2]->data[k];

                            break;

                            default:
                            break;
                        }
                    }
                    state.fragment_shader(frag, output, state.uniform_data);
                    output.output_color = output.output_color * 255;
                    state.image_color[image_index] = make_pixel(output.output_color[0], output.output_color[1], output.output_color[2]);
                    state.image_depth[image_index] = depth1;
                }
            }
        }
    }
}

