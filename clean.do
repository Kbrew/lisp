redo-ifchange ./config
source ./config

find -regextype egrep -iregex '.*[.](bc|o|exe)' | xargs rm -f

rm -f $BUILD_SCRIPTS
