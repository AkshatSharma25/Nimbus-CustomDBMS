#include <uv.h>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
uv_loop_t* loop;
uv_tcp_t server;

std::string http_response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, how are you!";

void on_write(uv_write_t* req, int status) {
    uv_close((uv_handle_t*)req->handle, nullptr);
    delete req;
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void on_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
    if (nread > 0) {
        uv_buf_t resbuf = uv_buf_init((char*)http_response.c_str(), http_response.size());
        cout<<"Received request: " << std::string(buf->base, nread) << "\n";
        uv_write_t* write_req = new uv_write_t;
        uv_write(write_req, client, &resbuf, 1, on_write);
    } else {
        uv_close((uv_handle_t*)client, nullptr);
    }
    delete[] buf->base;
}

void on_new_connection(uv_stream_t* server, int status) {
    if (status < 0) return;

    uv_tcp_t* client = new uv_tcp_t;
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_read_start((uv_stream_t*)client, alloc_buffer, on_read);
    } else {
        uv_close((uv_handle_t*)client, nullptr);
    }
}

int main() {
    loop = uv_default_loop();

    uv_tcp_init(loop, &server);

    sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 8080, &addr);
    uv_tcp_bind(&server, (const sockaddr*)&addr, 0);
    
    int r = uv_listen((uv_stream_t*)&server, 128, on_new_connection);
    if (r) {
        std::cerr << "Listen error: " << uv_strerror(r) << "\n";
        return 1;
    }

    std::cout << "Listening on http://localhost:8080\n";
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
