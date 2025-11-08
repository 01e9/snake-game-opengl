#ifndef INTERFACE_COMMON

// best practices for interface

#define INTERFACE_COMMON(TypeName) \
public: \
    virtual ~TypeName() noexcept = default; \
\
    TypeName(const TypeName&) = delete; \
    TypeName& operator=(const TypeName&) = delete; \
\
protected: \
    TypeName() = default;

#endif