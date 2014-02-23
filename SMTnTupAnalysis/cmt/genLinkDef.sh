#!/bin/bash
#
# W. H. Bell
#
# A script to create a LinkDef file from a list of header files.  This
# file is needed by Makefile.standalone to rebuild the analysis
# LinkDef.
#
#

if [[ -z $1 && -z $2 ]]; then
  echo "Usage $0 <LinkDef.h> <header1> [<header2>] ..."
  echo "  where <LinkDef.h> is a file name ending in LinkDef.h and <headerN> are assumed to be \"name of a class\".h"
  exit 1
fi

linkDef=$1

if [[ $linkDef != *"LinkDef.h" ]]; then
  echo "Error: the LinkDef.h file must end in LinkDef.h"
  exit 1
fi

cat > $linkDef <<EOF
/**
 ** A LinkDef file to build a dictionary for ROOT CINT and
 ** PyROOT use.
*/
#ifdef __MAKECINT__
EOF

# Remove the first command line option, which is the name of the
# LinkDef.h file
shift 1 

# Loop over header files.
for header in $* ; do
  class=$(echo "$header" | tr '/' '\n' | grep ".h" | sed 's/\.h//g')
  echo "#pragma link C++ class $class+;" >> $linkDef
done

echo "#endif" >> $linkDef
