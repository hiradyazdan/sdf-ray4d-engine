#pragma once

/**
 * @namespace constants
 * @brief This namespace serves as a static data
 * container for the entire lifecycle of the application
 *
 * @todo Replace this with a json-based manifest file
 */
namespace sdfRay4d::constants
{
  static constexpr const auto autoCompileInterval = 350; // milliseconds

  static constexpr const auto shaderVersion       = 450;
  static constexpr const auto shaderTmpl    = "/* ------ PLACEHOLDER (DO NOT CHANGE) ------ */";

  static constexpr const auto shadersPath   = "assets/shaders/";
  static constexpr const auto modelsPath    = "assets/models/";
  static constexpr const auto texturesPath  = "assets/textures/";

  /**
   * @namespace Models
   */
  namespace modelsPaths
  {
    static constexpr const auto actor = "block.buf";
  }

  /**
   * @namespace Shaders
   */
  namespace shadersPaths
  {
    /**
     * @namespace Depth Shaders
     */
    namespace depth
    {
      namespace vert
      {
        static constexpr const auto main = "Depth/depth_pass.vert";
        static constexpr const auto mainSPV = "Depth/depth_pass.vert.spv";
      }

      namespace frag
      {
        static constexpr const auto main = "Depth/depth_pass.frag";
        static constexpr const auto mainSPV = "Depth/depth_pass.frag.spv";
      }
    }

    /**
     * @namespace ACtor Shaders
     */
    namespace actor
    {
      namespace vert
      {
        static constexpr const auto main = "Actor/rasterized_mesh_pass.vert";
        static constexpr const auto mainSPV = "Actor/rasterized_mesh_pass.vert.spv";
      }

      namespace frag
      {
        static constexpr const auto main = "Actor/rasterized_mesh_pass.frag";
        static constexpr const auto mainSPV = "Actor/rasterized_mesh_pass.frag.spv";
      }
    }

    /**
     * @namespace SDF Raymarch Shaders
     */
    namespace raymarch
    {
      namespace vert
      {
        static constexpr const auto main = "Raymarch/static/sdfr_pass.vert";
        static constexpr const auto mainSPV = "Raymarch/static/sdfr_pass.vert.spv";
      }

      namespace frag
      {
        static constexpr const auto main = "Raymarch/dynamic/sdfr_pass.frag";
        static constexpr const auto mainSPV = "Raymarch/static/sdfr_pass.frag.spv";

        namespace partials
        {
          static constexpr const auto distanceFuncs = "Raymarch/_partials/distance_functions.partial.glsl";
          static constexpr const auto operations = "Raymarch/_partials/operations.partial.glsl";
        }
      }
    }
  }
}
