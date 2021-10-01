#!/usr/bin/env bash

# https://www.ics.com/blog/how-compile-qt-source-code-linux
# https://www.qt.io/blog/2017/06/16/vulkan-support-qt-5-10-part-2
# http://manpages.ubuntu.com/manpages/bionic/man1/qtchooser.1.html
# https://doc.qt.io/qt-5/configure-options.html
# https://unix.stackexchange.com/questions/481063/ubuntu-18-04-1-lts-x64-how-to-update-qt-5-10-from-qt-5-9-5
# https://forum.qt.io/topic/125582/qt-5-15-2-do-not-build-libqxcb-so-but-at-the-same-time-qt-5-14-0-successfully-build-libqxcb-so

QT_VERSION_PATTERN='^([0-9]+\.){2}(\*|[0-9]+)(-.*)?$' # semver format

if ! [[ ${QT_VERSION} =~ ${QT_VERSION_PATTERN} ]]; then
  echo "QT_VERSION should be in a valid format (i.e., major.minor.patch)!"
  return 1
fi

if [[ -z ${QT_SRC_DOWNLOAD_DIR} ]]; then
  echo "QT_SRC_DOWNLOAD_DIR should be set!"
  return 1
fi

if ! echo ${SYSTEM_PASSWORD} | sudo -Sk >/dev/null 2>&1 true; then
  echo "SYSTEM_PASSWORD is incorrect!"
  return 1
fi

QT_SOURCE_FILE="qt-everywhere-src-${QT_VERSION}.tar.xz"
QT_TARGET_DIR=$(echo "qt${QT_VERSION}" | sed -e 's/\.//g')
QT_INSTALL_DIR=/opt/${QT_TARGET_DIR}
QT_VERSION_MINOR=$(echo ${QT_VERSION} | sed -e 's/\.[^.]*$//')

if [[ -d "${QT_INSTALL_DIR}" ]] || [[ -d "${QT_TARGET_DIR}" ]]; then
  echo "** Qt v${QT_VERSION} has already been installed in ${QT_INSTALL_DIR}."
  echo "** If you need to reinstall, delete the following directories:"
  echo "** - ${QT_INSTALL_DIR}"
  echo "** - ${QT_SRC_DOWNLOAD_DIR}/${QT_TARGET_DIR}"
  return 0
fi

# NOTE:
#
# Required Dependencies (This is only for Ubuntu - if other distribution is used, modify accordingly)
# for Qt Vulkan on Linux at least xcb and its relevant packages need to be installed
# These are the mandatory ones:
# - libxcb-util-dev libx11-xcb-dev
# - libxkbcommon-dev libxkbcommon-x11-dev
#
echo ${SYSTEM_PASSWORD} | sudo -Sk apt-get install \
     bison build-essential flex gperf ibgstreamer-plugins-base* libasound2-dev libatkmm-1.6-dev libbz2-dev \
     libcap-dev libcups2-dev libdrm-dev libegl1-mesa-dev libfontconfig1-dev libfreetype6-dev libgcrypt11-dev \
     libglu1-mesa-dev libgstreamer*-dev libicu-dev libnss3-dev libpci-dev libpulse-dev libssl-dev libudev-dev \
     libx11-dev libxkbcommon-dev libxkbcommon-x11-dev libx11-xcb-dev libxcb-composite0 libxcb-composite0-dev \
     libxcb-cursor-dev libxcb-cursor0 libxcb-damage0 libxcb-damage0-dev libxcb-dpms0 libxcb-dpms0-dev libxcb-dri2-0 \
     libxcb-dri2-0-dev libxcb-dri3-0 libxcb-dri3-dev libxcb-ewmh-dev libxcb-ewmh2 libxcb-glx0 libxcb-glx0-dev \
     libxcb-icccm4 libxcb-icccm4-dev libxcb-image0 libxcb-image0-dev libxcb-keysyms1 libxcb-keysyms1-dev \
     libxcb-present-dev libxcb-present0 libxcb-randr0 libxcb-randr0-dev libxcb-record0 libxcb-record0-dev \
     libxcb-render-util0 libxcb-render-util0-dev libxcb-render0 libxcb-render0-dev libxcb-res0 libxcb-res0-dev \
     libxcb-screensaver0 libxcb-screensaver0-dev libxcb-shape0 libxcb-shape0-dev libxcb-shm0 libxcb-shm0-dev \
     libxcb-sync-dev libxcb-sync-dev libxcb-sync1 libxcb-util-dev libxcb-util0-dev libxcb-util1 libxcb-x* \
     libxcb-xf86dri0 libxcb-xf86dri0-dev libxcb-xfixes0 libxcb-xfixes0-dev libxcb-xinerama0 libxcb-xinerama0-dev \
     libxcb-xkb-dev libxcb-xkb1 libxcb-xtest0 libxcb-xtest0-dev libxcb-xv0 libxcb-xv0-dev libxcb-xvmc0 \
     libxcb-xvmc0-dev libxcb1 libxcb1-dev libxcomposite-dev libxcursor-dev libxdamage-dev libxext-dev libxfixes-dev \
     libxi-dev libxrandr-dev libxrender-dev libxslt1-dev libxss-dev libxtst-dev perl python ruby || return

cd ${QT_SRC_DOWNLOAD_DIR} || return

if [[ ! -d "${QT_TARGET_DIR}" ]]; then
  wget -nc https://download.qt.io/official_releases/qt/${QT_VERSION_MINOR}/${QT_VERSION}/single/${QT_SOURCE_FILE}

  if [[ -f ${QT_SOURCE_FILE} ]]; then
    mkdir ${QT_TARGET_DIR}
    tar xf ${QT_SOURCE_FILE} -C ${QT_TARGET_DIR} --strip-components=1
  fi
fi

if ! command -v vulkaninfo >/dev/null 2>&1; then
  echo "Error - Vulkan SDK not found!"
  return 1
fi

cd ${QT_TARGET_DIR} || return
./configure -prefix ${QT_INSTALL_DIR} \
            -opensource -confirm-license \
            -pch -bundled-xcb-xinput \
            -feature-vulkan \
            -skip qtconnectivity \
            -skip qtwebengine \
            -skip qtwebglplugin
make -j8
echo ${SYSTEM_PASSWORD} | sudo -Sk make install
