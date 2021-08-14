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
   * to be consumed by JSONParser
   */
   struct Constants
   {
     using str = const QString;

     str assetsPath   = "../assets/";

     // TODO: make these available
     str shadersRoot  = "shaders/";
     str modelsRoot   = "models/";
     str texturesRoot = "textures/";

     struct
     {
       str actor = "block.buf";
     } modelsPaths;

     struct
     {
       struct
       {
         struct
         {
           str main = "Raymarch/static/fullscreentri.vert";
         } vert;

         struct
         {
           str main = "Raymarch/dynamic/rtprimitives.frag";

           struct
           {
             str distanceFuncs = "Raymarch/_partials/distance_functions.partial.glsl";
             str operations = "Raymarch/_partials/operations.partial.glsl";
           } partials;
         } frag;
       } raymarch;

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
     } shadersPaths;
   };
}
