#include "handlers.hpp"

void upload (struct evhttp_request *req, void *arg)
{
    auto *in_buf = evbuffer_new();
    string data, src, dst;
    size_t len = 0;
    
    if (!in_buf)
      return;
    
    in_buf = evhttp_request_get_input_buffer(req);
    
    len = evbuffer_get_length(in_buf);

    data.resize(len);
    evbuffer_copyout(in_buf, &data[0], len);
    
    dst = get_file_place(data);
    
    auto *resp_buf = evbuffer_new();
    auto founded = check_exists(dst);
    
    if (founded.length() > 1) {
        evbuffer_add_printf(resp_buf, "\"%s\"", founded.c_str());
        evhttp_send_reply(req, 302, "Found", resp_buf);
    } else {
        auto storage = get_storage();
        if (place_file(data, dst, storage["path"])) {
            evbuffer_add_printf(resp_buf, "\"%s%s\"", storage["uri"].c_str(), dst.c_str());
            evhttp_send_reply(req, 201, "Created", resp_buf);
        } else {
            evbuffer_add_printf(resp_buf, "false");
            evhttp_send_reply(req, 500, "Internal Server Error", resp_buf);
        }
    }
    
   
    evbuffer_free(resp_buf);
    
    data.clear();
};

void status(struct evhttp_request *req, void *arg)
{
    struct statvfs storage_info;
    unsigned long available = 0;
    unsigned long used = 0;
    unsigned long total = 0;
    bool status = true;
    
    auto *resp_buf = evbuffer_new();
    
    evbuffer_add_printf(resp_buf, "{");
    for (auto storage: config) {
        statvfs(storage.second["path"].c_str(), &storage_info);
        
        available = storage_info.f_bavail * storage_info.f_bsize;
        used = (storage_info.f_blocks  - storage_info.f_bfree) * storage_info.f_frsize;
        total = used + available;
        
        evbuffer_add_printf(resp_buf, "\"%s\": ", storage.first.c_str());
        evbuffer_add_printf(
                resp_buf, 
                "{\"free\": %f, \"used\": %f, \"status:\": \"%s\"},",
                ((float)available/(float)total)*100,
                ((float)used/(float)total)*100,
                ((float)used/(float)total < 0.75)?"OK":"NOT ENOUGH FREE SPACE"
            );
        
        if (status && ((float)used/(float)total > 0.75)) {
            status = false;
        }
        
    }
    evbuffer_add_printf(resp_buf, "\"allstatus\":\"%s\"", status?"OK":"NOT ENOUGH FREE SPACE");
    evbuffer_add_printf(resp_buf, "}");
    
    evhttp_send_reply(req, HTTP_OK, "OK", resp_buf);
    evbuffer_free(resp_buf);
}

string get_file_place(string content)
{
    uint32_t hash[4];
    char *result = new char[255];
    string ext;
    
    MurmurHash3_x64_128(content.c_str(), content.size(), 42, hash);
    
    ext = get_extension(content);
    
    sprintf(result, "/%08x/%08x/%08x/%08x%s",  hash[0],hash[1],hash[2],hash[3], ext.c_str());
    
    return result;
}

bool place_file(string content, string dst, string path)
{
    path = path + dst.substr(0, dst.find_last_of("/"));
    string filename = path + dst.substr(dst.find_last_of("/"));
    
    struct stat st;
    if (stat(path.c_str(), &st) != 0 || S_ISDIR(st.st_mode)) {
        string cmd = "mkdir -p ";
        cmd += path;
        system(cmd.c_str());
    }
    
    ofstream os(filename);  
    if (!os) { 
        std::cerr << "Error writing " << filename << std::endl;
        return false;
    } else {
        os << content;  
    }
    
    return true;
}

unordered_map<string, string> get_storage()
{
    struct statvfs storage_info;
    unsigned long available = 0;
    unordered_map<string, string> selected;
    
    for (auto storage: config) {
        statvfs(storage.second["path"].c_str(), &storage_info);
        
        if (storage_info.f_bavail * storage_info.f_bsize > available) {
            available = storage_info.f_bavail * storage_info.f_bsize;
            selected = storage.second;
        } 
    }
    
    return selected;
}

string get_extension(string content)
{   
    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    string extension;
    magic_load(magic, NULL);
    const char *mime_type = magic_buffer(magic, content.c_str(), content.size());
    extension = mimetypes[mime_type];
    magic_close(magic);
    return extension;
}

string check_exists (string path)
{
    string filename;
    string founded = "";
    
    for (auto storage: config) {
        filename = storage.second["path"] + path;
        
        struct stat buffer;
        int res = stat (filename.c_str() ,&buffer);
        
        if (res == 0) {
            founded = storage.second["uri"] + path;
            break;
        }
    }
    
    return founded;
}