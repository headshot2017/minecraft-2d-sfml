#ifndef _GUIDEFS_H
#define _GUIDEFS_H

typedef void (*Callback)(void *pUserData);
typedef void (*IndexCallback)(int index, void *pUserData);
typedef void (*StringCallback)(std::string str, void *pUserData);

#endif // _GUIDEFS_H
