//
// Created by Rahul Nauni on 20/12/2023.
//

#include <iostream>
#include <hiredis/hiredis.h>

int main() {
    // Connect to redis server
    redisContext *context = redisConnect("localhost", 6379);
    if (context == nullptr || context->err) {
        std::cerr << "Error connecting to redis server: " << (context ? context->errstr: "can't allocate redis context") << std::endl;
        exit(1);
    }

    // Set an ip->hits mapping
    const char* ip = "192.168.92.128";
    const char* hits = "100";
    auto* reply = (redisReply*) redisCommand(context, "SET %s %s", ip, hits);
    freeReplyObject(reply);

    // Get the hits for the ip
    reply = (redisReply*) redisCommand(context, "GET %s", ip);
    if (reply->type == REDIS_REPLY_NIL) {
        std::cerr << "No hits for " << ip << std::endl;
        exit(1);
    } else if (reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "Error getting hits for " << ip << ": " << reply->str << std::endl;
        exit(2);
    } else {
        std::cout << "Hits for " << ip << ": " << reply->str << std::endl;
    }
    freeReplyObject(reply);

    // Disconnect from redis server
    redisFree(context);

    return 0;
}