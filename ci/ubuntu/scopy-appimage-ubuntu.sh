#!/bin/bash
set -xe

STAGING_AREA=$PWD/staging
STAGING_AREA_DEPS=$STAGING_AREA/dependencies
QT=/opt/Qt/5.15.2/gcc_64
APP_DIR_NAME=ScopyAppDir

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$STAGING_AREA_DEPS/lib:$QT/lib
export PATH=$QT:$PATH
export QMAKE=$QT/bin/qmake

wget https://github.com/linuxdeploy/linuxdeploy/releases/download/1-alpha-20231026-1/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-appimage/releases/download/1-alpha-20230713-1/linuxdeploy-plugin-appimage-x86_64.AppImage

chmod +x linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x linuxdeploy-plugin-appimage-x86_64.AppImage

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

./linuxdeploy-x86_64.AppImage \
	--appdir  $APP_DIR_NAME\
	--executable $SRC_DIR/build-x86_64/scopy \
	--custom-apprun $SRC_DIR/ci/general/AppRun \
	--desktop-file $SRC_DIR/ci/ubuntu/scopy.desktop \
	--icon-file $SRC_DIR/gui/res/scopy.png \
	--plugin qt

#python lipseste
cp $STAGING_AREA_DEPS/bin/iio-emu $APP_DIR_NAME/usr/bin
cp -r $STAGING_AREA_DEPS/lib/* $APP_DIR_NAME/usr/lib

./linuxdeploy-plugin-appimage-x86_64.AppImage --appdir $APP_DIR_NAME
