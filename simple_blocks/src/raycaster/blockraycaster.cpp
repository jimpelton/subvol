//
// Created by jim on 2/16/19.
//

#include "blockraycaster.h"
#include "constants.h"

#include <bd/graphics/shader.h>
#include <bd/graphics/vertexarrayobject.h>
#include <bd/geo/mesh.h>
#include <bd/log/gl_log.h>

#include <GL/glew.h>

#include <memory>

namespace subvol
{
namespace render
{

BlockingRaycaster::BlockingRaycaster(std::unique_ptr<subvol::BlockCollection> bc)
  : m_cube {
    {
       -1.0f, -1.0f, 1.0f,
       1.0f, -1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, 1.0f, -1.0f,
       -1.0f, 1.0f, -1.0f,
    }, 3,
    {
       // front
       0, 1, 2,
       0, 2, 3,
       // right
       1, 5, 6,
       1, 6, 2,
       // back
       5, 4, 7,
       5, 7, 6,
       // left
       4, 0, 3,
       4, 3, 7,
       // top
       2, 6, 7,
       2, 7, 3,
       // bottom
       4, 5, 1,
       4, 1, 0,
    } },
    m_blockCollection{ std::move(bc) }
{
}


///////////////////////////////////////////////////////////////////////////////
BlockingRaycaster::~BlockingRaycaster() noexcept
{

}


///////////////////////////////////////////////////////////////////////////////
bool
BlockingRaycaster::initialize()
{
  initShaders();
}


///////////////////////////////////////////////////////////////////////////////
void
BlockingRaycaster::draw()
{

}


///////////////////////////////////////////////////////////////////////////////
void
BlockingRaycaster::drawNonEmptyBoundingBoxes()
{

}


///////////////////////////////////////////////////////////////////////////////
void
BlockingRaycaster::drawAxis()
{

}


///////////////////////////////////////////////////////////////////////////////
void
BlockingRaycaster::drawNonEmptyBlocks()
{
  std::vector<bd::Block*> non_empties{ m_blockCollection->getNonEmptyBlocks() };
  for (auto &b : non_empties) {
    if (b->status() & bd::Block::GPU_RES) {
      setWorldMatrix(b->transform());
      b->texture()->bind(BLOCK_TEXTURE_UNIT);
      gl_check(glBindSampler(m_volumeSampler, BLOCK_TEXTURE_UNIT));
      m_alphaBlending->setUniform(VOLUME_MVP_MATRIX_UNIFORM_STR, getWorldViewProjectionMatrix());
      m_cube.draw();
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
void
BlockingRaycaster::setUniforms()
{

}


///////////////////////////////////////////////////////////////////////////////
void
BlockingRaycaster::sortBlocks()
{
  glm::vec3 const eye{ getCamera().getEye() };

  // Sort the blocks by their distance from the camera.
  // The origin of each block is used.
  std::vector<bd::Block*> &non_empties{ m_blockCollection->getNonEmptyBlocks() };
  std::sort(non_empties.begin(), non_empties.end(),
            [&eye](bd::Block *a, bd::Block *b) {
              float a_dist = glm::distance(eye, a->origin());
              float b_dist = glm::distance(eye, b->origin());
              return a_dist > b_dist;
            });
}


///////////////////////////////////////////////////////////////////////////////
void
BlockingRaycaster::initShaders()
{
  m_alphaBlending = std::unique_ptr<bd::ShaderProgram>();
  auto pid = m_alphaBlending->linkProgram("shaders/alpha_blending.vert", "shaders/alpha_blending.frag");
  if (pid == 0) {
    throw std::runtime_error("could not initialize shaders");
  }


  GLuint sampler_state{ 0 };
  gl_check(glGenSamplers(1, &sampler_state));
  if (sampler_state == 0) {
    throw std::runtime_error("could not generate a sampler object");
  }

  gl_check(glSamplerParameteri(sampler_state, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  gl_check(glSamplerParameteri(sampler_state, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  gl_check(glSamplerParameteri(sampler_state, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
  gl_check(glSamplerParameteri(sampler_state, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
  gl_check(glSamplerParameteri(sampler_state, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
//  gl_check(glSamplerParameterf(sampler_state, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f));
  gl_check(glBindSampler(sampler_state, BLOCK_TEXTURE_UNIT));
  m_volumeSampler = sampler_state;

}





}} // namespace subvol namespace renderer

