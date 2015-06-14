#ifdef MK1MF_PLATFORM_VC_WIN32
  /* auto-generated/updated by util/mk1mf.pl for crypto/cversion.c */
  #define CFLAGS "cl  /MD /Ox /O2 /Ob2 -O -DOPENSSL_USE_APPLINK -I. -DOPENSSL_NO_RC5 -DOPENSSL_NO_MD2 -DOPENSSL_NO_KRB5 -DOPENSSL_NO_JPAKE -DOPENSSL_NO_STATIC_ENGINE    "
  #define PLATFORM "VC-WIN32"
  #define DATE "Sat Dec 20 07:55:08 2014"
#endif
#ifdef MK1MF_PLATFORM_BC_NT
  /* auto-generated/updated by util/mk1mf.pl for crypto/cversion.c */
  #define CFLAGS "bcc32 -DWIN32_LEAN_AND_MEAN -q -w-ccc -w-rch -w-pia -w-aus -w-par -w-inl  -c -tWC -tWM -DOPENSSL_SYSNAME_WIN32 -DL_ENDIAN -DDSO_WIN32 -D_stricmp=stricmp -D_strnicmp=strnicmp -D_timeb=timeb -D_ftime=ftime -O2 -ff -fp -DBN_ASM -DMD5_ASM -DSHA1_ASM -DRMD160_ASM -DOPENSSL_NO_RC5 -DOPENSSL_NO_MD2 -DOPENSSL_NO_KRB5 -DOPENSSL_NO_JPAKE -DOPENSSL_NO_DYNAMIC_ENGINE    "
  #define PLATFORM "BC-NT"
  #define DATE "Sat Dec 20 07:55:08 2014"
#endif
