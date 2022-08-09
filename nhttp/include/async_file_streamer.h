#pragma once
#include "common.h"
#include "App.h"
#include "util.h"
class AsyncFileStreamer: public std::enable_shared_from_this<AsyncFileStreamer>  {
    std::string file_path_;
    uint64_t file_len_, range_begin_, req_len_;
    uWS::Loop *loop_;
    std::vector<char> buff_;
    std::shared_ptr<std::ifstream> ifs_;
    uWS::HttpResponse<false> *res_;
public:
    AsyncFileStreamer(std::string file_path, auto *res, auto *req)
    : file_path_(file_path), range_begin_(0), res_(res)
    {
        // shared_from_this can not be call in constructor
        // auto self = shared_from_this();
        loop_ = uWS::Loop::get();
        buff_.resize(128 * 1024);
        ifs_ = std::make_shared<std::ifstream>();
        ifs_->open(file_path_, std::ifstream::in | std::ios::binary | std::ios::ate);
        file_len_ = ifs_->tellg();
        // printf("[%s] len=%llu\n", file_path.c_str(), file_len_);
        // file_len_ = Util::file_size(file_path);
        // printf("[%s] len=%llu\n", file_path.c_str(), file_len_);
        // printf("print headers begin--------------------\n");
        // for(auto i = req->begin(); i != req->end(); i++)
        // {
        //     std::cout << (*i).first << "=" << (*i).second << std::endl;
        // }
        // printf("print headers end-----------------------\n");
        auto range = std::string{req->getHeader("range")};
        req_len_ = file_len_;
        if (!range.empty())
        {
            // std::cout << "received req range=" << range << std::endl;
            boost::replace_all(range, "bytes=", "");
            auto vs = Util::split(range, "-");
            uint64_t begin = std::stoull(vs[0]);
            uint64_t end = vs[1] == "" ? file_len_ : std::stoull(vs[1]);
            // [begin, end] not [begin, end)
            end = std::min(file_len_ - 1, end);
            req_len_ = (end - begin) + 1;
            range = "bytes " + vs[0] + "-" + std::to_string(end) + "/" + std::to_string(file_len_);
            range_begin_ = begin;
            // std::cout << "Content-Range=" << range << std::endl;
            // writeStatus must be first call
            res->writeStatus("206 Partial Content");
            res->writeHeader("Content-Range", range);   
        }
        if (Util::is_pac(file_path_))
        {
            res->writeHeader("Cache-Control", "no-cache, no-store, must-revalidate");
        }
        else
        {
            res->writeHeader("Cache-Control", "max-age=86400");
        }
        // for firefox need to know you can handle range seeking
        res->writeHeader("Accept-Ranges", "bytes");
        // res->writeHeader("Connection", "keep-alive");
        // printf("file_len_=%llu, req_len_=%llu, range_begin_=%llu\n", file_len_, req_len_, range_begin_);
        // res->tryEnd() already set "Content-Length"
        // res->writeHeader("Content-Length", std::to_string(req_len_));
        auto ft = Util::mime_type(file_path_);
        // LOG("%1% file type = %2%", file_path_, ft)
        res->writeHeader("Content-Type", ft);
    }
    void streamFile()
    {
        auto res = res_;
        // can not set header in recursive func
        auto self = shared_from_this();
        if (!*ifs_)
        {
            // or try to reopen it?
            ifs_->close();
            std::cout << "Reopening fin:"<< file_path_ << std::endl;
            ifs_->open(file_path_, std::ios::binary);
            if( ! ifs_->good() ) throw std::invalid_argument(file_path_ + " not exist");;
        }
        ifs_->seekg(range_begin_ + res->getWriteOffset(), std::ios::beg);
        std::streamsize rc = ifs_->read( &buff_[0], 
            std::min(buff_.size(), 
            static_cast<size_t>(file_len_ - range_begin_ )
            - res->getWriteOffset()) ).gcount();
        auto chunk = std::string_view(&buff_[0], rc);
        if (chunk.length() == 0)
        {
            // printf("[%s] chunk.length() == 0, exit\n", file_path_.c_str());
            return;
        }
            
        // LOG("serve[%1%] %2%, req_len_=%3%", file_path_, chunk.size(), req_len_);
        if (res->tryEnd(chunk, req_len_).first)
        {
            streamFile();
        }
        else
        {
            res->onWritable([self](int offset) {
                // printf("onWritable rewrite[%s] offset=%d\n", self->file_path_.c_str(), offset);
                // std::async(std::launch::async, [res, self]() {
                //     self->loop()->defer([res, self]() {
                        self->streamFile();
                //     });
                // });
                return false;
            })
            ->onAborted([res, self]() {
                // printf("in abort, res->getWriteOffset=%u, file_path_=%s\n", 
                //     res->getWriteOffset(), self->file_path_.c_str());
                // std::cout << self->file_path_ << ": ABORTED!" << std::endl;
            });
        }
    }
};
