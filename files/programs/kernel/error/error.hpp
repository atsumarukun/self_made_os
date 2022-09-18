#pragma once

#include <array>

using namespace std;

class Error {
    public:
        enum ErrorCode {
            Success,
            Test,
        };
        Error(ErrorCode error_code, const char* file, int line): error_message_{error_names_[error_code]}, file_{file}, line_{line} {}
        char* Message();

    private:
        const char* error_message_;
        const char* file_;
        int line_;
        static constexpr array error_names_ {
            "Cuccess",
            "Test",
        };
};

#define MakeError(error_code) Error(error_code, __FILE__, __LINE__)