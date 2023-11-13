#!/bin/bash
set -xe

STAGING_AREA=$PWD/staging
STAGING_AREA_DEPS=$STAGING_AREA/dependencies
APP_DIR_NAME=ScopyAppDir

SRC_DIR=$(git rev-parse --show-toplevel)

echo "
[Desktop Entry]
Version=1.0
Icon=scopy
Exec=scopy
Terminal=false
Type=Application
Categories=Science;
Name=Scopy
GenericName=Oscilloscope
Comment=A software oscilloscope
" > $SRC_DIR/ci/ubuntu/scopy.desktop

rm -rf $APP_DIR_NAME
rm -rf Scopy*.AppImage


git clone https://github.com/AppImage/appimagetool --recursive
git clone https://github.com/linuxdeploy/linuxdeploy --recursive
git clone https://github.com/linuxdeploy/linuxdeploy-plugin-qt --recursive
git clone https://github.com/linuxdeploy/linuxdeploy-plugin-appimage --recursive

sudo apt install -y wget libgpgme-dev libgcrypt20-dev gpg gnupg patchelf cimg-dev \
	nlohmann-json3-dev squashfs-tools desktop-file-utils zsync appstream 



linuxdeploy\
	--appdir  $APP_DIR_NAME\
	--executable $SRC_DIR/build-*/scopy \
	--custom-apprun $SRC_DIR/ci/general/AppRun \
	--desktop-file $SRC_DIR/ci/ubuntu/scopy.desktop \
	--icon-file $SRC_DIR/gui/res/scopy.png \
	--plugin qt

#cp $STAGING_AREA_DEPS/bin/iio-emu $APP_DIR_NAME/usr/bin
#cp -r $STAGING_AREA_DEPS/lib/* $APP_DIR_NAME/usr/lib

linuxdeploy-plugin-appimage --appdir $APP_DIR_NAME
