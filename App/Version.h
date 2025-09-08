#ifndef __VERSION_H__
#define __VERSION_H__

#define PROJECT_MAJOR_VERSION    0
#define PROJECT_MINOR_VERSION    1
#define PROJECT_BUILD_NUMBER     3

#define DEF_TO_STR_(X)    #X
#define DEF_TO_STR(X)     DEF_TO_STR_(X)
#define SW_VERSION_STR    DEF_TO_STR(PROJECT_MAJOR_VERSION) "."\
                          DEF_TO_STR(PROJECT_MINOR_VERSION) "."\
                          DEF_TO_STR(PROJECT_BUILD_NUMBER)

#endif
