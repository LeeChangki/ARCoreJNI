//
// Created by Changki Lee on 2020-12-14.
//

#ifndef SAMPLER_AUGMENTED_FACE_RENDERER_H
#define SAMPLER_AUGMENTED_FACE_RENDERER_H

#include <android/asset_manager.h>

#include "arcore_c_api.h"
#include "glm.h"
#include "obj_renderer.h"
#include "face_obj_renderer.h"

namespace hello_ar {
    // AugmentedFaceRenderer handles the display of image frame on ArAugmentedImage
    class AugmentedFaceRenderer {
    public:
        AugmentedFaceRenderer() = default;
        ~AugmentedFaceRenderer() = default;

        // Sets up OpenGL state.  Must be called on the OpenGL thread and before any
        // other methods below.
        void InitializeGlContent(AAssetManager* asset_manager);

        // Draws frames
        void Draw(const glm::mat4& projection_mat,
                  const glm::mat4& view_mat,
                  const glm::mat4& model_mat,
                  const float* color_correction4,
                  const float* color_tint_rgba,
                  const ArSession* ar_session,
                  const ArAugmentedFace_* ar_face) const;
    private:
        FaceObjRenderer face;
        ObjRenderer nose;
        ObjRenderer right_ear;
        ObjRenderer left_ear;
    };

}  // namespace hello_ar
#endif //SAMPLER_AUGMENTED_FACE_RENDERER_H
