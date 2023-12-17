#ifndef AA86D3DE_70A4_4D7F_A856_DEBE504BB6A2
#define AA86D3DE_70A4_4D7F_A856_DEBE504BB6A2
#ifndef C58909A3_3F8C_4C30_AA88_FEEF8839C95C
#define C58909A3_3F8C_4C30_AA88_FEEF8839C95C

    #include <FS.h>

    namespace DataToFs {

        boolean save(const char *file, void *data, size_t sz, fs::FS &fsRef) {
            char *dptr = (char*)data;

            File fh = fsRef.open(file,"w");

            if (fh){
                for (int i = 0; i < sz; i++) {
                    fh.write(dptr[i]);
                } 
                fh.close();
                return true;
            }else{
                Serial.println("Unable to save...");
                return false;
            } 
        }

        boolean load(const char *file, void *data, size_t sz, fs::FS &fsRef, bool checkVersion) {
            if (fsRef.exists(file)) {
                Serial.printf("File %s exists\n", file);

                char *dptr = (char*)data;

                File fh = fsRef.open(file,"r");

                if (fh.size() != sz) {
                    Serial.println("File size wrong!");
                    return false;
                }

                char old[sz];
                memcpy(old, data, sz);

                for (int i = 0; i < sz; i++) {
                    dptr[i] = fh.read();
                }

                fh.close();

                if (dptr[0] != old[0] && checkVersion) {
                    memcpy(data, old, sz);
                    Serial.println("Wrong version");
                    return false;
                } else {
                    Serial.printf("File %s loaded\n", file);
                    return true;
                }

                
            } else {
                Serial.printf("File %s doesn't exists\n", file);
                return false;
            }
        }

    }

#endif /* C58909A3_3F8C_4C30_AA88_FEEF8839C95C */


#endif /* AA86D3DE_70A4_4D7F_A856_DEBE504BB6A2 */
