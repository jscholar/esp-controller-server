#include "web_assets.h"


/******************************************************************************************* 
    Assets - Embedded Text
    See: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html#embedding-binary-data
*/

// Index.html
extern const unsigned char index_html_start[]
    asm("_binary_index_html_start");
extern const unsigned char index_html_end[]
    asm("_binary_index_html_end");
const char *web_index_html(void) {
    return (const char *)index_html_start;
}
size_t web_index_html_size(void) {
    return index_html_end - index_html_start;
}


