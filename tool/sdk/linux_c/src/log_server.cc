/******************************************************
# DESC    :
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-05-02 19:04
# FILE    : curl_example.cc
******************************************************/

#include "log_server.h"

#include "curl/curl.h"
#include "rapidjson/document.h"
#include "pbjson/pbjson.h"

using namespace rapidjson;
using namespace std;

enum {
    HTTP_OK = 200
};

static const char*  logHttpClientKey = "X-Forwarded-For:";
static const char*	logBizType       = "pplog-biztype";
static const char*	logBizVersion    = "pplog-bizversion";
static const char*	logBizProtocol   = "pplog-protocol";
static const char*  logBizTransport  = "udp";
static const char*  logURL           = "/api/server/log_servers";


static size_t write_callback(char *contents, size_t size, size_t nmemb, void *userdata)
{
    string http_response(contents, size * nmemb);
    *(string*)userdata = http_response;

    return http_response.size();
}

class curl_opt_handle
{
public:
    curl_opt_handle(void) {
        curl_global_init(CURL_GLOBAL_ALL); // 非线程安全，必须在多线程使用之前初始化
        handle_ = curl_easy_init();
        curl_easy_setopt(handle_, CURLOPT_HEADER, 0);
        curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(handle_, CURLOPT_DNS_CACHE_TIMEOUT, 60);
    }

    CURL * get_curl_handle(void) { return handle_; }

    ~curl_opt_handle(void) {
        curl_easy_cleanup(handle_);
        curl_global_cleanup();
    }

private:
    CURL *handle_;
};

static curl_opt_handle s_curl_opt;

static int parse_http_response(string response, Service& svc)
{
    Document doc;
    doc.Parse(response.c_str());
    if (doc.Parse(response.c_str()).HasParseError() || !doc.HasMember("status")) {
        return IllegalHTTPRsp;
    }

    const Value& status = doc["status"];
    if (status.IsObject()) {
        return HTTPRspStatusError;
    }

    int httpCode = doc["status"].GetInt();
    if (httpCode != HTTP_OK) {
        return HTTPRspStatusError;
    }

    if (!doc.HasMember("service")) {
        return HTTPRspServiceError;
    }

    const Value& service = doc["service"];
    string serviceString = service.GetString();

    std::string err;
    int ret =  pbjson::json2pb(serviceString, &svc, err);
    if (ret != 0) {
        return PbjsonErrorBase + ret;
    }

    return 0;
}

int get_log_nodes(string local_ip, string log_server_host,
            string service_name, string service_version, vector<Node>& nodes)
{
    int ret_code = GetLogNodesSuccess;

    CURL *curl;
    CURLcode code;
    string http_url;
    string http_response;
    Service svc;

    struct curl_slist *headers = NULL;
    struct curl_httppost *post=NULL;
    struct curl_httppost *last=NULL;

    curl = s_curl_opt.get_curl_handle();

    http_url = log_server_host + logURL;
    curl_easy_setopt(curl, CURLOPT_URL, http_url.c_str());

    // http header
    headers = curl_slist_append(headers, "Accept:application/json");
    headers = curl_slist_append(headers, "Cache-Control: no-cache");
    headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
    headers = curl_slist_append(headers, "charset: utf-8");
    headers = curl_slist_append(headers, string(string(logHttpClientKey) + local_ip).c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // post form
    curl_formadd(&post, &last,
            CURLFORM_COPYNAME, logBizType,
            CURLFORM_COPYCONTENTS, service_name.c_str(), CURLFORM_END);
    curl_formadd(&post, &last,
            CURLFORM_COPYNAME, logBizVersion,
            CURLFORM_COPYCONTENTS, service_version.c_str(), CURLFORM_END);
    curl_formadd(&post, &last,
            CURLFORM_COPYNAME, logBizProtocol,
            CURLFORM_COPYCONTENTS, logBizTransport, CURLFORM_END);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&http_response);

    // 此处会单独启动一个线程做DNS解析，每个curl handle都有一个DNS cache，默认超时时间为 60 s
    code = curl_easy_perform(curl);
    if (code != CURLE_OK) {
        // fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(code));
        ret_code = HTTPSendRequstFail;
        goto last;
    } else {
        int ret = parse_http_response(http_response, svc);
        if (ret != 0) {
            ret_code = ret;
            goto last;
        }

        const ::ServiceAttr& attr = svc.attr();
        if (attr.service() != service_name || attr.role() != SRT_Provider) {
            ret_code = HTTPRspWrongAttr;
            goto last;
        }

        if (svc.nodes_size() == 0) {
            ret_code = HTTPRspNodesEmpty;
            goto last;
        }

        nodes.clear();
        for (int i = 0; i < svc.nodes_size(); i++) {
            nodes.push_back(svc.nodes(i));
        }
    }

last:
    curl_slist_free_all(headers);

    return ret_code;
}
