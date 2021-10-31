#include <cpuid.h>
#include <cpuidflags.h>
#include <lib.h>
#include <utils.h>

#define BUFFER_SIZE 16

#define ISHEXA(x)                                                              \
  (((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F') || ISDIGIT(x))

#define TO_LOWER(c) ((c) >= 'a' && (c) <= 'z' ? (c) - ('a' - 'A') : (c))

void printDate() {
  dateType currDate;
  fillDate(&currDate);
  print_f(1, "Date: %d/%d/%d\nTime: %d:%d:%d (UTC)\n", currDate.day,
          currDate.month, currDate.year + 2000, currDate.hour, currDate.minute,
          currDate.second);
}

void printmem() {
  uint8_t arr[BYTES_TO_READ] = {0};
  char buffer[BUFFER_SIZE + 1] = {0};
  uint64_t dir = 0;
  int ans;

  do {
    print_f(1,
            "Ingrese una direccion de 64 bits a partir de la cual leer:\n0x");
    ans = get_s(buffer, BUFFER_SIZE);
  } while (ans == -1);

  for (int i = 0; i < ans; i++) {
    if (!ISHEXA(buffer[i])) {
      print_f(1, "\nNo es una direccion valida\n");
      return;
    }
  }

  sscan(buffer, "%x", &dir);

  // fillMem debería devolver la cantidad de bytes leídos correctamente
  // Actualmente como no hay validación, siempre es 32
  // print_f(1, "%x\n", fillMem(dir, arr, BYTES_TO_READ));
  fillMem(dir, arr, BYTES_TO_READ);
  for (int i = 0; i < BYTES_TO_READ; i++)
    print_f(1, "\n0x%x: %xh", dir + i, arr[i]);

  print_f(1, "\n");
}

void printFeatures() {
  // Chequear si se pude usar cpuid
  uint32_t eax, ebx, ecx, edx;

  if (supports_cpuid()) {
    print_f(1, "CPUID: YES\n");

    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    print_f(1, "MX: %s\n", (edx & CPUID_FEAT_EDX_MMX) == 0 ? "NO" : "YES");
    print_f(1, "SSE: %s\n", (edx & CPUID_FEAT_EDX_SSE) == 0 ? "NO" : "YES");
    print_f(1, "SSE2: %s\n", (edx & CPUID_FEAT_EDX_SSE2) == 0 ? "NO" : "YES");
    print_f(1, "SSE3: %s\n", (ecx & CPUID_FEAT_ECX_SSE3) == 0 ? "NO" : "YES");
    print_f(1, "SSE41: %s\n",
            (ecx & CPUID_FEAT_ECX_SSE4_1) == 0 ? "NO" : "YES");
    print_f(1, "SSE42: %s\n",
            (ecx & CPUID_FEAT_ECX_SSE4_2) == 0 ? "NO" : "YES");
    print_f(1, "AESNI: %s\n", (ecx & CPUID_FEAT_ECX_AES) == 0 ? "NO" : "YES");
    print_f(1, "PCLMULQDQ: %s\n",
            (ecx & CPUID_FEAT_ECX_PCLMUL) == 0 ? "NO" : "YES");
    print_f(1, "AVX: %s\n", (ecx & CPUID_FEAT_ECX_AVX) == 0 ? "NO" : "YES");
    print_f(1, "VAESNI: %s\n", supports_vaesni() == 0 ? "NO" : "YES");
    print_f(1, "VPCLMULQDQ: %s\n", supports_vpclmulqdq() == 0 ? "NO" : "YES");
    print_f(1, "F16C: %s\n", supports_f16c() == 0 ? "NO" : "YES");
    print_f(1, "FMA: %s\n", supports_fma() == 0 ? "NO" : "YES");
    print_f(1, "AVX2: %s\n", supports_avx2() == 0 ? "NO" : "YES");
  } else {
    print_f(1, "CPUID: NO\n");
  }
}

void printQuadraticRoots() {
  double a, b, c, x1, x2;
  char buffer[100 + 1] = {0};
  int ans;

  do {
    print_f(1, "Ingresar coeficientes a, b y c: ");
    ans = get_s(buffer, 100);
  } while (ans == -1);

  sscan(buffer, "%g %g %g", &a, &b, &c);

  put_char(1, '\n');

  print_f(1, "%c(x) = %gx%c%c%gx%c%g\n\n", 13, a, 14, b >= 0 ? '+' : 0, b,
          c >= 0 ? '+' : 0, c);
  ans = _quadratic(&a, &b, &c, &x1, &x2);
  switch (ans) {
  case 0:
    print_f(1, "Sol = {x%c = %g, x%c = %g}\n", 11, x1, 12, x2);
    break;
  case 1:
    print_f(1, "%c tiene raices complejas.\n", 13);
    break;
  case 2:
    print_f(1, "%c no es una funcion cuadratica.\n", 13);
    break;
  };
}

static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint() {
  m_z = 36969 * (m_z & 65535) + (m_z >> 16);
  m_w = 18000 * (m_w & 65535) + (m_w >> 16);
  return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max) {
  uint32_t u = GetUint();
  return (u + 1.0) * 2.328306435454494e-10 * max;
}

uint8_t memcheck(void *start, uint8_t value, uint32_t size) {
  uint8_t *p = (uint8_t *)start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value)
      return 0;

  return 1;
}

int atoi(const char *s) { return ((int)_Stoul(s, NULL, 10)); }

/* macros */
#define BASE_MAX 36 /* largest valid base */
/* static data */
static const char digits[] = {/* valid digits */
                              "0123456789abcdefghijklmnopqrstuvwxyz"};
static const char ndigs[BASE_MAX + 1] = {
    /* 32-bits! */
    0, 0, 33, 21, 17, 14, 13, 12, 11, 11, 10, 10, 9, 9, 9, 9, 9, 8, 8,
    8, 8, 8,  8,  8,  7,  7,  7,  7,  7,  7,  7,  7, 7, 7, 7, 7, 7,
};

void *memchr(const void *s, int c,
             size_t n) { /* find first occurrence of c in s[n] */
  const unsigned char uc = c;
  const unsigned char *su = (const unsigned char *)s;

  for (; 0 < n; ++su, --n)
    if (*su == uc)
      return ((void *)su);
  return (NULL);
}

unsigned long
_Stoul(const char *s, char **endptr,
       int base) { /* convert string to unsigned long, with checking */
  const char *sc, *sd;
  const char *s1, *s2;
  char dig, sign;
  ptrdiff_t n;
  unsigned long x, y;
  sc = s;
  sign = *sc == '-' || *sc == '+' ? *sc++ : '+';
  if (base < 0 || base == 1 || BASE_MAX < base) { /* silly base */
    if (endptr)
      *endptr = (char *)s;
    return (0);
  } else if (0 < base) { /* strip 0x or 0X */
    if (base == 16 && *sc == '0' && (sc[1] == 'x' || sc[1] == 'X'))
      sc += 2;
  } else if (*sc != '0')
    base = 10;
  else if (sc[1] == 'x' || sc[1] == 'X')
    base = 16, sc += 2;
  else
    base = 8;
  for (s1 = sc; *sc == '0'; ++sc)
    ; /* skip leading zeros */
  x = 0;
  for (s2 = sc; (sd = (char *)memchr(digits, TO_LOWER(*sc), base)) != NULL;
       ++sc) {                /* accumulate digits */
    y = x, dig = sd - digits; /* for overflow checking */
    x = x * base + dig;
  }
  if (s1 == sc) { /* check string validity */
    if (endptr)
      *endptr = (char *)s;
    return (0);
  }
  n = sc - s2 - ndigs[base];
  if (sign == '-') /* get final value */
    x = -x;
  if (endptr)
    *endptr = (char *)sc;
  return (x);
}

void *memset(void *s, int c,
             size_t n) { /* store c throughout unsigned char s[n] */
  const unsigned char uc = c;
  unsigned char *su = (unsigned char *)s;

  for (; 0 < n; ++su, --n)
    *su = uc;
  return (s);
}
