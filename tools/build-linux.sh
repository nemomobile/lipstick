#!/bin/sh

DIR_ROOT=`pwd`
DIR_ARTIFACTS=$DIR_ROOT/../meegotouchhome-artifacts

cleanup()
{
  echo "##teamcity[progressStart 'Killing everything on scratchbox...']"
  sb-execute harmattan-i386 $SCRATCHBOX_BUILDER_HOME/ sb-conf ka -s 9
  echo "##teamcity[progressFinish 'Everything is killed']"
}
cleanup
echo "##teamcity[progressStart 'Cleaning artifacts directory...']"
if [ ! -d $DIR_ARTIFACTS ]
then
        mkdir $DIR_ARTIFACTS
else
        rm -rf $DIR_ARTIFACTS/*
fi
echo "##teamcity[progressFinish 'Artifacts directory cleaned']"

echo "##teamcity[progressStart 'Installing dependencies...']"
sb-install harmattan-i386 duitheme libduivaluespace-dev libcontextprovider-dev libcontextsubscriber-dev libdui-dev libxcomposite-dev libxext-dev maemo-services-dev libmaemosec-dev libdbus-ruby libdbus-ruby1.8 duifw-tests-lib duifw-tests-dev buildtestxml-ruby libssl-dev
#debhelper (>= 5), doxygen (>=1.5.9), libdui-dev (>= 0.18), libqt4-dev (>= 4.6), libqt4-opengl-dev, libxcomposite-dev, libxext-dev, libcontextsubscriber-dev, maemo-services-dev, libmaemosec-dev, ruby, ruby1.8, rdoc1.8, ri1.8, rdoc, ri, libbuilder-ruby, libbuilder-ruby1.8, libmatti-ruby, libmatti-ruby1.8, librmagick-ruby, librmagick-ruby1.8, libxml2-utils, libdbus-ruby, libdbus-ruby1.8, duifw-tests-lib, duifw-tests-dev, buildtestxml-ruby
echo "##teamcity[progressFinish 'Dependencies installed']"

echo "##teamcity[progressStart 'Building meegotouchhome...']"
sb-buildpackage harmattan-i386 $SCRATCHBOX_BUILDER_HOME/build/projects/meegotouchhome
echo "##teamcity[progressFinish 'meegotouchhome build finished']"

echo "##teamcity[progressStart 'Removing libdui...']"
sb-uninstall harmattan-i386 duitheme libdui-dev
echo "##teamcity[progressFinish 'Libdui removed']"

echo "##teamcity[progressStart 'Publishing artifacts...']"
#MS  a bit not systematic, would be better to collect build artifacts in some directory meegotouchhome/artifacts..
mv ../meegotouchhome_* $DIR_ARTIFACTS/
mv ../duifw-home-tests*.deb $DIR_ARTIFACTS/
mv ../*.changes $DIR_ARTIFACTS/
mv ../meegotouchhome*.deb $DIR_ARTIFACTS/
mv ../libmeegotouchhome*.deb $DIR_ARTIFACTS/
if [ "$?" -ne "0" ]; then
  echo "##teamcity[progressFinish 'Failed to publish artifacts :(']"
  cleanup
  exit 1
else
  echo "##teamcity[progressFinish 'Artifacts published']"
fi

