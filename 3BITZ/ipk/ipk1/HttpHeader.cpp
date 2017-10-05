#include <ctime>

#include "HttpHeader.h"

namespace date {
    const char *days[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", NULL,};
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL,};
}

namespace http {
    const char *encoding[] = {
            "identity",
            "gzip",
            "deflate",
            NULL,
    };

    HttpHeader::HttpHeader(request_t req_type, file_t file_type, std::string url, std::string path) {
        this->request_type = req_type;
        this->file_type = file_type;
        this->url = url;
        this->path = path;
    }

    std::string HttpHeader::make_request() {
        std::string request;
        switch (this->request_type) {
            case GET:
                request += "GET";
                break;
            case PUT:
                request += "PUT";
                break;
            case DELETE:
                request += "DELETE";
                break;
        }

        request += " " + path + "?type=";

        switch (this->file_type) {
            case FILE:
                request += "file";
                break;
            case FOLDER:
                request += "folder";
                break;
            default:
                throw "WTF";
        }

        request += " HTTP/1.1" + std::string(http::CRLF);

        request += this->make_time_stamp() + http::CRLF;

        request += "Accept: "+ this->content_type + std::string(http::CRLF);
        request += "Accept-Encoding: identity" + std::string(http::CRLF);

        if (this->request_type == PUT) {
            if (this->file_type == FOLDER) {
                request += "Content-Type: text/plain" + std::string(http::CRLF);
                request += "Content-Length: 0" + std::string(http::CRLF);
            } else {
                request += "Content-Type: "+ this->content_type + std::string(http::CRLF);
                request += "Content-Length: " + this->content_lenght;
                request += http::CRLF;
            }
        }


        request += CRLF;

        return request;
    }


    std::string HttpHeader::make_response(int http_code, std::string http_status_msg) {
        std::string response;
        std::string CRLF = http::CRLF;
        response = "HTTP/1.1 " + std::to_string(http_code) + " " + http_status_msg + CRLF;

        response += this->make_time_stamp() + CRLF;

        if (this->request_type == GET) {
            response += "Content-Type: "+ this->content_type + CRLF;
            response += "Content-Length: " + this->content_lenght + CRLF;
            response += "Content-Encoding: " + std::string(encoding[this->content_encoding]) + CRLF ;
        }



        response += CRLF;

        return response;
    }

    std::string HttpHeader::make_time_stamp() {
        std::string timestamp;

        time_t current_time = time(0);
        tm *local_time = localtime(&current_time);

        timestamp += "Date: " + std::string(date::days[local_time->tm_wday]) + ", " +
                     std::to_string(local_time->tm_mday) + " " + date::months[local_time->tm_mon] + " " +
                     std::to_string(local_time->tm_year + 1900) + " " + std::to_string(local_time->tm_hour) +
                     ":" + std::to_string(local_time->tm_min) + ":" + std::to_string(local_time->tm_sec) + " " +
                     local_time->tm_zone;

        return timestamp;
    }

}