//
// Created by Changki Lee on 2020-12-14.
//

#include "augmented_face_renderer.h"
#include "util.h"

namespace hello_ar {
    namespace {
        const glm::vec4 kLightDirection(0.0f, 1.0f, 0.0f, 0.0f);
        constexpr char kVertexShaderFilename[] = "shaders/object.vert";
        constexpr char kFragmentShaderFilename[] = "shaders/object.frag";
    }  // namespace

    void AugmentedFaceRenderer::InitializeGlContent(AAssetManager* asset_manager) {
        face.InitializeGlContent(asset_manager, "models/freckles.png");
        nose.InitializeGlContent(
                asset_manager, "models/nose.obj", "models/nose_fur.png");
        right_ear.InitializeGlContent(
                asset_manager, "models/forehead_right.obj", "models/ear_fur.png");
        left_ear.InitializeGlContent(
                asset_manager, "models/forehead_left.obj", "models/ear_fur.png");

        face.SetMaterialProperty(0.0f, 1.0f, 0.1f, 6.0f);
        nose.SetMaterialProperty(0.0f, 1.0f, 0.1f, 6.0f);
        right_ear.SetMaterialProperty(0.0f, 1.0f, 0.1f, 6.0f);
        left_ear.SetMaterialProperty(0.0f, 1.0f, 0.1f, 6.0f);
    }

    void AugmentedFaceRenderer::Draw(const glm::mat4& projection_mat,
                                     const glm::mat4& view_mat,
                                     const glm::mat4& model_mat,
                                     const float* color_correction4,
                                     const float* color_tint_rgba,
                                     const ArSession* ar_session,
                                     const ArAugmentedFace_* ar_face) const {
        face.Draw(projection_mat, view_mat, model_mat, color_correction4, color_tint_rgba, ar_session, ar_face);

        glm::mat4 nose_mat = util::GetRegionPoseFromFace(ar_session, ar_face, AR_AUGMENTED_FACE_REGION_NOSE_TIP);
        nose.Draw(projection_mat, view_mat, model_mat*nose_mat, color_correction4, color_tint_rgba);

        glm::mat4 left_ear_mat = util::GetRegionPoseFromFace(ar_session, ar_face, AR_AUGMENTED_FACE_REGION_FOREHEAD_LEFT);
        left_ear.Draw(projection_mat, view_mat, model_mat*left_ear_mat, color_correction4, color_tint_rgba);

        glm::mat4 right_ear_mat = util::GetRegionPoseFromFace(ar_session, ar_face, AR_AUGMENTED_FACE_REGION_FOREHEAD_RIGHT);
        right_ear.Draw(projection_mat, view_mat, model_mat*right_ear_mat, color_correction4, color_tint_rgba);
    }
}  // namespace augmented_image