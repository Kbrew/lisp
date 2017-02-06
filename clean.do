redo-ifchange ./config
source ./config

find -regextype egrep -iregex '.*[.](bc|o|d|exe)' | xargs rm -f

rm -f $BUILD_SCRIPTS
