#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int a;
    int b;
    int p; // 소수
} EllipticCurve;

// 점 덧셈
Point add(Point P, Point Q, EllipticCurve curve) {
    if (P.x == 0 && P.y == 0) return Q;
    if (Q.x == 0 && Q.y == 0) return P;

    int lambda = (Q.y - P.y) * modInverse(Q.x - P.x, curve.p) % curve.p;
    int xR = (lambda * lambda - P.x - Q.x) % curve.p;
    int yR = (lambda * (P.x - xR) - P.y) % curve.p;

    return (Point) { (xR + curve.p) % curve.p, (yR + curve.p) % curve.p };
}

// 스칼라 곱셈
Point scalarMultiply(Point P, int k, EllipticCurve curve) {
    Point R = { 0, 0 }; // 무한대 점
    for (int i = 0; i < k; i++) {
        R = add(R, P, curve);
    }
    return R;
}

// 모듈로 역수 계산
int modInverse(int a, int p) {
    a = a % p;
    for (int x = 1; x < p; x++) {
        if ((a * x) % p == 1) {
            return x;
        }
    }
    return 1; // 역수가 없는 경우
}

// AES XOR 암호화 (단순화된 형태)
void aes_xor_encrypt(const uint8_t* input, uint8_t* output, const uint8_t* key, size_t length) {
    for (size_t i = 0; i < length; i++) {
        output[i] = input[i] ^ key[i % 16]; // 16바이트 키로 XOR
    }
}

// CBC 모드 암호화
void cbc_encrypt(const uint8_t* plaintext, uint8_t* ciphertext, const uint8_t* key, const uint8_t* iv, size_t length) {
    uint8_t block[16];
    memcpy(block, iv, 16); // 초기화 벡터

    for (size_t i = 0; i < length; i += 16) {
        for (int j = 0; j < 16 && (i + j) < length; j++) {
            block[j] ^= plaintext[i + j]; // 이전 블록과 XOR
        }
        aes_xor_encrypt(block, ciphertext + i, key, 16); // AES 암호화 호출 시 길이 인수 추가
        memcpy(block, ciphertext + i, 16); // 현재 블록을 다음 블록의 IV로 사용
    }
}

int main() {
    clock_t start = clock();
    EllipticCurve curve = { 2, 3, 97 }; // y^2 = x^3 + 2x + 3 (mod 97)
    Point P = { 3, 6 };
    Point Q = { 10, 7 };

    Point R = add(P, Q, curve);
    printf("P + Q = (%d, %d)\n", R.x, R.y);

    int k = 2;
    Point S = scalarMultiply(P, k, curve);
    printf("2P = (%d, %d)\n", S.x, S.y);

    // CBC 암호화
    uint8_t key[16] = "1234567890123456"; // 16바이트 키
    uint8_t plaintext[] = "Hello, World!!!"; // 16바이트 평문
    uint8_t ciphertext[sizeof(plaintext)];
    uint8_t iv[16] = "0000000000000000"; // 초기화 벡터

    cbc_encrypt(plaintext, ciphertext, key, iv, sizeof(plaintext));

    printf("Ciphertext: ");
    for (size_t i = 0; i < sizeof(ciphertext); i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    clock_t end = clock();
    printf("소요 시간: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
