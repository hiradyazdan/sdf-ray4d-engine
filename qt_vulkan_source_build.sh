#!/usr/bin/env bash

# https://www.ics.com/blog/how-compile-qt-source-code-linux
# https://www.qt.io/blog/2017/06/16/vulkan-support-qt-5-10-part-2
# http://manpages.ubuntu.com/manpages/bionic/man1/qtchooser.1.html
# https://doc.qt.io/qt-5/configure-options.html
# https://unix.stackexchange.com/questions/481063/ubuntu-18-04-1-lts-x64-how-to-update-qt-5-10-from-qt-5-9-5
# https://forum.qt.io/topic/125582/qt-5-15-2-do-not-build-libqxcb-so-but-at-the-same-time-qt-5-14-0-successfully-build-libqxcb-so

# NOTE
#
# Parallel Builds Thread-safety (avoid data race)
#
# This process is system-wide and should not run
# per build in parallel (synchronously) as otherwise
# it conflicts and fails. So, should wait for it to
# finish before continuing with the other parallel build
# configuration
#
QT_PARALLEL_BUILD_LOCK=./qt_parallel_build_lock.pid
if [[ -f ${QT_PARALLEL_BUILD_LOCK} ]]; then
  echo "** Qt source build is already running! Waiting for it to finish..."
  sleep 2073600 & # infinity

  echo $! > ${QT_PARALLEL_BUILD_LOCK}
  wait
else
  touch ${QT_PARALLEL_BUILD_LOCK}
fi

function removeLock() { rm ${QT_PARALLEL_BUILD_LOCK} >/dev/null 2>&1; }

QT_VERSION_PATTERN='^([0-9]+\.){2}(\*|[0-9]+)(-.*)?$' # semver format
QT_SRC_DOWNLOAD_DIR=$(eval echo "${QT_SRC_DOWNLOAD_DIR}")

if ! [[ ${QT_VERSION} =~ ${QT_VERSION_PATTERN} ]]; then
  echo "QT_VERSION should be in a valid format (i.e., major.minor.patch)!"

  removeLock
  return 1
fi

if [[ ! -d "${QT_SRC_DOWNLOAD_DIR}" ]]; then
  echo "QT_SRC_DOWNLOAD_DIR should be an existing directory!"

  removeLock
  return 1
fi

if ! echo ${SYSTEM_PASSWORD} | sudo -S >/dev/null 2>&1 true; then
  echo "SYSTEM_PASSWORD is incorrect!"

  removeLock
  return 1
fi

QT_SOURCE_FILE_NAME="qt-everywhere-src-${QT_VERSION}.tar.xz"
QT_SOURCE_FILE_PATH="${QT_SRC_DOWNLOAD_DIR}/${QT_SOURCE_FILE_NAME}"
QT_TARGET_DIR_NAME=$(echo "qt${QT_VERSION}" | sed -e 's/\.//g')
QT_TARGET_DIR_PATH="/opt/${QT_TARGET_DIR_NAME}-target" # untarring in same directory is very slow due to io priorities
QT_TARGET_TAR_COMPLETE=${QT_TARGET_DIR_PATH}/.tar_complete
QT_TARGET_ERROR_LOG_FILE="${PWD}/qt_build_error.log"
QT_INSTALL_DIR_PATH=/opt/${QT_TARGET_DIR_NAME}
QT_VERSION_MINOR=$(echo ${QT_VERSION} | sed -e 's/\.[^.]*$//')

if [[ -d "${QT_INSTALL_DIR_PATH}" ]]; then
  echo "** Qt v${QT_VERSION} has already been installed in ${QT_INSTALL_DIR_PATH}."
  echo "** If you need to reinstall, delete the following directory:"
  echo "** - ${QT_INSTALL_DIR_PATH}"

  removeLock
  return 0
fi

# NOTE:
#
# Required Dependencies (This is only for Ubuntu - if other distribution is used, modify accordingly)
# for Qt Vulkan on Linux at least xcb, vulkan libraries and their relevant packages need to be installed
# These are the mandatory ones:
# - libxcb-util-dev libx11-xcb-dev
# - libxkbcommon-dev libxkbcommon-x11-dev
# - libvulkan-dev vulkan-tools (20.x) / vulkan-utils (18.04)
#
# get the libvulkan-dev 1.2.x depending on the distro:
# wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
# - sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.2.189-focal.list https://packages.lunarg.com/vulkan/1.2.189/lunarg-vulkan-1.2.189-focal.list
# - sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.2.189-bionic.list https://packages.lunarg.com/vulkan/1.2.189/lunarg-vulkan-1.2.189-bionic.list

sudo apt-get -y install \
     bison build-essential flex gperf ibgstreamer-plugins-base* libasound2-dev libatkmm-1.6-dev libbz2-dev \
     libcap-dev libcups2-dev libdrm-dev libegl1-mesa-dev libfontconfig1-dev libfreetype6-dev libgcrypt20-dev \
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
     libxi-dev libxrandr-dev libxrender-dev libxslt1-dev libxss-dev libxtst-dev libvulkan-dev vulkan-utils \
     perl python ruby axel pv ccache || { removeLock && return; }

cd ${QT_SRC_DOWNLOAD_DIR} || { removeLock && return; }

if [[ ! -d "${QT_TARGET_DIR_PATH}" || ! -f ${QT_TARGET_TAR_COMPLETE} ]]; then
  echo "Retrieving ${QT_SOURCE_FILE_NAME} ..."
  wget -c --progress=bar:force \
  https://download.qt.io/official_releases/qt/${QT_VERSION_MINOR}/${QT_VERSION}/single/${QT_SOURCE_FILE_NAME}
# NOTE: In case download is very slow use parallel connections:
#  axel -a -n 10 -o \
#  ${QT_SOURCE_FILE_NAME} \
#  https://download.qt.io/official_releases/qt/${QT_VERSION_MINOR}/${QT_VERSION}/single/${QT_SOURCE_FILE_NAME}

  if [[ -f ${QT_SOURCE_FILE_PATH} ]]; then
    echo "Extracting ${QT_SOURCE_FILE_NAME} ..."
    sudo mkdir -p ${QT_TARGET_DIR_PATH}
    pv -f ${QT_SOURCE_FILE_PATH} | sudo tar -xJf- \
                                            --skip-old-files \
                                            --strip-components=1 \
                                            -C ${QT_TARGET_DIR_PATH} || { removeLock && return; }
    sudo touch ${QT_TARGET_TAR_COMPLETE}
  fi
fi

if ! command -v vulkaninfo &> /dev/null; then
  echo "Error - Vulkan SDK not found!"

  removeLock
  return 1
fi

cd ${QT_TARGET_DIR_PATH} || return

echo "Configuring ${QT_TARGET_DIR_NAME} ..."
touch ${QT_TARGET_ERROR_LOG_FILE}
sudo ./configure -prefix ${QT_INSTALL_DIR_PATH} \
                 -opensource -confirm-license \
                 -release \
                 -nomake examples \
                 -nomake tests \
                 -ccache \
                 -silent \
                 -pch \
                 -skip qtconnectivity \
                 -skip qtlocation \
                 -skip qtwebchannel \
                 -skip qtwebengine \
                 -skip qtwebglplugin \
                 -skip qtwebsockets \
                 -skip qtwebview \
                 2> >(tee -a ${QT_TARGET_ERROR_LOG_FILE})

echo "Building ${QT_TARGET_DIR_NAME} ..."
sudo make -j8 2>> ${QT_TARGET_ERROR_LOG_FILE}

echo "Installing ${QT_TARGET_DIR_NAME} ..."
sudo make install 2>> ${QT_TARGET_ERROR_LOG_FILE}

cd ../ && sudo rm -rf ${QT_TARGET_DIR_PATH} && rm ${QT_SOURCE_FILE_PATH}

# NOTE: Parallel Builds Thread-safety (avoid data race)
if [[ -f ${QT_PARALLEL_BUILD_LOCK} ]]; then
  kill "$(< ${QT_PARALLEL_BUILD_LOCK})"

  removeLock
fi

return 0
