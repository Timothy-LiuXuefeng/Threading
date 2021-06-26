#pragma once

#ifndef TIMOTHY_H

#define TIMOTHY_H

#ifndef __cplusplus
#error The library 'timothy' is only for C++.
#endif

#define TIMOTHY_NAMESPACE_BEGIN namespace timothy {
#define TIMOTHY_NAMESPACE_END }
#define TIMOTHY_CONCURRENT_NAMESPACE_BEGIN TIMOTHY_NAMESPACE_BEGIN namespace concurrent {
#define TIMOTHY_CONCURRENT_NAMESPACE_END TIMOTHY_NAMESPACE_END }

#endif // !TIMOTHY_H

