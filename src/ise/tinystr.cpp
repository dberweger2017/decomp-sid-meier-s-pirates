#include "tinystr.h"
#include <string.h>

// Original group o-ed797dc8037519569362 (libISELib.a(tinystr.o)).

ISEXmlString::Rep ISEXmlString::nullrep_ = { 0, 0, { '\0' } };

void ISEXmlString::init(size_type sz, size_type cap) {
    if (cap) {
        rep_ = (Rep*)new char[sizeof(Rep) + cap];
        rep_->str[rep_->size = sz] = '\0';
        rep_->capacity = cap;
    } else {
        rep_ = &nullrep_;
    }
}

void ISEXmlString::quit() {
    if (rep_ != &nullrep_) {
        delete[](char*)rep_;
    }
}

ISEXmlString::ISEXmlString(const ISEXmlString& copy) : rep_(0) {
    init(copy.length());
    memcpy(start(), copy.data(), length());
}

ISEXmlString::~ISEXmlString() {
    quit();
}

void ISEXmlString::reserve(size_type cap) {
    if (cap > capacity()) {
        ISEXmlString tmp;
        tmp.init(length(), cap);
        memcpy(tmp.start(), data(), length());
        swap(tmp);
    }
}

ISEXmlString& ISEXmlString::assign(const char* str, size_type len) {
    size_type cap = capacity();
    if (len > cap || cap == 0) {
        ISEXmlString tmp;
        tmp.init(len);
        memcpy(tmp.start(), str, len);
        swap(tmp);
    } else {
        memmove(start(), str, len);
        set_size(len);
    }
    return *this;
}

ISEXmlString& ISEXmlString::append(const char* str, size_type len) {
    size_type newsize = length() + len;
    if (newsize > capacity()) {
        reserve(newsize + capacity());
    }
    memmove(finish(), str, len);
    set_size(newsize);
    return *this;
}
