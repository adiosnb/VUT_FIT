#ifndef HTTP_HEADER
#define HTTP_HEADER

#include <iostream>

namespace http {

    enum request_t { GET = 0, PUT, DELETE, RESPONSE};
    enum file_t {FILE = 0, FOLDER, NOTHING};
    enum encoding_t {IDENTITY = 0, GZIP, DEFLATE};
    const char CRLF[] = "\r\n";
    const char header_end[] = "\r\n\r\n";


    /* http header example
     * DELETE /USER-ACCOUNT/REMOTE-PATH?type=folder HTTP/1.1
     * Date  - Timestamp klienta v době vytvoření požadavku.
     * Accept - Požadavaný typ obsahu pro odpověď       # only request
     * Accept-Encoding - Podporový způsob kódování dat (identity, gzip, deflate) # only request
     * Content-Type - MIME typ obsahu požadavku (pro PUT či POST)
     * Content-Length - Délka obsahu požadavku (pro PUT či POST)
     * Content-Encoding - typ kódování obsahu (identity, gzip, deflate)      #only response
     */


    /**
     * Class HttpHeader - stores values and create http header, which is send through socket to server/client.
     */
    class HttpHeader {
    public :

        request_t request_type;    /**< GET/PUT/DELETE */
        file_t file_type;          /**< FILE/FOLDER */
        std::string url;
        std::string path;

        std::string content_type = "application/octet-stream";
        std::string content_lenght = "0";
        encoding_t content_encoding = IDENTITY;

        /**
         * Constructor for HttpHeader. Initialize parameters for header.
         * @param req_type GET/PUT/DELETE
         * @param file_type FOLDER/FILE
         * @param url
         * @param path
         */
        HttpHeader(request_t req_type, file_t file_type, std::string url, std::string path);

        /**
         * Creating request message for client to contact server according to attributes.
         * @return String which includes http header.
         */
        std::string make_request();

        /**
         * Creating response to client message.
         * @param http_code 200/400/404
         * @param http_status_msg  Http status string.
         * @return string including http response.
         */
        std::string make_response(int http_code, std::string http_status_msg);

        /**
         * Creating timestamp for http header.
         * @return timestamp
         */
        std::string make_time_stamp();
    };


}

#endif //HTTP_HEADER