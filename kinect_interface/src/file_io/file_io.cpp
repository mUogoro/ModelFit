//#include <sys\stat.h>
#include <sys/stat.h>
#include <sstream>
#include <string>
#include "file_io/file_io.h"
#include "math/math_types.h"  // for uint
#include "string_util/string_util.h"
#if defined(WIN32) || defined(_WIN32)
  #include <Windows.h>
#endif
#include "data_str/vector_managed.h"

#if defined(GCC)
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#endif

namespace jtil {
namespace file_io {
  bool fileExists(const std::string& filename) {
    // TODO: This is a pretty stupid way to check if a file exists.  I think
    // opening a file handler is probably slow.  Rethink this.
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    bool ret_val = false;
    if (file.is_open()) {
      ret_val = true;
      file.close();
    }
    return ret_val;
  }

  PathType getPathType(const std::string& path) {
    struct stat s;
    if (stat(path.c_str(), &s) == 0) {
      if (s.st_mode & S_IFDIR) {
        return DIRECTORY_PATH;
      }
      else if(s.st_mode & S_IFREG) {
        return FILE_PATH;
      } else {
        return UNKNOWN_PATH;  // It's something else
      }
    } else {
      return UNKNOWN_PATH;  // Something went wrong
    }
  }

  void ls(const std::string& path, jtil::data_str::VectorManaged<char*>& files) {
#if defined(WIN32) || defined(_WIN32)
    // Clear the directory of existing saved frames
    WIN32_FIND_DATAW file_data;
    std::wstring wpath = jtil::string_util::ToWideString(path);
    HANDLE hFind = FindFirstFile(wpath.c_str(), &file_data);
    while (hFind != INVALID_HANDLE_VALUE) {
      std::string filename = 
        jtil::string_util::ToNarrowString(file_data.cFileName);
      char* filename_c_str = new char[filename.length()+1];
      strcpy(filename_c_str, filename.c_str());

      files.pushBack(filename_c_str);
      if (!FindNextFile(hFind, &file_data)) {
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
      }
    }
#elif defined(GCC)

    // Find the substring containing the base path and start iterating through files
    std::string basePath = path.substr(0, path.rfind("/"));
    boost::filesystem::path p(basePath);
    if (!boost::filesystem::exists(p) || !boost::filesystem::is_directory(p))
    {
      std::stringstream errss;
      errss << "Error: " << basePath << " does not exist or is not a directory" << std::endl;
      throw std::runtime_error(errss.str());
    }

    // Build the regex 
    std::string expr(path.substr(path.rfind("/")+1));
    //boost::regex r(expr), boost::regex::basic);
    for (boost::filesystem::directory_iterator it(p); it!=boost::filesystem::directory_iterator(); ++it)
    {
      if (boost::filesystem::is_regular_file(it->status()) &&
	  //boost::regex_match(it->path().filename().string(), r))
	  !it->path().filename().string().compare(0, expr.size()-2, expr, 0, expr.size()-2))
      {
	char* filename_c_str = new char[it->path().filename().string().size()+1];
	strcpy(filename_c_str, it->path().filename().string().c_str());
	files.pushBack(filename_c_str);
      }
    }
    
#else
    //throw std::runtime_error("Not yet implemented for non Windows OS");
    throw std::runtime_error("Not yet implemented on current OS");
#endif
  }

}  // namespace file_io
}  // namespace jtil
