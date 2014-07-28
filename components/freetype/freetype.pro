######################################################################
#Automatically generated by qmake(3.0)????2815:01:542014
######################################################################

TEMPLATE=lib
TARGET=freetype

CONFIG += staticlib
DEFINES += FT2_BUILD_LIBRARY

dir = ../..
include($$dir/common.pri)

win32-msvc*{
    QMAKE_CXXFLAGS += /wd4001 /wd4244 /wd4819
}

INCLUDEPATH += . \
	       include \

HEADERS += "include/ft2build.h"
HEADERS += "include/config/ftconfig.h"
HEADERS += "include/config/ftheader.h"
HEADERS += "include/config/ftmodule.h"
HEADERS += "include/config/ftoption.h"
HEADERS += "include/config/ftstdlib.h"

SOURCES += "src/ftdebug.c"
SOURCES += "src/autofit/autofit.c"
SOURCES += "src/bdf/bdf.c"
SOURCES += "src/cff/cff.c"
SOURCES += "src/base/ftbase.c"
SOURCES += "src/base/ftbitmap.c"
SOURCES += "src/cache/ftcache.c"
SOURCES += "src/base/ftfstype.c"
SOURCES += "src/base/ftgasp.c"
SOURCES += "src/base/ftglyph.c"
SOURCES += "src/gzip/ftgzip.c"
SOURCES += "src/base/ftinit.c"
SOURCES += "src/lzw/ftlzw.c"
SOURCES += "src/base/ftstroke.c"
SOURCES += "src/base/ftsystem.c"
SOURCES += "src/smooth/smooth.c"
SOURCES += "src/base/ftbbox.c"
SOURCES += "src/base/ftgxval.c"
SOURCES += "src/base/ftlcdfil.c"
SOURCES += "src/base/ftmm.c"
SOURCES += "src/base/ftotval.c"
SOURCES += "src/base/ftpatent.c"
SOURCES += "src/base/ftpfr.c"
SOURCES += "src/base/ftsynth.c"
SOURCES += "src/base/fttype1.c"
SOURCES += "src/base/ftwinfnt.c"
SOURCES += "src/base/ftxf86.c"
SOURCES += "src/pcf/pcf.c"
SOURCES += "src/pfr/pfr.c"
SOURCES += "src/psaux/psaux.c"
SOURCES += "src/pshinter/pshinter.c"
SOURCES += "src/psnames/psmodule.c"
SOURCES += "src/raster/raster.c"
SOURCES += "src/sfnt/sfnt.c"
SOURCES += "src/truetype/truetype.c"
SOURCES += "src/type1/type1.c"
SOURCES += "src/cid/type1cid.c"
SOURCES += "src/type42/type42.c"
SOURCES += "src/winfonts/winfnt.c"
