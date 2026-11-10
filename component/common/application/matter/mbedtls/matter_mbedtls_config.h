#ifndef MATTER_MBEDTLS_CONFIG_H
#define MATTER_MBEDTLS_CONFIG_H

#include <mbedtls/version.h>

#if MBEDTLS_VERSION_MAJOR == 2
#include "mbedtls_config.h"
#elif MBEDTLS_VERSION_MAJOR == 3
#include "mbedtls_config_v3.h"
#else
#error "Unsupported Mbed TLS version"
#endif

#endif /* MATTER_MBEDTLS_CONFIG_H */