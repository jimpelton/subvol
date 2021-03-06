#ifndef subvol_cmdline_h
#define subvol_cmdline_h

#include <tclap/CmdLine.h>
#include <string>

namespace subvol
{
struct CommandLineOptions
{
  /// raw file path
  std::string rawFilePath;
  /// transfer function file path
  std::string tfunc1dtPath;
  /// opacity transfer function path
  std::string opacityTFuncPath;
  /// color transfer function path
  std::string colorTFuncPath;
  /// index file path
  std::string indexFilePath;
  /// volume data type
  std::string dataType;
  /// number of blocks X
  size_t numblk_x;
  /// number of blocks Y
  size_t numblk_y;
  /// number of blocks Z
  size_t numblk_z;
  /// slices per block
  unsigned int num_slices;
  /// vol width
  size_t vol_w;
  /// vol height
  size_t vol_h;
  /// vol depth
  size_t vol_d;
  /// path to output file for performance counter values
  std::string perfOutPath;
  /// quit program when perf tests are completed.
  bool perfMode;
  /// screen width
  int windowWidth;
  /// screen height
  int windowHeight;
  /// gpu mem to use
  int64_t gpuMemoryBytes;
  /// cpu mem to use
  int64_t mainMemoryBytes;
  // sampling modifier (modifies the sample rate during reconstruction)
  float smod_x;
  float smod_y;
  float smod_z;
};


size_t
convertToBytes(std::string s);


///////////////////////////////////////////////////////////////////////////////
/// \brief Parses command line args and populates \c opts.
///
/// If non-zero arg was returned, then the parse was successful, but it does 
/// not mean that valid or all of the required args were provided on the 
/// command line.
///
/// \returns 0 on parse failure, non-zero if the parse was successful.
///////////////////////////////////////////////////////////////////////////////
int
parseThem(int argc, char *argv[], TCLAP::CmdLine &cmd, CommandLineOptions &opts);


void
printThem(CommandLineOptions &);
} // namespace subvol

#endif // subvol_cmdline_h
