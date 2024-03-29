#ifndef _ARITMETIC_H_INCLUDED_
#define _ARITMETIC_H_INCLUDED_

#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>

void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk);

void equal(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void gte(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void is_negative(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void negativo(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void minimum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void maximum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void shiftl(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void shiftr(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void u_shiftl(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void u_shiftr(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void add_bit(LweSample* result, LweSample* carry_out, const LweSample* a, const LweSample* b, const LweSample* carry_in, const TFheGateBootstrappingCloudKeySet* bk);

void add(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void sub(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void u_mult(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void mult(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void mult_float(LweSample* result, const LweSample* a, const LweSample* b, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void u_div(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void div(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void div_float(LweSample* result, const LweSample* a, const LweSample* b, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

void h_pow(LweSample* result, const LweSample* a, const int n, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void porDiez(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void entreDiez(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void u_reescala(LweSample* result, const LweSample* a, const int nb_bits_result, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);
void reescala(LweSample* result, const LweSample* a, const int nb_bits_result, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);


/*
  Función para transformar un float a un entero para
  trabajar con criptografía Homomórfica, con float_bits BITS de precisión

  @param i  Value to convert
  @param float_bits Bits asigned to the decimals
                    AKA Times the numer will be multiplied
                    by two: i << float_bits
  @return i << float_bits
*/
int64_t float2hint(float i, int float_bits);

/*
  Función para recuperar un float de un entero tras
  trabajar con criptografía Homomórfica, con float_bits BITS de precisión

  @param i  Value to convert
  @param float_bits Bits asigned to the decimals
                    AKA Times the numer will be multiplied
                    by two: i >> float_bits
  @return i >> float_bits
*/
float hint2float(int64_t i, int float_bits);


/**
  Save number to LweSample (like bootsCONSTANT)

  @param n Input number
  @param float_bits Bits which should be protected for decimals
*/
void num2lwe(LweSample* result, const int64_t n, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);

#endif
