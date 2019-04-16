#ifdef _WIN32
  #include <Windows.h>
#else
  #include <dlfcn.h>
#endif

#include "DynamicLibrary.h"
#include <sstream>
#include <iostream>

DynamicLibrary::DynamicLibrary(void * handle) : handle_(handle)
{
}

DynamicLibrary::~DynamicLibrary()
{
  if (handle_)
  {
  #ifndef _WIN32
    ::dlclose(handle_);
  #else
    ::FreeLibrary((HMODULE)handle_);
  #endif
  }
}

DynamicLibrary * DynamicLibrary::load(const std::string & name, 
                                      std::string & errorString)
{
  if (name.empty()) 
  {
    errorString = "Empty path.";
    return nullptr;
  }
  
  void * handle = nullptr;

  #ifdef _WIN32
    handle = ::LoadLibraryA(name.c_str());
    if (handle == NULL)
    {
      DWORD errorCode = ::GetLastError();
      std::stringstream ss;
      ss << std::string("LoadLibrary(") << name 
         << std::string(") Failed. errorCode: ") 
         << errorCode; 
      errorString = ss.str();
    }
  #else
    handle = ::dlopen(name.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!handle) 
    {
      std::string dlErrorString;
      const char *zErrorString = ::dlerror();
      if (zErrorString)
        dlErrorString = zErrorString;
      errorString += "Failed to load \"" + name + '"';
      if(dlErrorString.size())
        errorString += ": " + dlErrorString;
      return nullptr;
    }

  #endif
  return new DynamicLibrary(handle);
}

void * DynamicLibrary::getSymbol(const std::string & symbol)
{
  if (!handle_)
    return nullptr;
  
  #ifdef _WIN32
    return ::GetProcAddress((HMODULE)handle_, symbol.c_str());
  #else
    return ::dlsym(handle_, symbol.c_str());
  #endif
}

