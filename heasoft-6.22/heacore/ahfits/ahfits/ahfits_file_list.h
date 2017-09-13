/// \file ahfits_file_list.h
/// \brief ahfits: FITS file list operations and HDU navigation
/// \author James Peachey
/// \date $Date$

/// \addtogroup mod_ahfits
/// \section ahfits_file_list File Access Library - ahfits_file_list
///
/// This module handles lists of FITS files (\@file).
///

#ifndef AHFITS_AHFITS_FILE_LIST_H
#define AHFITS_AHFITS_FILE_LIST_H

#include "ahgen/ahversion.h"
AHVERSION(AHFITS_AHFITS_FILE_LIST, "$Id$")

#include "ahfits/ahfits_base.h"

#include <vector>
#include <string>

/// \ingroup mod_ahfits
namespace ahfits {

typedef std::vector<std::string> ListStringType;

void expandFileList(const std::string & filename, ListStringType & filelist);

} // namespace ahfits

#endif   /* AHFITS_AHFITS_FILE_LIST_H */

/* Revision Log
   $Log$
   Revision 1.3  2013/11/20 14:26:34  mwitthoe
   ahfits: make corrections to Doxygen documenation suggested by Bob; see issue #289

   Revision 1.2  2013/01/24 15:56:28  mwitthoe
   update afits Doxygen

   Revision 1.1  2012/11/29 02:57:18  peachey
   Add initial facilities for handling lists of files (@files).

*/

