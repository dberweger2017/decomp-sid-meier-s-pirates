#pragma once

#include <stddef.h>

class ISEXmlString {
public:
    typedef size_t size_type;
    static const size_type npos;

    struct Rep {
        size_type size;
        size_type capacity;
        char str[1];
    };

    ISEXmlString() : rep_(&nullrep_) {}
    ISEXmlString(const ISEXmlString& copy);
    ~ISEXmlString();

    size_type length() const { return rep_->size; }
    size_type capacity() const { return rep_->capacity; }
    const char* c_str() const { return rep_->str; }
    const char* data() const { return rep_->str; }
    bool empty() const { return length() == 0; }

    void clear() {
        quit();
        init(0, 0);
    }

    void reserve(size_type cap);
    ISEXmlString& assign(const char* str, size_type len);
    ISEXmlString& append(const char* str, size_type len);

    void swap(ISEXmlString& other) {
        Rep* r = rep_;
        rep_ = other.rep_;
        other.rep_ = r;
    }

private:
    void init(size_type sz) { init(sz, sz); }
    void init(size_type sz, size_type cap);
    void quit();
    void set_size(size_type sz) { rep_->str[rep_->size = sz] = '\0'; }
    char* start() const { return rep_->str; }
    char* finish() const { return rep_->str + rep_->size; }

    Rep* rep_;
    static Rep nullrep_;
};
