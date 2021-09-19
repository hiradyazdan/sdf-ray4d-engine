#pragma once

/**
 * @namespace Constants
 * @brief This namespace serves as a static data
 * container for the entire lifecycle of the application
 *
 * TODO: Replace this with a json-based manifest file
 */
namespace sdfRay4d::Constants
{
  constexpr auto autoCompileInterval  = 350; // milliseconds

  constexpr auto shaderVersion        = 450;
  constexpr auto shaderTmpl           = "/* ------ PLACEHOLDER (DO NOT CHANGE) ------ */";

  constexpr auto shadersPath          = "assets/shaders/";
  constexpr auto modelsPath           = "assets/models/";
  constexpr auto texturesPath         = "assets/textures/";

  /**
   * @namespace Models
   */
  namespace modelsPaths
  {
    constexpr auto actor = "block.buf";
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
        constexpr auto main = "Depth/depth_pass.vert";
        constexpr auto mainSPV = "Depth/depth_pass.vert.spv";
      }

      namespace frag
      {
        constexpr auto main = "Depth/depth_pass.frag";
        constexpr auto mainSPV = "Depth/depth_pass.frag.spv";
      }
    }

    /**
     * @namespace ACtor Shaders
     */
    namespace actor
    {
      namespace vert
      {
        constexpr auto main = "Actor/rasterized_mesh_pass.vert";
        constexpr auto mainSPV = "Actor/rasterized_mesh_pass.vert.spv";
      }

      namespace frag
      {
        constexpr auto main = "Actor/rasterized_mesh_pass.frag";
        constexpr auto mainSPV = "Actor/rasterized_mesh_pass.frag.spv";
      }
    }

    /**
     * @namespace SDF Raymarch Shaders
     */
    namespace raymarch
    {
      namespace vert
      {
        constexpr auto main = "Raymarch/static/sdfr_pass.vert";
        constexpr auto mainSPV = "Raymarch/static/sdfr_pass.vert.spv";
      }

      namespace frag
      {
        constexpr auto main = "Raymarch/dynamic/sdfr_pass.frag";
        constexpr auto mainSPV = "Raymarch/static/sdfr_pass.frag.spv";

        namespace partials
        {
          constexpr auto distanceFuncs = "Raymarch/_partials/distance_functions.partial.glsl";
          constexpr auto operations = "Raymarch/_partials/operations.partial.glsl";
        }
      }
    }
  }
}
