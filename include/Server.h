#pragma once

#include "websocket.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <signal.h>

#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <string>

extern std::queue<std::string> mvec;
extern std::condition_variable mvec_cv;
extern std::mutex mvec_lock;


class Server
{
public:
    using WSServer = websocket::WSServer<Server>;
    using WSConn = WSServer::Connection;
    void run();
    void stop();

    // called when a new websocket connection is about to open
    // optional: origin, protocol, extensions will be nullptr if not exist in the request headers
    // optional: fill resp_protocol[resp_protocol_size] to add protocol to response headers
    // optional: fill resp_extensions[resp_extensions_size] to add extensions to response headers
    // return true if accept this new connection
    bool onWSConnect(WSConn& conn, const char* request_uri, const char* host, const char* origin, const char* protocol,
        const char* extensions, char* resp_protocol, uint32_t resp_protocol_size, char* resp_extensions,
        uint32_t resp_extensions_size);

    // called when a websocket connection is closed
    // status_code 1005 means no status code in the close msg
    // status_code 1006 means not a clean close(tcp connection closed without a close msg)
    void onWSClose(WSConn& conn, uint16_t status_code, const char* reason);

    // onWSMsg is used if RecvSegment == false(by default), called when a whole msg is received
    void onWSMsg(WSConn& conn, uint8_t opcode, const uint8_t* payload, uint32_t pl_len);

    // onWSSegment is used if RecvSegment == true, called when a segment is received
    // pl_start_idx: index in the whole msg for the 1st byte of payload
    // fin: whether it's the last segment
    void onWSSegment(WSConn& conn, uint8_t opcode, const uint8_t* payload, uint32_t pl_len, uint32_t pl_start_idx,
        bool fin);

    ~Server();

    WSConn* conn;
private:
    WSServer wsserver;
    std::thread ws_thr;
    std::atomic<bool> running;
};

using WSServer = websocket::WSServer<Server>;
using WSConn = WSServer::Connection;

void warp_send(WSConn* conn, std::string msg);