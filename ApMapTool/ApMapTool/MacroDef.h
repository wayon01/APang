#pragma once
#define NULLPTR(p) { (p) = nullptr; }
#define SAFE_DELETE(p) { if(p) delete (p); NULLPTR(p); }
#define SAFE_DELETE_ARRAY(p) { if(p) delete[] (p); NULLPTR(p); }

// 복사 금지 매크로
#define MAKE_NO_COPY(CLASSNAME)                                             \
        private:                                                            \
               CLASSNAME(const CLASSNAME&);                                 \
               CLASSNAME& operator=(const CLASSNAME&);

// 싱클톤 패턴 생성 매크로
#define DECLARE_SINGLETONE(CLASSNAME)                                       \
        MAKE_NO_COPY(CLASSNAME)                                             \
        private:                                                            \
               CLASSNAME();                                               \
               static CLASSNAME* sInstance;                                 \
        public:                                                             \
               static CLASSNAME* getInstance();                                \
               static void delInstance();

// 싱글톤 패턴 구현 매크로
#define IMPLEMENT_SINGLETON(CLASSNAME)                              \
               CLASSNAME* CLASSNAME::sInstance= NULL;               \
                                                                    \
               CLASSNAME* CLASSNAME::getInstance() {                \
                       if(sInstance == NULL)                        \
                              sInstance = new CLASSNAME;              \
                              return sInstance;                    \
               }                                                    \
               void CLASSNAME::delInstance()                    \
               {                                                    \
                    SAFE_DELETE(sInstance);                        \
               }


#define TILE_SIZE 50

//#define TILE_GOAL	-3
//#define TILE_SPAWN	-2
#define TILE_NONE	-1
//#define TILE_NOMAL	0
//#define TILE_GRASS	1