#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int PE(int partial_sum, int arr, int h) {
  int multiply;
  multiply = ((arr * h) + (1 << 14)) >> 15;
  // for rounding...general rule for rounding is adding by half
  // the value of what im dividing by
  partial_sum += multiply;
  return partial_sum;
}

int FixedPoint(float a) {
  int FixedPointResult = (int)round(a * ((1 << 15) - 1));
  //-1 because range is [-2^15,2^15) for Q1.15

  // here i can also use a power function to return int value of 2^15 but i did
  // this in the proper way with 1<<15; this will shift 1 by 15 places to the
  // left giving 2^15...2^15 is done because i wanna have in Q1.15 format
  return FixedPointResult;
}

int main() {
  int partial_sum = 0;
  int x[8] = {0};
  float x_dup[8] = {0.0};

  float H[8] = {-0.00131688, 0.02637484, 0.15577481, 0.31916723,
                0.31916723,  0.15577481, 0.02637484, -0.00131688};
  int H1[8];
  for (int j = 0; j < 8; j++) {
    H1[j] = FixedPoint(H[j]);
  }
  float y[8], sum = 0;
  double error_sq = 0;
  float partial_sum_1;
  for (int n = 0; n < 8; n++) {
    partial_sum = 0;

    // SHIFT REG BLOCK
    int temp = x[0];
    int b;
    float i = (float)rand() / RAND_MAX * 2.0 - 1.0;

    // another array to use for exact values and calculate error
    float temp1 = x_dup[0];
    float b1;
    x_dup[0] = i;
    for (int i1 = 0; i1 < 7; i1++) {
      b1 = x_dup[i1 + 1];
      x_dup[i1 + 1] = temp1;
      temp1 = b1;
    }

    x[0] = FixedPoint(i);
    for (int i2 = 0; i2 < 7; i2++) {
      b = x[i2 + 1];
      x[i2 + 1] = temp;
      temp = b;
    }

    printf("x[i] values -> ");
    for (int x_1 = 0; x_1 < 8; x_1++) {
      printf("%d ", x[x_1]);
    }
    printf("\n");

    for (int j = 0; j < 8; j++) {
      partial_sum = PE(partial_sum, x[j], H1[j]);
    }

    y[n] = (float)partial_sum / ((1 << 15));

    // exact values by using fir logic without pe abstraction
    sum = 0.0;
    for (int N = 0; N < 8; N++) {
      sum += x_dup[N] * H[N]; // converting back to float from my Q1.15 value
    }

    float error = y[n] - sum;
    printf("y_fixed_point = %f | y_exact = %f | error = %f\n\n", y[n], sum,
           error);

    error_sq += error * error;
  }

  // MSE calc
  double MSE;
  MSE = (1.0 / 8.0) * error_sq;

  printf("The final y[n] values are \n");
  for (int k = 0; k < 8; k++) {
    printf("%f ", y[k]);
  }
  printf("\n\nMSE is %e", MSE);
  printf("\n");
}
