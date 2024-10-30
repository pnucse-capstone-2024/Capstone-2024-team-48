#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

// AES 암호화 (단순화된 형태)
void simpleAesEncrypt(const uint8_t* input, uint8_t* output, const uint8_t* key) {
    for (int i = 0; i < 16; i++) {
        output[i] = input[i] ^ key[i]; // XOR 연산으로 암호화
    }
}

// CTR 모드 암호화
void ctrEncrypt(const uint8_t* plaintext, uint8_t* ciphertext, const uint8_t* key, uint8_t* nonce, size_t length) {
    uint8_t counter[16];
    memcpy(counter, nonce, 12); // nonce로 초기화 (12바이트)
    counter[12] = 0; // 카운터 초기화
    counter[13] = 0;
    counter[14] = 0;
    counter[15] = 0;

    uint8_t keystream[16];

    for (size_t i = 0; i < length; i++) {
        if (i % 16 == 0) {
            simpleAesEncrypt(counter, keystream, key); // 카운터를 암호화하여 키 스트림 생성
            counter[15]++; // 카운터 증가
        }
        ciphertext[i] = plaintext[i] ^ keystream[i % 16]; // 평문과 키 스트림 XOR
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

    // CTR 암호화 예제
    uint8_t key[16] = "1234567890123456"; // 16바이트 키
    uint8_t plaintext[] = "Hello, World!!!"; // 평문
    uint8_t ciphertext[sizeof(plaintext)];
    uint8_t nonce[16] = "12345678"; // nonce (8바이트)

    ctrEncrypt(plaintext, ciphertext, key, nonce, sizeof(plaintext));

    printf("Ciphertext: ");
    for (size_t i = 0; i < sizeof(ciphertext); i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    clock_t end = clock();
    printf("소요 시간: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}

    return 0;
}
