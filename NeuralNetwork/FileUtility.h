/******************************************************************************
* @File: FileUtility.h
* @Author: Evan Gofourth
* @About: FileUtility is a C++17 library that offers static helpers for
*         reading directories and image files from disk.
*******************************************************************************/
#include<string>
#include<vector>

// Utilities for reading files and directories from disk.
class FileUtility{
public:

  // Default Constructor
  FileUtility();
  // Default Destructor
  ~FileUtility();

  /*******************************************************************************
  * @Method: GetAllFileNamesWithinFolder
  * @Access: PUBLIC
  * @Description: Returns a list of file names within a directory.
  * @param folder: the directory to scan.
  *******************************************************************************/
  static std::vector<std::string>* GetAllFileNamesWithinFolder(std::string folder);

  /*******************************************************************************
  * @Method: RGBAVectorFromImage
  * @Access: PUBLIC
  * @Description: Converts an image file (PNG, etc.) to a std::vector<std::vector<double>>.
  *   The child vectors are length 4: [R,G,B,A].
  * @param fileName: the name of the image file to read.
  * @param targetWidth, targetHeight: optional, both default to 0. When both are
  *   non-zero, the image is box-downsampled to this size (averaging blocks of
  *   source pixels into one output pixel) before conversion. Each must be <= the
  *   source image's matching dimension - this downsamples, it does not upsample.
  *   Leave both at 0 to use the image's native size, unchanged.
  *******************************************************************************/
  static std::vector<std::vector<double>*>* RGBAVectorFromImage(std::string fileName, int targetWidth = 0, int targetHeight = 0);

  /*******************************************************************************
  * @Method: FlattenedRGBAVectorFromImage
  * @Access: PUBLIC
  * @Description: Converts an image file (PNG, etc.) directly to a single flat
  *   std::vector<double> of [R,G,B,A,R,G,B,A,...], in the same pixel order as
  *   RGBAVectorFromImage (column-major: outer loop over x, inner loop over y).
  *   Equivalent to flattening RGBAVectorFromImage's result, but without
  *   allocating one std::vector<double> per pixel to get there - useful when
  *   you don't need the per-pixel grouping (e.g. feeding a flattened image into
  *   a model) and want to avoid RGBAVectorFromImage's per-pixel heap allocations.
  * @param fileName: the name of the image file to read.
  * @param targetWidth, targetHeight: optional, both default to 0. Same box-downsample
  *   behavior as RGBAVectorFromImage's targetWidth/targetHeight - see above.
  *******************************************************************************/
  static std::vector<double>* FlattenedRGBAVectorFromImage(std::string fileName, int targetWidth = 0, int targetHeight = 0);

private:
  static bool LoadImage(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y);

  /*******************************************************************************
  * @Method: LoadAndDownsampleImage
  * @Access: PRIVATE
  * @Description: Loads filename via LoadImage, then, if targetWidth/targetHeight
  *   are both non-zero, box-downsamples it to that size. Shared by
  *   RGBAVectorFromImage and FlattenedRGBAVectorFromImage. Outputs the resulting
  *   pixel buffer and its width/height (the downsampled size if downsampling
  *   happened, otherwise the image's native size).
  *******************************************************************************/
  static void LoadAndDownsampleImage(const std::string& filename, int targetWidth, int targetHeight, std::vector<unsigned char>& image, int& width, int& height);

  /*******************************************************************************
  * @Method: DownsampleImage
  * @Access: PRIVATE
  * @Description: Box-downsamples an RGBA pixel buffer from (srcWidth, srcHeight)
  *   to (targetWidth, targetHeight): each output pixel is the average of the
  *   block of source pixels it covers. targetWidth/targetHeight must each be
  *   <= the matching source dimension.
  *******************************************************************************/
  static std::vector<unsigned char> DownsampleImage(const std::vector<unsigned char>& image, int srcWidth, int srcHeight, int targetWidth, int targetHeight);
};
