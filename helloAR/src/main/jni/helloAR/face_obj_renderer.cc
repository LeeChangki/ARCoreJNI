//
// Created by Changki Lee on 2020-12-14.
//

#include "face_obj_renderer.h"
#include "util.h"

namespace hello_ar {
    namespace {
        const glm::vec4 kLightDirection(0.0f, 1.0f, 0.0f, 0.0f);
        constexpr char kVertexShaderFilename[] = "shaders/object.vert";
        constexpr char kFragmentShaderFilename[] = "shaders/object.frag";
    }  // namespace

    void FaceObjRenderer::InitializeGlContent(AAssetManager *asset_manager,
                                              const std::string &png_file_name) {
        compileAndLoadShaderProgram(asset_manager);
        position_attrib_ = glGetAttribLocation(shader_program_, "a_Position");
        tex_coord_attrib_ = glGetAttribLocation(shader_program_, "a_TexCoord");
        normal_attrib_ = glGetAttribLocation(shader_program_, "a_Normal");

        // loadTexture
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (!util::LoadPngFromAssetManager(GL_TEXTURE_2D, png_file_name)) {
            LOGE("Could not load png texture for planes.");
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        util::CheckGlError("obj_renderer::InitializeGlContent()");
    }


    void FaceObjRenderer::compileAndLoadShaderProgram(AAssetManager *asset_manager) {
        shader_program_ =
                util::CreateProgram(kVertexShaderFilename, kFragmentShaderFilename, asset_manager);
        if (!shader_program_) {
            LOGE("Could not create program.");
        }

        mvp_mat_uniform_ =
                glGetUniformLocation(shader_program_, "u_ModelViewProjection");
        mv_mat_uniform_ = glGetUniformLocation(shader_program_, "u_ModelView");
        texture_uniform_ = glGetUniformLocation(shader_program_, "u_Texture");

        lighting_param_uniform_ =
                glGetUniformLocation(shader_program_, "u_LightingParameters");
        material_param_uniform_ =
                glGetUniformLocation(shader_program_, "u_MaterialParameters");
        color_correction_param_uniform_ =
                glGetUniformLocation(shader_program_, "u_ColorCorrectionParameters");
        tint_color_uniform_ = glGetUniformLocation(shader_program_, "u_TintColor");
    }

    void FaceObjRenderer::SetMaterialProperty(float ambient, float diffuse, float specular,
                                              float specular_power) {
        ambient_ = ambient;
        diffuse_ = diffuse;
        specular_ = specular;
        specular_power_ = specular_power;
    }

    void FaceObjRenderer::Draw(const glm::mat4 &projection_mat, const glm::mat4 &view_mat,
                               const glm::mat4 &model_mat, const float *color_correction4,
                               const float* color_tint_rgba,
                               const ArSession *ar_session, const ArAugmentedFace* ar_face) const {
        if (!shader_program_) {
            LOGE("shader_program is null.");
            return;
        }

        // get augmented face data
        int32_t number_of_vertices = 0;
        const float* vertices;
        ArAugmentedFace_getMeshVertices(ar_session, ar_face, &vertices, &number_of_vertices);
        if (number_of_vertices <= 0) {
            return;
        }

        int32_t number_of_normals = 0;
        const float* normals;
        ArAugmentedFace_getMeshNormals(ar_session, ar_face, &normals, &number_of_normals);
        if (number_of_normals <= 0) {
            return;
        }

        int32_t number_of_uvs = 0;
        const float* uvs;
        ArAugmentedFace_getMeshTextureCoordinates(ar_session, ar_face, &uvs, &number_of_uvs);
        if (number_of_uvs <= 0) {
            return;
        }

        int32_t number_of_indices = 0;
        const uint16_t* indices;
        ArAugmentedFace_getMeshTriangleIndices(ar_session, ar_face, &indices, &number_of_indices);
        if (number_of_indices <= 0) {
            return;
        }

        glUseProgram(shader_program_);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(texture_uniform_, 0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);

        glm::mat4 mvp_mat = projection_mat * view_mat * model_mat;
        glm::mat4 mv_mat = view_mat * model_mat;
        glm::vec4 view_light_direction = glm::normalize(mv_mat * kLightDirection);

        glUniform4f(lighting_param_uniform_, view_light_direction[0],
                    view_light_direction[1], view_light_direction[2], 1.f);
        glUniform4f(material_param_uniform_, ambient_, diffuse_, specular_, specular_power_);
        glUniform4fv(color_correction_param_uniform_, 1, color_correction4);
        glUniform4fv(tint_color_uniform_, 1, color_tint_rgba);

        glUniformMatrix4fv(mvp_mat_uniform_, 1, GL_FALSE, glm::value_ptr(mvp_mat));
        glUniformMatrix4fv(mv_mat_uniform_, 1, GL_FALSE, glm::value_ptr(mv_mat));


        // Note: for simplicity, we are uploading the model each time we draw it.  A
        // real application should use vertex buffers to upload the geometry once.
        glEnableVertexAttribArray(position_attrib_);
        glVertexAttribPointer(position_attrib_, 3, GL_FLOAT, GL_FALSE, 0,
                              vertices);

        glEnableVertexAttribArray(normal_attrib_);
        glVertexAttribPointer(normal_attrib_, 3, GL_FLOAT, GL_FALSE, 0,
                              normals);

        glEnableVertexAttribArray(tex_coord_attrib_);
        glVertexAttribPointer(tex_coord_attrib_, 2, GL_FLOAT, GL_FALSE, 0,
                              uvs);

        glDepthMask(GL_TRUE);
        glEnable(GL_BLEND);

        // Textures are loaded with premultiplied alpha
        // (https://developer.android.com/reference/android/graphics/BitmapFactory.Options#inPremultiplied),
        // so we use the premultiplied alpha blend factors.
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_SHORT, indices);

        glDisable(GL_BLEND);
        glDisableVertexAttribArray(position_attrib_);
        glDisableVertexAttribArray(tex_coord_attrib_);
        glDisableVertexAttribArray(normal_attrib_);

        glUseProgram(0);
        util::CheckGlError("obj_renderer::Draw()");
    }
}  // namespace hello_ar
