#pragma once

#include <QString>

namespace sdfRay4d
{
  /**
   * @struct Constants
   * @brief This struct serves as a static data
   * container for the entire lifecycle of the application
   *
   * @note decided not to use constexpr for two reasons:
   * - can't use string for constexpr
   * - changing constexpr data here will trigger a fresh new hash to re-compile
   * the entire application from scratch and therefore makes it slow
   *
   * TODO:
   * Should replace this with a json-based manifest file
   * to be consumed by JSONParser, than consuming runtime memory
   * for constants
   */
   struct Constants
   {
     using str = QString;

     str assetsPath   = "assets/";

     // TODO: make these available
     str shadersRoot  = "shaders/";
     str modelsRoot   = "models/";
     str texturesRoot = "textures/";

     /**
      * @struct Models
      */
     struct
     {
       str actor = "block.buf";
     } modelsPaths;

     /**
      * @struct Shaders
      */
     struct
     {
       /**
        * @struct Depth Shaders
        */
       struct
       {
         struct
         {
           str main = "Depth/depth_pass.vert";
         } vert;

         struct
         {
           str main = "Depth/depth_pass.frag";
         } frag;
       } depth;

       /**
        * @struct ACtor Shaders
        */
       struct
       {
         struct
         {
           str main = "Actor/color_phong.vert";
         } vert;

         struct
         {
           str main = "Actor/color_phong.frag";
         } frag;
       } actor;

       /**
        * @struct SDF Raymarch Shaders
        */
       struct
       {
         struct
         {
           str main = "Raymarch/static/sdfr_pass.vert";
           str mainSPV = "Raymarch/static/sdfr_pass.vert.spv";
         } vert;

         struct
         {
           str main = "Raymarch/dynamic/sdfr_pass.frag";
           str mainSPV = "Raymarch/static/sdfr_pass.frag.spv";

           struct
           {
             str distanceFuncs = "Raymarch/_partials/distance_functions.partial.glsl";
             str operations = "Raymarch/_partials/operations.partial.glsl";
           } partials;
         } frag;
       } raymarch;
     } shadersPaths;
   };
}
