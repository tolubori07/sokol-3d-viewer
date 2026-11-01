#include "vendor/sokol/sokol_app.h"
#include "vendor/sokol/sokol_gfx.h"
#include "vendor/sokol/sokol_glue.h"

#include "./triangle_shader.h"

static struct {
  sg_pass_action pass_action;
  sg_bindings binding;
  sg_pipeline pipeline;
} state;

void init(void) {
  sg_setup(&(sg_desc){.environment = sglue_environment()});

  // 2 over lapping triangles
  /*  float vertices[] = {
        // x      y     z     r     g     b
        0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // triangle 1: top
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //             right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //             left

        0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // triangle 2: top
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, //             right
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, //             left
    };*/
  // quad
  float vertices[] = {
      // x      y     z     r     g     b
      -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // 0: top-left
      0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // 1: top-right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // 2: bottom-left
      0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // 3: bottom-right
  };
  state.binding.vertex_buffers[0] =
      sg_make_buffer(&(sg_buffer_desc){.data = SG_RANGE(vertices)});
  state.pipeline = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = sg_make_shader(triangle_shader_desc(sg_query_backend())),
      .primitive_type = SG_PRIMITIVETYPE_TRIANGLE_STRIP,
      .layout = {.attrs = {
                     [ATTR_triangle_pos].format = SG_VERTEXFORMAT_FLOAT3,
                     [ATTR_triangle_color].format = SG_VERTEXFORMAT_FLOAT3,
                 }}});
  state.pass_action =
      (sg_pass_action){.colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                                     .clear_value = {0.1, 0.1, 0.1, 1.0}}};
}

// draw frame for each pass action
void frame(void) {
  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(state.pipeline);
  sg_apply_bindings(&state.binding);
  sg_draw(0, 4, 1);
  sg_end_pass();
  sg_commit();
}

// everything must be cleaned out
void cleanup(void) { sg_shutdown(); }

void event(const sapp_event *ev) {}

// app running: passes the init, event, cleanup, and frame func to the app
// descriptor which is passed to the app
int main() {
  sapp_run(&(sapp_desc){.init_cb = init,
                        .event_cb = event,
                        .cleanup_cb = cleanup,
                        .frame_cb = frame});
}
