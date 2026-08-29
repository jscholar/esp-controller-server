#include "esp_http_server.h"
#include "esp_log.h"

// Defines the maximum accepted payload size for an incoming Websocket frame.
// Currently: 256 bytes, which is really small btw
#define MAX_WS_FRAME_SIZE 256

static esp_err_t ws_handler(httpd_req_t *req) {
    httpd_ws_frame_t frame = {0};

    esp_err_t err = httpd_ws_recv_frame(req, &frame, 0);
    if (err != ESP_OK) {
        return err;
    }

    if (frame.len > MAX_WS_FRAME_SIZE) {
        return ESP_ERR_INVALID_SIZE;
    }

    uint8_t *payload = calloc(frame.len + 1, sizeof(*payload));
    if (payload == NULL) {
        // calloc failed to allocate memory
        return ESP_ERR_NO_MEM;
    }
    frame.payload = payload;

    err = httpd_ws_recv_frame(req, &frame, frame.len);

    if (err == ESP_OK && frame.type == HTTPD_WS_TYPE_TEXT) {
        // Process the received text frame
        // This is where we would handle business logic for the received message.
        ESP_LOGI("ws_handler", "Received text frame: %s", (char *)payload);
    }

    free(payload);
    return err;
}
static const httpd_uri_t ws_uri_handler_options = {
        .uri       = "/ws",               // the address at which the resource can be found
        .method    = HTTP_GET,          // The HTTP method (HTTP_GET, HTTP_POST, ...)
        .handler   = ws_handler, // The function which process the request
        .user_ctx  = NULL,               // Additional user data for context

        .is_websocket = true,               // Mandatory: set to `true` to handler websocket protocol
        .handle_ws_control_frames = false,  // Optional: set to `true` for the handler to receive control packets, too
        .supported_subprotocol = "chat",    // Optional: set supported subprotocol for this handler
};

esp_err_t websocket_register(httpd_handle_t server) {
    return httpd_register_uri_handler(server, &ws_uri_handler_options);
}