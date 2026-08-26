// Minimal HTTP server: responds to GET "/" with "Hello World"

#include "esp_http_server.h"
#include "esp_log.h"
#include "web_assets.h"

static const char* TAG = "http_server";

static esp_err_t hello_get_handler(httpd_req_t *req) {
    const char* resp_str = web_index_html();
    httpd_resp_send(req, resp_str, web_index_html_size());
    return ESP_OK;
}

static const httpd_uri_t hello_world_uri= {
    .uri       = "/",               // the address at which the resource can be found
    .method    = HTTP_GET,          // The HTTP method (HTTP_GET, HTTP_POST, ...)
    .handler   = hello_get_handler, // The function which process the request
    .user_ctx  = NULL               // Additional user data for context
};


httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    esp_err_t err = httpd_start(&server, &config);

    httpd_register_uri_handler(server, &hello_world_uri);
    
    if (err == ESP_OK) {
        return server;
    }
    
    ESP_LOGE(TAG, "Failed to start HTTP server: %s", esp_err_to_name(err));
    return NULL;
}
