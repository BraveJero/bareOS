#include <keyboard.h>

static void zero_division();
static void invalid_opcode();

char *exceptionMessages[EXCEPTION_COUNT] = {
    "[Exception] Division By Zero", /* 0x00 */
    0,
    0,
    0,
    0,
    0,
    "[Exception] Invalid OpCode", /* 0x06 */
    0,
};

void exceptionDispatcher(int exception) {
  // Los registros se muestran en el dispatcher
  if (exception < 0 || exception >= EXCEPTION_COUNT)
    return;

  // Mostrar mensaje de qué excepción ocurrió
  char *message = exceptionMessages[exception];
  if (message != 0) {
    ncPrintAtt(message, &RED, &BLACK);
    ncNewline();
    ncNewline();
  }

  if (exception == ZERO_EXCEPTION_ID)
    zero_division();

  if (exception == INVALID_OPCODE_ID)
    invalid_opcode();

  // Llego y están apagadas las interrupciones
  // Pooling -> leer directo del puerto del teclado
  // while(1) hlt; readBuffer != -1
  // Esperar tecla
  ncPrint("Presione enter para continuar\n");
  char sc = 0;
  do {
    stdRead(&sc, 1);
  } while (sc != '\n');

  // Clear window
  clearWindow(getCurrentPrompt(), &BLACK);
  ((EntryPoint)sampleCodeModuleAddress)();
}

static void zero_division() {
  // Handler para manejar excepción
}

static void invalid_opcode() {
  // Handler para manejar excepción
}
