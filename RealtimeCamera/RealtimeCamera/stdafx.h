// stdafx.h: dołącz plik do standardowych systemowych plików dołączanych,
// lub specyficzne dla projektu pliki dołączane, które są często wykorzystywane, ale
// są rzadko zmieniane
//

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <cstdint>
#include <iostream>

#define LOG(msg) printf("LOG: %s\n", msg);
#define WIDTH 400
#define HEIGHT 300

typedef struct Frame {
	int* content = NULL;
} Frame;

const int NumerOfThreads = 4;


// TODO: W tym miejscu odwołaj się do dodatkowych nagłówków wymaganych przez program
