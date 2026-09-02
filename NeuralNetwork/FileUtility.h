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
  *******************************************************************************/
  static std::vector<std::vector<double>*>* RGBAVectorFromImage(std::string fileName);

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
  *******************************************************************************/
  static std::vector<double>* FlattenedRGBAVectorFromImage(std::string fileName);

private:
  static bool LoadImage(std::vector<unsigned char>& image, const std::string& filename, int& x, int& y);
};
