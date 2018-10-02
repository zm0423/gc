#ifndef ERRORR
#define ERRORR

#ifndef NO_ERROR
#define errorr(a) unsigned int _____key;\
                  Bdisp_AllClr_DDVRAM();\
                  fxprintf(1, 1, const_cast<char *>(a));\
                  while(1)GetKey(&_____key);

#endif

#endif
