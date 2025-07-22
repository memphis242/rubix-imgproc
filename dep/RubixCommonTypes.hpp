#pragma once

#include <map>
#include <array>

namespace Rubix
{
   enum class Face
   {
      Front,
      Left,
      Right,
      Top,
      Bottom,
      Back,
      NUM_OF_FACES
   };
   inline const size_t NumOfFaces = static_cast<size_t>(Face::NUM_OF_FACES);

   enum class Color
   {
      Red,
      Blue,
      Green,
      White,
      Yellow,
      Orange,
      NUM_OF_COLORS
   };
   inline const size_t NumOfColors = static_cast<size_t>(Color::NUM_OF_COLORS);

   const std::map<Face, Color> StandardCubeColorMap =
   {
      { Face::Front,  Color::Red    },
      { Face::Left,   Color::Blue   },
      { Face::Right,  Color::Green  },
      { Face::Top,    Color::White  },
      { Face::Bottom, Color::Yellow },
      { Face::Back,   Color::Orange }
   };

   using FaceMx = std::array< std::array<Color, 3>, 3 >;
   using CubeMx = std::map< Face, FaceMx >;
}
