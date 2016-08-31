//
// Created by jim on 10/22/15.
//

#ifndef blockrenderer_h__
#define blockrenderer_h__


#include "sliceset.h"
#include "renderer.h"


#include <bd/graphics/shader.h>
#include <bd/graphics/texture.h>
#include <bd/volume/blockcollection.h>
#include <bd/graphics/vertexarrayobject.h>

#include <glm/fwd.hpp>

#include <memory>

namespace subvol
{

class BlockRenderer : public Renderer
{

public:
  BlockRenderer();


  //////////////////////////////////////////////////////////////////////////////
  BlockRenderer(int numSlices,
                std::shared_ptr<bd::ShaderProgram> volumeShader,
                std::shared_ptr<bd::ShaderProgram> volumeShaderLighting,
                std::shared_ptr<bd::ShaderProgram> wireframeShader,
                std::vector<bd::Block*>* blocks,
                std::shared_ptr<bd::VertexArrayObject> blocksVAO,
                std::shared_ptr<bd::VertexArrayObject> bboxVAO,
                std::shared_ptr<bd::VertexArrayObject> axisVao);


  virtual ~BlockRenderer();
private:
  bool init();
public:
  /// \brief Set the transfer function texture.
  void setColorMapTexture(bd::Texture const &tfunc);

  void setColorMapScaleValue(float val);

  void setShaderNShiney(float n);
  void setShaderLightPos(glm::vec3 const &L);
//  void setShaderViewVec(glm::vec3 const &V);
  void setShaderMaterial(glm::vec3 const &M);
  void setBackgroundColor(glm::vec3 const &c);
  void setShouldUseLighting(bool b);
  bool getShouldUseLighting() const;

  void shouldDrawNonEmptyBoundingBoxes(bool b);

  /// \brief Draw each non-empty block.
  void draw();

  /// \brief Draw wireframe bounding boxes around the blocks.
  /// \param[in] vp View projection matrix.
  void drawNonEmptyBoundingBoxes();


private:

  //////////////////////////////////////////////////////////////////////////////
  /// \brief Disable GL_DEPTH_TEST and draw transparent slices
  void drawSlices(int baseVertex);

  void drawAxis();

  //////////////////////////////////////////////////////////////////////////////
  /// \brief Loop through the blocks and draw each one
  void drawNonEmptyBlocks_Forward();

//  void setInitialGLState();

  //////////////////////////////////////////////////////////////////////////////
  /// \brief Determine the viewing direction and draw the blocks in proper
  ///        order.


  //////////////////////////////////////////////////////////////////////////////
  /// \brief Compute the base vertex offset for the slices vertex buffer based
  ///        off the largest component of \c viewdir.
  int computeBaseVertexFromViewDir(glm::vec3 const &viewdir);


private:

  int m_numSlicesPerBlock;            ///< Number of slices per block
  float m_tfuncScaleValue;            ///< Transfer function scaling value
  bool m_drawNonEmptyBoundingBoxes;   ///< True to draw bounding boxes.
  bool m_shouldUseLighting;           ///< True to use Phong lighting shader.
  glm::vec3 m_backgroundColor;        ///< Current background color.

  bd::ShaderProgram *m_currentShader;

  SliceSet m_selectedSliceSet;
  std::shared_ptr<bd::ShaderProgram> m_volumeShader;
  std::shared_ptr<bd::ShaderProgram> m_volumeShaderLighting;
  std::shared_ptr<bd::ShaderProgram> m_wireframeShader;
  std::vector<bd::Block*> *m_blocks;
  bd::Texture const* m_colorMapTexture; ///< Transfer function texture
  std::shared_ptr<bd::VertexArrayObject> m_quadsVao;    ///< Quad geometry verts
  std::shared_ptr<bd::VertexArrayObject> m_boxesVao;    ///< bounding box wireframe verts
  std::shared_ptr<bd::VertexArrayObject> m_axisVao;



};

} // namepsace subvol

#endif // blockrenderer_h__

