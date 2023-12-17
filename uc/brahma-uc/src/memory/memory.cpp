#include "memory.h"
#include "../logging/logging.h"
#include <stdlib.h>

// void* malloc(size_t s)
// {
//     if (s > MAX_MAIN_MEMORY_BUFFER_SIZE || ESP.getFreeHeap() < USE_PSRAM_IF_MAIN_RAM_LESS_THAN) {
//         // Logging::warn(0, F("Big buffer allocated: %d"), s);
//         if (psramFound()) {
//             return ps_malloc(s);
//         } else {
//             return heap_caps_malloc(s, MALLOC_CAP_8BIT);
//         }
//     } else {
//         return heap_caps_malloc(s, MALLOC_CAP_8BIT);
//     }
// }
