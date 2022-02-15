#include "Server.h"

std::queue<std::string> mvec;
std::condition_variable mvec_cv;
std::mutex mvec_lock;


void Server::run() {

    if (!wsserver.init("0.0.0.0", 9999)) {
        std::cerr << "wsserver init failed: " << wsserver.getLastError() << std::endl;
        return;
    }

    running = true;
    ws_thr = std::thread([this]() {
        while (running.load(std::memory_order_relaxed)) {
            wsserver.poll(this);
            std::this_thread::yield();
        }
        });

    std::cout << "Server running..." << std::endl;
    ws_thr.detach(); //Daemon Thread
       
}

void Server::stop() {
    running = false; 
    std::cout << "Server stopped..." << std::endl;
}

// called when a new websocket connection is about to open
// optional: origin, protocol, extensions will be nullptr if not exist in the request headers
// optional: fill resp_protocol[resp_protocol_size] to add protocol to response headers
// optional: fill resp_extensions[resp_extensions_size] to add extensions to response headers
// return true if accept this new connection
bool Server::onWSConnect(WSConn& conn, const char* request_uri, const char* host, const char* origin, const char* protocol,
    const char* extensions, char* resp_protocol, uint32_t resp_protocol_size, char* resp_extensions,
    uint32_t resp_extensions_size) {
    struct sockaddr_in addr;
    conn.getPeername(addr);
    std::cout << "ws connection from: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;
    std::cout << "request_uri: " << request_uri << std::endl;
    std::cout << "host: " << host << std::endl;
    if (origin) {
        std::cout << "origin: " << origin << std::endl;
    }
    if (protocol) {
        std::cout << "protocol: " << protocol << std::endl;
    }
    if (extensions) {
        std::cout << "extensions: " << extensions << std::endl;
    }
    this->conn = &conn;

    return true;
}

// called when a websocket connection is closed
// status_code 1005 means no status code in the close msg
// status_code 1006 means not a clean close(tcp connection closed without a close msg)
void Server::onWSClose(WSConn& conn, uint16_t status_code, const char* reason) {
    std::cout << "ws close, status_code: " << status_code << ", reason: " << reason << std::endl;
}

// onWSMsg is used if RecvSegment == false(by default), called when a whole msg is received
void Server::onWSMsg(WSConn& conn, uint8_t opcode, const uint8_t* payload, uint32_t pl_len) {
    if (opcode == websocket::OPCODE_PING) {
        conn.send(websocket::OPCODE_PONG, payload, pl_len);
        return;
    }
    if (opcode != websocket::OPCODE_TEXT) {
        conn.close(1003, "not text msg");
        return;
    }



    //producter
    std::unique_lock<std::mutex> lk(mvec_lock);

    std::string resp((const char*)payload, pl_len);
    std::cout << "OnMessage: " << resp << std::endl;

    mvec.push(resp);
    mvec_cv.notify_all();
}

// onWSSegment is used if RecvSegment == true, called when a segment is received
// pl_start_idx: index in the whole msg for the 1st byte of payload
// fin: whether it's the last segment
void Server::onWSSegment(WSConn& conn, uint8_t opcode, const uint8_t* payload, uint32_t pl_len, uint32_t pl_start_idx,
    bool fin) {
    std::cout << "error: onWSSegment should not be called" << std::endl;
}

Server::~Server()
{
    this->stop();
}


void warp_send(WSConn* conn, std::string resp)
{
    conn->send(websocket::OPCODE_TEXT, (const uint8_t*)resp.data(), resp.size());
}
