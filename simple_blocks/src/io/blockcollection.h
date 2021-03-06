#ifndef block_collection_h__
#define block_collection_h__

#include "blockloader.h"
#include "classificationtype.h"
#include "messages/recipient.h"

#include <bd/volume/block.h>
#include <bd/io/indexfile/indexfile.h>
#include <bd/io/buffer.h>
#include <bd/util/util.h>
#include <bd/io/bufferpool.h>

#include <functional>
#include <list>
#include <vector>
#include <future>
#include <bd/io/indexfile/v2/jsonindexfile.h>

namespace subvol
{

class BlockCollection
    : public Recipient
{
public:
//  BlockCollection();

  BlockCollection(BlockLoader *loader, bd::indexfile::v2::JsonIndexFile const &index);


  virtual ~BlockCollection();


  /// \brief Initializes \c blocks from the provided vector of FileBlock.
  /// \note Blocks are sized to fit it within the world-extent of the volume data.
  /// \param fileBlocks[in] The FileBlocks generated from the IndexFile.
  /// \param vd[in]  The voxel dimensions of the volume data
  /// \param numblocks[in]  The number of blocks to generate in each dimension.
  void
  initBlocksFromFileBlocks(std::vector<bd::FileBlock> const &fileBlocks,
                           glm::u64vec3 const &numblocks);


  void
  filterBlocks();


  void
  updateBlockCache();


  void
  loadSomeBlocks();


  std::vector<bd::Block *> const &
  getBlocks() const;


  std::vector<bd::Block *> &
  getBlocks();


  std::vector<bd::Block *> &
  getNonEmptyBlocks();


  size_t
  getNumNonEmptyBlocks() const;


  size_t
  getTotalNumBlocks() const;


  double
  getRangeMin() const;


  void
  setRangeMin(double);


  double
  getRangeMax() const;


  void
  setRangeMax(double);


  bool
  getRangeChanged() const;

  /// Find the largest non-empty block and return the number of voxels.
  /// \return size_t that is the number of voxels in the largest block.
//  uint64_t
//  findLargestBlock(std::vector<bd::Block *> &blocks);


  /// Change the classification type. 
  /// The blocks are immediatly filtered and loading of the new
  /// set of blocks begins.
  void
  changeClassificationType(ClassificationType type);


  void
  filterBlocksByROV();


  void
  filterBlocksByAverage();


private:
  std::vector<bd::Block *> m_blocks;

  std::vector<bd::Block *> m_nonEmptyBlocks;

  std::vector<bd::Block *> m_emptyBlocks;

  bd::Volume m_volume;

  BlockLoader *m_loader;

  std::future<int> m_loaderFuture;

  ClassificationType m_classificationType;

  double m_rangeLow;
  double m_rangeHigh;

  bool m_rangeChanged;

  std::function<void(size_t)> m_visibleBlocksCb;

public:   /* public message bus handlers */

  void
  handle_MaxRangeChangedMessage(MaxRangeChangedMessage &m) override;


  void
  handle_MinRangeChangedMessage(MinRangeChangedMessage &m) override;

  //  BlockMemoryManager *m_man;

}; // BlockCollection


//template <typename Ty>
//bool
//BlockCollection::do_initBlockTextures(std::string const &file)
//{
//  Dbg() << "Initializing block textures";
//
//
//  std::ifstream is(file, std::ios::binary);
//  if (!is.is_open()) {
//    Err() << "Could not open rawfile: " << file << ".";
//    return false;
//  }
//
//
//  if (m_nonEmptyBlocks.size() == 0) {
//    Warn() << "There are no non-empty blocks to initialize textures for.";
//    return false;
//  }
//
//  // init buffers for the raw values and a
//  // texture buffer that contains normalized values.
//  size_t buf_size{ findLargestBlock(m_nonEmptyBlocks) };
//  Ty *buf{ new Ty[buf_size] };
//  float *tex{ new float[buf_size] };
//
//
//  int i{ 0 };
//  double const vol_min = m_volume.min(); //m_indexFile->getHeader().vol_min;
//  double const diff{ m_volume.max() - vol_min };
//
//  // Generate textures for each non-empty block
//  std::cout << std::endl;
//  int every = 0.1f * m_nonEmptyBlocks.size();
//  every = every == 0 ? 1 : every;
//
//  for (Block *b : m_nonEmptyBlocks) {
//
//    if (i % every == 0) {
//      std::cout << "\rInitializing texture block " << ++i
//                << "/" << m_nonEmptyBlocks.size();
//    }
//
//
//    // Read data for this block from the disk.
//    fillBlockData<Ty>(*b, is, buf);
//
//
//    // Normalize the data prior to generating the texture.
//    for (size_t idx{ 0 }; idx < buf_size; ++idx) {
//      tex[idx] = (buf[idx] - vol_min) / diff;
//    }
//
//    //TODO: try not normalizing the texture data and see what happens... :)
//    b->texture().subImage3D(0,0,0,
//                            (int)b->voxel_extent().x,
//                            (int)b->voxel_extent().y,
//                            (int)b->voxel_extent().z,
//                            bd::Texture::Format::RED,
//                            DataType::Float,
//                            tex);
//  } //for
//  std::cout << " ...done." << std::endl;
//
//  delete[] buf;
//  delete[] tex;
//
//  return true;
//}



//template <typename Ty>
//void
//BlockCollection::fillBlockData(Block const &b, std::istream &infile,
//                               Ty *blockBuffer) const
//{
//  // block's dimensions in voxels
//  glm::u64vec3 const &be{ b.voxel_extent() };
//
//  // volume's dimensions in voxels.
//  glm::u64vec2 const &ve{ m_volume.voxelDims().x,
//                          m_volume.voxelDims().y };
//
//  // start element = block index w/in volume * block size
//  const glm::u64vec3 start{ b.ijk() * be };
//
//  // block end element = block voxel start voxelDims + block size
//  const glm::u64vec3 end{ start + be };
//
//  // byte offset into file to read from
//  size_t offset{ b.fileBlock().data_offset };
//
//  // Loop through rows and slabs of volume reading rows of voxels into memory.
//  const size_t blockRowLength{ be.x };
//  for (auto slab = start.z; slab < end.z; ++slab) {
//    for (auto row = start.y; row < end.y; ++row) {
//
//      // seek to start of row
//      infile.seekg(offset, infile.beg);
//
//      // read the bytes of current row
//      infile.read(reinterpret_cast<char *>(blockBuffer), blockRowLength * sizeof(Ty));
//      blockBuffer += blockRowLength;
//
//      // offset of next row
//      offset = bd::to1D(start.x, row + 1, slab, ve.x, ve.y);
//      offset *= sizeof(Ty);
//    }
//  }
//
//
//}

} // namespace bd

#endif // !block_collection_h__
