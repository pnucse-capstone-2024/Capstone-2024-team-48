#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

void simpleAesEncrypt(const uint8_t* input, uint8_t* output, const uint8_t* key);

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

// CFB 모드 암호화
void cfbEncrypt(const uint8_t* plaintext, uint8_t* ciphertext, const uint8_t* key, uint8_t* iv, size_t length) {
    uint8_t feedback[16]; // AES 블록 크기
    memcpy(feedback, iv, 16); // 초기화 벡터를 피드백으로 설정

    for (size_t i = 0; i < length; i++) {
        uint8_t temp[16];
        simpleAesEncrypt(feedback, temp, key); // 피드백 블록을 암호화
        ciphertext[i] = plaintext[i] ^ temp[0]; // 평문과 암호화된 블록 XOR

        // 피드백 블록 업데이트
        memmove(feedback, feedback + 1, 15); // 왼쪽으로 시프트
        feedback[15] = ciphertext[i]; // 현재 암호문 바이트 추가
    }
}

// AES 암호화 (단순화된 형태)
void simpleAesEncrypt(const uint8_t* input, uint8_t* output, const uint8_t* key) {
    for (int i = 0; i < 16; i++) {
        output[i] = input[i] ^ key[i]; // XOR 연산으로 암호화
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

    // CFB 암호화 예제
    uint8_t key[16] = "1234567890123456"; // 16바이트 키
    uint8_t plaintext[] = "Hello, World!!!"; // 평문
    uint8_t ciphertext[sizeof(plaintext)];
    uint8_t iv[16] = "0000000000000000"; // 초기화 벡터

    cfbEncrypt(plaintext, ciphertext, key, iv, sizeof(plaintext));

    printf("Ciphertext: ");
    for (size_t i = 0; i < sizeof(ciphertext); i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    clock_t end = clock();
    printf("소요 시간: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
