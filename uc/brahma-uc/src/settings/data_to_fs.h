#ifndef C58909A3_3F8C_4C30_AA88_FEEF8839C95C
#define C58909A3_3F8C_4C30_AA88_FEEF8839C95C

    #include <FS.h>

    namespace DataToFs {

        boolean save(const char *file, void *data, size_t sz, fs::FS &fsRef);

        boolean load(const char *file, void *data, size_t sz, fs::FS &fsRef, bool checkVersion = true);

    }

#endif /* C58909A3_3F8C_4C30_AA88_FEEF8839C95C */
