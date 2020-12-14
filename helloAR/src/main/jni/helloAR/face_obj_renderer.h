//
// Created by Changki Lee on 2020-12-14.
//

#ifndef C_ARCORE_FACE_OBJ_RENDERER_H
#define C_ARCORE_FACE_OBJ_RENDERER_H
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include "arcore_c_api.h"
#include "glm.h"

namespace hello_ar {
    class FaceObjRenderer {
    public:
        FaceObjRenderer() = default;
        ~FaceObjRenderer() = default;

        // Loads the OBJ file and texture and sets up OpenGL resources used to draw
        // the model.  Must be called on the OpenGL thread prior to any other calls.
        void InitializeGlContent(AAssetManager* asset_manager,
                                 const std::string& png_file_name);

        // Sets the surface's lighting reflectace properties.  Diffuse is modulated by
        // the texture's color.
        void SetMaterialProperty(float ambient, float diffuse, float specular,
                                 float specular_power);

        // Draws frames on ArAugmentedFace
        void Draw(const glm::mat4& projection_mat,
                  const glm::mat4& view_mat,
                  const glm::mat4& model_mat,
                  const float* color_correction4,
                  const float* color_tint_rgba,
                  const ArSession* ar_session,
                  const ArAugmentedFace_* ar_face) const;

    private:
        void compileAndLoadShaderProgram(AAssetManager* asset_manager);

        float ambient_ = 0.3f;
        float diffuse_ = 1.0f;
        float specular_ = 1.0f;
        float specular_power_ = 6.0f;

        // Loaded TEXTURE_2D object name
        GLuint texture_id_;

        // Shader program details
        GLuint shader_program_;
        GLint position_attrib_;
        GLint tex_coord_attrib_;
        GLint normal_attrib_;
        GLint mvp_mat_uniform_;
        GLint mv_mat_uniform_;
        GLint texture_uniform_;
        GLint lighting_param_uniform_;
        GLint material_param_uniform_;
        GLint color_correction_param_uniform_;
        GLint tint_color_uniform_;
    };
}  // namespace hello_ar

#endif //C_ARCORE_FACE_OBJ_RENDERER_H
