#include "arithmetic.h"

// elementary full comparator gate that is used to compare the i-th bit:
//   input: ai and bi the i-th bit of a and b
//          lsb_carry: the result of the comparison on the lowest bits
//   algo: if (a==b) return lsb_carry else return b
void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk) {
  bootsXNOR(tmp, a, b, bk);
  bootsMUX(result, tmp, lsb_carry, a, bk);
}

void is_negative(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  bootsCOPY(&result[0], &a[nb_bits-1], bk);
}

// Devuelve -a
void negativo(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){

  LweSample* ha_cambiado = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* not_x = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  for(int i = 0; i < 2; i++){
    bootsCONSTANT(&ha_cambiado[i], 0, bk);
    bootsCONSTANT(&not_x[i], 0, bk);
  }

  for(int i = 0; i < nb_bits; i++){
    bootsNOT(&not_x[0], &a[i], bk);
    bootsMUX(&result[i], &ha_cambiado[0], &not_x[0], &a[i], bk);
    bootsOR(&ha_cambiado[0], &ha_cambiado[0], &a[i], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(2, ha_cambiado);
  delete_gate_bootstrapping_ciphertext_array(2, not_x);
}

// Returns a == b
void equal(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){

  LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  bootsCONSTANT(&result[0], 1, bk);

  for(int i = 0; i < nb_bits; i++){
    bootsXNOR(&tmps[0], &a[i], &b[i], bk);
    bootsAND(&result[0], &result[0], &tmps[0], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(2, tmps);

}

// this function compares two multibit words, and puts the min in result
void minimum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
  LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* aGreater = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  LweSample* minimumMismoSigno = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* minimumOneNegative = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* oneNegative = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* negativoA = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* negativoB = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  is_negative(negativoA, a, nb_bits, bk);
  is_negative(negativoB, b, nb_bits, bk);

  bootsXOR(&oneNegative[0], &negativoA[0], &negativoB[0], bk);

  // a > b = soloOneNegative & is_negative(b)
  bootsAND(&aGreater[0], &oneNegative[0], &negativoB[0], bk);
  for(int i = 0; i < nb_bits; i++){
    bootsMUX(&minimumOneNegative[i], &aGreater[0], &b[i], &a[i], bk);
  }

  //initialize the carry to 0
  bootsCONSTANT(&tmps[0], 0, bk);

  //run the elementary comparator gate n times
  for (int i=0; i<nb_bits; i++) {
      compare_bit(&tmps[0], &a[i], &b[i], &tmps[0], &tmps[1], bk);
  }

  //tmps[0] is the result of the comparaison: 0 if a is larger, 1 if b is larger
  //select the max and copy it to the result
  for (int i=0; i<nb_bits; i++) {
      bootsMUX(&minimumMismoSigno[i], &tmps[0], &b[i], &a[i], bk);
  }

  // Resultado en funcion de si comparamos entre mismo signo o no
  for (int i=0; i<nb_bits; i++) {
      bootsMUX(&result[i], &oneNegative[0], &minimumOneNegative[i], &minimumMismoSigno[i], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(2, tmps);
  delete_gate_bootstrapping_ciphertext_array(2, aGreater);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, minimumMismoSigno);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, minimumOneNegative);
  delete_gate_bootstrapping_ciphertext_array(2, oneNegative);
  delete_gate_bootstrapping_ciphertext_array(2, negativoA);
  delete_gate_bootstrapping_ciphertext_array(2, negativoB);
}

// this function compares two multibit words, and puts the min in result
void maximum(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
    LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);
    LweSample* aGreater = new_gate_bootstrapping_ciphertext_array(2, bk->params);

    LweSample* minimumMismoSigno = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* minimumOneNegative = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
    LweSample* oneNegative = new_gate_bootstrapping_ciphertext_array(2, bk->params);
    LweSample* negativoA = new_gate_bootstrapping_ciphertext_array(2, bk->params);
    LweSample* negativoB = new_gate_bootstrapping_ciphertext_array(2, bk->params);

    is_negative(negativoA, a, nb_bits, bk);
    is_negative(negativoB, b, nb_bits, bk);

    bootsXOR(&oneNegative[0], &negativoA[0], &negativoB[0], bk);

    // a > b = soloOneNegative & is_negative(b)
    bootsAND(&aGreater[0], &oneNegative[0], &negativoB[0], bk);
    for(int i = 0; i < nb_bits; i++){
      bootsMUX(&minimumOneNegative[i], &aGreater[0], &b[i], &a[i], bk);
    }

    //initialize the carry to 0
    bootsCONSTANT(&tmps[0], 0, bk);

    //run the elementary comparator gate n times
    for (int i=0; i<nb_bits; i++) {
        compare_bit(&tmps[0], &a[i], &b[i], &tmps[0], &tmps[1], bk);
    }

    //tmps[0] is the result of the comparaison: 0 if a is larger, 1 if b is larger
    //select the max and copy it to the result
    for (int i=0; i<nb_bits; i++) {
        bootsMUX(&minimumMismoSigno[i], &tmps[0], &b[i], &a[i], bk);
    }

    // Todo igual que en minimum, pero devolviendo el contrario
    for (int i=0; i<nb_bits; i++) {
        bootsMUX(&result[i], &oneNegative[0], &minimumMismoSigno[i], &minimumOneNegative[i], bk);
    }

    delete_gate_bootstrapping_ciphertext_array(2, tmps);
    delete_gate_bootstrapping_ciphertext_array(2, aGreater);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, minimumMismoSigno);
    delete_gate_bootstrapping_ciphertext_array(nb_bits, minimumOneNegative);
    delete_gate_bootstrapping_ciphertext_array(2, oneNegative);
    delete_gate_bootstrapping_ciphertext_array(2, negativoA);
    delete_gate_bootstrapping_ciphertext_array(2, negativoB);

}

void add_bit(LweSample* result, LweSample* carry_out, const LweSample* a, const LweSample* b, const LweSample* carry_in, const TFheGateBootstrappingCloudKeySet* bk){

  LweSample* s1 = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* c1 = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* c2 = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  bootsCONSTANT(&s1[0], 0, bk);
  bootsCONSTANT(&c1[0], 0, bk);
  bootsCONSTANT(&c2[0], 0, bk);

  bootsXOR(s1, a, b, bk);
  bootsXOR(result, s1, carry_in, bk);

  bootsAND(c1, s1, carry_in, bk);
  bootsAND(c2, a, b, bk);

  bootsOR(carry_out, c1, c2, bk);

  delete_gate_bootstrapping_ciphertext_array(2, s1);
  delete_gate_bootstrapping_ciphertext_array(2, c1);
  delete_gate_bootstrapping_ciphertext_array(2, c2);

}

void add(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
  LweSample* tmps_carry = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  //initialize the carry to 0
  bootsCONSTANT(&tmps_carry[0], 0, bk);

  //run the elementary comparator gate n times
  for (int i=0; i<nb_bits; i++) {
      add_bit(&result[i], &tmps_carry[0], &a[i], &b[i], &tmps_carry[0], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(2, tmps_carry);

}

void sub(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* restando = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  negativo(restando, b, nb_bits, bk);
  add(result, a, restando, nb_bits, bk);

  delete_gate_bootstrapping_ciphertext_array(nb_bits, restando);
}

// TODO Nueva multiplicación
void old_mult(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
  LweSample* tmps = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* factor = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* sumando = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* cero = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* uno = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  // TODO result y res_aux tienen que tener el doble de bits que nb_bits
  LweSample* res_aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  bootsCONSTANT(&tmps[0], 0, bk);
  for(int i=0; i < nb_bits; i++){
    bootsCONSTANT(&result[i], 0, bk);
    bootsCONSTANT(&aux[i], 0, bk);
    bootsCONSTANT(&aux2[i], 0, bk);
    bootsCONSTANT(&factor[i], 0, bk);
    bootsCONSTANT(&sumando[i], 0, bk);
    bootsCONSTANT(&cero[i], 0, bk);
    bootsCONSTANT(&uno[i], 0, bk);
  }
  bootsCONSTANT(&uno[0], 1, bk);

  for(int i = 0; i < pow(2, nb_bits); i++){

    equal(tmps, aux, b, nb_bits, bk);

    for(int j = 0; j < nb_bits; j++){
      bootsMUX(&sumando[j], &tmps[0], &cero[j], &uno[j], bk);
      bootsMUX(&factor[j], &tmps[0], &cero[j], &a[j], bk);
    }


    add(aux2, aux, sumando, nb_bits, bk);
    for(int j = 0; j < nb_bits; j++){
      bootsCOPY(&aux[j], &aux2[j], bk);
    }

    add(res_aux, result, factor, nb_bits, bk);
    for(int j = 0; j < nb_bits; j++){
      bootsCOPY(&result[j], &res_aux[j], bk);
    }

  }

  delete_gate_bootstrapping_ciphertext_array(2, tmps);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, factor);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, sumando);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, cero);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, uno);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, res_aux);
}

void u_mult(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {

  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  for(int i = 0; i < nb_bits; i++){
    bootsCONSTANT(&aux[i], 0, bk);
    bootsCONSTANT(&aux2[i], 0, bk);
  }

  // Multiplica opA * opB
  for(int i = 0; i < (nb_bits/2); i++) {

    // Resetea auxs
    for(int j = 0; j < nb_bits; j++){
      bootsCONSTANT(&aux[j], 0, bk);
      bootsCONSTANT(&aux2[j], 0, bk);
    }
    for(int j = 0; j < (nb_bits/2) + 1; j++)
      bootsAND(&aux[j+i] , &a[i], &b[j], bk);

    add(aux2, aux, result, nb_bits, bk);

    for(int j = 0; j < nb_bits; j++) {
      bootsCOPY(&result[j], &aux2[j], bk);
    }

  }

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux2);

}

void mult(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {

  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  // Parámetros para tener en cuenta numeros negativos
  LweSample* negatA = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* negatB = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* opA = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* opB = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* isNegativeA = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* isNegativeB = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  LweSample* corrige = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  // TODO Ajustar numero de bits para que: nb(result) = nb(a)+nb(b)
  for(int i = 0; i < nb_bits; i++){
    bootsCONSTANT(&aux[i], 0, bk);
    bootsCONSTANT(&aux2[i], 0, bk);
    bootsCONSTANT(&negatA[i], 0, bk);
    bootsCONSTANT(&negatB[i], 0, bk);
    bootsCONSTANT(&opA[i], 0, bk);
    bootsCONSTANT(&opB[i], 0, bk);
    bootsCONSTANT(&result[i], 0, bk);
  }

  for(int i = 0; i < 2; i++){
    bootsCONSTANT(&isNegativeA[i], 0, bk);
    bootsCONSTANT(&isNegativeB[i], 0, bk);
    bootsCONSTANT(&corrige[i], 0, bk);
  }

  // BEGIN LOGICA_SIGNO
  negativo(negatA, a, nb_bits, bk);
  negativo(negatB, b, nb_bits, bk);

  /**
    Ponemos los dos números en positivo
  */
  maximum(opA, negatA, a, nb_bits, bk);
  maximum(opB, negatB, b, nb_bits, bk);

  // Si solo uno de los dos es negativo, el resultado es negativo
  is_negative(isNegativeA, a, nb_bits, bk);
  is_negative(isNegativeB, b, nb_bits, bk);
  bootsXOR(corrige, isNegativeA, isNegativeB, bk);
  // END LOGICA_SIGNO

  u_mult(result, opA, opB, nb_bits, bk);

  // BEGIN LOGICA_SIGNO
  // Determinamos si devolver el resultado positivo o negativo
  negativo(aux, result, nb_bits, bk);

  for(int i = 0; i < nb_bits; i++){
    bootsMUX(&result[i], &corrige[0], &aux[i], &result[i], bk);
  }
  // END LOGICA_SIGNO

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux2);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, negatA);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, negatB);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, opA);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, opB);
  delete_gate_bootstrapping_ciphertext_array(2, isNegativeA);
  delete_gate_bootstrapping_ciphertext_array(2, isNegativeB);
  delete_gate_bootstrapping_ciphertext_array(2, corrige);
}

/*
 0 si a >= b
 ¡No tiene en cuenta el signo!
*/
void gte(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* eq = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  bootsCONSTANT(&result[0], 0, bk);
  for(int i = 0; i < nb_bits; i++){
    bootsXNOR(&eq[0], &a[i], &b[i], bk);
    bootsMUX(&result[0], &eq[0], &result[0], &a[i], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(2, eq);

}

// TODO shiftr, shiftl, divide con negativos
void shiftl(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* neg = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* is_neg = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* val = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* res = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  negativo(neg, a, nb_bits, bk);
  is_negative(is_neg, a, nb_bits, bk);

  for(int i = 0; i < nb_bits; i++){
    bootsMUX(&val[i], &is_neg[0], &neg[i], &a[i], bk);
  }

  for(int i = 0; i < nb_bits; i++){
    bootsCOPY(&result[i], &val[i], bk);
  }

  for(int i = 0; i < posiciones; i++){
    for(int j = 1; j < nb_bits; j++)
      bootsCOPY(&aux[j], &result[j-1], bk);

    bootsCONSTANT(&aux[0], 0, bk);

    for(int j = 0; j < nb_bits; j++)
      bootsCOPY(&result[j], &aux[j], bk);
  }

  negativo(aux, result, nb_bits, bk);
  for(int i = 0; i < nb_bits; i++){
    bootsMUX(&result[i], &is_neg[0], &aux[i], &result[i], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, neg);
  delete_gate_bootstrapping_ciphertext_array(2, is_neg);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, val);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, res);

}

void shiftr(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* is_neg = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* neg = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* val = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* res = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  negativo(neg, a, nb_bits, bk);
  is_negative(is_neg, a, nb_bits, bk);

  for(int i = 0; i < nb_bits; i++){
    bootsMUX(&val[i], &is_neg[0], &neg[i], &a[i], bk);
  }

  for(int i = 0; i < nb_bits; i++){
    bootsCOPY(&result[i], &val[i], bk);
  }

  for(int i = 0; i < posiciones; i++){

    for(int j = 0; j < nb_bits - 1; j++)
      bootsCOPY(&aux[j], &result[j+1], bk);

    bootsCONSTANT(&aux[nb_bits - 1], 0, bk);

    for(int j = 0; j < nb_bits; j++)
      bootsCOPY(&result[j], &aux[j], bk);
  }

  negativo(aux, result, nb_bits, bk);
  for(int i = 0; i < nb_bits; i++){
    bootsMUX(&result[i], &is_neg[0], &aux[i], &result[i], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
  delete_gate_bootstrapping_ciphertext_array(2, is_neg);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, neg);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, val);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, res);

}


// TODO shiftr, shiftl, divide con negativos
// unsigned shiftl
void u_shiftl(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  for(int i = 0; i < nb_bits; i++){
    bootsCOPY(&result[i], &a[i], bk);
  }

  for(int i = 0; i < posiciones; i++){
    for(int j = 1; j < nb_bits; j++)
      bootsCOPY(&aux[j], &result[j-1], bk);

    bootsCONSTANT(&aux[0], 0, bk);

    for(int j = 0; j < nb_bits; j++)
      bootsCOPY(&result[j], &aux[j], bk);
  }

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);

}

// unsigned shiftr
void u_shiftr(LweSample* result, const LweSample* a, const int posiciones, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  for(int i = 0; i < nb_bits; i++){
    bootsCOPY(&result[i], &a[i], bk);
  }

  for(int i = 0; i < posiciones; i++){

    for(int j = 0; j < nb_bits - 1; j++)
      bootsCOPY(&aux[j], &result[j+1], bk);

    bootsCONSTANT(&aux[nb_bits - 1], 0, bk);

    for(int j = 0; j < nb_bits; j++)
      bootsCOPY(&result[j], &aux[j], bk);
  }
  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);

}

void u_div(LweSample* cociente, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
  /*
  def div(dividendo, divisor):

      cociente = [i for i in cero]
      resto = [i for i in cero]

      padding = int(bits/2)


      divisor = shiftl(divisor, padding - 1)

      for i in range(padding):
          if debug:
              print(i)
              print(dividendo)
              print(divisor)
              print(cociente)
              print(resto)
              input()
          gt = 1 if gte(dividendo, divisor) else 0
          cociente[padding + i] = gt

          resto = sub(dividendo, divisor) if gt else resto
          dividendo = resto if gt else dividendo

          divisor = shiftr(divisor, 1)

      return cociente
  */
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* gt = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  LweSample* div_aux = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* div_aux2 = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* dividendo = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* divisor = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* resto = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);

  reescala(dividendo, a, 2*nb_bits, nb_bits, bk);

  reescala(div_aux, b, 2*nb_bits, nb_bits, bk);

  u_shiftl(divisor, div_aux, nb_bits - 1, 2*nb_bits, bk);



  for(int i = 0; i < nb_bits; i++) {
    // gt = dividendo >= divisor
    gte(gt, dividendo, divisor, 2*nb_bits, bk);

    bootsCOPY(&cociente[nb_bits-i-1], &gt[0], bk);

    // resto = gt? sub(dividendo, divisor) : resto
    sub(div_aux, dividendo, divisor, 2*nb_bits, bk);
    // divisor = shiftr(divisor, 1)
    u_shiftr(div_aux2, divisor, 1, 2*nb_bits, bk);
    for(int j = 0; j < 2*nb_bits; j++){
      bootsMUX(&resto[j], &gt[0], &div_aux[j], &dividendo[j], bk);
      // dividendo = gt ? resto : dividendo
      bootsMUX(&dividendo[j], &gt[0], &resto[j], &dividendo[j], bk);
      bootsCOPY(&divisor[j], &div_aux2[j], bk);
    }
  }

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux2);
  delete_gate_bootstrapping_ciphertext_array(2, gt);
  delete_gate_bootstrapping_ciphertext_array(2, div_aux);
  delete_gate_bootstrapping_ciphertext_array(2, div_aux2);
  delete_gate_bootstrapping_ciphertext_array(2, dividendo);
  delete_gate_bootstrapping_ciphertext_array(2, divisor);
  delete_gate_bootstrapping_ciphertext_array(2, resto);

}

void div(LweSample* result, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {

  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* negatA = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* negatB = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* opA = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* opB = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* gt = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* bit = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* corrige = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* isNegativeA = new_gate_bootstrapping_ciphertext_array(2, bk->params);
  LweSample* isNegativeB = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  LweSample* div_aux = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* div_aux2 = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* dividendo = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* divisor = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* cociente = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* resto = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);

  for(int i = 0; i < nb_bits; i++){
    bootsCONSTANT(&aux[i], 0, bk);
    bootsCONSTANT(&aux2[i], 0, bk);
    bootsCONSTANT(&opA[i], 0, bk);
    bootsCONSTANT(&opB[i], 0, bk);
  }

  for(int i = 0; i < 2*nb_bits; i++) {
    bootsCONSTANT(&dividendo[i], 0, bk);
    bootsCONSTANT(&div_aux[i], 0, bk);
    bootsCONSTANT(&div_aux2[i], 0, bk);
    bootsCONSTANT(&divisor[i], 0, bk);
    bootsCONSTANT(&cociente[i], 0, bk);
    bootsCONSTANT(&resto[i], 0, bk);
  }

  for(int i = 0; i < 2; i++){
    bootsCONSTANT(&gt[i], 0, bk);
    bootsCONSTANT(&bit[i], 0, bk);
  }

  // BEGIN LOGICA_SIGNO
  negativo(negatA, a, nb_bits, bk);
  negativo(negatB, b, nb_bits, bk);

  /**
    Ponemos los dos números en positivo
  */
  maximum(opA, negatA, a, nb_bits, bk);
  maximum(opB, negatB, b, nb_bits, bk);

  // Si solo uno de los dos es negativo, el resultado es negativo
  is_negative(isNegativeA, a, nb_bits, bk);
  is_negative(isNegativeB, b, nb_bits, bk);
  bootsXOR(corrige, isNegativeA, isNegativeB, bk);
  // END LOGICA_SIGNO

  u_div(result, opA, opB, nb_bits, bk);

  // BEGIN LOGICA_SIGNO
  // Determinamos si devolver el resultado positivo o negativo
  negativo(aux, result, nb_bits, bk);

  for(int i = 0; i < nb_bits; i++){
    bootsMUX(&result[i], &corrige[0], &aux[i], &result[i], bk);
  }
  // END LOGICA_SIGNO

  delete_gate_bootstrapping_ciphertext_array(nb_bits, aux);
}

void porDiez(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* auxA = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* auxB = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* n = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* corrige = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  negativo(auxA, a, nb_bits, bk);

  // Trabajaremos con el positivo
  maximum(n, auxA, a, nb_bits, bk);

  is_negative(corrige, a, nb_bits, bk);

  shiftl(auxA, n, 3, nb_bits, bk);
  shiftl(auxB, n, 1, nb_bits, bk);
  add(n, auxA, auxB, nb_bits, bk);

  negativo(auxA, n, nb_bits, bk);
  for(int i = 0; i < nb_bits; i++)
    bootsMUX(&result[i], &corrige[0], &auxA[i], &n[i], bk);
}

// Reescalar de nb_bits a nb_bits_result
void u_reescala(LweSample* result, const LweSample* a, const int nb_bits_result, const int nb_bits,  const TFheGateBootstrappingCloudKeySet* bk){

  for(int i=0; i < nb_bits_result; i++)
    bootsCONSTANT(&result[i], 0, bk);

  // TODO Si que hay que tener en cuenta el signo
  int bits = (nb_bits > nb_bits_result) ? nb_bits_result : nb_bits;
  for(int i=0; i < bits; i++)
    bootsCOPY(&result[i], &a[i], bk);
}

void reescala(LweSample* result, const LweSample* a, const int nb_bits_result, const int nb_bits,  const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* auxA = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* n = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* corrige = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  LweSample* aux_res = new_gate_bootstrapping_ciphertext_array(nb_bits_result, bk->params);
  LweSample* aux_res_neg = new_gate_bootstrapping_ciphertext_array(nb_bits_result, bk->params);

  negativo(auxA, a, nb_bits, bk);
  is_negative(corrige, a, nb_bits, bk);
  // Trabajaremos con el positivo
  maximum(n, auxA, a, nb_bits, bk);

  u_reescala(aux_res, n, nb_bits_result, nb_bits, bk);

  negativo(aux_res_neg, aux_res, nb_bits_result, bk);
  for(int i = 0; i < nb_bits_result; i++)
    bootsMUX(&result[i], &corrige[0], &aux_res_neg[i], &aux_res[i], bk);
}

// En funciones _flouat los datos intermedios ocupan el doble...
void mult_float(LweSample* result, const LweSample* a, const LweSample* b, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* res_aux = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* aux_a = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* aux_b = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);

  reescala(aux_a, a, 2*nb_bits, nb_bits, bk);
  reescala(aux_b, b, 2*nb_bits, nb_bits, bk);

  mult(aux, aux_a, aux_b, 2*nb_bits, bk);

  shiftr(res_aux, aux, float_bits, 2*nb_bits, bk); // Proteger los decimales

  reescala(result, res_aux, nb_bits, 2*nb_bits, bk);

}

void div_float(LweSample* result, const LweSample* a, const LweSample* b, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* aux_a = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* aux_b = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);
  LweSample* aux_res = new_gate_bootstrapping_ciphertext_array(2*nb_bits, bk->params);

  reescala(aux, a, 2*nb_bits, nb_bits, bk);
  shiftl(aux_a, aux, float_bits, 2*nb_bits, bk); // Proteger los decimales

  reescala(aux_b, b, 2*nb_bits, nb_bits, bk);

  div(aux_res, aux_a, aux_b, 2*nb_bits, bk);
  reescala(result, aux_res, nb_bits, 2*nb_bits, bk);
}


void entreDiez(LweSample* result, const LweSample* a, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){
  LweSample* q = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* aux2 = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* n = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);
  LweSample* corrige = new_gate_bootstrapping_ciphertext_array(2, bk->params);

  negativo(aux, a, nb_bits, bk);

  // Trabajaremos con el positivo
  maximum(n, aux, a, nb_bits, bk);

  is_negative(corrige, a, nb_bits, bk);

  //  q = (n >> 1) + (n >> 2)
  shiftr(aux, n, 1, nb_bits, bk);
  shiftr(aux2, n, 2, nb_bits, bk);
  add(q, aux, aux2, nb_bits, bk);

  // q = q + (q >> 4)
  shiftr(aux, q, 4, nb_bits, bk);
  add(aux2, aux, q, nb_bits, bk);
  for(int i=0; i < nb_bits; i++)
    bootsCOPY(&q[i], &aux2[i], bk);

  // q = q + (q >> 8)
  shiftr(aux, q, 8, nb_bits, bk);
  add(aux2, aux, q, nb_bits, bk);
  for(int i=0; i < nb_bits; i++)
    bootsCOPY(&q[i], &aux2[i], bk);

  // q = q + (q >> 16)
  shiftr(aux, q, 16, nb_bits, bk);
  add(aux2, aux, q, nb_bits, bk);
  for(int i=0; i < nb_bits; i++)
    bootsCOPY(&q[i], &aux2[i], bk);

  // q = q >> 3
  shiftr(aux, q, 3, nb_bits, bk);
  for(int i=0; i < nb_bits; i++)
    bootsCOPY(&q[i], &aux[i], bk);

  for(int i=0; i < nb_bits; i++)
    bootsCOPY(&result[i], &n[i], bk);

  // r = n - (((q << 2) + q) << 1)
  shiftl(aux, q, 2, nb_bits, bk);
  add(aux2, aux, q, nb_bits, bk);
  shiftl(aux, aux2, 1, nb_bits, bk);
  sub(aux2, n, aux, nb_bits, bk);

  // result = q + (r >> 9)
  shiftr(aux, q, 9, nb_bits, bk);


  add(aux2, q, aux, nb_bits, bk);

  negativo(aux, aux2, nb_bits, bk);
  for(int i = 0; i < nb_bits; i++)
    bootsMUX(&result[i], &corrige[0], &aux[i], &aux2[i], bk);
}


void h_pow(LweSample* result, const LweSample* a, const int n, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){

  LweSample* aux = new_gate_bootstrapping_ciphertext_array(nb_bits, bk->params);

  LweSample* cero = new_gate_bootstrapping_ciphertext_array(1, bk->params);
  bootsCONSTANT(&cero[0], 0, bk);

  // Inicializando result
  for(int i=0; i < nb_bits; i++){
    if(n > 0)
      bootsCOPY(&result[i], &a[i], bk);
    else
      bootsCONSTANT(&result[i], 0, bk);
  }

  if(n<=0)
    bootsCONSTANT(&result[0], 1, bk);

  for(int i = 0; i < n-1; i++){
    mult(aux, result, a, nb_bits, bk);
    for(int j = 0; j < nb_bits; j++)
      bootsCOPY(&result[j], &aux[j], bk);
  }

}

/*
  Función para transformar un float a un entero para
  trabajar con criptografía Homomórfica, con float_bits BITS de precisión
*/
int64_t float2hint(float i, int float_bits){
  return (int64_t) (i * pow(2, float_bits));
}

/*
  Función para recuperar un float de un entero tras
  trabajar con criptografía Homomórfica, con float_bits BITS de precisión
*/
float hint2float(int64_t i, int float_bits){
  return i/pow(2, float_bits);
}


/**
  Save number to LweSample (like bootsCONSTANT)

  @param  n             Input number
  @param  float_bits    Bits which should be protected for decimals
*/
void num2lwe(LweSample* result, const int64_t n, const int float_bits, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk){

  int64_t n_plain = float2hint(n, float_bits);

  for(int ci=0; ci < nb_bits; ci++){
    bootsCONSTANT(&result[ci], (n_plain>>ci)&1, bk);
  }

}
